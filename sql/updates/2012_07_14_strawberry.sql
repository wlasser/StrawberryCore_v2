

DELETE FROM strawberry_string WHERE entry=817;
INSERT INTO strawberry_string VALUES (817,'Warning: You\'ve entered a no-fly zone and are about to be dismounted!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

DROP TABLE IF EXISTS `conditions`;
CREATE TABLE `conditions` (
  `condition_entry` mediumint(8) unsigned NOT NULL auto_increment COMMENT 'Identifier',
  `type` tinyint(3) signed NOT NULL DEFAULT '0' COMMENT 'Type of the condition',
  `value1` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'data field one for the condition',
  `value2` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'data field two for the condition',
  PRIMARY KEY  (`condition_entry`),
  CONSTRAINT unique_conditions UNIQUE (type,value1,value2)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Condition System';

ALTER TABLE gossip_menu_option ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER cond_3_val_2;
ALTER TABLE gossip_menu ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER cond_2_val_2;

ALTER TABLE reference_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE creature_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE gameobject_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE pickpocketing_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE item_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE fishing_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE skinning_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE disenchant_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE mail_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE prospecting_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE spell_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE milling_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;

-- convert *_loot_template conditions -- Note to DB-devs If you create a dump for the new system in an updatepack, these queries are not required
INSERT IGNORE INTO conditions (type, value1, value2) SELECT lootcondition, condition_value1, condition_value2 FROM reference_loot_template WHERE lootcondition>0;
INSERT IGNORE INTO conditions (type, value1, value2) SELECT lootcondition, condition_value1, condition_value2 FROM creature_loot_template WHERE lootcondition>0;
INSERT IGNORE INTO conditions (type, value1, value2) SELECT lootcondition, condition_value1, condition_value2 FROM gameobject_loot_template WHERE lootcondition>0;
INSERT IGNORE INTO conditions (type, value1, value2) SELECT lootcondition, condition_value1, condition_value2 FROM pickpocketing_loot_template WHERE lootcondition>0;
INSERT IGNORE INTO conditions (type, value1, value2) SELECT lootcondition, condition_value1, condition_value2 FROM item_loot_template WHERE lootcondition>0;
INSERT IGNORE INTO conditions (type, value1, value2) SELECT lootcondition, condition_value1, condition_value2 FROM fishing_loot_template WHERE lootcondition>0;
INSERT IGNORE INTO conditions (type, value1, value2) SELECT lootcondition, condition_value1, condition_value2 FROM skinning_loot_template WHERE lootcondition>0;
INSERT IGNORE INTO conditions (type, value1, value2) SELECT lootcondition, condition_value1, condition_value2 FROM disenchant_loot_template WHERE lootcondition>0;
INSERT IGNORE INTO conditions (type, value1, value2) SELECT lootcondition, condition_value1, condition_value2 FROM mail_loot_template WHERE lootcondition>0;
INSERT IGNORE INTO conditions (type, value1, value2) SELECT lootcondition, condition_value1, condition_value2 FROM prospecting_loot_template WHERE lootcondition>0;
INSERT IGNORE INTO conditions (type, value1, value2) SELECT lootcondition, condition_value1, condition_value2 FROM spell_loot_template WHERE lootcondition>0;
INSERT IGNORE INTO conditions (type, value1, value2) SELECT lootcondition, condition_value1, condition_value2 FROM milling_loot_template WHERE lootcondition>0;

UPDATE reference_loot_template SET condition_id=(SELECT condition_entry FROM conditions WHERE type=lootcondition AND value1=condition_value1 AND value2=condition_value2 LIMIT 1) WHERE lootcondition>0;
UPDATE creature_loot_template SET condition_id=(SELECT condition_entry FROM conditions WHERE type=lootcondition AND value1=condition_value1 AND value2=condition_value2 LIMIT 1) WHERE lootcondition>0;
UPDATE gameobject_loot_template SET condition_id=(SELECT condition_entry FROM conditions WHERE type=lootcondition AND value1=condition_value1 AND value2=condition_value2 LIMIT 1) WHERE lootcondition>0;
UPDATE pickpocketing_loot_template SET condition_id=(SELECT condition_entry FROM conditions WHERE type=lootcondition AND value1=condition_value1 AND value2=condition_value2 LIMIT 1) WHERE lootcondition>0;
UPDATE item_loot_template SET condition_id=(SELECT condition_entry FROM conditions WHERE type=lootcondition AND value1=condition_value1 AND value2=condition_value2 LIMIT 1) WHERE lootcondition>0;
UPDATE fishing_loot_template SET condition_id=(SELECT condition_entry FROM conditions WHERE type=lootcondition AND value1=condition_value1 AND value2=condition_value2 LIMIT 1) WHERE lootcondition>0;
UPDATE skinning_loot_template SET condition_id=(SELECT condition_entry FROM conditions WHERE type=lootcondition AND value1=condition_value1 AND value2=condition_value2 LIMIT 1) WHERE lootcondition>0;
UPDATE disenchant_loot_template SET condition_id=(SELECT condition_entry FROM conditions WHERE type=lootcondition AND value1=condition_value1 AND value2=condition_value2 LIMIT 1) WHERE lootcondition>0;
UPDATE mail_loot_template SET condition_id=(SELECT condition_entry FROM conditions WHERE type=lootcondition AND value1=condition_value1 AND value2=condition_value2 LIMIT 1) WHERE lootcondition>0;
UPDATE prospecting_loot_template SET condition_id=(SELECT condition_entry FROM conditions WHERE type=lootcondition AND value1=condition_value1 AND value2=condition_value2 LIMIT 1) WHERE lootcondition>0;
UPDATE spell_loot_template SET condition_id=(SELECT condition_entry FROM conditions WHERE type=lootcondition AND value1=condition_value1 AND value2=condition_value2 LIMIT 1) WHERE lootcondition>0;
UPDATE milling_loot_template SET condition_id=(SELECT condition_entry FROM conditions WHERE type=lootcondition AND value1=condition_value1 AND value2=condition_value2 LIMIT 1) WHERE lootcondition>0;

ALTER TABLE creature_linking_template ADD COLUMN search_range MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0'  AFTER flag;

--
-- Table structure for table `gameobject_template_scripts`
--

DROP TABLE IF EXISTS `gameobject_template_scripts`;
CREATE TABLE `gameobject_template_scripts` (
  `id` mediumint(8) unsigned NOT NULL default '0',
  `delay` int(10) unsigned NOT NULL default '0',
  `command` mediumint(8) unsigned NOT NULL default '0',
  `datalong` mediumint(8) unsigned NOT NULL default '0',
  `datalong2` int(10) unsigned NOT NULL default '0',
  `buddy_entry` int(10) unsigned NOT NULL default '0',
  `search_radius` int(10) unsigned NOT NULL default '0',
  `data_flags` tinyint(3) unsigned NOT NULL default '0',
  `dataint` int(11) NOT NULL default '0',
  `dataint2` int(11) NOT NULL default '0',
  `dataint3` int(11) NOT NULL default '0',
  `dataint4` int(11) NOT NULL default '0',
  `x` float NOT NULL default '0',
  `y` float NOT NULL default '0',
  `z` float NOT NULL default '0',
  `o` float NOT NULL default '0',
  `comments` varchar(255) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `gameobject_template_scripts`
--

LOCK TABLES `gameobject_template_scripts` WRITE;
/*!40000 ALTER TABLE `gameobject_template_scripts` DISABLE KEYS */;
/*!40000 ALTER TABLE `gameobject_template_scripts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `creature_linking`
--

DROP TABLE IF EXISTS creature_linking;
CREATE TABLE `creature_linking` (
  `guid` int(10) UNSIGNED NOT NULL COMMENT 'creature.guid of the slave mob that is linked',
  `master_guid` int(10) UNSIGNED NOT NULL COMMENT 'master to trigger events',
  `flag` mediumint(8) UNSIGNED NOT NULL COMMENT 'flag - describing what should happen when',
  PRIMARY KEY  (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Creature Linking System';

DROP TABLE IF EXISTS `spell_template`;
CREATE TABLE `spell_template` (
  `id` int(11) unsigned NOT NULL DEFAULT '0',
  `proc_flags` int(11) unsigned NOT NULL DEFAULT '0',
  `proc_chance` int(11) unsigned NOT NULL DEFAULT '0',
  `duration_index` int(11) unsigned NOT NULL DEFAULT '0',
  `effect0` int(11) unsigned NOT NULL DEFAULT '0',
  `effect0_implicit_target_a` int(11) unsigned NOT NULL DEFAULT '0',
  `effect0_radius_idx` int(11) unsigned NOT NULL DEFAULT '0',
  `effect0_apply_aura_name` int(11) unsigned NOT NULL DEFAULT '0',
  `effect0_misc_value` int(11) unsigned NOT NULL DEFAULT '0',
  `effect0_trigger_spell` int(11) unsigned NOT NULL DEFAULT '0',
  `comments` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='MaNGOS server side spells';

INSERT INTO spell_template VALUES
-- ID   proc_flags chnce dur  ef0 tarA0 rad  aur  misc    trigger
(21387, 0x00000028,  15,  21,   6,   1,   0,  42, 0,      21388, 'Melt-Weapon trigger aura related used by Ragnaros'),
(62388, 0x00000000, 101,  21,   6,   1,   0,   4, 0,      0,     'Aura required for Demonic Circle 48020');

DELETE FROM spell_template WHERE id IN (23363, 25192);
INSERT INTO spell_template VALUES
(23363, 0x00000000, 101,  21,  76,  18,   0,   0, 179804, 0,     'Summon Drakonid Corpse Trigger'),
(25192, 0x00000000, 101,  21,  76,  18,   0,   0, 180619, 0,     'Summon Ossirian Crystal');


ALTER TABLE creature_movement_scripts ADD COLUMN temp MEDIUMINT(8) DEFAULT 0 AFTER command;

-- Move datalong4 -> 2, 3 -> 4, 2 -> 3 (right shift)
UPDATE creature_movement_scripts SET temp=datalong4 WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);
UPDATE creature_movement_scripts SET datalong4=datalong3, datalong3=datalong2, datalong2=temp WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);

ALTER TABLE creature_movement_scripts CHANGE COLUMN datalong3 buddy_entry MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE creature_movement_scripts CHANGE COLUMN datalong4 search_radius MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';

UPDATE creature_movement_scripts SET temp=0;

/* Chat*/
UPDATE creature_movement_scripts SET temp=temp | 0x04 WHERE command=0 AND (data_flags & 0x02) > 0; -- target self
UPDATE creature_movement_scripts SET temp=temp | 0x01 WHERE command=0 AND (data_flags & 0x04) > 0; -- buddy as target
UPDATE creature_movement_scripts SET temp=temp | 0x10 WHERE command=0 AND (data_flags & 0x01) > 0; -- will produce error
-- Note that old flag 0x01 // flag_target_player_as_source     0x01 could not be converted automatically, need to check every script

/* Emote*/
UPDATE creature_movement_scripts SET temp=temp | 0x02 WHERE command=1 AND (data_flags & 0x01 > 0) AND buddy_entry = 0; -- reverse order if no buddy defined
/*Summon */
UPDATE creature_movement_scripts SET temp=temp | 0x08 WHERE command=10 AND (data_flags & 0x01 > 0); -- Summon as active
/* Remove Aura */
UPDATE creature_movement_scripts SET temp=0x02 WHERE command=14 AND datalong2=1;
UPDATE creature_movement_scripts SET datalong2=0 WHERE command=14;
/* Cast */
UPDATE creature_movement_scripts SET temp=temp | 0x08 WHERE command=15 AND (datalong2 & 0x04 > 0); -- cast triggered
UPDATE creature_movement_scripts SET temp=temp | 0x04 WHERE command=15 AND datalong2=0x01; -- s->t
UPDATE creature_movement_scripts SET temp=temp | 0x06 WHERE command=15 AND datalong2=0x02; -- t->t
UPDATE creature_movement_scripts SET temp=temp | 0x02 WHERE command=15 AND datalong2=0x03; -- t->s
UPDATE creature_movement_scripts SET datalong2=0 WHERE command=15;
/* change faction */
UPDATE creature_movement_scripts SET datalong2=data_flags WHERE command=22;
/* morph/ mount */
UPDATE creature_movement_scripts SET temp=temp | 0x08 WHERE command IN (23, 24) AND (data_flags & 0x01 > 0); -- Summon as active
/* attack start */
UPDATE creature_movement_scripts SET temp=temp | 0x03 WHERE command=26 AND data_flags=0x02; -- b->s
UPDATE creature_movement_scripts SET temp=temp | 0x01 WHERE command=26 AND data_flags=0x04 AND buddy_entry!=0; -- s->b/t
UPDATE creature_movement_scripts SET temp=temp | 0x14 WHERE command=26 AND data_flags=0x06 AND buddy_entry!=0; -- s->s -- Throw error, this would be unreasonable if buddy defined
UPDATE creature_movement_scripts SET temp=temp | 0x04 WHERE command=26 AND data_flags=0x06 AND buddy_entry=0; -- s->s
/* stand state */
UPDATE creature_movement_scripts SET temp=temp | 0x02 WHERE command=28 AND (data_flags & 0x01 > 0) AND buddy_entry=0;
/* change npc flag */
UPDATE creature_movement_scripts SET datalong2=data_flags WHERE command=29;

UPDATE creature_movement_scripts SET data_flags=temp;

ALTER TABLE creature_movement_scripts DROP COLUMN temp;


ALTER TABLE event_scripts ADD COLUMN temp MEDIUMINT(8) DEFAULT 0 AFTER command;

-- Move datalong4 -> 2, 3 -> 4, 2 -> 3 (right shift)
UPDATE event_scripts SET temp=datalong4 WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);
UPDATE event_scripts SET datalong4=datalong3, datalong3=datalong2, datalong2=temp WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);

ALTER TABLE event_scripts CHANGE COLUMN datalong3 buddy_entry MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE event_scripts CHANGE COLUMN datalong4 search_radius MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';

UPDATE event_scripts SET temp=0;

/* Chat*/
UPDATE event_scripts SET temp=temp | 0x04 WHERE command=0 AND (data_flags & 0x02) > 0; -- target self
UPDATE event_scripts SET temp=temp | 0x01 WHERE command=0 AND (data_flags & 0x04) > 0; -- buddy as target
UPDATE event_scripts SET temp=temp | 0x10 WHERE command=0 AND (data_flags & 0x01) > 0; -- will produce error
-- Note that old flag 0x01 // flag_target_player_as_source     0x01 could not be converted automatically, need to check every script

/* Emote*/
UPDATE event_scripts SET temp=temp | 0x02 WHERE command=1 AND (data_flags & 0x01 > 0) AND buddy_entry = 0; -- reverse order if no buddy defined
/*Summon */
UPDATE event_scripts SET temp=temp | 0x08 WHERE command=10 AND (data_flags & 0x01 > 0); -- Summon as active
/* Remove Aura */
UPDATE event_scripts SET temp=0x02 WHERE command=14 AND datalong2=1;
UPDATE event_scripts SET datalong2=0 WHERE command=14;
/* Cast */
UPDATE event_scripts SET temp=temp | 0x08 WHERE command=15 AND (datalong2 & 0x04 > 0); -- cast triggered
UPDATE event_scripts SET temp=temp | 0x04 WHERE command=15 AND datalong2=0x01; -- s->t
UPDATE event_scripts SET temp=temp | 0x06 WHERE command=15 AND datalong2=0x02; -- t->t
UPDATE event_scripts SET temp=temp | 0x02 WHERE command=15 AND datalong2=0x03; -- t->s
UPDATE event_scripts SET datalong2=0 WHERE command=15;
/* change faction */
UPDATE event_scripts SET datalong2=data_flags WHERE command=22;
/* morph/ mount */
UPDATE event_scripts SET temp=temp | 0x08 WHERE command IN (23, 24) AND (data_flags & 0x01 > 0); -- Summon as active
/* attack start */
UPDATE event_scripts SET temp=temp | 0x03 WHERE command=26 AND data_flags=0x02; -- b->s
UPDATE event_scripts SET temp=temp | 0x01 WHERE command=26 AND data_flags=0x04 AND buddy_entry!=0; -- s->b/t
UPDATE event_scripts SET temp=temp | 0x14 WHERE command=26 AND data_flags=0x06 AND buddy_entry!=0; -- s->s -- Throw error, this would be unreasonable if buddy defined
UPDATE event_scripts SET temp=temp | 0x04 WHERE command=26 AND data_flags=0x06 AND buddy_entry=0; -- s->s
/* stand state */
UPDATE event_scripts SET temp=temp | 0x02 WHERE command=28 AND (data_flags & 0x01 > 0) AND buddy_entry=0;
/* change npc flag */
UPDATE event_scripts SET datalong2=data_flags WHERE command=29;

UPDATE event_scripts SET data_flags=temp;

ALTER TABLE event_scripts DROP COLUMN temp;

ALTER TABLE gameobject_scripts ADD COLUMN temp MEDIUMINT(8) DEFAULT 0 AFTER command;

-- Move datalong4 -> 2, 3 -> 4, 2 -> 3 (right shift)
UPDATE gameobject_scripts SET temp=datalong4 WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);
UPDATE gameobject_scripts SET datalong4=datalong3, datalong3=datalong2, datalong2=temp WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);

