#pragma once
#include "stdafx.h"
#include "../fh.h"

#include "filters.h"
#include "modes.h"
#include "aes.h"

class cryptFileHandler : public fileHandler {
private:
	byte key[CryptoPP::AES::MAX_KEYLENGTH];
	byte iv[CryptoPP::AES::BLOCKSIZE];
	
	QByteArray* decrypt(QByteArray* cipher) const;
	QByteArray* encrypt(QByteArray* data) const;
	
public:
	cryptFileHandler();
	bool writeJSON(QJsonDocument* jdoc, const QString& fname) override;
	QJsonDocument* readJSON(const QString& fname) override;
};