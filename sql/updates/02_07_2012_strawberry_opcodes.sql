DELETE FROM `opcodes` WHERE OpcodeValue in (12306, 7600);
INSERT INTO `opcodes` VALUES 
('CMSG_GUILD_REWARD_LIST', '12306', '15595', '15595'),
('SMSG_GUILD_REWARD_LIST', '7600', '15595', '15595');

