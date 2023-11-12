#pragma once
#ifndef __PROACTORSERVICE__
#define __PROACTORSERVICE__

#include <ace/Asynch_IO.h>
#include <ACE/OS.h>
#include <ACE/Message_Block.h>
#include "ISession.h"
#define WIN32

class InterlockedValue;

class ProactorService : public ACE_Service_Handler, public ISession
{
public:
	ProactorService();
	virtual ~ProactorService(void);

	virtual void open(ACE_HANDLE h, ACE_Message_Block& MessageBlock) override;
	virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result& Result) override;
	virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result& Result) override;
	virtual void handle_time_out(const ACE_Time_Value& tv, const void* act = 0) override;

	void PostRecv();

	virtual bool SendRequest(BasePacket* pPacket) override;
	void SendInternal(char* pBuffer, int BufferSize);
	void Disconnect();

	void SetSessionDesc(_SessionDesc& desc) { sessionDesc_ = desc; };
	void SetSerial(int serial)				{ serial_ = serial; }
	int GetSerial()							{ return serial_; }
protected:
	void RegisterTimer();
	void UnregisterTimer();
	void ReserveClose();

private:
	ACE_Asynch_Write_Stream asyncWriter_;
	ACE_Asynch_Read_Stream asyncReader_;

	int serial_;
	_SessionDesc sessionDesc_;
	InterlockedValue* pTimerLock_;

	bool bServiceCloseFlag_;
};


#endif