ALTER TABLE gameobject_scripts CHANGE COLUMN datalong3 buddy_entry MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE gameobject_scripts CHANGE COLUMN datalong4 search_radius MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';

UPDATE gameobject_scripts SET temp=0;

/* Chat*/
UPDATE gameobject_scripts SET temp=temp | 0x04 WHERE command=0 AND (data_flags & 0x02) > 0; -- target self
UPDATE gameobject_scripts SET temp=temp | 0x01 WHERE command=0 AND (data_flags & 0x04) > 0; -- buddy as target
UPDATE gameobject_scripts SET temp=temp | 0x10 WHERE command=0 AND (data_flags & 0x01) > 0; -- will produce error
-- Note that old flag 0x01 // flag_target_player_as_source     0x01 could not be converted automatically, need to check every script

/* Emote*/
UPDATE gameobject_scripts SET temp=temp | 0x02 WHERE command=1 AND (data_flags & 0x01 > 0) AND buddy_entry = 0; -- reverse order if no buddy defined
/*Summon */
UPDATE gameobject_scripts SET temp=temp | 0x08 WHERE command=10 AND (data_flags & 0x01 > 0); -- Summon as active
/* Remove Aura */
UPDATE gameobject_scripts SET temp=0x02 WHERE command=14 AND datalong2=1;
UPDATE gameobject_scripts SET datalong2=0 WHERE command=14;
/* Cast */
UPDATE gameobject_scripts SET temp=temp | 0x08 WHERE command=15 AND (datalong2 & 0x04 > 0); -- cast triggered
UPDATE gameobject_scripts SET temp=temp | 0x04 WHERE command=15 AND datalong2=0x01; -- s->t
UPDATE gameobject_scripts SET temp=temp | 0x06 WHERE command=15 AND datalong2=0x02; -- t->t
UPDATE gameobject_scripts SET temp=temp | 0x02 WHERE command=15 AND datalong2=0x03; -- t->s
UPDATE gameobject_scripts SET datalong2=0 WHERE command=15;
/* change faction */
UPDATE gameobject_scripts SET datalong2=data_flags WHERE command=22;
/* morph/ mount */
UPDATE gameobject_scripts SET temp=temp | 0x08 WHERE command IN (23, 24) AND (data_flags & 0x01 > 0); -- Summon as active
/* attack start */
UPDATE gameobject_scripts SET temp=temp | 0x03 WHERE command=26 AND data_flags=0x02; -- b->s
UPDATE gameobject_scripts SET temp=temp | 0x01 WHERE command=26 AND data_flags=0x04 AND buddy_entry!=0; -- s->b/t
UPDATE gameobject_scripts SET temp=temp | 0x14 WHERE command=26 AND data_flags=0x06 AND buddy_entry!=0; -- s->s -- Throw error, this would be unreasonable if buddy defined
UPDATE gameobject_scripts SET temp=temp | 0x04 WHERE command=26 AND data_flags=0x06 AND buddy_entry=0; -- s->s
/* stand state */
UPDATE gameobject_scripts SET temp=temp | 0x02 WHERE command=28 AND (data_flags & 0x01 > 0) AND buddy_entry=0;
/* change npc flag */
UPDATE gameobject_scripts SET datalong2=data_flags WHERE command=29;

