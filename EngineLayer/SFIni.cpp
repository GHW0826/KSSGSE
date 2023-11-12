#include "stdafx.h"
#include "SFIni.h"


#define DEF_PROFILE_NUM_LEN		64 // numeric string length, could be quite long for binary format
#define DEF_PROFILE_THRESHOLD	512 // temporary string length
#define DEF_PROFILE_DELIMITER	_T(",") // default string delimiter
#define DEF_PROFILE_TESTSTRING	_T("{63788286-AE30-4D6B-95DF-3B451C1C79F9}") // Uuid for internal use


/////////////////////////////////////////////////////////////////////////////////
// Constructors & Destructor
/////////////////////////////////////////////////////////////////////////////////
SFIni::SFIni()
	: pszPathName_(nullptr)
{}
SFIni::SFIni(LPCTSTR lpPathName)
	: pszPathName_(nullptr)
{
	SetPathName(lpPathName);
}
SFIni::~SFIni()
{
	if (pszPathName_ != nullptr)
		delete[] pszPathName_;
}


/////////////////////////////////////////////////////////////////////////////////
// Ini File Path Access
/////////////////////////////////////////////////////////////////////////////////

// Assign ini file path name
void SFIni::SetPathName(LPCTSTR lpPathName) {
	if (lpPathName == nullptr) {
		if (pszPathName_ != nullptr)
			*pszPathName_ = _T('\0');
	}
	else {
		if (pszPathName_ != nullptr)
			delete[] pszPathName_;
		pszPathName_ = _tcsdup(lpPathName);
	}
}

// Retrieve ini file path name
DWORD SFIni::GetPathName(LPTSTR lpBuffer, DWORD dwBufSize) const {
	*lpBuffer = _T('\0');
	DWORD dwLen = 0;
	if (lpBuffer != nullptr) {
		_tcsncpy(lpBuffer, pszPathName_, dwBufSize);
		dwLen = (DWORD)_tcslen(lpBuffer);
	}
	else {
		// just calculate the required buffer size
		dwLen = (DWORD)_tcslen(pszPathName_);
	}
	return dwLen;
}


/////////////////////////////////////////////////////////////////////////////////
// Primitive Data Type Access
/////////////////////////////////////////////////////////////////////////////////

// Get a signed integral value
int SFIni::GetInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault, int nBase) const {
	TCHAR sz[DEF_PROFILE_NUM_LEN + 1] = _T("");
	GetString(lpSection, lpKey, sz, DEF_PROFILE_NUM_LEN);
	return *sz == _T('\0') ? nDefault : int(_tcstoul(sz, NULL, __ValidateBase(nBase)));
}

/////////////////////////////////////////////////////////////////////////////////
// Raw String Access
/////////////////////////////////////////////////////////////////////////////////

// Get a profile string value, if the buffer size is not large enough, the result
// may be truncated.
DWORD SFIni::GetString(LPCTSTR lpSection, LPCTSTR lpKey, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDefault) const {

	if (lpBuffer != nullptr)
		*lpBuffer = _T('\0');

	LPTSTR psz = __GetStringDynamic(lpSection, lpKey, lpDefault);
	DWORD dwLen = (DWORD)_tcslen(psz);
	if (lpBuffer != NULL) {
		_tcsncpy(lpBuffer, psz, dwBufSize);
		dwLen = std::min(dwLen, dwBufSize);
	}

	if (psz)
		delete[] psz;
	return dwLen;
}

/////////////////////////////////////////////////////////////////////////////////
// Helper Functions
/////////////////////////////////////////////////////////////////////////////////

// Get a profile string value, return a heap pointer so we do not have to worry
// about the buffer size, however, this function requires the caller to manually
// free the memory.
// This function is the back-bone of all "Getxxx" functions of this class.
LPTSTR SFIni::__GetStringDynamic(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault) const {

	TCHAR* psz = nullptr;
	if (lpSection == nullptr || lpKey == nullptr) {
		// Invalid section or key name, just return the default string
		if (lpDefault == nullptr) {
			// Empty string
			psz = new TCHAR[1];
			*psz = _T('\0');
		}
		else {
			psz = new TCHAR[_tcslen(lpDefault) + 1];
			_tcscpy(psz, lpDefault);
		}
		return psz;
	}

	// Keep enlarging the buffer size until being certain on that the string we
	// retrieved was original(not truncated).
	DWORD dwLen = DEF_PROFILE_THRESHOLD;
	psz = new TCHAR[dwLen + 1];
	DWORD dwCopied = ::GetPrivateProfileString(lpSection, lpKey, lpDefault == nullptr ? _T("") : lpDefault, psz, dwLen, pszPathName_);
	while (dwCopied + 1 >= dwLen) {
		dwLen += DEF_PROFILE_THRESHOLD;
		delete[] psz;
		psz = new TCHAR[dwLen + 1];
		dwCopied = ::GetPrivateProfileString(lpSection, lpKey, lpDefault == nullptr ? _T("") : lpDefault, psz, dwLen, pszPathName_);
	}

	return psz; // !!! Requires the caller to free this memory !!!
}


// Make sure the base will be expected value
int SFIni::__ValidateBase(int nBase) {
	switch (nBase) {
	case BASE_BINARY:
	case BASE_OCTAL:
	case BASE_HEXADECIMAL:
		break;

	default:
		nBase = BASE_DECIMAL;
	}

	return nBase;
}
