#include "stdafx.h"

#ifdef compileWithCrypt
#include "cryptFH.h"

std::string* cryptFileHandler::encrypt(const std::string* plaintext) const {
	using namespace CryptoPP;
	auto* cipher = new std::string();
	try {
		CFB_Mode< AES >::Encryption e;
		e.SetKeyWithIV(key, sizeof(key), iv);
		StringSource(*plaintext, true, new StreamTransformationFilter(e, new StringSink(*cipher)));
		
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
	QFile file (fname + ".txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qWarning("Couldn't open file in write mode.");
		return false;
	}

	const std::string json = QString(jdoc->toJson()).toUtf8();
	auto* cipher = encrypt(&json);

	QTextStream outputStream(&file);
	outputStream << cipher->c_str();
	file.close();
	
	delete cipher;
	return true;
}

QJsonDocument* cryptFileHandler::readJSON(const QString& fname) {
	QFile file(fname + ".txt");
	if (!file.open(QIODevice::ReadOnly| QIODevice::Text)) {
		qWarning("Couldn't open file in read mode.");
		return false;
	}

	QTextStream inputStream(&file);
	std::string cipher = inputStream.readAll().toStdString();
	auto* plain = decrypt(&cipher);
	auto* json = new QJsonDocument();
	json->fromJson(plain->c_str());
	
	file.close();
	delete plain;
	return json;
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

void cryptFileHandler::test() {
	QJsonObject json_in;
	json_in["test"] = true;
	const QJsonDocument json_doc(json_in);
	const std::string json_str = json_doc.toJson();
	
	auto* cipher = encrypt(&json_str);
	auto* recover = decrypt(cipher);

	const QJsonDocument json_rec_doc = QJsonDocument::fromJson(recover->c_str());
	const QJsonObject json_rec = json_rec_doc.object();

	bool value = false;
	value = json_rec["test"].toBool();
}

#endif