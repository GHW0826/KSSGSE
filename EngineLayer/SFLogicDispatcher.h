#pragma once
#ifndef __SFLOGICDISPATCHER__
#define __SFLOGICDISPATCHER__

#include "ILogicDispatcher.h"

class SFLogicDispatcher : public ILogicDispatcher
{
public:
	SFLogicDispatcher() {}
	virtual ~SFLogicDispatcher() {}

	static bool ReleasePacket(BasePacket* pPacket);
};

#endif
