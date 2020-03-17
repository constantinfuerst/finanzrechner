#pragma once
#include "stdafx.h"

class transaction {
public:
	//DEFINITIONS
	static const bool EXPENSE = false;
	static const bool INCOME = true;

	//DATA
	double m_amount;
private:
	bool m_type;
	double m_category;
	QDate m_date;
	QString m_description;
	
public:
	QString m_transID;
	
	//FUNCTIONS
	//constructors
	transaction(const bool& type, const QString& transID, const double& category, const double& amount, const QString& description = "", const QDate& date = QDate(0,0,0));
	transaction(const QJsonObject& json);

	//json serializers / deserializers
	transaction* fromJSON(const QJsonObject& json) const;
	QJsonObject* toJSON() const;

	//comparisons
	bool operator==(const QString& id) const;
	bool operator==(const double& cat) const;
};