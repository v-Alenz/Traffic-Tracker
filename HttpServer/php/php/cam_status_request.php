<?php
/**
 * cam_status_request.php
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
    $id = $_GET["id"];
    if(!is_int((int)$id)){
    	die("id non valido");
    }
if(!($stmt_select = mysqli_prepare($connection, "SELECT DISTINCT scorrimento FROM registrazione WHERE telecamera = ? and data_rec = (SELECT MAX(data_rec) FROM registrazione WHERE telecamera = ?)"))){
    	die("<div class = 'error'>statement select non riuscita</div>");
    }
    if(!mysqli_stmt_bind_param($stmt_select, 'ii', $id,$id)){
        die("<div class = 'error'>bind parametri fallito</div>");
    }
    if(!mysqli_stmt_execute($stmt_select)){
         die("<div class = 'error'>esecuzione select fallita</div>");
    }
    if(!($res=mysqli_stmt_get_result($stmt_select))){
    	die("errore");
    }
    $resArray = array();
    $tmpArray = array();
    foreach($res as $row){
    	$tmpArray=$row;
        array_push($resArray,$tmpArray);
    }
    echo json_encode($resArray);
    mysqli_close($connection);
?>