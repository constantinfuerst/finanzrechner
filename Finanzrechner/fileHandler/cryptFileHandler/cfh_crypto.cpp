#include "stdafx.h"

#ifdef compileWithCrypt
#include "cryptFH.h"

bool cryptFileHandler::encrypt(const std::string& fname, const std::string* plaintext) {
	using namespace CryptoPP;

	std::string writeFile = fname + ".dat";
	std::string cipher; cipher.reserve(plaintext->size() + 32);
	
	auto* hkdf = generateHKDF();
	setHKDF(hkdf);

	try {
		CFB_Mode<AES>::Encryption e;
		e.SetKeyWithIV(key, sizeof(key), iv);
		StringSource(*plaintext, true, new StreamTransformationFilter(e, new StringSink(cipher)));
		StringSource(*hkdf + cipher, true, new Base64Encoder(new FileSink(writeFile.c_str())));

		delete hkdf;
		return true;
	}
	catch (const Exception & e) {
		qWarning("Encountered an error during decryption of file");
		return false;
	}
}

std::string* cryptFileHandler::decrypt(std::string* data) {
	using namespace CryptoPP;
	auto* plain = new std::string();
	std::string cipher;

	cipher.reserve(data->size());
	StringSource(*data, true, new Base64Decoder(new StringSink(cipher)));

	const std::string datastr = cipher.substr(80);
	std::string hkdf = cipher.substr(0, 80);
	setHKDF(&hkdf);
	
	try {
		CFB_Mode<AES>::Decryption d;
		d.SetKeyWithIV(key, sizeof(key), iv);
		StringSource(datastr, true, new StreamTransformationFilter(d, new StringSink(*plain)));
	}
	catch (const Exception & e) {
		qWarning("Encountered an error during decryption of string, most likely during checkKEY()");
	}

	return plain;
}

std::string* cryptFileHandler::decrypt(const std::string& fname) {
	using namespace CryptoPP;
	auto* plain = new std::string();
	std::string data;
	std::string cipher;

	std::ifstream read(fname + ".dat");
	if (!read.is_open())
		return plain;

	FileSource(read, true, new StringSink(data));
	cipher.reserve(data.size());
	StringSource(data, true, new Base64Decoder(new StringSink(cipher)));
	
	const std::string datastr = cipher.substr(80);
	std::string hkdf = cipher.substr(0, 80);
	setHKDF(&hkdf);
	
	try {
		CFB_Mode<AES>::Decryption d;
		d.SetKeyWithIV(key, sizeof(key), iv);
		StringSource(datastr, true, new StreamTransformationFilter(d, new StringSink(*plain)));
	}
	catch (const Exception & e) {
		qWarning("Encountered an error during decryption of file");
	}

	read.close();
	return plain;
}

#endif
