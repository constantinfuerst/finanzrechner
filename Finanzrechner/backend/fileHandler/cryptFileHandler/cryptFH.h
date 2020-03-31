#pragma once
#include "stdafx.h"

#ifdef COMPILE_WITH_CRYPT
#include "../fh.h"

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

class cryptFileHandler final : public fileHandler {
private:
	byte m_key[CryptoPP::AES::MAX_KEYLENGTH];
	byte m_iv[CryptoPP::AES::BLOCKSIZE];
	std::string m_password;

	static std::string* readFile(const std::string& fname);

	std::string* decrypt(std::string* data);
	bool encrypt(const std::string& fname, const std::string* plaintext);

	static std::string* generateHKDF();
	void setHKDF(const std::string& data);
	void setHKDF(std::string_view iv_data, std::string_view salt_data, std::string_view info_data);

	static void eraseString(std::string& str);
	static void eraseByte(byte* b);

public:
	cryptFileHandler();

	bool writeJSON(QJsonDocument* jdoc, const std::string& fname) override;
	QJsonDocument* readJSON(const std::string& fname) override;
	void setPassword(const std::string& password_in);
	bool checkPassword();
	//it is encouraged to call "clear" before destructing element to fully erase memory traces
	//for m_password and m_key/m_iv values
	void clear();

	bool updatePassword(const std::string& new_password);
};
#endif