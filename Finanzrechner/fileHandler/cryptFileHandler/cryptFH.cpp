#include "stdafx.h"
#include "cryptFH.h"

QByteArray* cryptFileHandler::decrypt(QByteArray* cipher) const {
	using namespace CryptoPP;
	auto* recover = new QByteArray();
	
	CBC_Mode<AES>::Decryption dec;
	dec.SetKeyWithIV(key, sizeof(key), iv, sizeof(iv));

	// Recovered text will be less than cipher text
	recover->resize(cipher->size());
	ArraySink rs(reinterpret_cast<byte*>(&recover[0]), recover->size());

	ArraySource(reinterpret_cast<byte*>(cipher->data()), cipher->size(), true,
		new StreamTransformationFilter(dec, new Redirector(rs)));

	// Set recovered text length now that its known
	recover->resize(rs.TotalPutLength());

	return recover;
}

QByteArray* cryptFileHandler::encrypt(QByteArray* data) const {
	using namespace CryptoPP;
	auto* cipher = new QByteArray();

	CBC_Mode<AES>::Encryption enc;
	enc.SetKeyWithIV(key, sizeof(key), iv, sizeof(iv));

	// Make room for padding
	cipher->resize(data->size() + AES::BLOCKSIZE);
	ArraySink cs(reinterpret_cast<byte*>(&cipher[0]), cipher->size());

	ArraySource(reinterpret_cast<byte*>(data->data()), data->size(), true,
		new StreamTransformationFilter(enc, new Redirector(cs)));

	// Set cipher text length now that its known
	cipher->resize(cs.TotalPutLength());

	return cipher;
}

bool cryptFileHandler::writeJSON(QJsonDocument* jdoc, const QString& fname) {
	QFile file (fname + ".dat");
	if (!file.open(QIODevice::WriteOnly)) {
		qWarning("Couldn't open file in write mode.");
		return false;
	}

	auto* bytes = new QByteArray(jdoc->toBinaryData());
	auto* cipher = encrypt(bytes);

	file.write(*cipher);
	file.close();
	
	delete cipher, bytes;
	return true;
}

QJsonDocument* cryptFileHandler::readJSON(const QString& fname) {
	QFile file(fname + ".dat");
	if (!file.open(QIODevice::ReadOnly)) {
		qWarning("Couldn't open file in read mode.");
		return false;
	}

	auto* cipher = new QByteArray(file.readAll());
	auto* bytes = decrypt(cipher);
	auto* json = new QJsonDocument();
	json->fromBinaryData(*bytes);
	
	file.close();
	delete cipher, bytes;
	return json;
}

cryptFileHandler::cryptFileHandler() {
	memset(key, 0x00, sizeof(key));
	memset(iv, 0x00, sizeof(iv));
}
