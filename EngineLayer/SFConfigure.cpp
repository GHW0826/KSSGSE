#include "stdafx.h"
#include "SFConfigure.h"

SFConfigure::SFConfigure(void)
{}
SFConfigure::~SFConfigure(void)
{}


bool SFConfigure::Initialize() {
	ConfigureInfo_.Clear();
	return true;
}

bool SFConfigure::Read(const WCHAR* pFileName) {

	// �������� ���丮�� ���� ���丮�� �����ϰ�, �α׸� �����.
	/*
	WCHAR szFilePath[MAX_PATH] = { 0, };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);

	WCHAR* path = SFUtil::ExtractPathInfo(szFilePath, SFUtil::PATH_DIR);
	SetCurrentDirectory(path);

	// LOG(INFO) << "SetCurrentDirectory. " << path;
	// google::FlushLogFiles(google::GLOG_INFO);

	IXMLStreamReader* pReader = SFXMLStreamReader::CreateXmlStreamReader(pFileName);
	if (nullptr == pReader || pReader->m_isXmlFileLoadSuccess == false) {
		return false;
	}

	pReader->Read(pFileName, *this);

	if (pReader)
		delete pReader;
		*/
	return true;
}

bool SFConfigure::Write(const WCHAR* pFileName) {
	/*
	IXMLStreamWriter* pWriter = SFXMLStreamWriter::CreateXmlStreamWriter(pFileName);
	if (nullptr == pWriter)
		return false;

	pWriter->Write(pFileName, *this);

	if (pWriter)
		delete pWriter;
		*/
	return true;
}