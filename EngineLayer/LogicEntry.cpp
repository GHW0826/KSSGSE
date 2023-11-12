#include "stdafx.h"
#include "Enginestructure.h"
#include "LogicEntry.h"
#include "ILogicEntry.h"

LogicEntry::LogicEntry(void)
	: pLogicEntry_(nullptr)
{}
LogicEntry::~LogicEntry(void)
{}

bool LogicEntry::ProcessPacket(BasePacket* pPacket) {

	if (pLogicEntry_) {
		_SessionDesc& desc = pPacket->GetSessionDesc();
		if (desc.sessionType <= 0)
			pLogicEntry_->ProcessPacket(pPacket);
		else
			pLogicEntry_->ProcessConnectorPacket(pPacket);
	}

	return true;
}

bool LogicEntry::DestroyLogic() {
	if (pLogicEntry_)
		delete pLogicEntry_;
	return true;
}