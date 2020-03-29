#include "stdafx.h"

#ifdef compileWithCrypt
#include "cryptFH.h"

//encrypt function storing plaintext in an CFB-AES-128 format
//as a base64 encoded string in a specified file name (requiring full path)
bool cryptFileHandler::encrypt(const std::string& fname, const std::string* plaintext) {
	using namespace CryptoPP;

	const std::string writeFile = fname + ".dat";
	std::string cipher; cipher.reserve(plaintext->size());

	//generate and set key and iv values
	auto* hkdf = generateHKDF();
	setHKDF(*hkdf);

	try {
		//encrypt the supplied plaintext
		CFB_Mode<AES>::Encryption e;
		e.SetKeyWithIV(key, sizeof(key), iv);
		StringSource(*plaintext, true, new StreamTransformationFilter(e, new StringSink(cipher)));
		//and store it with the ivbase, salt and info values prepended in base64 encoding to selected file
		StringSource(*hkdf + cipher, true, new Base64Encoder(new FileSink(writeFile.c_str())));

		delete hkdf;
		return true;
	}
	catch (const Exception & e) {
		return false;
	}
}

//decrypt function does not handle files but rather a string
//as to make it usable both by full file and check password functions
//will reserve strings with n amount of space
//where n defined as macro "stringreserve" in stadfx.h
std::string* cryptFileHandler::decrypt(std::string* encoded) {
	using namespace CryptoPP;
	auto* plain = new std::string();
	std::string data, datastr; data.reserve(stringreserve); datastr.reserve(stringreserve);

	//obtain data and iv, key values
	StringSource(*encoded, true, new Base64Decoder(new StringSink(data)));
	setHKDF(data);
	datastr = data.substr(80);

	try {
		//decrypt the data into string "plain"
		CFB_Mode<AES>::Decryption d;
		d.SetKeyWithIV(key, sizeof(key), iv);
		StringSource(data.data(), true, new StreamTransformationFilter(d, new StringSink(*plain)));
	}
	catch (const Exception & e) {
		//if any type of error occurs make sure to delete the encrypted data
		delete plain, plain = nullptr;
	}

	return plain;
}

#endif