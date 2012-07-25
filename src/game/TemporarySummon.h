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

#ifndef STRAWBERRYSERVER_TEMPSUMMON_H
#define STRAWBERRYSERVER_TEMPSUMMON_H

#include "Creature.h"
#include "ObjectAccessor.h"

class TemporarySummon : public Creature
{
    public:
        explicit TemporarySummon(ObjectGuid summoner = ObjectGuid());
        virtual ~TemporarySummon(){};

        void Update(uint32 update_diff, uint32 time) override;
        void Summon(TempSummonType type, uint32 lifetime);
        void UnSummon();
        void SaveToDB();
        ObjectGuid const& GetSummonerGuid() const { return m_summoner ; }
        Unit* GetSummoner() const { return ObjectAccessor::GetUnit(*this, m_summoner); }
    private:
        void SaveToDB(uint32, uint8, uint32)                // overwrited of Creature::SaveToDB     - don't must be called
        {
            STRAWBERRY_ASSERT(false);
        }
        void DeleteFromDB()                                 // overwrited of Creature::DeleteFromDB - don't must be called
        {
            STRAWBERRY_ASSERT(false);
        }

        TempSummonType m_type;
        uint32 m_timer;
        uint32 m_lifetime;
        ObjectGuid m_summoner;
};
#endif
