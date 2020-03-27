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
	std::string m_description;
	
public:
	std::string m_transID;
	
	//FUNCTIONS
	//constructors
	transaction(const bool& type, const std::string& transID, const double& category, const double& amount, const std::string& description = "", const QDate& date = QDate(1,1,1));
	transaction(const QJsonObject& json);
	transaction(const transaction& t);

	//json serializers / deserializers
	transaction* fromJSON(const QJsonObject& json) const;
	QJsonObject* toJSON() const;

	//comparisons
	bool operator==(const std::string& id) const;
	bool operator==(const double& cat) const;
};