#include "stdafx.h"
#include "plainFH.h"

QJsonDocument* plainFileHandler::readJSON(const QString& fname) {
	QFile loadFile(fname + ".json");

	if (!loadFile.open(QIODevice::ReadOnly)) {
		qWarning("Couldn't open save file.");
		return nullptr;
	}

	QByteArray data = loadFile.readAll();
	auto* jsonDoc = new QJsonDocument;
	jsonDoc->fromJson(data);
	return jsonDoc;
}

bool plainFileHandler::writeJSON(QJsonDocument* jdoc, const QString& fname) {
	QFile saveFile(fname + ".json");

	if (!saveFile.open(QIODevice::WriteOnly)) {
		qWarning("Couldn't open save file.");
		return false;
	}

	saveFile.write(jdoc->toJson(QJsonDocument::Compact));

	saveFile.close();
	return true;
}

