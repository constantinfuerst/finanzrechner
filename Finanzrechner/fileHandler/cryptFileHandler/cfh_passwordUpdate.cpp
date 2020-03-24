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
	
	for (auto& p : fs::recursive_directory_iterator(savedir)) {
		setPassword(old_password);
		if (p.is_regular_file()) {
			const std::string fname = p.path().generic_string();
			const std::string clearFname = fname.substr(0, fname.size() - 4);

			auto* data = readFile(fname);
			auto* dec = decrypt(data);
			delete data;
			
			if (dec == nullptr)
				return false;
			
			fs::remove(p);
			setPassword(new_password);
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