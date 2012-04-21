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

/** \file
    \ingroup u2w
*/

#include "Opcodes.h"
#include "WorldSession.h"

// Correspondence between opcodes and their names
OpcodeHandler opcodeTable[NUM_MSG_TYPES];
OpcodeTableContainer opcodeTableMap;

uint16 opcodesEnumToNumber[MAX_OPCODE_VALUE];

void OpcodeTableHandler::LoadOpcodesFromDB()
{
    QueryResult* result = WorldDatabase.Query("SELECT OpcodeName, OpcodeValue FROM Opcodes WHERE ClientBuild = 15595");
    if (!result)
        return;

    uint32 count = 0;
    do
    {
        Field *fields = result->Fetch();

        std::string OpcodeName = fields[0].GetString();
        uint16 OpcodeValue     = fields[1].GetUInt16();

        opcodeTableMap[OpcodeName] = OpcodeValue;

        count++;
    }
    while (result->NextRow());

    delete result;

    sLog.outString(">> Loaded %u opcode definitions", count);
    sLog.outString();
}

uint16 OpcodeTableHandler::GetOpcodeTable(const char* name)
{
    OpcodeTableContainer::iterator itr = opcodeTableMap.find(std::string(name));
    if (itr != opcodeTableMap.end())
        return itr->second;

    return NULL;
}

static void DefineOpcode(Opcodes opcodeEnum, const char* name, SessionStatus status, PacketProcessing packetProcessing, void (WorldSession::*handler)(WorldPacket& recvPacket) )
{
    uint16 opcode = sOpcodeTableHandler->GetOpcodeTable(name);

    if (opcode > 0)
    {
        opcodesEnumToNumber[opcodeEnum] = opcode;

        opcodeTable[opcode].name = name;
        opcodeTable[opcode].status = status;
        opcodeTable[opcode].packetProcessing = packetProcessing;
        opcodeTable[opcode].handler = handler;
        opcodeTable[opcode].opcodeEnum = opcodeEnum;
    }
    else
        sLog.outError("SOE: No valid value for %s", name); // Should be removed later. One opcode have the value 0
}

#define OPCODE( name, status, packetProcessing, handler ) DefineOpcode( name, #name, status, packetProcessing, handler )