UPDATE gameobject_scripts SET data_flags=temp;

ALTER TABLE gameobject_scripts DROP COLUMN temp;


ALTER TABLE gossip_scripts ADD COLUMN temp MEDIUMINT(8) DEFAULT 0 AFTER command;

-- Move datalong4 -> 2, 3 -> 4, 2 -> 3 (right shift)
UPDATE gossip_scripts SET temp=datalong4 WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);
UPDATE gossip_scripts SET datalong4=datalong3, datalong3=datalong2, datalong2=temp WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);

ALTER TABLE gossip_scripts CHANGE COLUMN datalong3 buddy_entry MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE gossip_scripts CHANGE COLUMN datalong4 search_radius MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';

UPDATE gossip_scripts SET temp=0;

/* Chat*/
UPDATE gossip_scripts SET temp=temp | 0x04 WHERE command=0 AND (data_flags & 0x02) > 0; -- target self
UPDATE gossip_scripts SET temp=temp | 0x01 WHERE command=0 AND (data_flags & 0x04) > 0; -- buddy as target
UPDATE gossip_scripts SET temp=temp | 0x10 WHERE command=0 AND (data_flags & 0x01) > 0; -- will produce error
-- Note that old flag 0x01 // flag_target_player_as_source     0x01 could not be converted automatically, need to check every script

