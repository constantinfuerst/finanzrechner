#pragma once
#include "stdafx.h"

#ifdef compileWithCrypt
#include "../fh.h"

#include "cryptlib.h"
#include "shake.h"
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
	~cryptFileHandler() override;
	
	bool writeJSON(QJsonDocument* jdoc, const QString& fname) override;
	QJsonDocument* readJSON(const QString& fname) override;
	void setKEY(const QString& password);
};
#endif