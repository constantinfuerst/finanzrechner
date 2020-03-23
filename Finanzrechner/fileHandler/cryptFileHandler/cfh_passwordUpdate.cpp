#include "stdafx.h"

#ifdef compileWithCrypt
#include "cryptFH.h"

bool cryptFileHandler::updatePassword(const QString& new_password) {
	namespace fs = std::filesystem;
	const QString old_password = QString::fromStdString(password);
	
	for (auto& p : fs::recursive_directory_iterator(savedir)) {
		setPassword(old_password);
		std::string fname = p.path().generic_string();
		auto* dec = decrypt(fname);
		fs::remove(p);
		setPassword(new_password);
		encrypt(fname, dec);
		delete dec;
	}
	return false;
}

#endif