/* Emote*/
UPDATE gossip_scripts SET temp=temp | 0x02 WHERE command=1 AND (data_flags & 0x01 > 0) AND buddy_entry = 0; -- reverse order if no buddy defined
/*Summon */
UPDATE gossip_scripts SET temp=temp | 0x08 WHERE command=10 AND (data_flags & 0x01 > 0); -- Summon as active
/* Remove Aura */
UPDATE gossip_scripts SET temp=0x02 WHERE command=14 AND datalong2=1;
UPDATE gossip_scripts SET datalong2=0 WHERE command=14;
/* Cast */
UPDATE gossip_scripts SET temp=temp | 0x08 WHERE command=15 AND (datalong2 & 0x04 > 0); -- cast triggered
UPDATE gossip_scripts SET temp=temp | 0x04 WHERE command=15 AND datalong2=0x01; -- s->t
UPDATE gossip_scripts SET temp=temp | 0x06 WHERE command=15 AND datalong2=0x02; -- t->t
UPDATE gossip_scripts SET temp=temp | 0x02 WHERE command=15 AND datalong2=0x03; -- t->s
UPDATE gossip_scripts SET datalong2=0 WHERE command=15;
/* change faction */
UPDATE gossip_scripts SET datalong2=data_flags WHERE command=22;
/* morph/ mount */
UPDATE gossip_scripts SET temp=temp | 0x08 WHERE command IN (23, 24) AND (data_flags & 0x01 > 0); -- Summon as active
/* attack start */
UPDATE gossip_scripts SET temp=temp | 0x03 WHERE command=26 AND data_flags=0x02; -- b->s
UPDATE gossip_scripts SET temp=temp | 0x01 WHERE command=26 AND data_flags=0x04 AND buddy_entry!=0; -- s->b/t
UPDATE gossip_scripts SET temp=temp | 0x14 WHERE command=26 AND data_flags=0x06 AND buddy_entry!=0; -- s->s -- Throw error, this would be unreasonable if buddy defined
UPDATE gossip_scripts SET temp=temp | 0x04 WHERE command=26 AND data_flags=0x06 AND buddy_entry=0; -- s->s
/* stand state */
UPDATE gossip_scripts SET temp=temp | 0x02 WHERE command=28 AND (data_flags & 0x01 > 0) AND buddy_entry=0;
/* change npc flag */
UPDATE gossip_scripts SET datalong2=data_flags WHERE command=29;

