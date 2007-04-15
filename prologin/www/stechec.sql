-- MySQL dump 9.11
--
-- Host: localhost    Database: stechec
-- ------------------------------------------------------
-- Server version	4.0.24_Debian-2-log

DROP DATABASE IF EXISTS stechec;
CREATE DATABASE stechec;
USE stechec;

--
-- Table structure for table `champion`
--

CREATE TABLE `champion` (
  `id` int(11) NOT NULL auto_increment,
  `id_candidat` int(11) NOT NULL default '0',
  `id_game` int(11) NOT NULL default '0',
  `name` text NOT NULL,
  `comment` text NOT NULL,
  `status` tinyint(4) NOT NULL default '0',
  `level` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

--
-- Table structure for table `cmt`
--

CREATE TABLE `cmt` (
  `id` int(11) NOT NULL auto_increment,
  `id_target` int(11) NOT NULL default '0',
  `id_auteur` int(11) NOT NULL default '0',
  `date` datetime NOT NULL default '0000-00-00 00:00:00',
  `text` text NOT NULL,
  `visibility` tinyint(4) NOT NULL default '8',
  `mark` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `id_target` (`id_target`),
  KEY `id_auteur` (`id_auteur`)
) TYPE=MyISAM;


--
-- Table structure for table `competiteur`
--

CREATE TABLE `competiteur` (
  `id` int(11) NOT NULL auto_increment,
  `id_champion` int(11) NOT NULL default '0',
  `id_match` int(11) NOT NULL default '0',
  `score` int(11) NOT NULL default '0',
  `log` text,
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

--
-- Table structure for table `game`
--

CREATE TABLE `game` (
  `id` int(11) NOT NULL auto_increment,
  `nom` text NOT NULL,
  `comment` text NOT NULL,
  `repertoire` text NOT NULL,
  `lib_name` text NOT NULL,
  `max_champion` int(11) NOT NULL default '0',
  `status` tinyint(4) NOT NULL default '0',
  `arg_server` text NOT NULL,
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

--
-- Table structure for table `machine`
--

CREATE TABLE `machine` (
  `id` int(11) NOT NULL auto_increment,
  `ip` text NOT NULL,
  `port_meta` int(11) NOT NULL default '0',
  `port_server` int(11) NOT NULL default '0',
  `nb_pross` int(11) NOT NULL default '0',
  `status` int(11) NOT NULL default '0',
  `exec_server` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

--
-- Table structure for table `matchs`
--

CREATE TABLE `matchs` (
  `id` int(11) NOT NULL auto_increment,
  `id_game` int(11) NOT NULL default '0',
  `status` tinyint(4) NOT NULL default '0',
  `type` tinyint(4) NOT NULL default '0',
  `opt_match` text NOT NULL,
  `date` datetime default NULL,
  `is_competition` tinyint(4) default NULL,
  `id_createur` int(11) default NULL,
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

--
-- Table structure for table `matchs_opt`
--

CREATE TABLE `matchs_opt` (
  `id` int(11) NOT NULL auto_increment,
  `id_game` int(11) NOT NULL default '0',
  `min` tinyint(4) NOT NULL default '0',
  `max` tinyint(4) NOT NULL default '0',
  `use_competition` tinyint(4) NOT NULL default '0',
  `opt` text NOT NULL,
  `comment` text NOT NULL,
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

--
-- Table structure for table `news`
--

CREATE TABLE `news` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `title` tinytext NOT NULL,
  `body` text NOT NULL,
  `date` datetime NOT NULL default '0000-00-00 00:00:00',
  `author_id` int(10) unsigned NOT NULL default '0',
  `visible` tinyint(2) unsigned NOT NULL default '1',
  PRIMARY KEY  (`id`),
  KEY `author_id` (`author_id`)
) TYPE=MyISAM;

--
-- Table structure for table `page`
--

CREATE TABLE `page` (
  `id_page` int(4) NOT NULL default '0',
  `nom` varchar(32) NOT NULL default '',
  `level` tinyint(4) NOT NULL default '1',
  `file` varchar(32) NOT NULL default '',
  `parent` tinyint(4) NOT NULL default '0',
  `hidden` tinyint(4) NOT NULL default '0',
  `access_count` bigint(11) NOT NULL default '0',
  `access_level` int(11) NOT NULL default '255',
  `sort` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`id_page`),
  UNIQUE KEY `id_page` (`id_page`)
) TYPE=MyISAM;

--
-- Dumping data for table `page`
--

INSERT INTO `page` VALUES (0,'Accueil',0,'accueil.php',0,0,0,511,0);
INSERT INTO `page` VALUES (1,'Informations',0,'',0,0,0,511,0);
INSERT INTO `page` VALUES (3,'Jeux',0,'',0,0,0,511,0);
INSERT INTO `page` VALUES (4,'Meta serveur',0,'',0,0,0,254,0);
INSERT INTO `page` VALUES (5,'Statistiques',0,'',0,0,0,254,0);
INSERT INTO `page` VALUES (11,'Faq',1,'info/faq.php',1,0,0,511,0);
INSERT INTO `page` VALUES (43,'Mes process',1,'perso/tasks.php',4,0,0,255,4);
INSERT INTO `page` VALUES (10,'ReadMe',1,'info/doc.php',1,0,0,511,0);
INSERT INTO `page` VALUES (39,'Classement',1,'project/game_classement.php',3,0,0,255,6);
INSERT INTO `page` VALUES (20,'Candidats',1,'individus/candidat.php',2,0,0,511,0);
INSERT INTO `page` VALUES (21,'Orgas',1,'individus/orgas.php',2,0,0,511,0);
INSERT INTO `page` VALUES (41,'Mon profil',1,'perso/my_profile.php',4,0,0,511,1);
INSERT INTO `page` VALUES (200,'Info individu',1,'perso/profile.php',2,1,0,511,0);
INSERT INTO `page` VALUES (32,'Informations',1,'project/game_info.php',3,0,0,511,1);
INSERT INTO `page` VALUES (35,'Créer',1,'project/game_new.php',3,0,0,194,9);
INSERT INTO `page` VALUES (30,'Liste',1,'project/game_list.php',3,0,0,511,0);
INSERT INTO `page` VALUES (201,'Nouveau commentaire',1,'comment/comment_new.php',2,1,0,254,0);
INSERT INTO `page` VALUES (202,'Info commentaire',1,'comment/comment_view.php',2,1,0,254,0);
INSERT INTO `page` VALUES (25,'Commentaires',1,'comment/comment_list.php',2,0,0,254,0);
INSERT INTO `page` VALUES (141,'Modifier une news',1,'news/news_edit.php',1,1,0,254,0);
INSERT INTO `page` VALUES (140,'Poster une news',1,'news/news_post.php',1,1,0,254,0);
INSERT INTO `page` VALUES (666,'Pretty printer',1,'pretty_printer/pretty_choose.php',3,1,0,255,0);
INSERT INTO `page` VALUES (14,'News',1,'news/news_disp.php',1,0,0,254,0);
INSERT INTO `page` VALUES (36,'Fichiers',1,'project/files.php',3,0,0,511,5);
INSERT INTO `page` VALUES (42,'Mes notes',1,'perso/my_notes.php',4,0,0,1,0);
INSERT INTO `page` VALUES (24,'Trombi',1,'individus/candidat_photo.php',2,0,0,254,0);
INSERT INTO `page` VALUES (38,'Match',1,'project/game_match.php',3,0,0,255,7);
INSERT INTO `page` VALUES (55,'Pages',1,'stats/stats_access.php',5,0,0,511,1);
INSERT INTO `page` VALUES (46,'Mes acces',1,'perso/my_access.php',4,0,0,128,5);
INSERT INTO `page` VALUES (54,'Connexions',1,'stats/stats_connec.php',5,0,0,254,2);
INSERT INTO `page` VALUES (26,'Recherche',1,'individus/search.php',2,0,0,255,0);
INSERT INTO `page` VALUES (300,'Champion',1,'project/game_champion.php',3,0,0,255,8);
INSERT INTO `page` VALUES (241,'Trombi (assistant)',1,'individus/orga_photo.php',2,1,0,254,0);
INSERT INTO `page` VALUES (400,'Status',1,'meta/status.php',4,0,0,254,1);
INSERT INTO `page` VALUES (401,'Machines',1,'project/game_champion.php',4,0,0,254,2);
INSERT INTO `page` VALUES (402,'Matchs',1,'project/game_champion.php',4,0,0,254,3);

--
-- Table structure for table `profil`
--

CREATE TABLE `profil` (
  `id` int(11) NOT NULL default '0',
  `libelle` varchar(128) NOT NULL default '',
  `access` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id_profil` (`id`)
) TYPE=MyISAM;

--
-- Dumping data for table `profil`
--

INSERT INTO `profil` VALUES (1,'Candidat',1);
INSERT INTO `profil` VALUES (2,'Organisateur',2);

--
-- Table structure for table `session`
--

CREATE TABLE `session` (
  `id` int(11) unsigned NOT NULL auto_increment,
  `valid` tinyint(4) NOT NULL default '0',
  `log_host` varchar(128) NOT NULL default '',
  `cookie_data` varchar(32) NOT NULL default '',
  `log_date` datetime NOT NULL default '0000-00-00 00:00:00',
  `log_period` int(11) NOT NULL default '0',
  `uid` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

--
-- Table structure for table `sondage`
--

CREATE TABLE `sondage` (
  `id` int(11) NOT NULL auto_increment,
  `titre` varchar(255) NOT NULL default '',
  `question` text NOT NULL,
  `nb_choix` int(11) NOT NULL default '1',
  `statut` int(11) NOT NULL default '0',
  `votes` int(11) NOT NULL default '0',
  `date` datetime NOT NULL default '0000-00-00 00:00:00',
  `createur` int(11) NOT NULL default '0',
  `is_main` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

--
-- Table structure for table `sondage_props`
--

CREATE TABLE `sondage_props` (
  `id` int(11) NOT NULL auto_increment,
  `sondage_id` int(11) NOT NULL default '0',
  `proposition` varchar(255) NOT NULL default '',
  `answers_max` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `sondage_id` (`sondage_id`)
) TYPE=MyISAM;

--
-- Table structure for table `sondage_votes`
--

CREATE TABLE `sondage_votes` (
  `prop_id` int(11) NOT NULL default '0',
  `voter_id` int(11) NOT NULL default '0',
  `sondage_id` int(11) NOT NULL default '0',
  KEY `prop_id` (`prop_id`),
  KEY `voter_id` (`voter_id`),
  KEY `sondage_id` (`sondage_id`)
) TYPE=MyISAM;

--
-- Table structure for table `style`
--

CREATE TABLE `style` (
  `id` tinyint(4) NOT NULL auto_increment,
  `libelle` varchar(32) NOT NULL default '',
  `file` varchar(32) NOT NULL default '',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

--
-- Dumping data for table `style`
--

INSERT INTO `style` VALUES (1,'Sky','style/sky.css');

--
-- Table structure for table `user`
--

CREATE TABLE `user` (
  `id` int(11) unsigned NOT NULL auto_increment,
  `id_profil` int(11) NOT NULL default '0',
  `id_style` tinyint(4) NOT NULL default '1',
  `nom` varchar(64) NOT NULL default '',
  `prenom` varchar(64) NOT NULL default '',
  `nickname` varchar(16) NOT NULL default '',
  `login` varchar(8) NOT NULL default '',
  `email` text NOT NULL,
  `photo` varchar(64) NOT NULL default 'photo/none.jpg',
  `adresse` varchar(128) NOT NULL default '',
  `phone` varchar(16) NOT NULL default '',
  `gsm` varchar(16) NOT NULL default '',
  `date_naiss` date NOT NULL default '0000-00-00',
  `codep` varchar(5) NOT NULL default '',
  `ville` varchar(24) NOT NULL default '',
  `sexe` tinyint(4) NOT NULL default '0',
  `visit_count` int(11) NOT NULL default '0',
  `passwd` varchar(50) NOT NULL default '',
  `login_count` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `uid` (`id`),
  UNIQUE KEY `login` (`login`),
  UNIQUE KEY `login_2` (`login`),
  KEY `login_3` (`login`)
) TYPE=MyISAM;

--
-- Dumping data for table `user`
--

INSERT INTO `user` VALUES (1,2,1,'Marchand','Joseph','alcooladmin','prolo','prologin@epita.fr','photo/fun/clown.jpg','','','','2005-01-01','','',0,0,'1a1dc91c907325c69271ddf0c944bc72',0);
INSERT INTO `user` VALUES (2,1,1,'At','Candide','Cand','cand','cand@gaim.com','photo/fun/clown.jpg','','','','2005-01-01','','',0,23,'1a1dc91c907325c69271ddf0c944bc72',0);

