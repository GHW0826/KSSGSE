#pragma once
#ifndef __INETWORKENGINE__
#define __INETWORKENGINE__

#pragma warning( push )
#pragma warning (disable : 4100) 

#ifdef NETWORKENGINE_EXPORTS
#define NETWORKENGINEDECL __declspec(dllexport)
#else
#define NETWORKENGINEDECL __declspec(dllimport)
#endif

class IEngine;
class BasePacket;

class INetworkEngine
{
public:
	INetworkEngine(IEngine* pEngine) 
		: pEngine_(pEngine) 
	{}
	virtual ~INetworkEngine() {};

	virtual bool Init(int ioThreadCnt) = 0;

	virtual bool Start(char* szIP, unsigned short Port) = 0;

	// ���� ����
	virtual bool Shutdown() = 0;

	// Desc : ������ ����
	virtual bool SendRequest(BasePacket* pPacket) = 0;

	// ������ ������ �����ϰ� ���� ��
	virtual bool Disconnect(int Serial) = 0;

	// ���������� Ÿ�̸� �̺�Ʈ�� �߻���ų �� �ִ��� Ȯ��
	virtual bool CheckTimerImpl()																{ return false; } //default timer ���
	virtual long AddTimer(unsigned int TimerID, unsigned int StartTime, unsigned int Period)	{ return false; } //Ÿ�̸� ����
	virtual bool CancelTimer(int timerID)														{ return false; }

	// �ٸ� ������ ���� �����ϵ��� Ŀ���͸� �߰��Ѵ�.
	virtual int AddConnector(int connectorId, char* szIP, unsigned short port)					{ return -1; }

	//���� �����ʸ� ����.
	virtual int AddListener(char* szIP, unsigned short port, bool bDefaultListener = false)		{ return -1; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	IEngine* GetEngine()	{ return pEngine_; }
protected:
	IEngine* pEngine_;
};

#pragma warning( pop )

typedef INetworkEngine* (CREATENETWORKENGINE)(bool Server, IEngine* pEngine);
extern "C" NETWORKENGINEDECL INetworkEngine* CreateNetworkEngine(bool Server, IEngine * pEngine);

#endif