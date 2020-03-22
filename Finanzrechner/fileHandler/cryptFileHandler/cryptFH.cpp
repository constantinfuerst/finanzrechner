#include "stdafx.h"

#ifdef compileWithCrypt
#include "cryptFH.h"

bool cryptFileHandler::encrypt(const QString& fname, const std::string* plaintext) const {
	using namespace CryptoPP;

	std::string ivstr; ivstr.reserve(32);
	AutoSeededRandomPool prng;
	RandomNumberSource(prng, AES::BLOCKSIZE, true, new ArraySink(const_cast<byte*>(&iv[0]), AES::BLOCKSIZE));
	ArraySource(iv, AES::BLOCKSIZE, true, new StringSink(ivstr));
	
	std::string writeFile = fname.toStdString() + ".dat";
	std::string cipher; cipher.reserve(plaintext->size() + 32);
	
	try {
		CFB_Mode<AES>::Encryption e;
		e.SetKeyWithIV(key, sizeof(key), iv);
		StringSource(*plaintext, true, new StreamTransformationFilter(e, new StringSink(cipher)));
		StringSource(ivstr + cipher, true, new Base64Encoder(new FileSink(writeFile.c_str())));
		
		return true;
	}
	catch (const Exception & e) {
		return false;
	}
}

std::string* cryptFileHandler::decrypt(std::string* data) const {
	using namespace CryptoPP;
	auto* plain = new std::string();
	std::string cipher;
	
	cipher.reserve(data->size());
	StringSource(*data, true, new Base64Decoder(new StringSink(cipher)));

	const std::string ivstr = cipher.substr(0, 16);
	const std::string datastr = cipher.substr(17);
	
	StringSource(ivstr, true, new ArraySink(const_cast<byte*>(&iv[0]), AES::BLOCKSIZE));
	
	try {
		CFB_Mode<AES>::Decryption d;
		d.SetKeyWithIV(key, sizeof(key), iv);
		StringSource(datastr, true, new StreamTransformationFilter(d, new StringSink(*plain)));

		return plain;
	}
	catch (const Exception & e) {
		return plain;
	}
}

std::string* cryptFileHandler::decrypt(const QString& fname) const {
	using namespace CryptoPP;
	auto* plain = new std::string();
	std::string data, cipher;
	
	std::ifstream read(fname.toStdString() + ".dat");
	if (!read.is_open())
		return plain;
	
	FileSource(read, true, new StringSink(data));
	cipher.reserve(data.size());
	StringSource(data, true, new Base64Decoder(new StringSink(cipher)));

	const std::string ivstr = cipher.substr(0, 16);
	const std::string datastr = cipher.substr(17);
	
	StringSource(ivstr, true, new ArraySink(const_cast<byte*>(&iv[0]), AES::BLOCKSIZE));
	
	try {
		CFB_Mode<AES>::Decryption d;
		d.SetKeyWithIV(key, sizeof(key), iv);
		StringSource(datastr, true, new StreamTransformationFilter(d, new StringSink(*plain)));

		read.close();
		return plain;
	}
	catch (const Exception & e) {
		read.close();
		return plain;
	}
}

bool cryptFileHandler::writeJSON(QJsonDocument* jdoc, const QString& fname) {
	const auto jsonstr = jdoc->toJson(QJsonDocument::Compact).toStdString();
	return encrypt(fname, &jsonstr);
}

QJsonDocument* cryptFileHandler::readJSON(const QString& fname) {
	auto* plain = decrypt(fname);
	auto* jdoc = new QJsonDocument(QJsonDocument::fromJson(plain->c_str()));

	delete plain;
	return jdoc;
}

cryptFileHandler::cryptFileHandler() {
	eraseKEY();
}

cryptFileHandler::~cryptFileHandler() {
	eraseKEY();
}

bool cryptFileHandler::checkKEY() const {
	std::string settingsFname = std::string(savedir) + "settings.dat";

	std::ifstream fin(settingsFname);
	if (!fin.is_open())
		return false;
	
	char ch; std::string start; short count = 0; short limit = 24 + 3;
	while (fin >> std::noskipws >> ch) {
		start += ch; count++; if (count == limit) break;
	}

	auto* dec = decrypt(&start);
	std::string result = *dec; delete dec;
	
	if (result[0] == -98 && result[1] == 13)
		return true;
	return false;
}

void cryptFileHandler::setKEY(const QString& password) {
	using namespace CryptoPP;
	SHAKE256 h_shake256;

	h_shake256.Update(reinterpret_cast<const byte*>(password.data()), password.size());
	h_shake256.Final(key);
}

void cryptFileHandler::eraseKEY() {
	memset(key, 0x00, sizeof(key));
	memset(iv, 0x00, sizeof(iv));
}

#endif