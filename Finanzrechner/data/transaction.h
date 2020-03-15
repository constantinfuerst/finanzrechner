#pragma once
#include "stdafx.h"

class transaction {
public:
	//DEFINITIONS
	static const bool EXPENSE = false;
	static const bool INCOME = true;

private:
	//DATA
	bool m_type;
	bool m_recurring;
	double m_category;
	double m_amount;
	QDate m_date;
	
public:
	QString m_transID;
	
	//FUNCTIONS
	//constructors
	transaction(const bool& type, const QString& transID, const double& category, const double& amount, const QDate& date = QDate(0,0,0));
	transaction(const QJsonObject& json);

	//json serializers / deserializers
	transaction* fromJSON(const QJsonObject& json) const;
	QJsonObject* toJSON() const;
};