UPDATE gossip_scripts SET data_flags=temp;

ALTER TABLE gossip_scripts DROP COLUMN temp;


ALTER TABLE quest_end_scripts ADD COLUMN temp MEDIUMINT(8) DEFAULT 0 AFTER command;

-- Move datalong4 -> 2, 3 -> 4, 2 -> 3 (right shift)
UPDATE quest_end_scripts SET temp=datalong4 WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);
UPDATE quest_end_scripts SET datalong4=datalong3, datalong3=datalong2, datalong2=temp WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);

ALTER TABLE quest_end_scripts CHANGE COLUMN datalong3 buddy_entry MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE quest_end_scripts CHANGE COLUMN datalong4 search_radius MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';

UPDATE quest_end_scripts SET temp=0;

/* Chat*/
UPDATE quest_end_scripts SET temp=temp | 0x04 WHERE command=0 AND (data_flags & 0x02) > 0; -- target self
UPDATE quest_end_scripts SET temp=temp | 0x01 WHERE command=0 AND (data_flags & 0x04) > 0; -- buddy as target
UPDATE quest_end_scripts SET temp=temp | 0x10 WHERE command=0 AND (data_flags & 0x01) > 0; -- will produce error
-- Note that old flag 0x01 // flag_target_player_as_source     0x01 could not be converted automatically, need to check every script

