#pragma once
#include "stdafx.h"
#include "../fh.h"

class plainFileHandler : public fileHandler {
public:
	~plainFileHandler() override = default;
	bool writeJSON(QJsonDocument* jdoc, const QString& fname) override;
	QJsonDocument* readJSON(const QString& fname) override;
};