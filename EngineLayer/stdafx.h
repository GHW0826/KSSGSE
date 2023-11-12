// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

// TODO 추가 종속성
// libprotobuf.lib;liblzf.lib;zlib.lib;libglog.lib;aced.lib;
// commonlayer.lib;msgpackd.lib;avrocpp_s.lib;liblfds611.lib;http-parser.lib;httpxx.lib

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <tchar.h>
#include <map>

// TODO: reference additional headers your program requires here
#include <windows.h>
#include <string>

#include "Macro.h"
#include "SFConstant.h"
#include "SFSingleton.h"
#include "SFStructure.h"
#include "BasePacket.h"


#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES

/*

#include "CommonHeader.h"

#include "BasePacket.h"

#include "Macro.h"
#include "SFString.h"
#include "SFCompressLzf.h"
#include "SFSinglton.h"
#include "SFPacketProtocol.H"
#include "SFBridgeThread.h"

#include "glog/logging.h"

*/