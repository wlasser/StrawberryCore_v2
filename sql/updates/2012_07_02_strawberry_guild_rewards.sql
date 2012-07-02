/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50515
Source Host           : localhost:3306
Source Database       : world

Target Server Type    : MYSQL
Target Server Version : 50515
File Encoding         : 65001

Date: 2012-07-02 17:59:48
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `guild_rewards`
-- ----------------------------
DROP TABLE IF EXISTS `guild_rewards`;
CREATE TABLE `guild_rewards` (
  `item_entry` int(11) NOT NULL,
  `price` int(11) NOT NULL,
  `achievement` int(11) NOT NULL,
  `standing` int(11) NOT NULL,
  `races` bigint(15) NOT NULL,
  PRIMARY KEY (`item_entry`),
  KEY `1` (`item_entry`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of guild_rewards
-- ----------------------------
INSERT INTO `guild_rewards` VALUES ('61931', '15000000', '4946', '5', '0');
INSERT INTO `guild_rewards` VALUES ('61935', '15000000', '4946', '5', '0');
INSERT INTO `guild_rewards` VALUES ('61936', '15000000', '4946', '5', '0');
INSERT INTO `guild_rewards` VALUES ('61937', '15000000', '4946', '5', '0');
INSERT INTO `guild_rewards` VALUES ('61942', '15000000', '4946', '5', '0');
INSERT INTO `guild_rewards` VALUES ('61958', '15000000', '4946', '5', '0');
INSERT INTO `guild_rewards` VALUES ('62038', '12000000', '4944', '5', '0');
INSERT INTO `guild_rewards` VALUES ('62039', '12000000', '4944', '5', '0');
INSERT INTO `guild_rewards` VALUES ('62040', '12000000', '4944', '5', '0');
INSERT INTO `guild_rewards` VALUES ('62286', '100000000', '4943', '4', '0');
INSERT INTO `guild_rewards` VALUES ('62287', '200000000', '5158', '6', '946');
INSERT INTO `guild_rewards` VALUES ('62298', '15000000', '4912', '7', '2098253');
INSERT INTO `guild_rewards` VALUES ('62799', '1500000', '5467', '5', '0');
INSERT INTO `guild_rewards` VALUES ('62800', '1500000', '5036', '5', '0');
INSERT INTO `guild_rewards` VALUES ('63125', '30000000', '4988', '7', '0');
INSERT INTO `guild_rewards` VALUES ('63138', '3000000', '5812', '7', '4294967295');
INSERT INTO `guild_rewards` VALUES ('63206', '3000000', '4945', '5', '2098253');
INSERT INTO `guild_rewards` VALUES ('63207', '3000000', '4945', '5', '946');
INSERT INTO `guild_rewards` VALUES ('63352', '1500000', '4989', '5', '2098253');
INSERT INTO `guild_rewards` VALUES ('63353', '1500000', '4989', '5', '946');
INSERT INTO `guild_rewards` VALUES ('63359', '1500000', '4860', '5', '2098253');
INSERT INTO `guild_rewards` VALUES ('63398', '3000000', '5144', '6', '0');
INSERT INTO `guild_rewards` VALUES ('64398', '2000000', '5143', '5', '2098253');
INSERT INTO `guild_rewards` VALUES ('64399', '3000000', '5422', '5', '2098253');
INSERT INTO `guild_rewards` VALUES ('64400', '1500000', '4860', '5', '946');
INSERT INTO `guild_rewards` VALUES ('64401', '2000000', '5143', '5', '946');
INSERT INTO `guild_rewards` VALUES ('64402', '3000000', '5422', '5', '946');
INSERT INTO `guild_rewards` VALUES ('65274', '5000000', '5035', '6', '946');
INSERT INTO `guild_rewards` VALUES ('65360', '5000000', '5035', '6', '2098253');
INSERT INTO `guild_rewards` VALUES ('65361', '3000000', '5031', '5', '2098253');
INSERT INTO `guild_rewards` VALUES ('65362', '3000000', '5179', '5', '946');
INSERT INTO `guild_rewards` VALUES ('65363', '5000000', '5201', '6', '2098253');
INSERT INTO `guild_rewards` VALUES ('65364', '5000000', '5201', '6', '946');
INSERT INTO `guild_rewards` VALUES ('65435', '1500000', '5465', '5', '0');
INSERT INTO `guild_rewards` VALUES ('65498', '1500000', '5024', '5', '0');
INSERT INTO `guild_rewards` VALUES ('67107', '15000000', '5492', '7', '946');
INSERT INTO `guild_rewards` VALUES ('68136', '200000000', '5152', '6', '2098253');
INSERT INTO `guild_rewards` VALUES ('69209', '1250000', '0', '4', '4294967295');
INSERT INTO `guild_rewards` VALUES ('69210', '2500000', '0', '5', '4294967295');
INSERT INTO `guild_rewards` VALUES ('69887', '15000000', '4946', '5', '0');
INSERT INTO `guild_rewards` VALUES ('69892', '12000000', '4944', '5', '0');
INSERT INTO `guild_rewards` VALUES ('71033', '15000000', '5840', '7', '0');