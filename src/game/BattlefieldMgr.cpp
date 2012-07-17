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

#include "pchdef.h"
#include "BattlefieldMgr.h"
#include "BattlefieldWG.h"
#include "BattlefieldTB.h"

INSTANTIATE_SINGLETON_1(BattlefieldMgr);

BattlefieldMgr::BattlefieldMgr()
{
    
}

BattlefieldMgr::~BattlefieldMgr()
{
    for(BattlefieldList::iterator itr = m_battlefieldList.begin(); itr != m_battlefieldList.end(); ++itr)
        delete itr->second;
}

void BattlefieldMgr::Initialize()
{
    sLog.outDebug("Creating Battlefields");

    Battlefield * WG = new BattlefieldWG();
    m_battlefieldList[WG->getGuid()] = WG;
    m_queueMap[WG->getGuid()] = new BattlefieldQueue(BATTLEFIELD_WG);
	Battlefield * TB = new BattlefieldTB();
	m_battlefieldList[TB->getGuid()] = TB;
	m_queueMap[TB->getGuid()] = new BattlefieldQueue(BATTLEFIELD_TB);
}

void BattlefieldMgr::Update(uint32 uiDiff)
{
    for(BattlefieldList::iterator itr = m_battlefieldList.begin(); itr != m_battlefieldList.end(); ++itr)
    {
        (*itr).second->Update(uiDiff);
    }
}

void BattlefieldMgr::SendInvitePlayerToQueue(Player * player)
{
    //WorldPacket send_data(
    
    //player->GetSession()->SendPacket(&send_data);
}

void BattlefieldMgr::ChangeState(Battlefield * battlefield)
{
    WorldPacket send_data(SMSG_BATTLEFIELD_MGR_STATE_CHANGED,17);
    uint8 guidMask[] = { 5, 3, 7, 2, 4, 6, 1, 0};
    uint8 guidBytes[] = { 6, 1, 3, 4, 2, 0, 7, 5};

    send_data.WriteGuidMask(battlefield->getGuid(),guidMask,8);
    send_data.FlushBits();

    send_data.WriteGuidBytes(battlefield->getGuid(),guidBytes,1,0);
    send_data << uint32(1); //unk
    send_data.WriteGuidBytes(battlefield->getGuid(),guidBytes,5,1);
    send_data << uint32(1); //unk
    send_data.WriteGuidBytes(battlefield->getGuid(),guidBytes,2,7);
    
}

Battlefield * BattlefieldMgr::FindBattlefield(ObjectGuid guid)
{
    BattlefieldList::const_iterator itr = m_battlefieldList.find(guid);
    if(itr != m_battlefieldList.end())
        return itr->second;
    else
        return NULL;
}

