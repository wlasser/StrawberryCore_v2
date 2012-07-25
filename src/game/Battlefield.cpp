/*
 * Copyright (C) 2010-2012 Strawberry-Pr0jcts <http://strawberry-pr0jcts.com/>
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

#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "DBCStores.h"
#include "SharedDefines.h"

Battlefield::Battlefield(uint8 BattleId)
{
    m_battleId = BattleId;
    m_battlefiledGuid = ObjectGuid(HIGHGUID_BATTLEFIELD,uint32(131072 + m_battleId));

    for(uint8 i = 0; i < MAX_TEAM; ++i)
    {
        m_raidGroup[i] = new Group();
    }

    m_battleInProgress = false;
    m_invitationSent = false;
}
Battlefield::~Battlefield()
{
    for(uint8 i = 0; i < MAX_TEAM; ++i)
    {
        delete m_raidGroup[i];
    }
}

void Battlefield::Update(uint32 uiDiff)
{
    if(m_battleInProgress)
    {
        if(m_battleDurationTimer <= uiDiff)
        {
            BattleEnd();
            m_battleInProgress = false;
        }
        else
            m_battleDurationTimer -= uiDiff;
    }
    else
    {
        if(m_nextBattleTimer <= uiDiff)
        {
            BattleStart();
            m_battleInProgress = true;
        }
        else 
            m_nextBattleTimer -= uiDiff;

        if(m_preBattleTimer <= uiDiff && !m_invitationSent)
        {
            sBattlefieldMgr.ChangeState(this);

            sLog.outDebug("Inviting players in queue to battle");

            WorldPacket send_data(SMSG_BATTLEFIELD_MGR_ENTRY_INVITE,13);

            uint8 guidMask[] = { 5, 3, 7, 2, 6, 1, 0, 4 };
            uint8 guidBytes[] = { 1, 2, 5, 4, 7, 0, 3, 6 };

            send_data.WriteGuidMask(getGuid(),guidMask,8);
            send_data.FlushBits();

            send_data.WriteGuidBytes(getGuid(),guidBytes,2,0);
            send_data << uint32(time(NULL) + 20); // unk1
            send_data.WriteGuidBytes(getGuid(),guidBytes,6,2);

            BattlefieldQueue * queue = sBattlefieldMgr.GetQueueForBattlefield(getGuid());
            for(PlayerQueue::iterator itr = queue->m_inQueue.begin(); itr != queue->m_inQueue.end(); ++itr)
            {
                (*itr)->GetSession()->SendPacket(&send_data);
            }
            m_invitationSent = true;
        }
        else
            m_preBattleTimer -= uiDiff;
    }

    OnUpdate(uiDiff);
}

void Battlefield::BattleStart()
{
    BeforeBattleStarted();
}

void Battlefield::BattleEnd()
{
    AfterBattleEnded();
}

bool Battlefield::AddPlayerToGroup(Player * player)
{
    Group* grp;
    if(player->GetTeam() == ALLIANCE)
        grp = m_raidGroup[TEAM_ALLIANCE];
    else
        grp = m_raidGroup[TEAM_HORDE];

    if(!grp->IsCreated())
    {
        grp->Create(player->GetObjectGuid(),player->GetName());
        grp->ConvertToRaid();
        grp->AddMember(player->GetObjectGuid(),player->GetName());
    }
    else if(!grp->IsFull())
    {
        grp->AddMember(player->GetObjectGuid(),player->GetName());
    }
    else
        return false;

    return true;
}

