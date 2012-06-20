ALTER TABLE `character_talent`
	ADD COLUMN `tab_id` SMALLINT(3) UNSIGNED NOT NULL DEFAULT '0' AFTER `spec`,
	DROP PRIMARY KEY,
	ADD PRIMARY KEY (`guid`, `tab_id`, `talent_id`, `spec`),
	ADD INDEX `tab_id` (`tab_id`);