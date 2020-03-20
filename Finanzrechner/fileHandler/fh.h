#pragma once

class fileHandler {
public:
	virtual ~fileHandler() = default;
	virtual bool writeJSON(QJsonDocument* jdoc, const QString& fname);
	virtual QJsonDocument* readJSON(const QString& fname);
};