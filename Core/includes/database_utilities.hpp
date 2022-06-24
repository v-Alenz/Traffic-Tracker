/**
 * \file database_utilities.hpp
 * \author Andrea Valenzano
 * \date 2022
 * \copyright AGPLv3: Affero General Public License version 3
 * \pre Libreria per l'interazione con il database
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

#ifndef DATABASE_UTILITIES_H
#define DATABASE_UTILITIES_H

#include <cstdlib>
#include <vector>
#include <exception>
#include <iostream>
#include <mysql++.h>
#include <mysql++/connection.h>
#include <mysql++/query.h>
#include <mysql++/result.h>
#include <ostream>
#include <string>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/node/parse.h>


namespace Database {

    /**
     *  \class Database
     *  \brief Classe che implementa le funzionalita' richieste per interagire con il database
     */
    class Database{

        private:

        std::string database;
        std::string server;
        std::string user;
        std::string passwd;
        unsigned int port;

        public:

        /**
         * \fn Database(std::string config_path)
         * \brief Costruttore della classe Database
         * \param[in] config_path Indirizzo in memoria del file di configurazione
         */
        Database(std::string config_path){

            //Inizializzo i dati leggendoli dal database
            YAML::Node config_file = YAML::LoadFile(config_path);
            try{
                database = config_file["database"].as<std::string>();
                server = config_file["server"].as<std::string>();
                user = config_file["user"].as<std::string>();
                passwd = config_file["passwd"].as<std::string>();
                port = config_file["port"].as<unsigned>();
            }catch(YAML::Exception e){
                std::cout << e.what() << std::endl;
                exit(EXIT_FAILURE);
            }

        }
        /**
         * \fn bool check_conneciton()
         * \brief Metodo che verifica se il Database e' raggiungibile
         * \return true se il Database e' raggiungibile, false altrimenti
         */
        bool check_conneciton(){
            mysqlpp::Connection connection(false);
            return connection.connect(database.c_str(), server.c_str(), user.c_str(), passwd.c_str(), port);
        }

        /**
         * \fn bool insert_registrazione(unsigned int id, unsigned int state)
         * \brief Inserisce una nuova registrazione nel Database
         * \param[in] id L'id della telecamera che ha effettuato la rilevazione
         * \param[in] state Lo stato della registrazione
         * \return true se l'inserimento e' andato a buon fine, false altrimenti
         */
        bool insert_registrazione(unsigned int id, unsigned int state){
            std::string scorrimento = (state == 0)?"scorrevole":(state == 1)?"traffico":(state == 2)?"traffico intenso":"Error";
            if(scorrimento == "Error") return false;
            mysqlpp::Connection connection(false);
            if(!connection.connect(database.c_str(), server.c_str(), user.c_str(), passwd.c_str(), port)) return false;
            mysqlpp::Query query = connection.query("INSERT INTO registrazione VALUES(CURRENT_TIMESTAMP, \"" + scorrimento + "\", " + std::to_string(id) + ")");
            mysqlpp::StoreQueryResult result = query.store();
            if(query.affected_rows() == 0) return false;
            return true;
        }

        /**
         * \fn bool insert_telecamere(float lat, float lng)
         * \brief Inserische una nuova telecamera nel Database con ID autogenerato
         * \param[in] lat La latitudine della telecamera
         * \param[in] lng La longitudine della telecamera
         * \return true se l'inserimento nel Database e' andato a buon fine, false altrimenti
         */
        bool insert_telecamere(float lat, float lng){
            if(lat > 90 || lat < 0 || lng > 90 || lng < 0) return false;
            mysqlpp::Connection connection(false);
            if(!connection.connect(database.c_str(), server.c_str(), user.c_str(), passwd.c_str(), port)) return false;
            mysqlpp::Query query = connection.query("INSERT INTO telecamere VALUES(NULL, " + std::to_string(lat) + ", " + std::to_string(lng) + ", \"OFF\")");
            mysqlpp::StoreQueryResult result = query.store();
            if(query.affected_rows() == 0) return false;
            return true;
        }

        /**
         * \fn bool insert_telecamere_by_id(int id, float lat, float lng)
         * \brief Inserische una nuova telecamera nel Database
         * \param[in] id L' ID della telecamera
         * \param[in] lat La latitudine della telecamera
         * \param[in] lng La longitudine della telecamera
         * \return true se l'inserimento nel Database e' andato a buon fine, false altrimenti
         */
        bool insert_telecamere_by_id(int id, float lat, float lng){
            if(id < 0 || lat > 90 || lat < 0 || lng > 90 || lng < 0) return false;
            mysqlpp::Connection connection(false);
            if(!connection.connect(database.c_str(), server.c_str(), user.c_str(), passwd.c_str(), port)) return false;
            mysqlpp::Query query = connection.query("INSERT INTO telecamere VALUES(" + std::to_string(id) + ", " + std::to_string(lat) + ", " + std::to_string(lng) + ", \"OFF\")");
            mysqlpp::StoreQueryResult result = query.store();
            if(query.affected_rows() == 0) return false;
            return true;
        }

        /**
         * \fn bool start_telecamera(int id)
         * \brief Aggiorna lo stato della telecamera sul Database a ON
         * \param[in] id L'id della telecamera
         * \return true se l'operazione e' andata a buon fine, false altrimenti
         */
        bool start_telecamera(int id){
            mysqlpp::Connection connection(false);
            if(!connection.connect(database.c_str(), server.c_str(), user.c_str(), passwd.c_str(), port)) return false;
            mysqlpp::Query query = connection.query("UPDATE telecamere SET status = 'ON' WHERE id = " + std::to_string(id));
            mysqlpp::StoreQueryResult result = query.store();
            if(query.affected_rows() == 0) return false;
            return true;
        }

        /**
         * \fn bool start_telecamera(int id)
         * \brief Aggiorna lo stato della telecamera sul Database a OFF
         * \param[in] id L'id della telecamera
         * \return true se l'operazione e' andata a buon fine, false altrimenti
         */
        bool stop_telecamera(int id){
            mysqlpp::Connection connection(false);
            if(!connection.connect(database.c_str(), server.c_str(), user.c_str(), passwd.c_str(), port)) return false;
            mysqlpp::Query query = connection.query("UPDATE telecamere SET status = 'OFF' WHERE id = " + std::to_string(id));
            mysqlpp::StoreQueryResult result = query.store();
            if(query.affected_rows() == 0) return false;
            return true;
        }

        /**
         * \fn int get_cameras_count()
         * \brief Ritorna il numero di telecamere nel Database
         * \return Il numero di telecamere inserite nel Database
         */
        int get_cameras_count(){
            mysqlpp::Connection connection(false);
            if(!connection.connect(database.c_str(), server.c_str(), user.c_str(), passwd.c_str(), port)) return -1;
            mysqlpp::Query query = connection.query("SELECT * FROM telecamere");
            mysqlpp::StoreQueryResult result = query.store();
            return query.affected_rows();
        }
        /**
         * \fn std::vector<int> get_cameras_ids()
         * \brief Funzione che ritona un std::vector contenente gli id delle telecamere presenti nel database
         * \return std::vector contenente gli id delle telecamere
         */
        std::vector<int> get_cameras_ids(){
            std::vector<int> buffer;
            mysqlpp::Connection connection(false);
            if(!connection.connect(database.c_str(), server.c_str(), user.c_str(), passwd.c_str(), port)) return buffer;
            mysqlpp::Query query = connection.query("SELECT * FROM telecamere");
            mysqlpp::StoreQueryResult result = query.store();
            for(auto id : result){
                buffer.push_back((int)id["id"]);
            }
            return buffer;
        }
    };


}

#endif