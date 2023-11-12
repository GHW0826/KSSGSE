#pragma once
#ifndef __LOGICENTRY__
#define __LOGICENTRY__

class ILogicEntry;

class LogicEntry
{
public:
	LogicEntry(void);
	virtual ~LogicEntry(void);

	virtual bool ProcessPacket(BasePacket* pPacket);
	virtual bool DestroyLogic();

	void SetLogic(ILogicEntry* pLogic) {
		pLogicEntry_ = pLogic;
	}
private:
	ILogicEntry* pLogicEntry_;
};

#endif

