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
	const auto jsonstr = jdoc->toJson().toStdString();
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
	auto* plain = decrypt(cipher);

	auto* jdoc = new QJsonDocument();
	*jdoc = QJsonDocument::fromJson(plain->c_str());

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
	QFile file(fname + ".dat");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qWarning("Couldn't open file in write mode.");
		return false;
	}

	QTextStream stream(&file);
	stream.setCodec("UTF-8");
	auto scipher = ciphertext->c_str();
	
	stream << scipher;

	file.close();
	return true;
}

std::string* cryptFileHandler::readString(const QString& fname) {
	auto* str = new std::string();
	QFile file(fname + ".dat");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning("Couldn't open file in read mode.");
		return nullptr;
	}

	auto* f = new char[file.size() + 1];
	f[file.size()] = '\0';
	QTextStream stream(&file);
	stream.setCodec("UTF-8");
	stream >> f;
	
	*str = f;
	file.close();
	delete[] f;

	return str;
}

void cryptFileHandler::testWrite() {
	const std::string data = "Hello";
	QString fname = savedir; fname += "test_pwrite";

	if (!writeString(fname, &data))
		DebugBreak();

	auto* str = readString(fname);
	if (str == nullptr)
		DebugBreak();

	if (*str != data)
		DebugBreak();

	delete str;
}

void cryptFileHandler::testCrypto() const {
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

	if (value == false)
		DebugBreak();

	delete cipher, recover;
}

void cryptFileHandler::testCryptoWrite() const {
	QJsonObject json_in;
	json_in["test"] = true;
	const QJsonDocument json_doc(json_in);
	const std::string json_str = json_doc.toJson();

	auto* cipher = encrypt(&json_str);
	///////////////////

	QString fname = savedir; fname += "test_cwrite";
	writeString(fname, cipher);
	auto* recoveredCipher = readString(fname);

	///////////////////
	auto* recover = decrypt(recoveredCipher);

	const QJsonDocument json_rec_doc = QJsonDocument::fromJson(recover->c_str());
	const QJsonObject json_rec = json_rec_doc.object();

	bool value = false;
	value = json_rec["test"].toBool();

	if (value == false)
		DebugBreak();

	delete cipher, recover;
}

#endif