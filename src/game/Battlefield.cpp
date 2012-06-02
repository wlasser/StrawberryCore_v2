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

Battlefield::Battlefield(uint8 BattleId,uint32 zoneId, uint32 WarTime,uint32 NoWarTime)
{
    m_battleId = BattleId;
    m_zoneId = zoneId;
    m_nextBattleTimer = NoWarTime;
    m_battleDurationTimer = WarTime;
}

Battlefield::~Battlefield()
{

}

void Battlefield::Update(uint32 uiDiff)
{

}

void Battlefield::InvitePlayersInZone()
{
    Map::PlayerList playersInMap = sMapMgr.FindMap(571)->GetPlayers();
    for(Map::PlayerList::iterator itr = playersInMap.begin(); itr != playersInMap.end(); ++itr)
    {
        Player* plr = (*itr).getSource();
        uint16 team = plr->GetTeam();
        if((team == ALLIANCE && (uint32)m_queuedPlayers[TEAM_ALLIANCE].size() <= 40) || (team == ALLIANCE && (uint32)m_queuedPlayers[TEAM_ALLIANCE].size() <= 40))
        {
            sBattlefieldMgr.SendInvitePlayerToQueue(plr);
        }
    }

}
