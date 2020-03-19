#pragma once
#include "stdafx.h"

#include "cryptlib.h"
#include "filters.h"
#include "files.h"
#include "modes.h"
#include "hex.h"
#include "aes.h"
#include "osrng.h"

class cryptFileHandler {
private:
	byte key[CryptoPP::AES::MAX_KEYLENGTH];
	byte iv[CryptoPP::AES::BLOCKSIZE];
	
	QByteArray* decrypt(QByteArray* cipher);
	QByteArray* encrypt(QByteArray* data);
	
public:
	cryptFileHandler();
	
	bool writeJSON(QJsonDocument* jdoc, const QString& fname);
	QJsonDocument* readJSON(const QString& fname);

	static cryptFileHandler& get();
};