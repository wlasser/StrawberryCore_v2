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

#include <ace/ACE.h>
#include <ace/Log_Msg.h>
#include <ace/Reactor.h>
#include <ace/Reactor_Impl.h>
#include <ace/TP_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include <ace/Guard_T.h>
#include <ace/Atomic_Op.h>
#include <ace/os_include/arpa/os_inet.h>
#include <ace/os_include/netinet/os_tcp.h>
#include <ace/os_include/sys/os_types.h>
#include <ace/os_include/sys/os_socket.h>

#include <set>

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Policies/SingletonImp.h"
#include "Opcodes.h"
#include "WorldSession.h"

// Correspondence between opcodes and their names
OpcodeHandler opcodeTable[NUM_MSG_TYPES];
OpcodeTableContainer opcodeTableMap;

uint16 opcodesEnumToNumber[MAX_OPCODE_VALUE];

void OpcodeTableHandler::LoadOpcodesFromDB()
{
    QueryResult* result = WorldDatabase.Query("SELECT OpcodeName, OpcodeValue FROM opcodes WHERE ClientBuild = 15595");
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

    OPCODE(MSG_WOW_CONNECTION,                STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleEarlyProccess           );
    OPCODE(SMSG_AUTH_CHALLENGE,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_AUTH_SESSION,                 STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleEarlyProccess           );
    OPCODE(SMSG_AUTH_RESPONSE,                STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_REALM_SPLIT_STATE,            STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandleRealmSplitState         );
    OPCODE(SMSG_REALM_SPLIT_MSG,              STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_ADDON_INFO,                   STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_REQUEST_CHARACTER_ENUM,       STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandleResponseCharacterEnumOpcode          );
    OPCODE(SMSG_RESPONSE_CHARACTER_ENUM,      STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_REQUEST_CHARACTER_CREATE,     STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandleResponseCharacterCreateOpcode        );
    OPCODE(SMSG_RESPONSE_CHARACTER_CREATE,    STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_REQUEST_CHARACTER_DELETE,     STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandleCharDeleteOpcode        );
    OPCODE(SMSG_CHAR_DELETE,                  STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_LOGOUT_REQUEST,               STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleLogoutRequestOpcode     );
    OPCODE(CMSG_PLAYER_LOGOUT,                STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandlePlayerLogoutOpcode      );
    OPCODE(CMSG_LOGOUT_CANCEL,                STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleLogoutCancelOpcode      );
    OPCODE(SMSG_LOGOUT_CANCEL,                STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_LOGOUT_RESPONSE,              STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_LOGOUT_COMPLETE,              STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_PLAYER_LOGIN,                 STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandlePlayerLoginOpcode       );
    OPCODE(CMSG_LOADING_SCREEN_NOTIFY,        STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandleLoadingScreenNotify     );
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
    OPCODE(CMSG_UPDATE_PROGRESS_BAR,          STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleEarlyProccess           );
    OPCODE(CMSG_PING,                         STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleEarlyProccess           );
    OPCODE(SMSG_PONG,                         STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_LOGIN_VERIFY_WORLD,           STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_INIT_WORLD_STATES,            STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_FEATURE_SYSTEM_STATUS,        STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_ACTION_BUTTONS,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_MONSTER_MOVE,                 STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_CLIENTCACHE_VERSION,          STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_TUTORIAL_FLAG,                STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleTutorialFlagOpcode      );
    OPCODE(SMSG_TUTORIAL_FLAGS,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_SET_SELECTION,                STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleSetSelectionOpcode      );
    OPCODE(SMSG_INITIAL_SPELLS,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_SPELL_START,                  STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_SPELL_GO,                     STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_CAST_SPELL,                   STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleCastSpellOpcode         );
    OPCODE(SMSG_CHANNEL_NOTIFY,               STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_CHAT_DOWN,                    STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_CHAT_IS_DOWN,                 STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_CHAT_RECONNECT,               STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_CHANNEL_LIST,                 STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_MESSAGE_CHAT,                 STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
    OPCODE(SMSG_PHASE_SHIFT_CHANGE,           STATUS_NEVER,    PROCESS_INPLACE, &WorldSession::HandleServerSide                   );
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
    OPCODE(CMSG_CHAT_MESSAGE_SAY,             STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode       );
    OPCODE(CMSG_JOIN_CHANNEL,                 STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleJoinChannelOpcode       );
    OPCODE(CMSG_CREATURE_STATS,               STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleCreatureStatsOpcode     );
    OPCODE(SMSG_CREATURE_STATS,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_GAME_OBJECT_STATS,            STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleGameObjectStatsOpcode   );
    OPCODE(SMSG_GAME_OBJECT_STATS,            STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_NAME_CACHE,                   STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleNameCacheOpcode         );
    OPCODE(SMSG_NAME_CACHE,                   STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    //OPCODE(SMSG_COMPRESSED_UPDATE_OBJECT,     STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_PLAYER_MOVE,                  STATUS_AUTHED,   PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(MSG_START_MOVE_FORWARD,            STATUS_LOGGEDIN, PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes         );
    OPCODE(MSG_MOVE_HEARTBEAT,                STATUS_LOGGEDIN, PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes         );
    OPCODE(CMSG_GOSSIP_HELLO,                 STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleGossipHelloOpcode       );
    OPCODE(SMSG_GOSSIP_MESSAGE,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_GOSSIP_COMPLETE,              STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_GOSSIP_POI,                   STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_GOSSIP_SELECT_OPTION,         STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleGossipSelectOptionOpcode);
    OPCODE(CMSG_LIST_INVENTORY,               STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleListInventoryOpcode     );
    OPCODE(CMSG_SPIRIT_HEALER_ACTIVATE,       STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleSpiritHealerActivateOpcode);
    OPCODE(CMSG_ATTACKSWING,                  STATUS_LOGGEDIN, PROCESS_INPLACE,      &WorldSession::HandleAttackSwingOpcode       );
    OPCODE(SMSG_ATTACKSTART,                  STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_ATTACKSTOP,                   STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_ATTACKERSTATEUPDATE,          STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_ATTACKSWING_NOTINRANGE,       STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_ATTACKSWING_BADFACING,        STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_ATTACKSWING_DEADTARGET,       STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_ATTACKSWING_CANT_ATTACK,      STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_ENVIRONMENTALDAMAGELOG,       STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_CLEAR_TARGET,                 STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_COMBAT_EVENT_FAILED,          STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_PERIODICAURALOG,              STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_ENCHANTMENTLOG,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_PARTYKILLLOG,                 STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_PROCRESIST,                   STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_DISPEL_FAILED,                STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_DESTRUCTIBLE_BUILDING_DAMAGE, STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_SWAP_ITEM,                    STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleSwapItem                );
    OPCODE(CMSG_SWAP_INV_ITEM,                STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleSwapInvItemOpcode       );
    OPCODE(CMSG_AUTOEQUIP_ITEM,               STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleAutoEquipItemOpcode     );
    OPCODE(SMSG_LIST_INVENTORY,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_MOTD,                         STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_SAVE_BIND_POINT,              STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_OPENING_CINEMATIC,            STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleNULL                    );
    OPCODE(SMSG_START_CINEMATIC,              STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_INITIALIZE_FACTIONS,          STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_UI_TIME_REQUEST,              STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleUITimeRequestOpcode     );
    OPCODE(SMSG_UI_TIME,                      STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_NEW_WORLD,                    STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_BATTLEFIELD_MGR_QUEUE_REQ,    STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleBattlefieldJoinQueueOpcode);
    OPCODE(SMSG_BATTLEFIELD_MGR_QUEUE_REQ_RESP,STATUS_NEVER,   PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_TRAINER_LIST,                 STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleTrainerListOpcode       );
    OPCODE(SMSG_TRAINER_LIST,                 STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_BUY_TRAINER_SERVICE,		  STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleTrainerBuySpellOpcode   );
    OPCODE(SMSG_TRAINER_BUY_SUCCEEDED,		  STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide			  );
    OPCODE(SMSG_TRAINER_BUY_FAILED,           STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide			  );
    OPCODE(CMSG_NPC_CACHE,                    STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleNpcTextQueryOpcode      );
    OPCODE(SMSG_NPC_CACHE,                    STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_PAGE_TEXT_CACHE,              STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandlePageTextQueryOpcode     );
    OPCODE(SMSG_PAGE_TEXT_CACHE,              STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_PET_NAME_CACHE,               STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandlePetNameQueryOpcode      );
    OPCODE(SMSG_PET_NAME_CACHE,               STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_WORLD_TELEPORT,               STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleWorldTeleportOpcode     );
    OPCODE(MSG_MOVE_TELEPORT_ACK,             STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleMoveTeleportAckOpcode   );
    OPCODE(CMSG_VIOLENCE_LEVEL,               STATUS_AUTHED,   PROCESS_THREADUNSAFE, &WorldSession::HandlePlayerViolenceLevel     );
    //OPCODE(SMSG_WARDEN_DATA,                  STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_AURA_UPDATE,                  STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_AURA_UPDATE_ALL,              STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_PLAY_DANCE,                   STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_STOP_DANCE,                   STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_NOTIFY_DANCE,                 STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_GET_LEARNED_DANCE_MOVES,      STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(MSG_SET_DUNGEON_DIFFICULTY,        STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleSetDungeonDifficultyOpcode);
    OPCODE(SMSG_TALENT_UPDATE,                STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    //OPCODE(SMSG_INSTANCE_DIFFICULTY,          STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_SEND_UNLEARN_SPELLS,          STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_LEARNED_SPELL,				  STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide			  );
    OPCODE(SMSG_EQUIPMENT_SET,                STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_EQUIPMENT_SET_ID,             STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_EQUIPMENT_SET_RESULT,         STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_GAMESPEED_SET,                STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_LOGIN_SETTIMESPEED,           STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_GAMETIME_UPDATE,              STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_SERVERTIME,                   STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_GAMETIME_SET,                 STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_PLAYED_TIME,                  STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandlePlayedTime              );
    OPCODE(CMSG_SET_ACTIONBAR_TOGGLES,        STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleSetActionBarTogglesOpcode);
    OPCODE(CMSG_REQUEST_RAID_INFO,            STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleRequestRaidInfoOpcode   );
    OPCODE(CMSG_QUERY_TIME,                   STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleQueryTimeOpcode         );
    OPCODE(CMSG_QUEST_POI_QUERY,              STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleQuestPOIQueryOpcode     );
    OPCODE(MSG_QUERY_NEXT_MAIL_TIME,          STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleQueryNextMailTime       );
    OPCODE(SMSG_MAIL_COMMAND_RESULT,          STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_MAIL_LIST_RESULT,             STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_RECEIVED_MAIL,                STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_REQUEST_BATTLEFIELD_STATUS,   STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleBattlefieldStatusOpcode );
    OPCODE(MSG_TALENT_WIPE_CONFIRM,           STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleTalentWipeConfirmOpcode );
    OPCODE(CMSG_GET_CALENDER,                 STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarGetCalendar     );
    OPCODE(CMSG_VOICE_SESSION_ENABLE,         STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleVoiceSessionEnableOpcode);
    OPCODE(CMSG_QUESTGIVER_STATUS_QUERY,      STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverStatusQueryOpcode);
    OPCODE(CMSG_QUESTGIVER_STATUS_MULTIPLE_QUERY,STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverStatusMultipleQuery);
    OPCODE(CMSG_QUESTGIVER_HELLO,             STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverHelloOpcode   );
    OPCODE(CMSG_QUEST_CONFIRM_ACCEPT,         STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleQuestConfirmAccept      );
    OPCODE(CMSG_QUESTGIVER_CHOOSE_REWARD,     STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverChooseRewardOpcode);
    OPCODE(CMSG_QUESTGIVER_REQUEST_REWARD,    STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverRequestRewardOpcode);
    OPCODE(CMSG_QUESTGIVER_COMPLETE_QUEST,    STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleQueryQuestsCompletedOpcode);
    OPCODE(CMSG_QUESTGIVER_ACCEPT_QUEST,      STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverAcceptQuestOpcode);
    OPCODE(CMSG_QUESTGIVER_QUERY_QUEST,       STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverQueryQuestOpcode);
    OPCODE(SMSG_QUESTGIVER_STATUS_MULTIPLE,   STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_QUEST_CONFIRM_ACCEPT,         STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_QUESTGIVER_OFFER_REWARD,      STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_QUESTGIVER_REQUEST_ITEMS,     STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_QUESTGIVER_QUEST_INVALID,     STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_QUESTGIVER_QUEST_COMPLETE,    STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_QUESTLOG_FULL,                STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_QUESTGIVER_QUEST_FAILED,      STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_QUESTGIVER_QUEST_DETAILS,     STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_QUESTGIVER_QUEST_LIST,        STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(SMSG_QUESTGIVER_STATUS,            STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide              );
    OPCODE(CMSG_MOVE_TIME_SKIPPED,            STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleMoveTimeSkippedOpcode   );
	OPCODE(SMSG_PLAY_SPELL_VISUAL,			  STATUS_NEVER,    PROCESS_INPLACE,      &WorldSession::HandleServerSide			  );
};
