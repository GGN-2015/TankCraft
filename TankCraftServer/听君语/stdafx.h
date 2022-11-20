#pragma once
//*********************************************************
//
// 预编译头文件
//
//*********************************************************

#define WIN32_LEAN_AND_MEAN  // 从windows库中，排除一些不常用的东西
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE

#include <Windows.h>
#include <mmsystem.h>
#include <pix.h>
#include <shellapi.h>
#include <stdio.h>
#include <tchar.h>
#include <wrl.h>

#include <array>
#include <bitset>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#pragma comment(lib, "winmm")