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
    m_battlefieldList[WG->getGuid().GetRawValue()] = WG;
    m_queueMap[WG->getGuid().GetRawValue()] = new BattlefieldQueue(BATTLEFIELD_WG);
}

void BattlefieldMgr::Update(uint32 uiDiff)
{
    //sLog.outDebug("Updating the Battlefields");
    for(BattlefieldList::iterator itr = m_battlefieldList.begin(); itr != m_battlefieldList.end(); ++itr)
    {
        (*itr).second->Update(uiDiff);
    }
}

void BattlefieldMgr::SendInvitePlayerToQueue(Player * player)
{
    uint8 GuidMask[] = { 0, 2, 3, 6, 1, 5, 4, 7};
    uint8 GuidByte[] = { 2, 3, 6, 5, 0, 4, 1, 7};

    WorldPacket send_data(SMSG_BATTLEFIELD_MGR_QUEUE_INVITE, 31);

    send_data.WriteBit(1);          // unk bit +6
    send_data.WriteBit(1);          // unk bit +36
    send_data.WriteGuidMask(player->GetObjectGuid().GetRawValue(),GuidMask,1);
    send_data.WriteBit(1);          // +10
    send_data.WriteGuidMask(player->GetObjectGuid().GetRawValue(),GuidMask,3,1);
    send_data.WriteBit(1);          // zoneId
    send_data.FlushBits();
    send_data.WriteBit(1);          // +48
    send_data.WriteGuidMask(player->GetObjectGuid().GetRawValue(),GuidMask,2,4);
    send_data.WriteBit(1);          // +7
    send_data.WriteGuidMask(player->GetObjectGuid().GetRawValue(),GuidMask,2,6);
    send_data.WriteBit(1);          // +11
    send_data.FlushBits();

    send_data.WriteGuidBytes(player->GetObjectGuid().GetRawValue(),GuidByte,1,0);
    send_data << uint32(2783); // +10
    send_data.WriteGuidBytes(player->GetObjectGuid().GetRawValue(),GuidByte,2,1);
    send_data << uint8(4);  // +36
    send_data.WriteGuidBytes(player->GetObjectGuid().GetRawValue(),GuidByte,2,3);
    send_data << uint32(4875); // +6
    send_data << uint32(9849); // +11
    send_data << uint32(4197); // zoneId
    send_data.WriteGuidBytes(player->GetObjectGuid().GetRawValue(),GuidByte,1,6);
    send_data << uint32(6741); // +7
    send_data.WriteGuidBytes(player->GetObjectGuid().GetRawValue(),GuidByte,2,7);
    
    player->GetSession()->SendPacket(&send_data);
}

Battlefield * BattlefieldMgr::FindBattlefield(uint64 guid)
{
    BattlefieldList::const_iterator itr = m_battlefieldList.find(guid);
    if(itr != m_battlefieldList.end())
        return itr->second;
    else
        return NULL;
}
