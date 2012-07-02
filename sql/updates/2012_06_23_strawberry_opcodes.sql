DELETE FROM `opcodes` WHERE OpcodeValue IN (26887,27669,3622);
INSERT INTO `opcodes` (`OpcodeName`, `OpcodeValue`, `ClientBuild`, `StructureBuild`) VALUES 
('SMSG_WHO', 26887, 15595, 15595),
('CMSG_WHO', 27669, 15595, 15595),
('CMSG_CANCEL_AURA', 3622, 15595, 15595);