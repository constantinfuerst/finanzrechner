#include "stdafx.h"
#include "plainFH.h"

//reads a qjsondocument from plaintext stored in a file
//no file type ending required
QJsonDocument* plainFileHandler::readJSON(const std::string& fname) {
	const std::string lfname = fname + ".json";
	QFile load_file(lfname.c_str());

	if (!load_file.open(QIODevice::ReadOnly)) {
		qWarning("Couldn't open save file.");
		return nullptr;
	}

	const QByteArray data = load_file.readAll();
	auto* json_doc = new QJsonDocument;
	*json_doc = QJsonDocument::fromJson(data);
	return json_doc;
}

//writes a qjsondocument as plaintext into a file using qfile hanlder
//no file type ending required
bool plainFileHandler::writeJSON(QJsonDocument* jdoc, const std::string& fname) {
	const std::string lfname = fname + ".json";
	QFile save_file(lfname.c_str());

	if (!save_file.open(QIODevice::WriteOnly)) {
		qWarning("Couldn't open save file.");
		return false;
	}

	save_file.write(jdoc->toJson(QJsonDocument::Compact));

	save_file.close();
	return true;
}