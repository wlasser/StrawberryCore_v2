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

#ifndef __BATTLEFIELD_MGR_H
#define __BATTLEFIELD_MGR_H

#include "Player.h"
#include <map>

typedef std::list<Player*> PlayerQueue;

class Player;

class BattlefieldMgr
{
    public:

        void Initialize();
        void Update(uint32 uiDiff);

    private:
        PlayerQueue m_queue;
};

#define sBattlefieldMrg Strawberry::Singleton<BattlefieldMgr>::Instance()
#endif