void InitOpcodeTable()
{
    for(uint16 i = 0; i < NUM_MSG_TYPES; ++i)
    {
        opcodeTable[i].name             = "UNKNOWN";
        opcodeTable[i].status           = STATUS_NEVER;
        opcodeTable[i].packetProcessing = PROCESS_INPLACE;
        opcodeTable[i].handler          = &WorldSession::HandleNULL;
    }

    // Authentication
    OPCODE(MSG_WOW_CONNECTION,                STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleEarlyProccess           );
    OPCODE(SMSG_AUTH_CHALLENGE,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_AUTH_SESSION,                 STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleEarlyProccess           );
    OPCODE(SMSG_AUTH_RESPONSE,                STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );

    // Realmlist
    OPCODE(CMSG_REALM_SPLIT_STATE,            STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandleRealmSplitState         );
    OPCODE(SMSG_REALM_SPLIT_MSG,              STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );

    // Addons
    OPCODE(SMSG_ADDON_INFO,                   STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );

    // Characterlist
    OPCODE(CMSG_REQUEST_CHARACTER_ENUM,       STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandleResponseCharacterEnumOpcode          );
    OPCODE(SMSG_RESPONSE_CHARACTER_ENUM,      STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_REQUEST_CHARACTER_CREATE,     STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandleResponseCharacterCreateOpcode        );
    OPCODE(SMSG_RESPONSE_CHARACTER_CREATE,    STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_REQUEST_CHARACTER_DELETE,     STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandleCharDeleteOpcode        );
    OPCODE(SMSG_CHAR_DELETE,                  STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );

    // World enter
    OPCODE(CMSG_PLAYER_LOGIN,                 STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandlePlayerLoginOpcode       );
    OPCODE(CMSG_LOADING_SCREEN_NOTIFY,        STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandleLoadingScreenNotify     );

    // World
    OPCODE(SMSG_UPDATE_OBJECT,                STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_DESTROY_OBJECT,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_UPDATE_OBJECT_FAILURE,        STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleUpdateObjectFailure     );
    OPCODE(CMSG_READY_FOR_ACCOUNT_DATA_TIMES, STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandleReadyForAccountDataTimes);
    OPCODE(SMSG_ACCOUNT_DATA_INITIALIZED,     STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_REQUEST_ACCOUNT_DATA,         STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandleRequestAccountData      );
    OPCODE(CMSG_UPDATE_ACCOUNT_DATA,          STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandleUpdateAccountData       );
    OPCODE(SMSG_UPDATE_ACCOUNT_DATA,          STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_UPDATE_ACCOUNT_DATA_COMPLETE, STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_TIME_SYNC_RESPONSE,           STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleTimeSyncResp            );
    OPCODE(SMSG_TIME_SYNC_REQ,                STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_POWER_UPDATE,                 STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_ZONEUPDATE,                   STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleZoneUpdateOpcode        );
    OPCODE(CMSG_KEEP_ALIVE,                   STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleEarlyProccess           );
    OPCODE(CMSG_PING,                         STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleEarlyProccess           );
    OPCODE(SMSG_PONG,                         STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_LOGIN_VERIFY_WORLD,           STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_INIT_WORLD_STATES,            STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_FEATURE_SYSTEM_STATUS,        STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_ACTION_BUTTONS,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_MONSTER_MOVE,                 STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );

    // Spells
    OPCODE(SMSG_INITIAL_SPELLS,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_SPELL_START,                  STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_SPELL_GO,                     STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_CAST_SPELL,                   STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleCastSpellOpcode         );


    // Chat
    OPCODE(SMSG_CHANNEL_NOTIFY,               STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_CHAT_DOWN,                    STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_CHAT_IS_DOWN,                 STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_CHAT_RECONNECT,               STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_CHANNEL_LIST,                 STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_MESSAGE_CHAT,                 STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_TEXT_EMOTE,                   STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_ZONE_UNDER_ATTACK,            STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_DEFENSE_MESSAGE,              STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_SERVER_MESSAGE,               STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_RAID_INSTANCE_MESSAGE,        STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_INSTANCE_RESET,               STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_INSTANCE_RESET_FAILED,        STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_UPDATE_INSTANCE,              STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_UPDATE_INSTANCE_OWNERSHIP,    STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_CHECK_RELOAD_SPAM_FILTER,     STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_TITLE_EARNED,                 STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_RESET_FAILED_NOTIFY,          STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_GM_MESSAGE_CHAT,              STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_LOG_XP_GAIN,                  STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_DURABILITY_DAMAGE_DEATH,      STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_CHANNEL_MEMBER_COUNT,         STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_COMSAT_RECONNECT_ATTEMPT,     STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_COMSAT_CONNECT_DISCONNECT,    STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_COMSAT_CONNECT_FAIL,          STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_VOICE_STATUS_CHANGED,         STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_USER_LIST_ADDED,              STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_USER_LIST_REMOVE,             STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_USER_LIST_UPDATE,             STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_VOICESESSION_FULL,            STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_SERVER_FIRST_ACHIEVEMENT,     STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );

    OPCODE(SMSG_MESSAGE_CHAT,                 STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );

    // Chat Channel
    OPCODE(CMSG_JOIN_CHANNEL,                 STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleJoinChannelOpcode       );

    // Stats and Caches
    OPCODE(CMSG_CREATURE_STATS,               STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleCreatureStatsOpcode     );
    OPCODE(SMSG_CREATURE_STATS,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_GAME_OBJECT_STATS,            STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleGameObjectStatsOpcode   );
    OPCODE(SMSG_GAME_OBJECT_STATS,            STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_NAME_CACHE,                   STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleNameCacheOpcode         );
    OPCODE(SMSG_NAME_CACHE,                   STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );

    // Compressed
    //OPCODE(SMSG_COMPRESSED_UPDATE_OBJECT,     STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );

    // Movement
    OPCODE(SMSG_PLAYER_MOVE,                  STATUS_AUTHED,   PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
};
