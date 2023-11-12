#pragma once
#include "EchoCallback.h"
#include "SFNetworkEntry.h"
#include "SFPacketProtocol.h"
#include "ACEHeader.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "ACEHeader.h"
#include "Macro.h"

void ProcessInput() {
	//EchoInputThread���� ������ �Է��� �޾� ������ ��Ŷ�� ����.
	// ��Ŷ ���������� JSON���� �����Ͽ����Ƿ� SFJsonPacket ��ü�� ������ ������ �Է��� ��� ����
	// ������ ���� �����͸� �״�� Ŭ�� ���ٶ��� Ŭ��� �ݹ� ��ü�� EchoCallback���� ������ ���� �����͸� ó��.
	int inputThreadID = ACE_Thread_Manager::instance()->spawn_n(1, (ACE_THR_FUNC)EchoInputThread, NULL, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY);
	SFASSERT(inputThreadID != -1);

	// 
	while (SFNetworkEntry::GetInstance()->IsConnected() && g_bExit == false) {
		SFNetworkEntry::GetInstance()->Update();
		Sleep(1);
	}
	ACE_Thread_Manager::instance()->wait_grp(inputThreadID);
}


int _tmain(int argc, _TCHAR* argv[])
{
	EchoCallback* pCallback = new EchoCallback();

	// SFNetworkEntry�� ���������� SFEngine ��ü�� ��� Ŭ���̾�Ʈ�� ���� ����� �߰��� ���� Ŭ����
	// ������ ���������� ��Ŷ ���������� �����ϰ� �ݹ� ��ü�� ������ ��ü�� �Ѱ���.
	// Run�� ȣ���� ��Ʈ��ũ ������ �õ�.
	SFNetworkEntry::GetInstance()->Initialize(pCallback, new SFPacketProtocol<SFJsonProtocol>);
	SFNetworkEntry::GetInstance()->Run();

	// ���⼭ ������ �Է��� ó���ϰ� ��Ʈ��ũ �̺�Ʈ�� �������� ���� �ӽ� ���� �Լ�
	// �޼ҵ� ȣ���� ���� �����忡�� �ϹǷ� ������ �����͸� �����ϱ� ���ؼ��� ������ �����带 �����ؾ� �Ѵ�.
	// ������ ���̺귯���� �����ϱ� ���� ACE ���̺귯�� Ȱ��
	// loop�� sleep(1)�� ���� ������ CPU ���� ���� ����. GUI �ڵ忡���� sleep�� ����� ��.
	ProcessInput();

	SFNetworkEntry::GetInstance()->ShutDown();

	return 0;
}