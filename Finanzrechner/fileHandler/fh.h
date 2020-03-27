#pragma once

/////////
/// Abstract class providing interchangeable json file handlers
/////////

class fileHandler {
public:
	virtual ~fileHandler() = default;
	virtual bool writeJSON(QJsonDocument* jdoc, const std::string& fname) = 0;
	virtual QJsonDocument* readJSON(const std::string& fname) = 0;
};