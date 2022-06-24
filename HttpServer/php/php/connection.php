<?php
/**
 * connection.php
 * 
 * Connessione al database
 * 
 * @author Devide Giovanetti
 * @license AGPLv3: Affero General Public License version 3 
 */

/* 
 * Copyright (C) 2022  Davide Giovanetti
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

    $Sname ='iot_database';
    $Uname = 'root';
    $Spassword = 'example';
    $db_name = 'IotDatabase';
    $connection = mysqli_connect($Sname, $Uname, $Spassword, $db_name);
    if(!$connection){
        die("<div class = 'error'>connessione non riuscita <br> <a href=../home_page.php>torna alla home page</a></div>");
    }
?>