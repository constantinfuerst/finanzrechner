#include <stdafx.h>

#ifdef COMPILE_WITH_CRYPT
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

	auto* data = new std::string(); data->reserve(STRINGRESERVE);
	FileSource(read, true, new StringSink(*data));
	return data;
}

cryptFileHandler::cryptFileHandler() {
	eraseByte(m_key);
	eraseByte(m_iv);
}

void cryptFileHandler::clear() {
	eraseByte(m_key);
	eraseString(m_password);
}

//checks the validity of a supplied m_password
//relies on generated and present settings file being readable
//returns true should an error occur during file read process
bool cryptFileHandler::checkPassword() {
	const auto settings_fname = std::string(SAVEDIR) + "\\settings.dat";

	//open the encrypted settings file
	std::ifstream fin(settings_fname);
	if (!fin.is_open())
		return true;

	//read the values required for hkdf (actual size 80, base 64 size ~110) and the first few characters
	char ch; std::string start; short count = 0; const short limit = 110 + 3;
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

//sets the internal m_password variable, does not set KEY or IV parameters
void cryptFileHandler::setPassword(const std::string& password_in) {
	m_password = password_in;
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

//generates random m_iv base, m_key salt and info used for secure aes encryption
//based on cryptopp randomness
//returns values as string (requires encoding for printing purposes)
std::string* cryptFileHandler::generateHKDF() {
	using namespace CryptoPP;
	std::string iv_base, key_salt, info;
	auto* data = new std::string();

	AutoSeededRandomPool prng;
	RandomNumberSource(prng, AES::BLOCKSIZE, true, new StringSink(iv_base));
	prng.Reseed();
	RandomNumberSource(prng, AES::MAX_KEYLENGTH, true, new StringSink(key_salt));
	prng.Reseed();
	RandomNumberSource(prng, AES::MAX_KEYLENGTH, true, new StringSink(info));
	prng.Reseed();

	*data = iv_base + key_salt + info;
	return data;
}

//derives m_iv and m_key from supplied random values and m_password
void cryptFileHandler::setHKDF(const std::string_view iv_data, const std::string_view salt_data, const std::string_view info_data) {
	using namespace CryptoPP;

	const HKDF<SHA256> hkdf;
	byte key_salt[AES::MAX_KEYLENGTH];
	byte info[AES::MAX_KEYLENGTH];
	byte iv_base[AES::BLOCKSIZE];

	StringSource(iv_data.data(), true, new ArraySink(iv_base, AES::MAX_KEYLENGTH));
	StringSource(salt_data.data(), true, new ArraySink(key_salt, AES::MAX_KEYLENGTH));
	StringSource(info_data.data(), true, new ArraySink(info, AES::MAX_KEYLENGTH));

	hkdf.DeriveKey(m_key, sizeof(m_key), reinterpret_cast<const byte*>(m_password.c_str()), m_password.size(), key_salt, AES::MAX_KEYLENGTH, info, AES::MAX_KEYLENGTH);
	hkdf.DeriveKey(m_iv, sizeof(m_iv), iv_base, AES::BLOCKSIZE, key_salt, AES::MAX_KEYLENGTH, info, AES::MAX_KEYLENGTH);
}

//supplies actual "setHKDF" with values taken from a string split into string_views
void cryptFileHandler::setHKDF(const std::string& data) {
	const std::string_view iv_str(data.c_str(), 16);
	const std::string_view salt_str(data.c_str() + 16, 32);
	const std::string_view info_str(data.c_str() + 48, 32);
	setHKDF(iv_str, salt_str, info_str);
}

#endif