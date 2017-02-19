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
INSERT INTO `account` VALUES ('mamaorha','c8837b23ff8aaa8a2dde915473ce0991','mamaorha@gmail.com','2018-01-31 11:44:58.093030',4,5,2);
/*!40000 ALTER TABLE `account` ENABLE KEYS */;
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
INSERT INTO `application_properties` VALUES ('game_packet_handler_thread_pool_size','10',NULL),('game_server_ip','0.0.0.0',NULL),('game_server_msg_timeout','3000',NULL),('game_server_port','15001',NULL),('game_server_recieve_buffer_size','1048576',NULL),('login_packet_handler_thread_pool_size','10',NULL),('login_server_ip','0.0.0.0',NULL),('login_server_msg_timeout','3000',NULL),('login_server_port','15100',NULL),('login_server_recieve_buffer_size','1048576',NULL),('news','Welcome to OpenKO Java server',NULL),('server_version','1298',NULL);
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
-- Table structure for table `character_data`
--

DROP TABLE IF EXISTS `character_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_data` (
  `character_name` varchar(45) NOT NULL,
  `username` varchar(50) NOT NULL,
  `select_character_position` tinyint(4) NOT NULL,
  `race` tinyint(4) NOT NULL,
  `speciality` smallint(6) NOT NULL,
  `hair_color` tinyint(4) NOT NULL,
  `face` tinyint(4) NOT NULL,
  `level` tinyint(4) NOT NULL DEFAULT '1',
  `exp` int(11) NOT NULL DEFAULT '0',
  `loyalty` int(11) NOT NULL DEFAULT '500',
  `loyalty_monthly` int(11) NOT NULL DEFAULT '0',
  `manner_point` int(11) NOT NULL DEFAULT '0',
  `health_point` smallint(6) NOT NULL DEFAULT '100',
  `mana_point` smallint(6) NOT NULL DEFAULT '100',
  `strength` tinyint(4) NOT NULL,
  `stamina` tinyint(4) NOT NULL,
  `dexterity` tinyint(4) NOT NULL,
  `intelligence` tinyint(4) NOT NULL,
  `magic_power` tinyint(4) NOT NULL,
  `character_points` smallint(6) NOT NULL DEFAULT '0',
  `skill_point` smallint(6) NOT NULL DEFAULT '0',
  `gold` int(11) NOT NULL DEFAULT '0',
  `zone` tinyint(4) NOT NULL DEFAULT '1',
  `location_x` int(11) NOT NULL DEFAULT '31200',
  `location_y` int(11) NOT NULL DEFAULT '0',
  `location_z` int(11) NOT NULL DEFAULT '40200',
  `create_time` timestamp(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
  `update_time` timestamp(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) ON UPDATE CURRENT_TIMESTAMP(6),
  PRIMARY KEY (`character_name`),
  KEY `character_data_select_character_position_idx` (`select_character_position`),
  KEY `character_data_username_idx` (`username`),
  KEY `character_data_zone_idx` (`zone`),
  CONSTRAINT `character_data_select_character_position` FOREIGN KEY (`select_character_position`) REFERENCES `select_character_position` (`id`) ON UPDATE CASCADE,
  CONSTRAINT `character_data_username` FOREIGN KEY (`username`) REFERENCES `account` (`username`) ON UPDATE CASCADE,
  CONSTRAINT `character_data_zone` FOREIGN KEY (`zone`) REFERENCES `zone` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `character_data`
--

LOCK TABLES `character_data` WRITE;
/*!40000 ALTER TABLE `character_data` DISABLE KEYS */;
INSERT INTO `character_data` VALUES ('center','mamaorha',1,12,204,0,0,1,0,500,0,0,100,100,70,60,70,50,50,0,0,0,1,31200,0,40200,'2017-02-06 12:36:29.705225','2017-02-06 12:36:29.705225'),('right','mamaorha',2,13,201,0,0,1,0,500,0,0,100,100,60,50,70,70,50,0,0,0,1,31200,0,40200,'2017-02-06 12:37:02.259486','2017-02-06 12:37:02.259486');
/*!40000 ALTER TABLE `character_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `character_inventory`
--

DROP TABLE IF EXISTS `character_inventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_inventory` (
  `character_name` varchar(45) NOT NULL,
  `slot` tinyint(4) NOT NULL,
  `item_id` int(11) NOT NULL,
  `item_count` tinyint(4) NOT NULL DEFAULT '0',
  `item_durability` tinyint(4) NOT NULL DEFAULT '0',
  `item_serial` bigint(8) NOT NULL,
  UNIQUE KEY `item_serial_UNIQUE` (`item_serial`),
  KEY `character_inventory_character_name_idx` (`character_name`),
  CONSTRAINT `character_inventory_character_name` FOREIGN KEY (`character_name`) REFERENCES `character_data` (`character_name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `character_inventory`
--

LOCK TABLES `character_inventory` WRITE;
/*!40000 ALTER TABLE `character_inventory` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_inventory` ENABLE KEYS */;
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
  KEY `online_user_server_ip_idx` (`server_ip`,`server_name`),
  KEY `online_user_account_character_idx` (`character_name`,`username`),
  KEY `online_user_username_idx` (`username`),
  CONSTRAINT `online_user_character_name` FOREIGN KEY (`character_name`) REFERENCES `character_data` (`character_name`) ON UPDATE CASCADE,
  CONSTRAINT `online_user_username` FOREIGN KEY (`username`) REFERENCES `account` (`username`) ON UPDATE CASCADE
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
-- Table structure for table `select_character_position`
--

DROP TABLE IF EXISTS `select_character_position`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `select_character_position` (
  `id` tinyint(4) NOT NULL,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `select_character_position`
--

LOCK TABLES `select_character_position` WRITE;
/*!40000 ALTER TABLE `select_character_position` DISABLE KEYS */;
INSERT INTO `select_character_position` VALUES (0,'LEFT'),(1,'CENTER'),(2,'RIGHT');
/*!40000 ALTER TABLE `select_character_position` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `server_list`
--

DROP TABLE IF EXISTS `server_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `server_list` (
  `server_ip` varchar(50) NOT NULL DEFAULT '127.0.0.1',
  `server_name` varchar(45) NOT NULL DEFAULT 'TEST | Server 1',
  `premium_limit` varchar(45) DEFAULT '3000',
  `free_limit` varchar(45) DEFAULT '2000',
  PRIMARY KEY (`server_ip`),
  UNIQUE KEY `server_name_UNIQUE` (`server_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `server_list`
--

LOCK TABLES `server_list` WRITE;
/*!40000 ALTER TABLE `server_list` DISABLE KEYS */;
INSERT INTO `server_list` VALUES ('192.168.0.102','Server 1','3000','2000');
/*!40000 ALTER TABLE `server_list` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `zone`
--

DROP TABLE IF EXISTS `zone`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zone` (
  `id` tinyint(4) NOT NULL,
  `name` varchar(50) NOT NULL,
  `smd` varchar(50) NOT NULL,
  `init_x` int(11) NOT NULL,
  `init_y` int(11) NOT NULL,
  `init_z` int(11) NOT NULL,
  `type` tinyint(4) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `zone`
--

LOCK TABLES `zone` WRITE;
/*!40000 ALTER TABLE `zone` DISABLE KEYS */;
INSERT INTO `zone` VALUES (1,'Moradon','moradon_0826.smd',31200,0,40200,0);
/*!40000 ALTER TABLE `zone` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'knight_online'
--
/*!50003 DROP PROCEDURE IF EXISTS `ACCOUNT_LOGIN` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ACCOUNT_LOGIN`(
	IN i_username	varchar(50),
	IN i_password	varchar(150),
	OUT o_res		smallint)
root:BEGIN
	DECLARE v_authority smallint;
	DECLARE v_nation smallint;

	SELECT authority, nation INTO v_authority, v_nation FROM ACCOUNT WHERE username = i_username AND `password` = i_password;

	IF (FOUND_ROWS() = 0)
	THEN
		SET o_res = 0;
        LEAVE root;
	END IF;
    
    if(v_authority = 3)
    THEN
		SET o_res = 4;
        LEAVE root;
	END IF;
    
    SET o_res = v_nation + 1;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-02-10 13:35:34
