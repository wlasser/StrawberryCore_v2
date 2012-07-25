/*
 * Copyright (C) 2010-2012 Strawberry-Pr0jcts <http://strawberry-pr0jcts.com/>
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "QuestDef.h"
#include "Player.h"
#include "World.h"
#include "DBCStores.h"

Quest::Quest(Field * questRecord)
{
    QuestId = questRecord[0].GetUInt32();
    QuestMethod = questRecord[1].GetUInt32();
    ZoneOrSort = questRecord[2].GetInt32();
    MinLevel = questRecord[3].GetUInt32();
    QuestLevel = questRecord[4].GetInt32();
    Type = questRecord[5].GetUInt32();
    RequiredClasses = questRecord[6].GetUInt32();
    RequiredRaces = questRecord[7].GetUInt32();
    RequiredSkill = questRecord[8].GetUInt32();
    RequiredSkillValue = questRecord[9].GetUInt32();
    RepObjectiveFaction = questRecord[10].GetUInt32();
    RepObjectiveValue = questRecord[11].GetInt32();
    RequiredMinRepFaction = questRecord[12].GetUInt32();
    RequiredMinRepValue = questRecord[13].GetInt32();
    RequiredMaxRepFaction = questRecord[14].GetUInt32();
    RequiredMaxRepValue = questRecord[15].GetInt32();
    RequiredSpell = questRecord[16].GetInt32();
    SuggestedPlayers = questRecord[17].GetUInt32();
    LimitTime = questRecord[18].GetUInt32();
    m_QuestFlags = questRecord[19].GetUInt16();
    m_SpecialFlags = questRecord[20].GetUInt16();
    CharTitleId = questRecord[21].GetUInt32();
    PlayersSlain = questRecord[22].GetUInt32();
    BonusTalents = questRecord[23].GetUInt32();
    PortraitGiver = questRecord[24].GetUInt32();
    PortraitTurnIn = questRecord[25].GetUInt32();
    PrevQuestId = questRecord[26].GetInt32();
    NextQuestId = questRecord[27].GetInt32();
    ExclusiveGroup = questRecord[28].GetInt32();
    NextQuestInChain = questRecord[29].GetUInt32();
    RewXPId = questRecord[30].GetUInt32();
    SrcItemId = questRecord[31].GetUInt32();
    SrcItemCount = questRecord[32].GetUInt32();
    SrcSpell = questRecord[33].GetUInt32();
    Title = questRecord[34].GetCppString();
    Details = questRecord[35].GetCppString();
    Objectives = questRecord[36].GetCppString();
    OfferRewardText = questRecord[37].GetCppString();
    RequestItemsText = questRecord[38].GetCppString();
    EndText = questRecord[39].GetCppString();
    CompletedText = questRecord[40].GetCppString();
    PortraitGiverText = questRecord[41].GetCppString();
    PortraitGiverName = questRecord[42].GetCppString();
    PortraitTurnInText = questRecord[43].GetCppString();
    PortraitTurnInName = questRecord[44].GetCppString();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        ObjectiveText[i] = questRecord[45+i].GetCppString();

    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
        ReqItemId[i] = questRecord[49+i].GetUInt32();

    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
        ReqItemCount[i] = questRecord[55+i].GetUInt32();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        ReqSourceId[i] = questRecord[61+i].GetUInt32();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        ReqSourceCount[i] = questRecord[65+i].GetUInt32();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        ReqCreatureOrGOId[i] = questRecord[69+i].GetInt32();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        ReqCreatureOrGOCount[i] = questRecord[73+i].GetUInt32();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        ReqSpell[i] = questRecord[77+i].GetUInt32();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewChoiceItemId[i] = questRecord[81+i].GetUInt32();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewChoiceItemCount[i] = questRecord[87+i].GetUInt32();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewItemId[i] = questRecord[93+i].GetUInt32();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewItemCount[i] = questRecord[97+i].GetUInt32();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewRepFaction[i] = questRecord[101+i].GetUInt32();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewRepValueId[i] = questRecord[106+i].GetInt32();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewRepValue[i] = questRecord[111+i].GetInt32();

    RewHonorAddition = questRecord[116].GetUInt32();
    RewHonorMultiplier = questRecord[117].GetFloat();
    RewOrReqMoney = questRecord[118].GetInt32();
    RewMoneyMaxLevel = questRecord[119].GetUInt32();
    RewSpell = questRecord[120].GetUInt32();
    RewSpellCast = questRecord[121].GetUInt32();
    RewMailTemplateId = questRecord[122].GetUInt32();
    RewMailDelaySecs = questRecord[123].GetUInt32();
    PointMapId = questRecord[124].GetUInt32();
    PointX = questRecord[125].GetFloat();
    PointY = questRecord[126].GetFloat();
    PointOpt = questRecord[127].GetUInt32();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmote[i] = questRecord[128+i].GetUInt32();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmoteDelay[i] = questRecord[132+i].GetUInt32();

    IncompleteEmote = questRecord[136].GetUInt32();
    CompleteEmote = questRecord[137].GetUInt32();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmote[i] = questRecord[138+i].GetInt32();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmoteDelay[i] = questRecord[142+i].GetInt32();

    for (int i = 0; i < QUEST_CURRENCY_REWARD_COUNT; ++i)
        CurrencyRewardId[i] = questRecord[146+i].GetInt32();

    for (int i = 0; i < QUEST_CURRENCY_REWARD_COUNT; ++i)
        CurrencyRewardCount[i] = questRecord[150+i].GetInt32();

    for (int i = 0; i < QUEST_CURRENCY_REQUIRED_COUNT; ++i)
        CurrencyRequiredId[i] = questRecord[154+i].GetInt32();

    for (int i = 0; i < QUEST_CURRENCY_REQUIRED_COUNT; ++i)
        CurrencyRequiredCount[i] = questRecord[158+i].GetInt32();

    SoundAcceptId = questRecord[162].GetUInt32();
    SoundTurnInId = questRecord[163].GetUInt32();
    QuestStartScript = questRecord[164].GetUInt32();
    QuestCompleteScript = questRecord[165].GetUInt32();

    m_isActive = true;

    m_reqitemscount = 0;
    m_reqCreatureOrGOcount = 0;
    m_rewitemscount = 0;
    m_rewchoiceitemscount = 0;

    for (int i=0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
    {
        if ( ReqItemId[i] )
            ++m_reqitemscount;
    }

    for (int i=0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        if ( ReqCreatureOrGOId[i] )
            ++m_reqCreatureOrGOcount;
    }

    for (int i=0; i < QUEST_REWARDS_COUNT; ++i)
    {
        if ( RewItemId[i] )
            ++m_rewitemscount;
    }

    for (int i=0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
    {
        if (RewChoiceItemId[i])
            ++m_rewchoiceitemscount;
    }
}

uint32 Quest::XPValue(Player *pPlayer) const
{
    if (pPlayer)
    {
        uint32 realXP = 0;
        uint32 xpMultiplier = 0;
        int32 baseLevel = 0;
        int32 playerLevel = pPlayer->getLevel();

        // formula can possibly be organized better, using less if's and simplify some.

        if (QuestLevel != -1)
            baseLevel = QuestLevel;

        if (((baseLevel - playerLevel) + 10)*2 > 10)
        {
            baseLevel = playerLevel;

            if (QuestLevel != -1)
                baseLevel = QuestLevel;

            if (((baseLevel - playerLevel) + 10)*2 <= 10)
            {
                if (QuestLevel == -1)
                    baseLevel = playerLevel;

                xpMultiplier = 2 * (baseLevel - playerLevel) + 20;
            }
            else
            {
                xpMultiplier = 10;
            }
        }
        else
        {
            baseLevel = playerLevel;

            if (QuestLevel != -1)
                baseLevel = QuestLevel;

            if (((baseLevel - playerLevel) + 10)*2 >= 1)
            {
                baseLevel = playerLevel;

                if (QuestLevel != -1)
                    baseLevel = QuestLevel;

                if (((baseLevel - playerLevel) + 10)*2 <= 10)
                {
                    if (QuestLevel == -1)
                        baseLevel = playerLevel;

                    xpMultiplier = 2 * (baseLevel - playerLevel) + 20;
                }
                else
                {
                    xpMultiplier = 10;
                }
            }
            else
            {
                xpMultiplier = 1;
            }
        }

        // not possible to reward XP when baseLevel does not exist in dbc
        if (const QuestXPLevel* pXPData = sQuestXPLevelStore.LookupEntry(baseLevel))
        {
            uint32 rawXP = xpMultiplier * pXPData->xpIndex[RewXPId] / 10;

            // round values
            if (rawXP > 1000)
                realXP = ((rawXP + 25) / 50 * 50);
            else if (rawXP > 500)
                realXP = ((rawXP + 12) / 25 * 25);
            else if (rawXP > 100)
                realXP = ((rawXP + 5) / 10 * 10);
            else
                realXP = ((rawXP + 2) / 5 * 5);
        }

        return realXP;
    }

    return 0;
}

int32  Quest::GetRewOrReqMoney() const
{
    if(RewOrReqMoney <=0)
        return RewOrReqMoney;

    return int32(RewOrReqMoney * sWorld.getConfig(CONFIG_FLOAT_RATE_DROP_MONEY));
}

bool Quest::IsAllowedInRaid() const
{
    if (Type == QUEST_TYPE_RAID || Type == QUEST_TYPE_RAID_10 || Type == QUEST_TYPE_RAID_25)
        return true;

    return sWorld.getConfig(CONFIG_BOOL_QUEST_IGNORE_RAID);
}

uint32 Quest::CalculateRewardHonor(uint32 level) const
{
    if (level > GT_MAX_LEVEL)
        level = GT_MAX_LEVEL;

    uint32 honor = 0;

    if(GetRewHonorAddition() > 0 || GetRewHonorMultiplier() > 0.0f)
    {
        // values stored from 0.. for 1...
        /* not exist in 4.x
        TeamContributionPoints const* tc = sTeamContributionPoints.LookupEntry(level-1);
        if(!tc)
            return 0;
        */
        uint32 i_honor = uint32(/*tc->Value*/1.0f * GetRewHonorMultiplier() * 0.1f);
        honor = i_honor + GetRewHonorAddition();
    }

    return honor;
}