/* Emote*/
UPDATE quest_end_scripts SET temp=temp | 0x02 WHERE command=1 AND (data_flags & 0x01 > 0) AND buddy_entry = 0; -- reverse order if no buddy defined
/*Summon */
UPDATE quest_end_scripts SET temp=temp | 0x08 WHERE command=10 AND (data_flags & 0x01 > 0); -- Summon as active
/* Remove Aura */
UPDATE quest_end_scripts SET temp=0x02 WHERE command=14 AND datalong2=1;
UPDATE quest_end_scripts SET datalong2=0 WHERE command=14;
/* Cast */
UPDATE quest_end_scripts SET temp=temp | 0x08 WHERE command=15 AND (datalong2 & 0x04 > 0); -- cast triggered
UPDATE quest_end_scripts SET temp=temp | 0x04 WHERE command=15 AND datalong2=0x01; -- s->t
UPDATE quest_end_scripts SET temp=temp | 0x06 WHERE command=15 AND datalong2=0x02; -- t->t
UPDATE quest_end_scripts SET temp=temp | 0x02 WHERE command=15 AND datalong2=0x03; -- t->s
UPDATE quest_end_scripts SET datalong2=0 WHERE command=15;
/* change faction */
UPDATE quest_end_scripts SET datalong2=data_flags WHERE command=22;
/* morph/ mount */
UPDATE quest_end_scripts SET temp=temp | 0x08 WHERE command IN (23, 24) AND (data_flags & 0x01 > 0); -- Summon as active
/* attack start */
UPDATE quest_end_scripts SET temp=temp | 0x03 WHERE command=26 AND data_flags=0x02; -- b->s
UPDATE quest_end_scripts SET temp=temp | 0x01 WHERE command=26 AND data_flags=0x04 AND buddy_entry!=0; -- s->b/t
UPDATE quest_end_scripts SET temp=temp | 0x14 WHERE command=26 AND data_flags=0x06 AND buddy_entry!=0; -- s->s -- Throw error, this would be unreasonable if buddy defined
UPDATE quest_end_scripts SET temp=temp | 0x04 WHERE command=26 AND data_flags=0x06 AND buddy_entry=0; -- s->s
/* stand state */
UPDATE quest_end_scripts SET temp=temp | 0x02 WHERE command=28 AND (data_flags & 0x01 > 0) AND buddy_entry=0;
/* change npc flag */
UPDATE quest_end_scripts SET datalong2=data_flags WHERE command=29;

