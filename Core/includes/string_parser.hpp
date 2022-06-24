/**
 * \file string_parser.hpp
 * \author Andrea Valenzano
 * \date 2022
 * \copyright AGPLv3: Affero General Public License version 3
 * \pre Libreria per la gestione delle stringhe in input al programma
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

#ifndef STRING_PARSER_HPP
#define STRING_PARSER_HPP

#include <iostream>
#include <queue>

namespace stringParser{

    /**
     * \class Parser
     * \brief Classe che implementa il manipolatore di stringhe
     */
    class Parser{

        private:

            std::queue<std::string> buffer; /*!< Variabile in cui conservare il risultato del parsing dell'input*/

        public:

            /**
             * \fn int parser_init(std::string input)
             * \brief Funzione di parsing
             * \param[in] input La stringa in formato std::string di cui fare il parsing
             * \return 0 se tutto e' andato a buon fine, -1 altrimenti
             * Funzione che preso in input una stringa ne fa il parsing e salva il contenuto nella queue buffer dell'oggetto
             */
             
            int parser_init(std::string input){

                std::string token;
                size_t pos = 0;
                std::string delimiter = " ";
                
                try{

                    while(!buffer.empty()){
                        buffer.pop();
                    }

                    while ((pos = input.find(delimiter)) != std::string::npos) {
                        token = input.substr(0, pos);
                        input.erase(0, pos + delimiter.length());
                        if(token != ""){
                            buffer.push(token);
                        }
                    }
                    if(input != ""){
                            buffer.push(input);
                        }

                } catch( std::exception e){
                    e.what();
                    return -1;
                }

                return (int)buffer.size();
            }

            /**
             * \fn int print_buffer()
             * \brief Stampa il contenuto del buffer
             * \return 0 se tutto e' andato a buon fine, -1 altriemnti
             * Funzione che stampa il contenuto della queue di buffer sullo standart output.
             * Se la funzione e' andata a buon fine il buffer sara' identico a come era prima dell'invocazione, in caso di fallimento non e' invece garantita questa consistenza.
             */
            int print_buffer(){

                std::string aux;
                
                try{
                    for(unsigned int i=0; i<buffer.size(); i++){
                        
                        aux = buffer.front();
                        std::cout << aux << std::endl;
                        buffer.push(aux);
                        buffer.pop();

                    }
                } catch (std::exception e){
                    e.what();
                    return -1;
                }
                return 0;
            }

            /**
             * \fn std::queue<std::string> get_buffer()
             * \brief Ritorna il buffer
             * \return Il buffer dell'oggetto allo stato attuale
             * Funzione "getter" che ritorna il buffer dell'oggetto allo stato attuale
             */
            std::queue<std::string> get_buffer(){
                return buffer;
            }

            /**
             * \fn Parser()
             * \brief Costruttore della classe Parser
             * Costruttore di default, non e' necessario compiere azioni al momento del'inizializazione di un nuovo oggetto Parser
             */
            Parser(){}

            /**
             * \fn ~Parser()
             * \brief Distruttore dela classe Parser
             * Il distruttre svuta i contenuto del buffer prima di eliminare l'oggetto. Questa azione non dovrebbe essere necessaria essendo il buffer
             * inizializato staticamente, ma come si dice il divolo e' tanto fino e i puntatori son figli suoi.
             */
            ~Parser(){
                while(!buffer.empty()){
                    buffer.pop();
                }
            }
    };
}

#endif //STRING_PARSER_HPP