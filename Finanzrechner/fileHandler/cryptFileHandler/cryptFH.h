#pragma once
#include "stdafx.h"

#ifdef compileWithCrypt
#include "../fh.h"

#include <fstream>
#include <filesystem>

#include <files.h>
#include <cryptlib.h>
#include <filters.h>
#include <modes.h>
#include <aes.h>
#include <sha.h>
#include <base64.h>
#include <osrng.h>
#include <hkdf.h>

class cryptFileHandler : public fileHandler {
private:
	byte key[CryptoPP::AES::MAX_KEYLENGTH];
	byte iv[CryptoPP::AES::BLOCKSIZE];
	std::string password;

	std::string* readFile(const std::string& fname);
	
	std::string* decrypt(std::string* data);
	bool encrypt(const std::string& fname, const std::string* plaintext);

	std::string* generateHKDF();
	void setHKDF(const std::string& data);
	void setHKDF(std::string_view ivDATA, std::string_view saltDATA, std::string_view infoDATA);
	
	static void eraseString(std::string& str);
	static void eraseByte(byte* b);
	
public:
	cryptFileHandler();
	~cryptFileHandler() override;

	bool writeJSON(QJsonDocument* jdoc, const QString& fname) override;
	QJsonDocument* readJSON(const QString& fname) override;
	void setPassword(const QString& password);
	bool checkPassword();

	bool updatePassword(const QString& new_password);
};
#endif