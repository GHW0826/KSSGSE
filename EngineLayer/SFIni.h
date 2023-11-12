#pragma once
#ifndef __SFINI__
#define __SFINI__


// Number bases
#define BASE_BINARY			2
#define BASE_OCTAL			8
#define BASE_DECIMAL		10
#define BASE_HEXADECIMAL	16

class SFIni
{
public:
	//-----------------------------------------------------------
	//    Constructors & Destructor
	//-----------------------------------------------------------
	SFIni(); // Default constructor
	SFIni(LPCTSTR lpPathName); // Construct with a given file name
	virtual ~SFIni();

	//-----------------------------------------------------------
	//    Ini File Path Name Access
	//-----------------------------------------------------------
	void SetPathName(LPCTSTR lpPathName); // Specify a new file name
	DWORD GetPathName(LPTSTR lpBuffer, DWORD dwBufSize) const; // Retrieve current file name
#ifdef __AFXWIN_H__
	CString GetPathName() const;
#endif

	//------------------------------------------------------------
	//    Primitive Data Type Access
	//------------------------------------------------------------
	int GetInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault, int nBase = BASE_DECIMAL) const;

	//------------------------------------------------------------
	//    String Access
	//------------------------------------------------------------	
	DWORD GetString(LPCTSTR lpSection, LPCTSTR lpKey, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDefault = nullptr) const;
protected:
	LPTSTR __GetStringDynamic(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault = nullptr) const;

	//------------------------------------------------------------
	//    Helper Functions
	//------------------------------------------------------------
	static int __ValidateBase(int nBase);
protected:
	//------------------------------------------------------------
	//    Member Data
	//------------------------------------------------------------
	LPTSTR pszPathName_; // Stores path of the associated ini file
};


#endif