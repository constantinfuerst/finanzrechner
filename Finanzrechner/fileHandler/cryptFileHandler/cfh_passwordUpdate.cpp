#include "stdafx.h"

#ifdef compileWithCrypt
#include "cryptFH.h"

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
			
			auto* dec = decrypt(clearFname);
			if (dec == nullptr)
				return false;
			
			fs::remove(p);
			setPassword(new_password);
			if (!encrypt(clearFname, dec))
				return false;
			
			delete dec;
		}
	}
	return true;
}

#endif