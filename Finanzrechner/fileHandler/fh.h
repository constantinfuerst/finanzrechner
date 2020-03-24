#pragma once

/////////
/// Abstract class providing interchangeable json file handlers
/////////

class fileHandler {
public:
	virtual ~fileHandler() = default;
	virtual bool writeJSON(QJsonDocument* jdoc, const QString& fname) = 0;
	virtual QJsonDocument* readJSON(const QString& fname) = 0;
};