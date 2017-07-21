-- MySQL dump 10.13  Distrib 5.7.17, for Linux (x86_64)
--
-- Host: localhost    Database: mcal
-- ------------------------------------------------------
-- Server version	5.7.17

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `grb`
--

DROP TABLE IF EXISTS `grb`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `grb` (
  `id_grb` int(11) NOT NULL AUTO_INCREMENT,
  `trigger_number` varchar(255) NOT NULL,
  `t_zero` double NOT NULL,
  `s_on_n` double NOT NULL,
  `background` double NOT NULL,
  `threshold` double NOT NULL,
  `counts` int(11) NOT NULL,
  `id_orbit` int(11) NOT NULL,
  PRIMARY KEY (`id_grb`),
  UNIQUE KEY `t_zero` (`t_zero`),
  KEY `id_orbit` (`id_orbit`),
  CONSTRAINT `grb_ibfk_1` FOREIGN KEY (`id_orbit`) REFERENCES `orbit` (`id_orbit`)
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lc_grb`
--

DROP TABLE IF EXISTS `lc_grb`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lc_grb` (
  `id_lc_grb` int(11) NOT NULL AUTO_INCREMENT,
  `file_name` varchar(255) NOT NULL,
  `id_grb` int(11) NOT NULL,
  PRIMARY KEY (`id_lc_grb`),
  KEY `id_grb` (`id_grb`),
  CONSTRAINT `lc_grb_ibfk_1` FOREIGN KEY (`id_grb`) REFERENCES `grb` (`id_grb`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lc_ste`
--

DROP TABLE IF EXISTS `lc_ste`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lc_ste` (
  `id_lc_ste` int(11) NOT NULL AUTO_INCREMENT,
  `file_name` varchar(255) NOT NULL,
  `id_ste` int(11) NOT NULL,
  PRIMARY KEY (`id_lc_ste`)
) ENGINE=InnoDB AUTO_INCREMENT=133 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lc_tgf`
--

DROP TABLE IF EXISTS `lc_tgf`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lc_tgf` (
  `id_lc_tgf` int(11) NOT NULL AUTO_INCREMENT,
  `file_name` varchar(255) NOT NULL,
  `id_tgf` int(11) NOT NULL,
  PRIMARY KEY (`id_lc_tgf`),
  KEY `id_tgf` (`id_tgf`),
  CONSTRAINT `lc_tgf_ibfk_1` FOREIGN KEY (`id_tgf`) REFERENCES `tgf` (`id_tgf`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `orbit`
--

DROP TABLE IF EXISTS `orbit`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `orbit` (
  `id_orbit` int(11) NOT NULL AUTO_INCREMENT,
  `orbit_number` varchar(255) NOT NULL,
  `trend` varchar(255) NOT NULL,
  PRIMARY KEY (`id_orbit`),
  UNIQUE KEY `number` (`orbit_number`)
) ENGINE=InnoDB AUTO_INCREMENT=41 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `ste`
--

DROP TABLE IF EXISTS `ste`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ste` (
  `id_ste` int(11) NOT NULL AUTO_INCREMENT,
  `trigger_number` varchar(255) NOT NULL,
  `bin` double NOT NULL,
  `s_on_n` double NOT NULL,
  `t_zero` double NOT NULL,
  `background` double NOT NULL,
  `threshold` double NOT NULL,
  `counts` int(11) NOT NULL,
  `id_orbit` int(11) NOT NULL,
  PRIMARY KEY (`id_ste`),
  UNIQUE KEY `trigger_number` (`trigger_number`,`t_zero`),
  KEY `id_orbit` (`id_orbit`),
  KEY `id_orbit_2` (`id_orbit`),
  CONSTRAINT `ste_ibfk_1` FOREIGN KEY (`id_orbit`) REFERENCES `orbit` (`id_orbit`)
) ENGINE=InnoDB AUTO_INCREMENT=135 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tgf`
--

DROP TABLE IF EXISTS `tgf`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tgf` (
  `id_tgf` int(11) NOT NULL AUTO_INCREMENT,
  `date` date NOT NULL,
  `time` time NOT NULL,
  `t_zero` double NOT NULL,
  `counts` int(11) NOT NULL,
  `hr` double NOT NULL,
  `duration` double NOT NULL,
  `e_max` double NOT NULL,
  `e_avg` double NOT NULL,
  `lon` double NOT NULL,
  `lat` double NOT NULL,
  `id_orbit` int(11) NOT NULL,
  PRIMARY KEY (`id_tgf`),
  UNIQUE KEY `t_zero` (`t_zero`),
  KEY `id_orbit` (`id_orbit`),
  CONSTRAINT `tgf_ibfk_1` FOREIGN KEY (`id_orbit`) REFERENCES `orbit` (`id_orbit`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-07-21 13:44:47
