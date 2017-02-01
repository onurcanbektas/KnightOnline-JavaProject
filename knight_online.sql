CREATE DATABASE  IF NOT EXISTS `knight_online` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `knight_online`;
-- MySQL dump 10.13  Distrib 5.7.12, for Win32 (AMD64)
--
-- Host: 127.0.0.1    Database: knight_online
-- ------------------------------------------------------
-- Server version	5.7.17-log

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
-- Table structure for table `account`
--

DROP TABLE IF EXISTS `account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account` (
  `username` varchar(50) NOT NULL,
  `password` varchar(150) NOT NULL,
  `email` varchar(250) NOT NULL,
  `premium_expire_time` timestamp(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
  `premium` tinyint(4) NOT NULL DEFAULT '0',
  `authority` tinyint(4) NOT NULL DEFAULT '1',
  `nation` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`username`),
  KEY `account_nation_idx` (`nation`),
  KEY `account_authority_idx` (`authority`),
  KEY `account_premium_idx` (`premium`),
  CONSTRAINT `account_authority` FOREIGN KEY (`authority`) REFERENCES `authority` (`id`) ON UPDATE CASCADE,
  CONSTRAINT `account_nation` FOREIGN KEY (`nation`) REFERENCES `nation` (`id`) ON UPDATE CASCADE,
  CONSTRAINT `account_premium` FOREIGN KEY (`premium`) REFERENCES `premium` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `account`
--

LOCK TABLES `account` WRITE;
/*!40000 ALTER TABLE `account` DISABLE KEYS */;
INSERT INTO `account` VALUES ('mamaorha','c8837b23ff8aaa8a2dde915473ce0991','mamaorha@gmail.com','2018-01-31 11:44:58.093030',4,5,0);
/*!40000 ALTER TABLE `account` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `account_character`
--

DROP TABLE IF EXISTS `account_character`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account_character` (
  `username` varchar(50) NOT NULL,
  `chararcter_name` varchar(45) NOT NULL,
  PRIMARY KEY (`username`,`chararcter_name`),
  CONSTRAINT `account_char_username` FOREIGN KEY (`username`) REFERENCES `account` (`username`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `account_character`
--

LOCK TABLES `account_character` WRITE;
/*!40000 ALTER TABLE `account_character` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_character` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `application_properties`
--

DROP TABLE IF EXISTS `application_properties`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `application_properties` (
  `key` varchar(50) NOT NULL,
  `value` varchar(500) NOT NULL,
  `notes` varchar(1000) DEFAULT NULL,
  PRIMARY KEY (`key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `application_properties`
--

LOCK TABLES `application_properties` WRITE;
/*!40000 ALTER TABLE `application_properties` DISABLE KEYS */;
INSERT INTO `application_properties` VALUES ('login_packet_handler_thread_pool_size','10',NULL),('login_server_ip','0.0.0.0',NULL),('login_server_msg_timeout','3000',NULL),('login_server_port','15100',NULL),('login_server_recieve_buffer_size','1048576',NULL),('news','Welcome to OpenKO Java server',NULL),('server_version','1298',NULL);
/*!40000 ALTER TABLE `application_properties` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `authority`
--

DROP TABLE IF EXISTS `authority`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `authority` (
  `id` tinyint(4) NOT NULL,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `authority`
--

LOCK TABLES `authority` WRITE;
/*!40000 ALTER TABLE `authority` DISABLE KEYS */;
INSERT INTO `authority` VALUES (1,'user'),(2,'mute'),(3,'ban'),(4,'gm'),(5,'admin');
/*!40000 ALTER TABLE `authority` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `nation`
--

DROP TABLE IF EXISTS `nation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `nation` (
  `id` tinyint(4) NOT NULL,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `nation`
--

LOCK TABLES `nation` WRITE;
/*!40000 ALTER TABLE `nation` DISABLE KEYS */;
INSERT INTO `nation` VALUES (0,'NO_NATION'),(1,'KARUS'),(2,'ELMORAD');
/*!40000 ALTER TABLE `nation` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `online_user`
--

DROP TABLE IF EXISTS `online_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `online_user` (
  `username` varchar(50) NOT NULL,
  `character_name` varchar(45) NOT NULL,
  `server_name` varchar(45) NOT NULL,
  `server_ip` varchar(50) NOT NULL,
  `client_ip` varchar(50) NOT NULL,
  PRIMARY KEY (`username`,`character_name`),
  KEY `online_user_server_ip_idx` (`server_ip`,`server_name`),
  CONSTRAINT `online_user_account_character` FOREIGN KEY (`username`, `character_name`) REFERENCES `account_character` (`username`, `chararcter_name`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `online_user`
--

LOCK TABLES `online_user` WRITE;
/*!40000 ALTER TABLE `online_user` DISABLE KEYS */;
/*!40000 ALTER TABLE `online_user` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `premium`
--

DROP TABLE IF EXISTS `premium`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `premium` (
  `id` tinyint(4) NOT NULL,
  `name` varchar(45) NOT NULL,
  `gold_bonus` tinyint(4) NOT NULL,
  `exp_bonus` tinyint(4) NOT NULL,
  `drop_bonus` tinyint(4) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `premium`
--

LOCK TABLES `premium` WRITE;
/*!40000 ALTER TABLE `premium` DISABLE KEYS */;
INSERT INTO `premium` VALUES (0,'none',0,0,0),(1,'bronze',10,10,5),(2,'silver',20,20,10),(3,'gold',30,30,15),(4,'plat',40,40,20);
/*!40000 ALTER TABLE `premium` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `server_list`
--

DROP TABLE IF EXISTS `server_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `server_list` (
  `server_ip` varchar(50) NOT NULL DEFAULT '127.0.0.1',
  `lan_ip` varchar(50) NOT NULL DEFAULT '127.0.0.1',
  `server_name` varchar(45) NOT NULL DEFAULT 'TEST | Server 1',
  `group_id` int(11) NOT NULL DEFAULT '1',
  `premium_limit` varchar(45) DEFAULT '3000',
  `free_limit` varchar(45) DEFAULT '2000',
  PRIMARY KEY (`server_ip`,`lan_ip`,`server_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `server_list`
--

LOCK TABLES `server_list` WRITE;
/*!40000 ALTER TABLE `server_list` DISABLE KEYS */;
INSERT INTO `server_list` VALUES ('127.0.0.1','127.0.0.1','Server 1',1,'3000','2000'),('127.0.0.1','127.0.0.1','Server 2',1,'3000','2000');
/*!40000 ALTER TABLE `server_list` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-01-31 13:46:31
