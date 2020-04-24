#pragma once

#include <QtQuick>
#include <QtCore>
#include <QDateTime>

#include <string>
#include <vector>
#include <fstream>
#include <array>
#include <filesystem>

constexpr int STRINGRESERVE = 2000;
constexpr char SAVEDIR[] = "C:\\Users\\Admin\\Desktop\\financecalc\\";
#define COMPILE_WITH_CRYPT

#ifdef COMPILE_WITH_CRYPT
	#ifdef DEBUG
		#pragma comment(lib, "C:\\Projects\\LIBRARIES\\crypto-cpp\\x64\\Output\\Debug\\cryptlib.lib")
	#else
		#pragma comment(lib, "C:\\Projects\\LIBRARIES\\crypto-cpp\\x64\\Output\\Release\\cryptlib.lib")
	#endif
#endif
