ALTER TABLE `character_talent`
	ADD COLUMN `tabId` SMALLINT(3) UNSIGNED NOT NULL DEFAULT '0' AFTER `spec`,
	DROP PRIMARY KEY,
	ADD PRIMARY KEY (`guid`, `tabId`, `talent_id`, `spec`),
	ADD INDEX `tabId` (`tabId`);