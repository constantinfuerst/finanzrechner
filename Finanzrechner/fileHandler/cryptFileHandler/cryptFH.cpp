#include "stdafx.h"

#ifdef compileWithCrypt
#include "cryptFH.h"

QByteArray* cryptFileHandler::decrypt(QByteArray* ba) const {
	using namespace CryptoPP;
	ByteQueue recover, cipher;

	for (auto b : *ba)
		cipher.Put(b);
	
	CBC_Mode<AES>::Decryption dec;
	dec.SetKeyWithIV(key, sizeof(key), iv, sizeof(iv));

	StreamTransformationFilter f2(dec, new Redirector(recover));
	cipher.CopyTo(f2);
	f2.MessageEnd();

	auto* ret_ba = new QByteArray;
	for (auto i = 0; i < recover.CurrentSize(); i++)
		ret_ba->append(recover[i]);
	
	return ret_ba;
}

QByteArray* cryptFileHandler::encrypt(QByteArray* data) const {
	using namespace CryptoPP;
	ByteQueue plain, cipher;

	plain.Put(reinterpret_cast<const byte*>(&data[0]), data->size());

	CBC_Mode<AES>::Encryption enc;
	enc.SetKeyWithIV(key, sizeof(key), iv, sizeof(iv));

	StreamTransformationFilter f1(enc, new Redirector(cipher));
	plain.CopyTo(f1);
	f1.MessageEnd();

	auto* ret_ba = new QByteArray;
	for (auto i = 0; i < cipher.CurrentSize(); i++)
		ret_ba->append(cipher[i]);

	return ret_ba;
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

cryptFileHandler::~cryptFileHandler() {
	memset(key, 0x00, sizeof(key));
	memset(iv, 0x00, sizeof(iv));
}

void cryptFileHandler::setKEY(const QString& password) {
	using namespace CryptoPP;
	SHAKE128 h_shake128;
	SHAKE256 h_shake256;


	h_shake128.Update(reinterpret_cast<const byte*>(password.data()), password.size());
	h_shake128.Final(iv);

	h_shake256.Update(reinterpret_cast<const byte*>(password.data()), password.size());
	h_shake256.Final(key);
}

#endif