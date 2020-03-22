#pragma once

#include <QtWidgets>
#include <QtCore>
#include <QDateTime>

#define savedir "C:\\Users\\Admin\\Desktop\\financecalc\\"
#define compileWithCrypt

#ifdef compileWithCrypt
	#ifdef DEBUG
		#pragma comment(lib, "C:\\Projects\\LIBRARIES\\crypto-cpp\\x64\\Output\\Debug\\cryptlib.lib")
	#else
		#pragma comment(lib, "C:\\Projects\\LIBRARIES\\crypto-cpp\\x64\\Output\\Release\\cryptlib.lib")
	#endif
#endif
