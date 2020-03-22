#pragma once
#include "stdafx.h"

#ifdef compileWithCrypt
#include "../fh.h"

#include "files.h"
#include "cryptlib.h"
#include "shake.h"
#include "filters.h"
#include "modes.h"
#include "aes.h"
#include "base64.h"
#include "../plainFileHandler/plainFH.h"

class cryptFileHandler : public fileHandler {
private:
	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	byte iv[CryptoPP::AES::BLOCKSIZE];
	
	std::string* decrypt(const QString& fname) const;
	bool encrypt(const QString& fname, const std::string* plaintext) const;
	
public:
	cryptFileHandler();
	~cryptFileHandler() override;
	
	bool writeJSON(QJsonDocument* jdoc, const QString& fname) override;
	QJsonDocument* readJSON(const QString& fname) override;
	void setKEY(const QString& password);
	void eraseKEY();
};
#endif