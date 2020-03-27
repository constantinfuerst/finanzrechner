#pragma once
#include "stdafx.h"

#ifdef compileWithCrypt
#include "../fh.h"

#include <fstream>
#include <filesystem>

#include <crypto-cpp/cryptlib.h>
#include <crypto-cpp/files.h>
#include <crypto-cpp/filters.h>
#include <crypto-cpp/modes.h>
#include <crypto-cpp/aes.h>
#include <crypto-cpp/sha.h>
#include <crypto-cpp/base64.h>
#include <crypto-cpp/osrng.h>
#include <crypto-cpp/hkdf.h>

/////////
/// Class used to securely store data with CBF-AES-128 encryption,
/// providing the fileHandler interface
/////////

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

	bool writeJSON(QJsonDocument* jdoc, const std::string& fname) override;
	QJsonDocument* readJSON(const std::string& fname) override;
	void setPassword(const std::string& password);
	bool checkPassword();

	bool updatePassword(const std::string& new_password);
};
#endif