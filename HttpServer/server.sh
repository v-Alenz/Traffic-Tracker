# server.sh: Script per automatizzare il Deployment e l'utilizzo sel server IotHttpServer
# Programma scritto durante lo svolgimento del progetto finale per il 
# corso di Multimedia App and Internet of Things dell'Universita' degl Studi di Genova

# Copyright (C) 2022  Andrea Valenzano

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

#!/bin/bash

print_help(){
    printf "\nserver.sh : Script per automatizare il Deployment del server per l'esame di IoT\n\n";
    printf "Uso: server.sh [comando]\n\n";
    printf "     deploy:  installa il server e/o lo avvia nel terminale\n";
    printf "     start:   avvia il server in background\n";
    printf "     stop:    ferma tutti i container del server\n";
    printf "     clean:   elimina i container del server\n";
    printf "     purge:   come clean ma rimuove anche le immagini dei container\n";
    printf "     help:    stampa questa guida\n";
    printf "     version: stampa informazioni sulla versione\n\n";
}

print_version(){
    printf "\nserver.sh (IotHtmlServer) 0.1.dev\n\n";
    printf "Copyright (C) 2022 Andrea Valenzano\n";
    printf "License AGPLv3+: GNU AGPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n";
    printf "This is free software: you are free to change and redistribute it.\n";
    printf "There is NO WARRANTY, to the extent permitted by law.\n\n";
    printf "Scritto da Andrea Valenzano\n\n";
}

check_docker(){
    if ! command -v docker &> /dev/null
    then
        printf "[server.sh]: Fatal Error: unable to find docker on your system.\n\n"
        exit;
    fi
    
    if ! command -v docker-compose &> /dev/null
    then
        printf "[server.sh]: Fatal Error: unable to find docker-compose on your system.\n\n"
        exit;
    fi
}


if [ $# == 0 ] || [ $1 == "help" ]
then
    check_docker && print_help;
    exit;
fi

if [ $1 == "version" ]
then
    print_version;
    exit;
fi

if [ $1 == "deploy" ]
then
    printf "\n[server.sh]: Starting IotHttpServer's containers...\n\n";
    check_docker && docker compose up;
    exit;
fi

if [ $1 == "start" ]
then
    printf "\n[server.sh]: Starting IotHttpServer's containers...\n\n";
    check_docker && docker compose up -d;
    printf "\n";
    exit;
fi

if [ $1 == "stop" ]
then
    printf "\n[server.sh]: Stopping IotHttpServer's containers...\n\n";
    check_docker && docker compose stop;
    printf "\n";
    exit;
fi

if [ $1 == "clean" ]
then
    printf "\n[server.sh]: Removing IotHttpServer's containers...\n\n";
    check_docker && docker rm iot_phpmyadmin iot_http iot_database;
    printf "\n";
    exit;
fi

if [ $1 == "purge" ]
then
    printf "\n[server.sh]: Your heresy will be eradicated!...\n\n";
    check_docker && docker rm iot_phpmyadmin iot_http iot_database; docker image rm iot_php_server:dev iot_mysql_server:dev;
    printf "\n";
    exit;
fi