UPDATE quest_end_scripts SET data_flags=temp;

ALTER TABLE quest_end_scripts DROP COLUMN temp;


ALTER TABLE quest_start_scripts ADD COLUMN temp MEDIUMINT(8) DEFAULT 0 AFTER command;

-- Move datalong4 -> 2, 3 -> 4, 2 -> 3 (right shift)
UPDATE quest_start_scripts SET temp=datalong4 WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);
UPDATE quest_start_scripts SET datalong4=datalong3, datalong3=datalong2, datalong2=temp WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);

ALTER TABLE quest_start_scripts CHANGE COLUMN datalong3 buddy_entry MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE quest_start_scripts CHANGE COLUMN datalong4 search_radius MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';

UPDATE quest_start_scripts SET temp=0;

/* Chat*/
UPDATE quest_start_scripts SET temp=temp | 0x04 WHERE command=0 AND (data_flags & 0x02) > 0; -- target self
UPDATE quest_start_scripts SET temp=temp | 0x01 WHERE command=0 AND (data_flags & 0x04) > 0; -- buddy as target
UPDATE quest_start_scripts SET temp=temp | 0x10 WHERE command=0 AND (data_flags & 0x01) > 0; -- will produce error
-- Note that old flag 0x01 // flag_target_player_as_source     0x01 could not be converted automatically, need to check every script

/* Emote*/
UPDATE quest_start_scripts SET temp=temp | 0x02 WHERE command=1 AND (data_flags & 0x01 > 0) AND buddy_entry = 0; -- reverse order if no buddy defined
/*Summon */
UPDATE quest_start_scripts SET temp=temp | 0x08 WHERE command=10 AND (data_flags & 0x01 > 0); -- Summon as active
/* Remove Aura */
UPDATE quest_start_scripts SET temp=0x02 WHERE command=14 AND datalong2=1;
UPDATE quest_start_scripts SET datalong2=0 WHERE command=14;
/* Cast */
UPDATE quest_start_scripts SET temp=temp | 0x08 WHERE command=15 AND (datalong2 & 0x04 > 0); -- cast triggered
UPDATE quest_start_scripts SET temp=temp | 0x04 WHERE command=15 AND datalong2=0x01; -- s->t
UPDATE quest_start_scripts SET temp=temp | 0x06 WHERE command=15 AND datalong2=0x02; -- t->t
UPDATE quest_start_scripts SET temp=temp | 0x02 WHERE command=15 AND datalong2=0x03; -- t->s
UPDATE quest_start_scripts SET datalong2=0 WHERE command=15;
/* change faction */
UPDATE quest_start_scripts SET datalong2=data_flags WHERE command=22;
/* morph/ mount */
UPDATE quest_start_scripts SET temp=temp | 0x08 WHERE command IN (23, 24) AND (data_flags & 0x01 > 0); -- Summon as active
/* attack start */
UPDATE quest_start_scripts SET temp=temp | 0x03 WHERE command=26 AND data_flags=0x02; -- b->s
UPDATE quest_start_scripts SET temp=temp | 0x01 WHERE command=26 AND data_flags=0x04 AND buddy_entry!=0; -- s->b/t
UPDATE quest_start_scripts SET temp=temp | 0x14 WHERE command=26 AND data_flags=0x06 AND buddy_entry!=0; -- s->s -- Throw error, this would be unreasonable if buddy defined
UPDATE quest_start_scripts SET temp=temp | 0x04 WHERE command=26 AND data_flags=0x06 AND buddy_entry=0; -- s->s
/* stand state */
UPDATE quest_start_scripts SET temp=temp | 0x02 WHERE command=28 AND (data_flags & 0x01 > 0) AND buddy_entry=0;
/* change npc flag */
UPDATE quest_start_scripts SET datalong2=data_flags WHERE command=29;

UPDATE quest_start_scripts SET data_flags=temp;

ALTER TABLE quest_start_scripts DROP COLUMN temp;


ALTER TABLE spell_scripts ADD COLUMN temp MEDIUMINT(8) DEFAULT 0 AFTER command;

-- Move datalong4 -> 2, 3 -> 4, 2 -> 3 (right shift)
UPDATE spell_scripts SET temp=datalong4 WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);
UPDATE spell_scripts SET datalong4=datalong3, datalong3=datalong2, datalong2=temp WHERE command IN (0, 1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29);

ALTER TABLE spell_scripts CHANGE COLUMN datalong3 buddy_entry MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE spell_scripts CHANGE COLUMN datalong4 search_radius MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0';

UPDATE spell_scripts SET temp=0;

