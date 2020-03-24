#pragma once
#include "stdafx.h"
#include "../fh.h"

/////////
/// Plain file handler only for cases where compiling with cryptopp is not viable
/// to only compile plainFileHandler comment out the "compileWithCrypto" macro in stadfx.h
/// then replace any cryptoFileHandler specific actions and replace the definition
/////////

class plainFileHandler : public fileHandler {
public:
	~plainFileHandler() override = default;
	bool writeJSON(QJsonDocument* jdoc, const QString& fname) override;
	QJsonDocument* readJSON(const QString& fname) override;
};