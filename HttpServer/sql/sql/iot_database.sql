-- phpMyAdmin SQL Dump
-- version 4.1.7
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Mag 26, 2022 alle 18:13
-- Versione del server: 8.0.26
-- PHP Version: 5.6.40

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `my_sawproject`
--

CREATE SCHEMA IF NOT EXISTS IotDatabase;

USE IotDatabase;

-- --------------------------------------------------------

--
-- Struttura della tabella `registrazione`
--

CREATE TABLE IF NOT EXISTS `registrazione` (
  `data_rec` datetime DEFAULT NULL,
  `scorrimento` enum('scorrevole','traffico','traffico intenso') DEFAULT NULL,
  `telecamera` int NOT NULL,
  KEY `telecamera` (`telecamera`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci AUTO_INCREMENT=1 ;

--
-- Dump dei dati per la tabella `registrazione`
--

INSERT INTO `registrazione` (`data_rec`, `scorrimento`, `telecamera`) VALUES
('2022-05-07 00:00:00', 'scorrevole', 1),
('2022-05-25 00:00:00', 'scorrevole', 10),
('2022-05-25 00:00:00', 'scorrevole', 10),
('2022-05-25 11:30:40', 'traffico', 2),
('2022-05-25 11:30:54', 'traffico', 2),
('2022-05-25 11:31:16', 'traffico', 10),
('2022-05-25 12:06:43', 'traffico', 10);

-- --------------------------------------------------------

--
-- Struttura della tabella `telecamere`
--

CREATE TABLE IF NOT EXISTS `telecamere` (
  `id` int NOT NULL AUTO_INCREMENT,
  `lat` double NOT NULL,
  `lng` double NOT NULL,
  `status` enum('ON','OFF') NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci AUTO_INCREMENT=5 ;

--
-- Dump dei dati per la tabella `telecamere`
--

INSERT INTO `telecamere` (`id`, `lat`, `lng`, `status`) VALUES
(1, 40, 9, 'ON'),
(2, 60, 15, 'OFF'),
(4, 43.8809694, 7.7660597, 'OFF');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
