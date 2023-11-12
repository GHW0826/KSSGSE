#pragma once
#ifndef __SFCASUALGAMEDISPATCHER__
#define __SFCASUALGAMEDISPATCHER__
#include "SFLogicDispatcher.h"

class SFCasualGameDispatcher : public SFLogicDispatcher
{
public:
	SFCasualGameDispatcher(void);
	virtual ~SFCasualGameDispatcher(void);

	virtual void Dispatch(BasePacket* pPacket) override;
	virtual bool CreateLogicSystem(ILogicEntry* pLogicEntry) override;
	virtual bool ShutDownLogicSystem() override;
	
	virtual bool AddRPCService(IRPCService* pService) override;

private:
	static bool bLogicEnd_;
	int nLogicThreadCnt_;
	int logicThreadGroupId_;
	int rpcThreadGroupId_;
	IRPCService* pRPCService_;

	static void LogicThreadProc(void* Args);
	static void RPCThreadProc(void* Args);
};

#endif

