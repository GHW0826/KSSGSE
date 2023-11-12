#pragma once
#ifndef __SFSINGLETON__
#define __SFSINGLETON__

#include "ACEHeader.h"
#include "SFRPCGateway.h"
#include "SFLogicGateway.h"
#include "LogicEntry.h"
#include "SFPacketSendGateway.h"
#include "SFObjectPool.h"
#include "SFPacket.h"

using RPCGatewaySingleton	= ACE_Singleton<SFRPCGateway, ACE_Null_Mutex>;
using LogicGatewaySingleton = ACE_Singleton<SFLogicGateway, ACE_Null_Mutex>;
using LogicEntrySingleton	= ACE_Singleton<LogicEntry, ACE_Null_Mutex>;
using PacketSendSingleton	= ACE_Singleton<SFPacketSendGateway, ACE_Null_Mutex>;
using PacketPoolSingleton	= ACE_Singleton<SFObjectPool<SFPacket>, ACE_Null_Mutex>;

#endif
