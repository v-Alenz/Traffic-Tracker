/**
 * \file main.cpp
 * \author Andrea Valenzano
 * \date 2022
 * \copyright AGPLv3: Affero General Public License version 3
 * \pre Server per la gestione delle telecamere,
 * Interfaccia per la gestione e l'analisi dell'input delle telecamenre di Traffic Traker.
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

#include "../includes/string_parser.hpp"
#include "../includes/hub_utilities.hpp"
#include "../includes/camera_threads.hpp"
#include "../includes/database_utilities.hpp"


#include <cctype>
#include <cstdlib>
#include <iostream>
#include <readline/history.h>
#include <vector>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>
#include <time.h>
#include <stdlib.h>

#define VERSION "0.1.0 alpha"

std::vector<camera::Camera> cameras;

/**
 * \fn int main(int argc,char** argv)
 * \brief Funzione main
 * Funzione che inplementa il main loop del programma e l'interfaccia per l'iserimento dei comandi da parte del'utente.
 */
int main(int argc,char** argv){

    /* Instanzio un oggetto di tipo Database e verifico che possa effettivamente connettersi al database*/
    std::cout << std::endl << "  Traffic Tracker" << std::endl << std::endl << "Getting started...." << std::endl;
    Database::Database database("database_config.yaml");
    if(!database.check_conneciton()){
        std::cout << std::endl << "Database not found!" << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    /* Leggo le varie impostazioni dal file di configurazione config.yaml */
    YAML::Node config = YAML::LoadFile("config.yaml");

    bool prompt_colors;
    if(config["prompt_colors"]){
        try{
            prompt_colors = config["prompt_colors"].as<bool>();
        } catch(std::exception e){
            e.what();
            exit(EXIT_FAILURE);
        }
    } else {
        std::cout << std::endl << "Unable to load configurations!" << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    if(config["cameras"]){
        camera::camera_stopper.push_back(false);
        std::vector<int> cameras_ids = database.get_cameras_ids();
        try{
            for(auto camera : config["cameras"]){
                if(!camera["load_camera"].as<bool>())
                    continue;
                int id = camera["id"].as<int>();
                std::string name = camera["name"].as<std::string>();
                bool cuda = camera["cuda"].as<bool>();
                bool debug = camera["debug"].as<bool>();
                int fps = camera["fps_target"].as<int>();
                std::string source = camera["source"].as<std::string>();
                int traffico = camera["traffico"].as<int>();
                int traffico_intenso = camera["traffico_intenso"].as<int>();
                
                bool camera_found = false;

                for(auto ids : cameras_ids){
                    if(id == ids){
                        camera::camera_stopper.push_back(false);
                        cameras.push_back(camera::Camera(id, source, traffico, traffico_intenso, name, cuda, debug, fps, prompt_colors));
                        camera_found = true;
                    }
                }

                if(camera_found){
                    continue;
                }

                std::cout << "Camera not recognised: (" << id << ") " << name << std::endl;
                std::cout << "Load target camera? (Y/n):";
                std::string insert;
                std::cin >> insert;

                if(insert == "Y"){
                    float lat = camera["lat"].as<float>();
                    float lng = camera["lng"].as<float>();
                    if(!database.insert_telecamere_by_id(id, lat, lng)){
                        std::cout << "Unable to load target camera, Database error!" << std::endl << std::endl;
                    }
                    camera::camera_stopper.push_back(false);
                    cameras.push_back(camera::Camera(id, source, traffico, traffico_intenso, name, cuda, debug, fps, prompt_colors));
                }

            }
        } catch(std::exception e){
            e.what();
            exit(EXIT_FAILURE);
        }
    } else {
        std::cout << std::endl << "Unable to load any camera!" << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    time_t start_timer = time(NULL);

    stringParser::Parser parser;
    utilities::Prompt prompt(prompt_colors);

    std::string buffer;
    std::queue<std::string> opt;
    utilities::colors colors;

    using_history(); // Uso GNU History per rendere il prompt piu' interattivo.


    std::cout << "Configuration Done!" << std::endl << std::endl;


    /* Main Loop
     * Predo in input dall'utente una lista di comandi e dopo aver fatto il parsing dell'input provo ad eseguirli
     * lanciando un un messaggio di errore appropriato in caso di fallimento.
     */
    while(true){

        // Chiedo l'input all'utente
        buffer = prompt.get_prompt();
        if(buffer.length() > 0){
            add_history(buffer.c_str());
        }
        
        // Faccio il parsing dell'input
        parser.parser_init(buffer);
        opt = parser.get_buffer();

        // EXEC OPTIONS
        // Eseguo il comando inserito dall'utente

        //Basic options
        if(opt.front() == "" || opt.empty()) continue;
        if(opt.front() == "exit") break;
        if(opt.front() == "help" || opt.front() == "h"){
            prompt.prompt_help();
            continue;
        }
        if(opt.front() == "version") { std::cout << std::endl << VERSION << std::endl << std::endl; continue; }
        if(opt.front() == "uptime"){
            time_t uptime = time(NULL) - start_timer;
            std::cout << std::endl << "Uptime: " <<(int)(uptime/60)/60 << " H : " << (int)uptime/60 << " M : " << (int)uptime%60 << " S" << std::endl << std::endl; 
            continue;
        }

        // Camera operations
        if(opt.front() == "camera"){
            opt.pop();

            //STATUS
            if(opt.front() == "status"){
                opt.pop();
                if(opt.empty() || opt.front() == ""){
                    prompt.camera_help(1);
                    continue;
                }

                if(opt.front() == "all"){
                    for(auto camera : cameras){
                        camera.print_status();
                    }
                    continue;
                }

                if(utilities::is_number(opt.front())){
                    try{
                        cameras.at(atoi(opt.front().c_str())-1).print_status();
                        continue;
                    } catch (std::exception e){
                        std::cout << std::endl << "Index out of range!" << std::endl << std::endl;
                        continue;
                    }
                }

                if(opt.front() == "help"){
                    prompt.camera_help(1);
                    continue;
                }

                prompt.camera_status_error(opt.front());
                continue;

            }

            // START
            if(opt.front() == "start"){
                opt.pop();
                
                if(opt.empty() || opt.front() == ""){
                    prompt.camera_help(2);
                    continue;
                }
                
                if(utilities::is_number(opt.front())){
                    try{
                        cameras.at(atoi(opt.front().c_str())-1).start();
                        continue;
                    } catch (std::exception e){
                        std::cout << std::endl << "Index out of range!" << std::endl << std::endl;
                        continue;
                    }
                }

                if(opt.front() == "help"){
                    prompt.camera_help(2);
                    continue;
                }

                prompt.camera_start_error(opt.front());
                continue;
            }

            // STOP
            if(opt.front() == "stop"){
                opt.pop();

                if(opt.empty() || opt.front() == ""){
                    prompt.camera_help(3);
                    continue;
                }
                
                if(utilities::is_number(opt.front())){
                    try{
                        cameras.at(atoi(opt.front().c_str())-1).stop();
                        continue;
                    } catch (std::exception e){
                        std::cout << std::endl << "Index out of range!" << std::endl << std::endl;
                        continue;
                    }
                }

                if(opt.front() == "help"){
                    prompt.camera_help(3);
                    continue;
                }

                prompt.camera_stop_error(opt.front());
                continue;    
            }
            
            // HELP
            if(opt.front() == "help" || opt.front() == "" || opt.empty()){
                prompt.camera_help(0);
                continue;
            }

            prompt.camera_error(opt.front());
            continue;
        }

        // SYSTEM
        if(opt.front() == "system"){
            opt.pop();

            std::string command = "";
            while(!opt.empty()){
                command += opt.front() + " ";
                opt.pop();
            }

            system(command.c_str());
            continue;
        }

        prompt.command_error(opt.front());

    }

    clear_history(); // Prima di uscire cancello la cronologia di GNU HIstory

    exit(EXIT_SUCCESS);
}

