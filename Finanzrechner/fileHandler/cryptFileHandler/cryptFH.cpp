#include "stdafx.h"

#ifdef compileWithCrypt
#include "cryptFH.h"

bool cryptFileHandler::writeJSON(QJsonDocument* jdoc, const QString& fname) {
	const auto jsonstr = jdoc->toJson(QJsonDocument::Compact).toStdString();
	return encrypt(fname.toStdString(), &jsonstr);
}

QJsonDocument* cryptFileHandler::readJSON(const QString& fname) {
	auto* plain = decrypt(fname.toStdString());
	auto* jdoc = new QJsonDocument(QJsonDocument::fromJson(plain->c_str()));

	delete plain;
	return jdoc;
}

cryptFileHandler::cryptFileHandler() {
	eraseByte(key);
	eraseByte(iv);
}

cryptFileHandler::~cryptFileHandler() {
	eraseByte(key);
	eraseString(password);
}

bool cryptFileHandler::checkPassword() {
	std::string settingsFname = std::string(savedir) + "\\data\\03-2020.dat";

	std::ifstream fin(settingsFname);
	if (!fin.is_open())
		return true;
	
	char ch; std::string start; short count = 0; short limit = 110 + 3;
	while (fin >> std::noskipws >> ch) {
		start += ch; count++; if (count == limit) break;
	}

	auto* dec = decrypt(&start);
	std::string result = *dec; delete dec;
	
	if (result[0] == '{' && result[1] == '\"')
		return true;
	return false;
}

void cryptFileHandler::setPassword(const QString& password_in) {
	password = password_in.toStdString();
}

void cryptFileHandler::eraseByte(byte* b) {
	memset(b, 0x00, sizeof(*b));
}

void cryptFileHandler::eraseString(std::string& str) {
	auto* start = &str[0];
	memset(start, 0x00, str.size());
	str.clear();
	str = "";
}

std::string* cryptFileHandler::generateHKDF() {
	using namespace CryptoPP;
	std::string ivBASE, keySALT, info;
	auto* data = new std::string();
	
	AutoSeededRandomPool prng;
	RandomNumberSource(prng, AES::BLOCKSIZE, true, new StringSink(ivBASE));
	prng.Reseed();
	RandomNumberSource(prng, AES::MAX_KEYLENGTH, true, new StringSink(keySALT));
	prng.Reseed();
	RandomNumberSource(prng, AES::MAX_KEYLENGTH, true, new StringSink(info));

	*data = ivBASE + keySALT + info;
	return data;
}

void cryptFileHandler::setHKDF(std::string* dataPLAIN) {
	using namespace CryptoPP;

	HKDF<SHA256> hkdf;
	byte keySALT[AES::MAX_KEYLENGTH];
	byte info[AES::MAX_KEYLENGTH];
	byte ivBASE[AES::BLOCKSIZE];

	StringSource(dataPLAIN->substr(0, 16), true, new ArraySink(ivBASE, AES::MAX_KEYLENGTH));
	StringSource(dataPLAIN->substr(16, 32), true, new ArraySink(keySALT, AES::MAX_KEYLENGTH));
	StringSource(dataPLAIN->substr(48, 32), true, new ArraySink(info, AES::MAX_KEYLENGTH));

	hkdf.DeriveKey(key, sizeof(key), reinterpret_cast<const byte*>(password.c_str()), password.size(), keySALT, AES::MAX_KEYLENGTH, info, AES::MAX_KEYLENGTH);
	hkdf.DeriveKey(iv, sizeof(iv), ivBASE, AES::BLOCKSIZE, keySALT, AES::MAX_KEYLENGTH, info, AES::MAX_KEYLENGTH);
}
#endif