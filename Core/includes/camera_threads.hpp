/**
 * \file camera_threads.hpp
 * \author Andrea Valenzano
 * \date 2022
 * \copyright AGPLv3: Affero General Public License version 3
 * \pre Libreria per la gestione delle telecamere
 */ 

/* Copyright (C) 2022  Andrea Valenzano
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CAMERA_THREADS_HPP
#define CAMERA_THREADS_HPP

#include "hub_utilities.hpp"
#include "database_utilities.hpp"

#include <array>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <ios>
#include <iostream>
#include <thread>
#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/video/background_segm.hpp>

using namespace cv;
using namespace  dnn;

namespace camera{

    std::vector<bool> camera_stopper; /*!< Vettore usato per fermare i thread delle telecamere */

    /**
     * \class Camera
     * \brief Classe che implemente e gestisce le telecamere per il riconoscimento e e il tracciamento degli oggetti
     */
    class Camera{

        private:

            unsigned int id;
            std::string name;

            bool is_colored;
            utilities::colors colors;

            std::string status;
            bool CUDA;
            bool DEBUG;
            int FPS_TARGET;
            std::string source_file;

            int traffico;
            int traffico_intenso;

            time_t start_time;

        public:
            /**
            * \fn void print_status()
            * \brief Stampa lo status della telecamera
            */
            void print_status(){
                if(is_colored){
                    std::cout << std::endl << (status=="active"?colors.green+" ● "+colors.defaul+"(":colors.red+" ● "+colors.defaul+"(") << id << ") "<< name << std::endl;
                    std::cout << " mode:    " << (CUDA?"GPU":"CPU") << std::endl; 
                    std::cout << " max FPS: "; FPS_TARGET==-1? std::cout << "Unlimited": std::cout << FPS_TARGET; std::cout <<  std::endl;
                    std::cout << " status:  " << (status=="active"?colors.green:colors.red) << status << colors.defaul << std::endl;
                    std::cout << " uptime:  " ;
                    if(status == "active"){
                        time_t uptime = time(NULL) - start_time;
                        std::cout << (int)(uptime/60)/60 << " H : " << (int)uptime/60 << " M : " << (int)uptime%60 << " S" << std::endl;
                    } else {
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;               
                    return;
                }

                std::cout << std::endl << " - (" << id << ") "<< name << std::endl;
                std::cout << " mode:    " << (CUDA?"GPU":"CPU") << std::endl; 
                std::cout << " max FPS: "; FPS_TARGET==-1? std::cout << "Unlimited": std::cout << FPS_TARGET; std::cout <<  std::endl;
                std::cout << " status:  " << status << std::endl;
                std::cout << " uptime:  " ;
                if(status == "active"){
                    time_t uptime = time(NULL) - start_time;
                    std::cout << (int)(uptime/60)/60 << " H : " << (int)uptime/60 << " M : " << (int)uptime%60 << " S" << std::endl;
                } else {
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }

            /**
            * \fn void start()
            * \brief Metodo che avvia la telecamera e l'analisi dei dati in input
            */
            void start(){
                if(camera_stopper.at(id) == true) return;
                camera_stopper.at(id) = true;
                status = "active";
                start_time = time(NULL);
                try{
                    std::thread this_camera_thread(&Camera::camera_thread, this);
                    this_camera_thread.detach();
                } catch(std::exception e){
                    std::cout << e.what() << std::endl;
                }
            }
            
            /**
            * \fn void stop()
            * \brief Metodo che ferma il thread della telecamera
            */
            void stop(){
                camera_stopper.at(id) = false;
                status = "stopped";
            }

            /**
            * \fn vaid camera_thread()
            * \brief Analisi e riconoscimento delle immagini in input
            * Metodo che viene lanciato come thread separato in modalita' detached e che si occupa di recuperare l'input video dal source,
            * di analizzarlo e di scrivere i risultati nel database
            */
            void camera_thread(){

                Database::Database database("database_config.yaml");

                database.start_telecamera(id);

                float FPS_TARGET_FREQ = 1.0/FPS_TARGET;
                std::string MODE;
                unsigned int FRAME_COUNT = 0;
                unsigned int CAR_COUT = 0;
                bool VISUALIZE = true;
                short FPS;
                float FPS_CURR_FREQ;

                int scrolling = 0;
                database.insert_registrazione(id, scrolling);
                
                Mat frame;

                // Tracking
                std::vector<std::array<int, 3>> tracker_1;
                std::vector<std::array<int, 3>> tracker_2;
                std::vector<std::array<int, 3>> tracker_3;


                std::vector<int> classIds;
                std::vector<float> confidences;
                std::vector<Rect> boxes;

                std::vector<std::string> classes;
                std::ifstream file("dnn_model/classes.txt");
                std::string line;
                while (std::getline(file, line)){
                    classes.push_back(line);
                }
                file.close();

                Net net = readNetFromDarknet("dnn_model/traffictracker.cfg", "dnn_model/traffictracker.weights");
                if (CUDA){
                    try{
                        net.setPreferableBackend(DNN_BACKEND_CUDA);
                        net.setPreferableTarget(DNN_TARGET_CUDA_FP16);
                        MODE = "GPU";
                    } catch (const Exception e){
                        MODE = "CPU";
                    }
                }
                DetectionModel model = DetectionModel(net);
                model.setInputParams(1.0/255.0, Size(320, 320), Scalar(), true);

                VideoCapture source = VideoCapture(source_file);
                source.set(CAP_PROP_FPS, 30);
                source.set(CAP_PROP_FRAME_WIDTH, 640.0);
                source.set(CAP_PROP_FRAME_HEIGHT, 480.0);

                int64 t_start;
                int64 t_end;


                for (;camera_stopper.at(id);){

                    t_start = getTickCount();

                    //Catturo il frame dal video
                    try{
                        source.read(frame);
                        FRAME_COUNT++;
                    } catch(const Exception e){
                        destroyAllWindows();
                        break;
                    }

                    // Faccio il riconoscimento
                    model.detect(frame, classIds, confidences, boxes, 0.6, 0.6);

                    std::array<int, 3> point_aux;

                    // Tracker
                    std::vector<std::array<int, 3>> tracker_aux;

                    for (int i=0; i<classIds.size(); i++){
                        
                        point_aux[0] = (boxes[i].x*2+boxes[i].width)/2;
                        point_aux[1] = (boxes[i].y*2+boxes[i].height)/2;

                        float distance = 20.0;

                        for (auto traker : {tracker_1, tracker_2, tracker_3}){
                            for (auto t_point : traker){
                                //controllo la distanza tra il punto centrale della box e quelli registrati nei frame precedente
                                if( sqrt(pow(t_point[0]-point_aux[0],2) + pow(t_point[1] - point_aux[1], 2) * 1.0) < distance){
                                    // Se si sono punti che rappresentano lo stesso oggetto
                                    point_aux[2] = t_point[2];
                                    goto point_found;
                                }
                            }
                            // In caso negativo passo al traker del frame precedente aumentando la distanza (purtroppo aggiungendo frame si perde precisione)
                            distance += 20.0;
                        }

                        // Se non trovo nulla in nessuno dei frame precedenti vuol dire che ho un nuovo oggetto!
                        CAR_COUT++;
                        point_aux[2] = CAR_COUT;

                        // Aggiungo il punto al nuovo traker
                        point_found:
                        tracker_aux.push_back(point_aux);
                    }

                    tracker_3 = tracker_2;
                    tracker_2 = tracker_1;
                    tracker_1 = tracker_aux;

                    // Ogni miniuto aggiorno il database (se necessario)
                    if(FRAME_COUNT%(FPS_TARGET*60) == 0){
                        if(tracker_1.size() >= traffico_intenso){
                            if(scrolling != 2){
                                scrolling = 2;
                                database.insert_registrazione(id, scrolling);
                            }
                        }else if (tracker_1.size() >= traffico) {
                            if(scrolling != 1){
                                scrolling = 1;
                                database.insert_registrazione(id, scrolling);
                            }
                        }else{
                            if(scrolling != 0){
                                scrolling = 0;
                                database.insert_registrazione(id, scrolling);
                            }
                        }
                    }

                    t_end = getTickCount();

                    FPS_CURR_FREQ = 1.0/(getTickFrequency()/(t_end-t_start));

                    std::this_thread::sleep_for(std::chrono::milliseconds((FPS_TARGET_FREQ > FPS_CURR_FREQ) ? short(1000*(FPS_TARGET_FREQ-FPS_CURR_FREQ)) : 0));
                    
                    FPS = short(getTickFrequency()/(getTickCount() - t_start));
                    
                }

                database.stop_telecamera(id);
                return;

            }

            /**
            * \fn Camera(unsigned int id, std::string source_file, int traffico, int traffico_intenso, std::string name = "", bool cuda = false, bool debug = false,  int fps = -1,bool is_colored = false)
            * \brief Construttore della clasee Camera
            * \param[in] id L'id della telecamera, riferimento della telecamera nel database
            * \param[in] source_file La/il (pipe || source ID || file || data stream || file descriptor) che identifica l'input video
            * \param[in] traffico Il numero di veicoli a schermo sopra la quale si considera la strada trafficata
            * \param[in] traffico_intenso Il numero di veicoli sopra la quale si considera la strada molto trafficata
            * \param[in] name Il nome da associare alla telecamera (Nome fittizio solo per riconoscerla meglio). default: ""
            * \param[in] cuda Identifica se si intende utilizzare CUDA per effettuare i calcoli della rete neurale sulla GPU. default: false
            * \param[in] debug Identifica se si intende abilitare i messaggi di debug(in questa versione non sono ancora inplementati). default: false
            * \param[in] fps Identifica il limite massimo di fps a cui viene analizzato il file/source video. In caso di connessione diretta con telecamere lasciare a -1. default: -1
            * \param[in] is_colored Se vero lo status della telecamera viene stampato a colori e con caratteri UTF-8. In caso contrario viene stampato in ascii. default: false
            */
            Camera(unsigned int id, std::string source_file, int traffico, int traffico_intenso, std::string name = "", bool cuda = false, bool debug = false,  int fps = -1,bool is_colored = false){
                CUDA = cuda;
                DEBUG = debug;
                FPS_TARGET = fps;
                this->is_colored = is_colored;
                this->id = id;
                this->name = name;
                this->source_file = source_file;
                this->traffico = traffico;
                this->traffico_intenso = traffico_intenso;

                status = "stopped";
            }

            ~Camera(){}

    };

}

#endif //CAMERA_THREADS_HPP