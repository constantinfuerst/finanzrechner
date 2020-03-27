#include "stdafx.h"

#ifdef compileWithCrypt
#include "cryptFH.h"

//composer to write a json document to a file
//this overwrite uses the encryption functions
//fname does not require a filetype ending
bool cryptFileHandler::writeJSON(QJsonDocument* jdoc, const std::string& fname) {
	const auto jsonstr = jdoc->toJson(QJsonDocument::Compact).toStdString();
	return encrypt(fname, &jsonstr);
}

//composer to read a json document from file
//this overwrite reads a file encrypted by the crypto functions
//fname does not require a filetype ending
QJsonDocument* cryptFileHandler::readJSON(const std::string& fname) {
	auto* data = readFile(fname);
	auto* plain = decrypt(data);
	auto* jdoc = new QJsonDocument(QJsonDocument::fromJson(plain->c_str()));

	delete plain, data;
	return jdoc;
}

//reads a file to string* using std::filestream and cryptopp::filesource
std::string* cryptFileHandler::readFile(const std::string& fname) {
	using namespace CryptoPP;
	std::ifstream read(fname + ".dat");
	if (!read.is_open())
		return nullptr;

	auto* data = new std::string();
	FileSource(read, true, new StringSink(*data));
}

cryptFileHandler::cryptFileHandler() {
	eraseByte(key);
	eraseByte(iv);
}

cryptFileHandler::~cryptFileHandler() {
	eraseByte(key);
	eraseString(password);
}

//checks the validity of a supplied password
//relies on generated and present settings file being readable
//returns true should an error occur during file read process
bool cryptFileHandler::checkPassword() {
	std::string settingsFname = std::string(savedir) + "\\settings.dat";

	//open the encrypted settings file
	std::ifstream fin(settingsFname);
	if (!fin.is_open())
		return true;

	//read the values required for hkdf (actual size 80, base 64 size ~110) and the first few characters
	char ch; std::string start; short count = 0; short limit = 110 + 3;
	while (fin >> std::noskipws >> ch) {
		start += ch; count++; if (count == limit) break;
	}

	//decrypt this first sequence of the file
	auto* dec = decrypt(&start);
	std::string result = *dec; delete dec;

	//check if we find valid json data structure start
	if (result[0] == '{' && result[1] == '\"')
		return true;
	return false;
}

//sets the internal password variable, does not set KEY or IV parameters
void cryptFileHandler::setPassword(const std::string& password_in) {
	password = password_in;
}

//zeros out a byte sequence to fully erase memory traces
void cryptFileHandler::eraseByte(byte* b) {
	memset(b, 0x00, sizeof(*b));
}

//zeros out a char sequence (as std::string) to fully erase memory traces
void cryptFileHandler::eraseString(std::string& str) {
	auto* start = &str[0];
	memset(start, 0x00, str.size());
	str.clear();
	str = "";
}

//generates random iv base, key salt and info used for secure aes encryption
//based on cryptopp randomness
//returns values as string (requires encoding for printing purposes)
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
	prng.Reseed();

	*data = ivBASE + keySALT + info;
	return data;
}

//derives iv and key from supplied random values and password
void cryptFileHandler::setHKDF(std::string_view ivDATA, std::string_view saltDATA, std::string_view infoDATA) {
	using namespace CryptoPP;

	HKDF<SHA256> hkdf;
	byte keySALT[AES::MAX_KEYLENGTH];
	byte info[AES::MAX_KEYLENGTH];
	byte ivBASE[AES::BLOCKSIZE];

	StringSource(ivDATA.data(), true, new ArraySink(ivBASE, AES::MAX_KEYLENGTH));
	StringSource(saltDATA.data(), true, new ArraySink(keySALT, AES::MAX_KEYLENGTH));
	StringSource(infoDATA.data(), true, new ArraySink(info, AES::MAX_KEYLENGTH));

	hkdf.DeriveKey(key, sizeof(key), reinterpret_cast<const byte*>(password.c_str()), password.size(), keySALT, AES::MAX_KEYLENGTH, info, AES::MAX_KEYLENGTH);
	hkdf.DeriveKey(iv, sizeof(iv), ivBASE, AES::BLOCKSIZE, keySALT, AES::MAX_KEYLENGTH, info, AES::MAX_KEYLENGTH);
}

//supplies actual "setHKDF" with values taken from a string split into string_views
void cryptFileHandler::setHKDF(const std::string& data) {
	const std::string_view iv_str(data.c_str(), 16);
	const std::string_view salt_str(data.c_str() + 16, 32);
	const std::string_view info_str(data.c_str() + 48, 32);
	setHKDF(iv_str, salt_str, info_str);
}

#endif