/* Chat*/
UPDATE spell_scripts SET temp=temp | 0x04 WHERE command=0 AND (data_flags & 0x02) > 0; -- target self
UPDATE spell_scripts SET temp=temp | 0x01 WHERE command=0 AND (data_flags & 0x04) > 0; -- buddy as target
UPDATE spell_scripts SET temp=temp | 0x10 WHERE command=0 AND (data_flags & 0x01) > 0; -- will produce error
-- Note that old flag 0x01 // flag_target_player_as_source     0x01 could not be converted automatically, need to check every script

/* Emote*/
UPDATE spell_scripts SET temp=temp | 0x02 WHERE command=1 AND (data_flags & 0x01 > 0) AND buddy_entry = 0; -- reverse order if no buddy defined
/*Summon */
UPDATE spell_scripts SET temp=temp | 0x08 WHERE command=10 AND (data_flags & 0x01 > 0); -- Summon as active
/* Remove Aura */
UPDATE spell_scripts SET temp=0x02 WHERE command=14 AND datalong2=1;
UPDATE spell_scripts SET datalong2=0 WHERE command=14;
/* Cast */
UPDATE spell_scripts SET temp=temp | 0x08 WHERE command=15 AND (datalong2 & 0x04 > 0); -- cast triggered
UPDATE spell_scripts SET temp=temp | 0x04 WHERE command=15 AND datalong2=0x01; -- s->t
UPDATE spell_scripts SET temp=temp | 0x06 WHERE command=15 AND datalong2=0x02; -- t->t
UPDATE spell_scripts SET temp=temp | 0x02 WHERE command=15 AND datalong2=0x03; -- t->s
UPDATE spell_scripts SET datalong2=0 WHERE command=15;
/* change faction */
UPDATE spell_scripts SET datalong2=data_flags WHERE command=22;
/* morph/ mount */
UPDATE spell_scripts SET temp=temp | 0x08 WHERE command IN (23, 24) AND (data_flags & 0x01 > 0); -- Summon as active
/* attack start */
UPDATE spell_scripts SET temp=temp | 0x03 WHERE command=26 AND data_flags=0x02; -- b->s
UPDATE spell_scripts SET temp=temp | 0x01 WHERE command=26 AND data_flags=0x04 AND buddy_entry!=0; -- s->b/t
UPDATE spell_scripts SET temp=temp | 0x14 WHERE command=26 AND data_flags=0x06 AND buddy_entry!=0; -- s->s -- Throw error, this would be unreasonable if buddy defined
UPDATE spell_scripts SET temp=temp | 0x04 WHERE command=26 AND data_flags=0x06 AND buddy_entry=0; -- s->s
/* stand state */
UPDATE spell_scripts SET temp=temp | 0x02 WHERE command=28 AND (data_flags & 0x01 > 0) AND buddy_entry=0;
/* change npc flag */
UPDATE spell_scripts SET datalong2=data_flags WHERE command=29;

UPDATE spell_scripts SET data_flags=temp;

ALTER TABLE spell_scripts DROP COLUMN temp;


-- Update teleport
UPDATE creature_movement_scripts SET data_flags=data_flags|8 WHERE command=3 AND datalong2=0 AND (x!=0 AND y!=0 AND z!=0);
UPDATE event_scripts SET data_flags=data_flags|8 WHERE command=3 AND datalong2=0 AND (x!=0 AND y!=0 AND z!=0);
UPDATE gameobject_scripts SET data_flags=data_flags|8 WHERE command=3 AND datalong2=0 AND (x!=0 AND y!=0 AND z!=0);
UPDATE gossip_scripts SET data_flags=data_flags|8 WHERE command=3 AND datalong2=0 AND (x!=0 AND y!=0 AND z!=0);
UPDATE quest_end_scripts SET data_flags=data_flags|8 WHERE command=3 AND datalong2=0 AND (x!=0 AND y!=0 AND z!=0);
UPDATE quest_start_scripts SET data_flags=data_flags|8 WHERE command=3 AND datalong2=0 AND (x!=0 AND y!=0 AND z!=0);
UPDATE spell_scripts SET data_flags=data_flags|8 WHERE command=3 AND datalong2=0 AND (x!=0 AND y!=0 AND z!=0);

-- Set all move commands to default creature speed
UPDATE creature_movement_scripts SET datalong2=0 WHERE command=3;
UPDATE event_scripts SET datalong2=0 WHERE command=3;
UPDATE gameobject_scripts SET datalong2=0 WHERE command=3;
UPDATE gossip_scripts SET datalong2=0 WHERE command=3;
UPDATE quest_end_scripts SET datalong2=0 WHERE command=3;
UPDATE quest_start_scripts SET datalong2=0 WHERE command=3;
UPDATE spell_scripts SET datalong2=0 WHERE command=3;

DELETE FROM strawberry_string WHERE entry IN (555,556);
DELETE FROM command WHERE name LIKE 'hover';
