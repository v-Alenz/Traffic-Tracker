/**
 * \file hub_utilities.hpp
 * \author Andrea Valenzano
 * \date 2022
 * \copyright AGPLv3: Affero General Public License version 3
 * \pre Libreria che implementa le Utilities  per la gestione dell'interfaccia utente
 */ 
 
/* Programma scritto durante lo svolgimento del progetto finale per il 
 * corso di Multimedia App and Internet of Things dell'Universita' degl Studi di Genova
 *
 * Copyright (C) 2022  Andrea Valenzano
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

#ifndef HUB_UTILITIES_HPP
#define HUB_UTILITIES_HPP

#include <iostream>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string>


namespace utilities {

    /**
     * Struct colors.
     * Struct che contiene il codice ANSI dei colori 
     */
    struct colors{
        std::string red = "\033[31m";
        std::string blue = "\033[34m";
        std::string green = "\033[32m";
        std::string magenta = "\033[35m";
        std::string cyan = "\033[36m";
        std::string yellow = "\033[33m";
        std::string defaul = "\033[0m";
    };

    /**
     * \fn inline bool is_number(const std::string& s)
     * \brief Funzione che verifica se una stringa e' numerica
     * \param[in] s std::string da verificare
     * \return Vero se la stringa e' numerica, Falso altrimenti
     */
    inline bool is_number(const std::string& s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it)) ++it;
        return !s.empty() && it == s.end();
    }
 
    /**
     *  \class Prompt
     *  \brief Classe contenitore per tutti i metodi usati nell'interazione grafica con l'utente
     */
    class Prompt{

        private:

            bool is_colored;
            utilities::colors color;

        public:

            /**
             * \fn std::string get_prompt()
             * \brief Visualizza il prompt a schermo e richiede in inputun comando all'utente
             * \return Il comando inserito dall'utente
             * Questa funzione e' impelementata usando la libreria GNU readline/readline.h per una migliore interazione con l'utente
             */
            std::string get_prompt(){
                std::string prompt;
                if(is_colored){ 
                    prompt = color.green + "◆" + color.cyan + " Server" + color.defaul + "Iot" + color.green + "++" + color.defaul + " " + color.cyan + "❯" + color.defaul + "❯" + color.green + "❯ " + color.defaul;
                }else
                    prompt  =  "[ServerIot++] >>> ";

                std::string input(readline(prompt.c_str()));
                return input;
            }

            /**
             * \fn void prompt_help()
             * \brief Stampa a schermo la guida ai comandi
             */
            void prompt_help(){
                std::cout << std::endl << "ServerIot 'help':" << std::endl;
                std::cout << std::endl << "     - camera " << std::endl;
                std::cout << "          Command to handle cameras" << std::endl;
                std::cout << std::endl << "     - system " << std::endl;
                std::cout << "          Execute a command on default shell" << std::endl;
                std::cout << std::endl << "     - help " << std::endl;
                std::cout << "          Show commands list" << std::endl;
                std::cout << std::endl << "     - uptime " << std::endl;
                std::cout << "          Show server uptime" << std::endl;
                std::cout << std::endl << "     - version " << std::endl;
                std::cout << "          Show program version" << std::endl;
                std::cout << std::endl << "     - exit " << std::endl;
                std::cout << "          Quit program" << std::endl;
                std::cout << std::endl;
            }

            /**
             * \fn void command_error(std::string command)
             * \brief Stampa il messaggio di errore per l'inserimento di un comando non riconosciuto
             * \param[in] command Il comando non riconosciuto
             */
            void command_error(std::string command){
                std::cout << std::endl << ((is_colored==true)?color.red:"") + "Error!" + color.defaul + " command \"" << command << "\" not found!" << std::endl;
                std::cout << "Type \"" + ((is_colored==true)?color.magenta:"") + "help" + color.defaul + "\" for hints" << std::endl << std::endl;
            }

            /**
             * \fn void camera_error(std::string command)
             * \brief Stampa il messaggio di errore per l'inserimento di un parametro non riconosciuto al comando 'camera'
             * \param[in] command Il parametro non riconosciuto
             */
            void camera_error(std::string command){
                std::cout << std::endl << ((is_colored==true)?color.red:"") + "Error!" + color.defaul + " option \"" << command << "\" for command \"camera\" not found!" << std::endl;
                std::cout << "Type \"" + ((is_colored==true)?color.magenta:"") + "camera help" + color.defaul + "\" for hints" << std::endl << std::endl;
            }

            /**
             * \fn void camera_status_error(std::string command)
             * \brief Stampa il messaggio di errore per l'inserimento di un parametro non riconosciuto al comando 'camera status'
             * \param[in] command Il parametro non riconosciuto
             */
            void camera_status_error(std::string command){
                std::cout << std::endl << ((is_colored==true)?color.red:"") + "Error!" + color.defaul + " option \"" << command << "\" for command \"camera status\" not found!" << std::endl;
                std::cout << "Type \"" + ((is_colored==true)?color.magenta:"") + "camera status help" + color.defaul + "\" for hints" << std::endl << std::endl;
            }

            /**
             * \fn void camera_start_error(std::string command)
             * \brief Stampa il messaggio di errore per l'inserimento di un parametro non riconosciuto al comando 'camera start'
             * \param[in] command Il parametro non riconosciuto
             */
            void camera_start_error(std::string command){
                std::cout << std::endl << ((is_colored==true)?color.red:"") + "Error!" + color.defaul + " option \"" << command << "\" for command \"camera start\" not found!" << std::endl;
                std::cout << "Type \"" + ((is_colored==true)?color.magenta:"") + "camera start help" + color.defaul + "\" for hints" << std::endl << std::endl;
            }

            /**
             * \fn void camera_stop_error(std::string command)
             * \brief Stampa il messaggio di errore per l'inserimento di un parametro non riconosciuto al comando 'camera stop'
             * \param[in] command Il parametro non riconosciuto
             */
            void camera_stop_error(std::string command){
                std::cout << std::endl << ((is_colored==true)?color.red:"") + "Error!" + color.defaul + " option \"" << command << "\" for command \"camera stop\" not found!" << std::endl;
                std::cout << "Type \"" + ((is_colored==true)?color.magenta:"") + "camera stop help" + color.defaul + "\" for hints" << std::endl << std::endl;
            }

            /**
             * \fn void camera_help(unsigned int help_code)
             * \brief Stampa l'help
             * \param[in] help_code Il codice del comando di cui stampare l'help
             */
            void camera_help(unsigned int help_code){
                switch (help_code) {
                    case 1:
                        std::cout << std::endl << "camera status 'help':" << std::endl;
                        std::cout << std::endl << "     - camera status {'camera number' / all}" << std::endl;
                        std::cout << "          Show Cameras status and uptime" << std::endl;
                        std::cout << std::endl;
                    break;
                    
                    case 2:
                        std::cout << std::endl << "camera start 'help':" << std::endl;
                        std::cout << std::endl << "     - camera start {'camera number/numbers'}" << std::endl;
                        std::cout << "          Start target camera(s)" << std::endl;
                        std::cout << std::endl;
                    break;

                    case 3:
                        std::cout << std::endl << "camera stop 'help':" << std::endl;
                        std::cout << std::endl << "     - camera stop {'camera number/numbers'}" << std::endl;
                        std::cout << "          Stope target camera(s)" << std::endl;
                        std::cout << std::endl;
                    break;

                    case 0:
                    default:
                        std::cout << std::endl << "camera 'help':" << std::endl;
                        std::cout << std::endl << "     - status {'camera number' / all}" << std::endl;
                        std::cout << "          Show cameras status and uptime" << std::endl;
                        std::cout << std::endl << "     - start {'camera number/numbers'}" << std::endl;
                        std::cout << "          Start target camera(s)" << std::endl;
                        std::cout << std::endl << "     - stop {'camera number/numbers'}" << std::endl;
                        std::cout << "          Stop target camera(s)" << std::endl;
                        std::cout << std::endl;
                }
            }

            /**
             * \fn Prompt(bool colors = false)
             * \brief Costruttore per la classe Prompt
             * \param[in] colors Specifica se il terminale in uso supporta i colori oppure no
             * Inizializza un oggetto che stampera' usando i colori ed i caratteri Unicode oppure con colori e caratteri base
             */
            Prompt(bool colors = false){
                is_colored = colors;
            }

            ~Prompt(){}

    };

}

#endif //HUB_UTILITIES_HPP