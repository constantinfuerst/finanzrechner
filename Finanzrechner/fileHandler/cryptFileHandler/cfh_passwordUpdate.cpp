#include "stdafx.h"

#ifdef compileWithCrypt
#include "cryptFH.h"

//iterates over every file in savedir (specified by "savedir" macro in stadfx.h)
//decrypts file with original encryption password and then re-encrypts it with supplied new password
//also generates new info, salt and iv values for every file
//depending on amount of files this is a very costly function and may take extraordinarily long to complete
bool cryptFileHandler::updatePassword(const QString& new_password) {
	namespace fs = std::filesystem;
	const QString old_password = QString::fromStdString(password);

	if (!checkPassword())
		return false;

	//iterate over all elements in directory
	for (auto& p : fs::recursive_directory_iterator(savedir)) {
		//set the password as the old one
		setPassword(old_password);
		//if the found element is a file
		if (p.is_regular_file()) {
			//obtain file path
			const std::string fname = p.path().generic_string();
			const std::string clearFname = fname.substr(0, fname.size() - 4);

			//read and decrypt file
			auto* data = readFile(fname);
			auto* dec = decrypt(data);
			delete data;
			
			if (dec == nullptr)
				return false;

			//delete file
			fs::remove(p);
			//set password to new version
			setPassword(new_password);
			//encrypt the data obtained and save to file
			if (!encrypt(clearFname, dec)) {
				delete dec;
				return false;
			}
			
			delete dec;
		}
	}
	return true;
}

#endif