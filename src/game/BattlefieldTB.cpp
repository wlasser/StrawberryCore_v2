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

#include "BattlefieldTB.h"

BattlefieldTB::BattlefieldTB() : Battlefield(BATTLEFIELD_TB)
{
    m_map = sMapMgr.FindMap(708);
    m_zoneId = 5095;
    m_preBattleTimer = 4 * 60 * IN_MILLISECONDS;
    m_nextBattleTimer = 8 * 60 * IN_MILLISECONDS;
    m_battleDurationTimer = 5 * 60 * IN_MILLISECONDS;
}

void BattlefieldTB::OnUpdate(uint32 uiDiff)
{

}

void BattlefieldTB::BeforeBattleStarted()
{
    m_preBattleTimer = 4 * 60 * IN_MILLISECONDS;
    m_nextBattleTimer = 8 * 60 * IN_MILLISECONDS;
}

void BattlefieldTB::AfterBattleEnded()
{
    m_battleDurationTimer = 5 * 60 * IN_MILLISECONDS;
}