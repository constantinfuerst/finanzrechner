#include "stdafx.h"

#ifdef compileWithCrypt
#include "cryptFH.h"

std::string* cryptFileHandler::encrypt(const std::string* plaintext) const {
	using namespace CryptoPP;
	auto* cipher = new std::string();
	try {
		CFB_Mode< AES >::Encryption e;
		e.SetKeyWithIV(key, sizeof(key), iv);
		StringSource(*plaintext, true, new StreamTransformationFilter(e, new FileSink()));
		
		return cipher;
	}
	catch (const Exception & e) {
		return cipher;
	}
}

std::string* cryptFileHandler::decrypt(const std::string* ciphertext) const {
	using namespace CryptoPP;
	auto* plain = new std::string();
	try {
		CFB_Mode< AES >::Decryption d;
		d.SetKeyWithIV(key, sizeof(key), iv);
		StringSource s(*ciphertext, true, new StreamTransformationFilter(d, new StringSink(*plain)));

		return plain;
	}
	catch (const Exception & e) {
		return plain;
	}
}

bool cryptFileHandler::writeJSON(QJsonDocument* jdoc, const QString& fname) {
	const auto jsonstr = jdoc->toJson(QJsonDocument::Compact).toStdString();
	auto* cipher = encrypt(&jsonstr);

	if (writeString(fname, cipher)) {
		delete cipher;
		return true;
	}
	delete cipher;
	return false;
}

QJsonDocument* cryptFileHandler::readJSON(const QString& fname) {
	auto* cipher = readString(fname);
	if (cipher == nullptr) return nullptr;
	auto* plain = decrypt(cipher);
	auto* error = new QJsonParseError;
	
	auto* jdoc = new QJsonDocument(QJsonDocument::fromJson(plain->c_str(), error));

	delete cipher, plain;
	return jdoc;
}

cryptFileHandler::cryptFileHandler() {
	eraseKEY();
}

cryptFileHandler::~cryptFileHandler() {
	eraseKEY();
}

void cryptFileHandler::setKEY(const QString& password) {
	using namespace CryptoPP;
	SHAKE128 h_shake128;

	h_shake128.Update(reinterpret_cast<const byte*>(password.data()), password.size());
	h_shake128.Final(key);

	h_shake128.Update(key, 16);
	h_shake128.Final(iv);
}

void cryptFileHandler::eraseKEY() {
	memset(key, 0x00, sizeof(key));
	memset(iv, 0x00, sizeof(iv));
}

bool cryptFileHandler::writeString(const QString& fname, const std::string* ciphertext) {
	const std::string fnameStr = fname.toStdString() + ".dat";
	std::ofstream ostream(fnameStr);
	if (!ostream.is_open()) {
		qWarning("Couldn't open file in write mode.");
		return false;
	}

	ostream << ciphertext->c_str();

	ostream.close();
	return true;
}

std::string* cryptFileHandler::readString(const QString& fname) {
	const std::string fnameStr = fname.toStdString() + ".dat";
	std::ifstream istream(fnameStr);
	

	if (!istream.is_open()) {
		qWarning("Couldn't open file in read mode.");
		return nullptr;
	}

	istream.seekg(0, std::ios::end);
	size_t len = istream.tellg();
	istream.seekg(0);
	auto* str = new std::string(len + 1, '\0');
	istream.read(&(*str)[0], len);
	
	istream.close();
	return str;
}

#endif