#include "stdafx.h"

#ifdef compileWithCrypt
#include "cryptFH.h"

bool cryptFileHandler::encrypt(const QString& fname, const std::string* plaintext) const {
	using namespace CryptoPP;
	std::string writeFile = fname.toStdString() + ".dat";
	std::string cipher; cipher.reserve(plaintext->size());
	
	try {
		CFB_Mode< AES >::Encryption e;
		e.SetKeyWithIV(key, sizeof(key), iv);
		StringSource(*plaintext, true, new StreamTransformationFilter(e, new StringSink(cipher)));
		StringSource(cipher, true, new Base64Encoder(new FileSink(writeFile.c_str())));
		
		return true;
	}
	catch (const Exception & e) {
		return false;
	}
}

std::string* cryptFileHandler::decrypt(const QString& fname) const {
	using namespace CryptoPP;
	auto* plain = new std::string();
	std::string data;
	std::string cipher;
	
	std::ifstream read(fname.toStdString() + ".dat");
	if (!read.is_open())
		return plain;
	
	FileSource(read, true, new StringSink(data));
	cipher.reserve(data.size());
	StringSource(data, true, new Base64Decoder(new StringSink(cipher)));
	
	try {
		CFB_Mode< AES >::Decryption d;
		d.SetKeyWithIV(key, sizeof(key), iv);
		StringSource(cipher, true, new StreamTransformationFilter(d, new StringSink(*plain)));

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

#endif