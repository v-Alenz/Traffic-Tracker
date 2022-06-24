<?php
/**
 * cam_request.php
 * 
 * Genera un json a partire dai dati sul database
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


	require_once("connection.php");
	$sql = "SELECT * FROM telecamere";
    
    if($res= mysqli_query($connection,$sql)){
    	$resArray = array();
        $tmpArray = array();
        foreach($res as $row){
        	$tmpArray=$row;
            array_push($resArray,$tmpArray);
        }
  
        echo json_encode($resArray);
    }
    mysqli_close($connection);
?>