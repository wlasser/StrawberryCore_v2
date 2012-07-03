/*
Navicat MySQL Data Transfer

Source Server         : local
Source Server Version : 50519
Source Host           : localhost:3306
Source Database       : udb

Target Server Type    : MYSQL
Target Server Version : 50519
File Encoding         : 65001

Date: 2012-07-03 01:18:56
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `opcodes`
-- ----------------------------
DROP TABLE IF EXISTS `opcodes`;
CREATE TABLE `opcodes` (
  `OpcodeName` varchar(255) NOT NULL DEFAULT '',
  `OpcodeValue` mediumint(5) NOT NULL DEFAULT '0',
  `ClientBuild` mediumint(5) NOT NULL DEFAULT '15595',
  `StructureBuild` mediumint(5) NOT NULL,
  PRIMARY KEY (`OpcodeName`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of opcodes
-- ----------------------------
INSERT INTO `opcodes` VALUES ('CMSG_ADD_FRIEND', '25895', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_ADD_IGNORE', '18214', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_ARENA_TEAM_CACHE', '1300', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_ATTACKSTOP', '16646', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_ATTACKSWING', '2342', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_AUTH_SESSION', '1097', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_AUTOEQUIP_ITEM', '17156', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_BUY_TRAINER_SERVICE', '17429', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_CANCEL_AURA', '3622', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_CAST_SPELL', '19463', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_CHAT_MESSAGE_AFK', '3396', '15595', '0');
INSERT INTO `opcodes` VALUES ('CMSG_CHAT_MESSAGE_DND', '10566', '15595', '0');
INSERT INTO `opcodes` VALUES ('CMSG_CHAT_MESSAGE_GUILD', '14678', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_CHAT_MESSAGE_OFFICER', '6470', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_CHAT_MESSAGE_SAY', '4436', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_CHAT_MESSAGE_WHISPER', '3414', '15354', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_CHAT_MESSAGE_YELL', '13636', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_CONTACT_LIST', '17716', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_CREATURE_STATS', '9990', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_DANCE_CACHE', '19975', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_DEL_FRIEND', '27157', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_DEL_IGNORE', '27942', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_DF_GET_SYSTEM_INFO', '1042', '15595', '0');
INSERT INTO `opcodes` VALUES ('CMSG_EMOTE', '19494', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GAME_OBJECT_STATS', '16407', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GET_CALENDER', '10260', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GOSSIP_HELLO', '17701', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GOSSIP_SELECT_OPTION', '534', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GROUP_INVITE', '1299', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GROUP_SET_LEADER', '19479', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_ADD_RANK', '12336', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_BANK_BUY_TAB', '3127', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_CACHE', '17446', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_DEL_RANK', '12852', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_INFO', '12848', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_INFO_TEXT', '12839', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_INVITE', '9392', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_LEAVE', '4129', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_MOTD', '4149', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_ORDER_RANK', '4641', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_RANK', '4132', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_RANKS', '4646', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_REWARDS_LIST', '12306', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_GUILD_ROSTER', '4134', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_INVALIDATE_DANCE', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('CMSG_INVALIDATE_PLAYER', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('CMSG_ITEM_TEXT_CACHE', '9222', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_JOIN_CHANNEL', '342', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_JUMP', '6672', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_LEARN_PREVIEW_TALENTS', '9237', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_LEARN_TALENT', '774', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_LIST_INVENTORY', '10246', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_LOADING_SCREEN_NOTIFY', '9250', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_LOGOUT_CANCEL', '8996', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_LOGOUT_REQUEST', '2597', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_MOVE_TIME_SKIPPED', '31242', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_NAME_CACHE', '8740', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_NPC_CACHE', '20004', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_OPENING_CINEMATIC', '2582', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_PAGE_TEXT_CACHE', '26132', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_PETITION_CACHE', '17444', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_PET_NAME_CACHE', '28452', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_PING', '17485', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_PLAYED_TIME', '2052', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_PLAYER_LOGIN', '1457', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_PLAYER_LOGOUT', '27143', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_QUERY_TIME', '2614', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_QUESTGIVER_ACCEPT_QUEST', '27447', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_QUESTGIVER_CHOOSE_REWARD', '8485', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_QUESTGIVER_COMPLETE_QUEST', '276', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_QUESTGIVER_HELLO', '3351', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_QUESTGIVER_QUERY_QUEST', '12052', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_QUESTGIVER_REQUEST_REWARD', '9524', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_QUESTGIVER_STATUS_MULTIPLE_QUERY', '25349', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_QUESTGIVER_STATUS_QUERY', '17415', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_QUEST_CACHE', '3334', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_QUEST_CONFIRM_ACCEPT', '3349', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_QUEST_NPC_QUERY', '26669', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_QUEST_POI_QUERY', '16439', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_READY_FOR_ACCOUNT_DATA_TIMES', '1282', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_REALM_SPLIT_STATE', '10502', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_REPORT_LAG_SUBMIT', '15597', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_REQUEST_ACCOUNT_DATA', '25861', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_REQUEST_BATTLEFIELD_STATUS', '9472', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_REQUEST_CATEGORY_COOLDOWNS', '28930', '15595', '0');
INSERT INTO `opcodes` VALUES ('CMSG_REQUEST_CHARACTER_CREATE', '18998', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_REQUEST_CHARACTER_DELETE', '25637', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_REQUEST_CHARACTER_ENUM', '11030', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_REQUEST_RAID_INFO', '12070', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_SET_ACTIONBAR_TOGGLES', '9478', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_SET_CONTACT_NOTES', '24885', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_SET_SELECTION', '1286', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_SPIRIT_HEALER_ACTIVATE', '11814', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_START_ASCENSION', '4949', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_START_FALL', '8180', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_START_MOVE_BACKWARD', '13066', '15595', '0');
INSERT INTO `opcodes` VALUES ('CMSG_START_TURN_LEFT', '28684', '15595', '0');
INSERT INTO `opcodes` VALUES ('CMSG_START_TURN_RIGHT', '28672', '15595', '0');
INSERT INTO `opcodes` VALUES ('CMSG_STOP_ASCENSION', '5776', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_STOP_DESCENSION', '5776', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_STOP_FALL', '6001', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_STOP_MOVE', '12810', '15595', '0');
INSERT INTO `opcodes` VALUES ('CMSG_STOP_TURN', '13086', '15595', '0');
INSERT INTO `opcodes` VALUES ('CMSG_SURVEY_INFO_SUBMIT', '13612', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_SWAP_INV_ITEM', '9748', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_SWAP_ITEM', '25382', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_TEXT_EMOTE', '11812', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_TICKET_INFO_CREATE_TICKET', '28324', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_TICKET_INFO_DELETE_TICKET', '9836', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_TICKET_INFO_GET_TICKET', '9068', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_TICKET_INFO_UPDATE_TICKET', '13284', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_TIME_SYNC_RESPONSE', '15116', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_TRAINER_LIST', '9014', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_TUTORIAL_FLAG', '27686', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_UI_TIME_REQUEST', '17925', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_UPDATE_ACCOUNT_DATA', '18230', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_UPDATE_OBJECT_FAILURE', '14344', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_UPDATE_PROGRESS_BAR', '21', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_VALIDATE_TIMER', '29186', '15595', '0');
INSERT INTO `opcodes` VALUES ('CMSG_VIOLENCE_LEVEL', '30742', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_VOICE_SESSION_ENABLE', '8980', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_WHO', '27669', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_WHOIS', '27397', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_WORLD_PORT_RESPONSE', '9985', '15354', '0');
INSERT INTO `opcodes` VALUES ('CMSG_WORLD_TELEPORT', '9394', '15595', '15595');
INSERT INTO `opcodes` VALUES ('CMSG_ZONEUPDATE', '20279', '15595', '15595');
INSERT INTO `opcodes` VALUES ('MSG_MOVE_HEARTBEAT', '14612', '15595', '15595');
INSERT INTO `opcodes` VALUES ('MSG_MOVE_TELEPORT_ACK', '14604', '15595', '15595');
INSERT INTO `opcodes` VALUES ('MSG_QUERY_NEXT_MAIL_TIME', '3844', '15595', '15595');
INSERT INTO `opcodes` VALUES ('MSG_SET_DUNGEON_DIFFICULTY', '18725', '15595', '15595');
INSERT INTO `opcodes` VALUES ('MSG_START_MOVE_FORWARD', '30740', '15595', '15595');
INSERT INTO `opcodes` VALUES ('MSG_TALENT_WIPE_CONFIRM', '263', '15595', '15595');
INSERT INTO `opcodes` VALUES ('MSG_WOW_CONNECTION', '20311', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ACCOUNT_DATA_INITIALIZED', '19205', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ACHIEVEMENT_DELETED', '27158', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_ACHIEVEMENT_EARNED', '17413', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_ACHIEVEMENT_RESPONSE', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_ACTION_BUTTONS', '14517', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ADDON_INFO', '11284', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ALL_ACHIEVEMENT_DATA', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_ALL_GUILD_ACHIEVEMENTS', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_ALL_QUEST_DATA', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_ARENA_TEAM_CACHE', '25398', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ARENA_TEAM_ROSTER', '10007', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_ARENA_TEAM_STATS', '17445', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_ATTACKERSTATEUPDATE', '2853', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ATTACKSTART', '11541', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ATTACKSTOP', '2356', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ATTACKSWING_BADFACING', '27655', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ATTACKSWING_CANT_ATTACK', '22', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ATTACKSWING_DEADTARGET', '11046', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ATTACKSWING_NOTINRANGE', '2870', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_AUCTION_COMMAND_RESULT', '19493', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_AUCTION_HELLO', '8967', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_AUCTION_LIST', '26167', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_AUCTION_REMOVED_NOTIFICATION', '9012', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_AURA_UPDATE', '18183', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_AURA_UPDATE_ALL', '26902', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_AUTH_CHALLENGE', '17730', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_AUTH_RESPONSE', '23990', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_BARBER_SHOP_RESULT', '24869', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BATTLEFIELD_LIST', '29109', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BATTLEFIELD_PLAYER_JOINED', '22950', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BATTLEFIELD_PLAYER_LEFT', '20656', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BATTLEFIELD_PLAYER_POSITIONS', '22708', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BATTLEFIELD_PORT_DENIED', '23712', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BATTLEFIELD_STATUS_ACTIVE', '22944', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BATTLEFIELD_STATUS_FAILED', '30114', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BATTLEFIELD_STATUS_NEED_CONFIRMATION', '29860', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BATTLEFIELD_STATUS_NONE', '32161', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BATTLEFIELD_STATUS_QUEUED', '13729', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BATTLEFIELD_STATUS_WAIT_FOR_GROUPS', '29095', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BF_MGR_EJECTED', '13474', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BF_MGR_EJECT_PENDING', '32183', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BF_MGR_ENTERING', '13731', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BF_MGR_ENTRY_INVITE', '13748', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BF_MGR_QUEUE_INVITE', '5542', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BF_MGR_QUEUE_REQUEST_RESPONSE', '31158', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BF_MGR_STATE_CHANGED', '13491', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BIDDER_AUCTION_LIST', '39', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_BIDDER_NOTIFICATION', '20007', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_CHANGE_PLAYER_DIFFICULTY_RESULT', '8727', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_CHANNEL_LIST', '8724', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_CHANNEL_MEMBER_COUNT', '25620', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_CHANNEL_NOTIFY', '2085', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_CHAR_DELETE', '772', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_CHAT_DOWN', '17959', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_CHAT_IS_DOWN', '27956', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_CHAT_RECONNECT', '26885', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_CHECK_RELOAD_SPAM_FILTER', '19766', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_CHECK_WARGAME_ENTRY', '15780', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_CLEAR_TARGET', '19238', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_CLIENTCACHE_VERSION', '10036', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_COMBAT_EVENT_FAILED', '11015', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_COMMENTATOR_PARTY_INFO', '14512', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_COMPRESSED_UPDATE_OBJECT', '7363', '15050', '15050');
INSERT INTO `opcodes` VALUES ('SMSG_COMSAT_CONNECT_DISCONNECT', '790', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_COMSAT_CONNECT_FAIL', '25367', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_COMSAT_RECONNECT_ATTEMPT', '19765', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_CONTACT_LIST', '24599', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_CREATURE_STATS', '24612', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_CRITERIA_DELETED', '10517', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_CRITERIA_UPDATE', '28215', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_DANCE_CACHE', '12038', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_DB_REPLY', '14500', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_DEFENSE_MESSAGE', '788', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_DESTROY_OBJECT', '18212', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_DESTRUCTIBLE_BUILDING_DAMAGE', '18469', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_DISPEL_FAILED', '775', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_DUEL_COMPLETE', '9511', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_DUEL_COUNTDOWN', '18486', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_DUEL_IN_BOUNDS', '2599', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_DUEL_OUT_OF_BOUNDS', '3110', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_DUEL_REQUESTED', '17668', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_DUEL_WINNER', '11574', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_DURABILITY_DAMAGE_DEATH', '19495', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_ECHO_SQUELCH', '19718', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_EMOTE', '2612', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ENABLE_BARBER_SHOP', '11542', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_ENCHANTMENTLOG', '24629', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ENVIRONMENTALDAMAGELOG', '27653', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_EQUIPMENT_SET', '11780', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_EQUIPMENT_SET_ID', '8726', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_EQUIPMENT_SET_RESULT', '8726', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_EVENT_LIST', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_EVENT_REMOVED_ALERT', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_EVENT_UPDATED_ALERT', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_FEATURE_SYSTEM_STATUS', '15799', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_FLIGHT_SPLINE_SYNC', '2340', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_FRIEND_STATUS', '1815', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GAMESPEED_SET', '20020', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GAMETIME_SET', '20', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GAMETIME_UPDATE', '16679', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GAME_OBJECT_STATS', '2325', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GET_EVENT', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_GET_LEARNED_DANCE_MOVES', '3589', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GM_MESSAGE_CHAT', '25652', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GOSSIP_COMPLETE', '2054', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GOSSIP_MESSAGE', '8245', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GOSSIP_POI', '17174', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GROUP_LIST', '19492', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GROUP_SET_LEADER', '1318', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_ACHIEVEMENT_DELETED', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_ACHIEVEMENT_EARNED', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_ACHIEVEMENT_MEMBERS', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_BANK_LIST', '30885', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_BANK_LOG', '12466', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_BANK_MONEY_WITHDRAWN', '23988', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_BANK_TEXT', '30115', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_CACHE', '3590', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_CRITERIA_DELETED', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_CRITERIA_UPDATE', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_EVENT', '1797', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_RANKS', '12468', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_REWARDS_LIST', '7600', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_GUILD_ROSTER', '15779', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_INITIALIZE_FACTIONS', '17972', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INITIAL_INVITE_LIST', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INITIAL_SPELLS', '260', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_INIT_WORLD_STATES', '27444', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_INSPECT_ARENA_TEAMS', '9988', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INSPECT_HONOR_STATS', '31141', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INSPECT_RATED_BG_STATS', '6565', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INSTANCE_DIFFICULTY', '0', '0', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_INSTANCE_RESET', '28421', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INSTANCE_RESET_FAILED', '18213', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INSTANCE_SAVE_RESULT', '292', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INVALIDATE_DANCE', '3623', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_INVALIDATE_PLAYER', '25381', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_INVITE', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INVITE_ALERT', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INVITE_MODERATOR_STATUS_ALERT', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INVITE_NOTES', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INVITE_NOTES_ALERT', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INVITE_REMOVED', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INVITE_REMOVED_ALERT', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INVITE_STATUS', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_INVITE_STATUS_ALERT', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_ITEM_TEXT_CACHE', '10021', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_LEARNED_SPELL', '22690', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_BOOT_PLAYER', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_DISABLED', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_JOIN_RESULT', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_LIST', '2069', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_OFFER_CONTINUE', '27431', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_PARTY_INFO', '8997', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_PLAYER_REWARD', '26676', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_PROPOSAL_UPDATE', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_QUEUE_STATUS', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_ROLE_CHECK_UPDATE', '822', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_TELEPORT_DENIED', '3604', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_UPDATE_PARTY', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_UPDATE_PLAYER', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LFG_UPDATE_SEARCH', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LF_GUILD_APPLICANT_LIST_CHANGED', '4260', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LF_GUILD_APPLICATIONS', '7333', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LF_GUILD_APPLICATIONS_LIST_CHANGED', '29093', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LF_GUILD_BROWSE', '7587', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LF_GUILD_COMMAND_RESULT', '21670', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LF_GUILD_POST', '13751', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LF_GUILD_RECRUITS', '7346', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LIST_INVENTORY', '31920', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_LOCKOUT_ADDED', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LOCKOUT_REMOVED', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LOCKOUT_UPDATED', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LOGIN_SETTIMESPEED', '19733', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_LOGIN_VERIFY_WORLD', '8197', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_LOGOUT_CANCEL', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_LOGOUT_COMPLETE', '8503', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_LOGOUT_RESPONSE', '1316', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_LOG_XP_GAIN', '17684', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_MAIL_COMMAND_RESULT', '18727', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_MAIL_LIST_RESULT', '16919', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_MESSAGE_CHAT', '8230', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_MINIGAME_SETUP', '26407', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_MINIGAME_STATE', '11799', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_MONSTER_MOVE', '28183', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_MOTD', '2613', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_MSG_LFG_PLAYER_INFO', '19254', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_NAME_CACHE', '28164', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_NEW_WORLD', '31153', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_NOTIFY_DANCE', '18692', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_NPC_CACHE', '17462', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_OFFER_PETITION_ERROR', '10006', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_ON_RESET_FAILED_NOTIFY', '17942', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_ON_TITLE_EARNED', '9254', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_OPEN_FROM_GOSSIP', '15541', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_OWNER_AUCTION_LIST', '27700', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_OWNER_NOTIFICATION', '16662', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_OWNER_PENDING_SALES_LIST', '27175', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PAGE_TEXT_CACHE', '11028', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_PARTYKILLLOG', '18743', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_PARTY_COMMAND_RESULT', '28167', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_PERIODICAURALOG', '1046', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_PETITION_CACHE', '19255', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_PET_ACTION_FEEDBACK', '2055', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PET_ADDED', '15525', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PET_BROKEN', '11815', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PET_GUIDS', '11558', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PET_MODE', '8757', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PET_NAME_CACHE', '19511', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_PET_RENAMEABLE', '11047', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PET_SLOT_UPDATED', '20899', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PET_SPELLS', '16660', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PET_SPELL_CHANGES', '1287', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PET_SPELL_CHANGES1', '27140', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PET_UPDATE_COMBO_POINTS', '17189', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PHASE_SHIFT_CHANGE', '28832', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_PLAYED_TIME', '24631', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_PLAYER_MOVE', '31138', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_PLAYER_SQUELCHED', '19718', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PLAY_DANCE', '18180', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_PONG', '19778', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_POSTMASTER_ACTIVATE', '9508', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_POWER_UPDATE', '18951', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_PROCRESIST', '1062', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_PVP_LOG_DATA', '21667', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_PVP_OPTIONS_ENABLED', '23972', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_QUESTGIVER_OFFER_REWARD', '9255', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_QUESTGIVER_QUEST_COMPLETE', '21924', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_QUESTGIVER_QUEST_DETAILS', '9253', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_QUESTGIVER_QUEST_FAILED', '16950', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_QUESTGIVER_QUEST_INVALID', '16406', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_QUESTGIVER_QUEST_LIST', '308', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_QUESTGIVER_REQUEST_ITEMS', '25142', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_QUESTGIVER_STATUS', '8469', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_QUESTGIVER_STATUS_MULTIPLE', '20261', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_QUESTLOG_FULL', '3638', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_QUEST_CACHE', '26934', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_QUEST_CONFIRM_ACCEPT', '28423', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_RAID_INSTANCE_INFO', '26150', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_RAID_INSTANCE_MESSAGE', '28181', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_RAID_TARGET_UPDATE', '11318', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_RATED_BATTLEFIELD_INFO', '23730', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_RATED_BG_STATS', '13473', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_READY_CHECK', '8964', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_READY_CHECK_CONFIRM', '20229', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_READY_CHECK_ERROR', '9735', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_READY_CHECK_FINISHED', '11797', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_REALM_SPLIT_MSG', '10004', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_RECEIVED_MAIL', '10532', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_RECEIVE_MAP_INFO', '807', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_RECEIVE_PLAYER_INFO', '12086', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_RESEARCH_COMPLETE', '13734', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_RESET_WEEKLY_CURRENCY', '15521', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_RESPONSE_CHARACTER_CREATE', '11525', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_RESPONSE_CHARACTER_ENUM', '4272', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ROLE_CHOSEN', '0', '0', '0');
INSERT INTO `opcodes` VALUES ('SMSG_RWHOIS', '9271', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_SAVE_BIND_POINT', '1319', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_SEND_UNLEARN_SPELLS', '20005', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_SERVERTIME', '4684', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_SERVER_FIRST_ACHIEVEMENT', '25636', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_SERVER_MESSAGE', '27652', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_SETUP_CURRENCY', '5541', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_SETUP_RESEARCH_HISTORY', '4278', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_SET_CURRENCY', '22960', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_SET_FACTION_AT_WAR', '16918', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_SET_FACTION_VISIBLE', '9509', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_SET_FACTION_VISIBLE1', '26423', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_SET_FATION_STANDING', '294', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_SET_FORCED_REACTIONS', '17941', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_SET_MAX_WEEKLY_QUANTITY', '28839', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_SKIRMISH_PLAYERS_IN_QUEUE', '8486', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_SKIRMISH_PLAYERS_MODE_STATE', '26644', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_SPELL_GO', '28182', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_SPELL_START', '25621', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_STABLE_COMMAND_RESULT', '8708', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_STABLE_LIST', '2100', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_START_CINEMATIC', '27687', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_STATE_CHANGED', '1847', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_STOP_DANCE', '17975', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_STREAMING_MOVIES', '5559', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_TALENT_UPDATE', '28454', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_TEXT_EMOTE', '2821', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_TIME_SYNC_REQ', '15524', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_TRADE_STATUS', '23715', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_TRADE_UPDATED', '28834', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_TRAINER_BUY_FAILED', '4', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_TRAINER_BUY_SUCCEEDED', '7141', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_TRAINER_LIST', '17428', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_TUTORIAL_FLAGS', '2869', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_UI_TIME', '18964', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_UPDATE_ACCOUNT_DATA', '26679', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_UPDATE_ACCOUNT_DATA_COMPLETE', '8213', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_UPDATE_INSTANCE', '1079', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_UPDATE_INSTANCE_OWNERSHIP', '18709', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_UPDATE_OBJECT', '18197', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_USER_LIST_ADDED', '3895', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_USER_LIST_REMOVE', '8198', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_USER_LIST_UPDATE', '309', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_VOICESESSION_FULL', '25125', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_VOICE_STATUS_CHANGED', '3861', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_WARDEN_DATA', '12704', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_WARGAME_REQUEST_SUCCESSFULLY_SENT_TO_OPPONENT', '22962', '15595', '0');
INSERT INTO `opcodes` VALUES ('SMSG_WHO', '26887', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_WHOIS', '26903', '15595', '15595');
INSERT INTO `opcodes` VALUES ('SMSG_ZONE_UNDER_ATTACK', '2566', '15595', '15595');
