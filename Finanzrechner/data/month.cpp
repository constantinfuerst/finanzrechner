#include "stdafx.h"
#include "month.h"

month::month(const QDate& monthDate) {
	m_month = monthDate;
}

QString month::generateID(const QDate& date, const bool& type1, const bool& type2) {
	static int idCounter = 0;
	QString id = date.toString("MM.yyyy");
	id += int(type1);
	id += int(type2);
	id += QString(idCounter);
	return id;
}