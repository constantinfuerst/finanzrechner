#pragma once
#include "stdafx.h"
#include "../dataFunctions/filter.h"

class transaction {
public:
	//DEFINITIONS
	//ttypes
	static const bool EXPENSE = false;
	static const bool INCOME = true;
	//booking types
	static const bool BUDGET = false;
	static const bool TRANSACTION = true;

	//DATA
	double m_amount;
	bool m_bookingType;
	bool m_tType;
	
private:
	double m_category;
	QDate m_date;
	std::string m_description;
	
public:
	std::string m_transID;
	
	//FUNCTIONS
	//constructors
	transaction(
		const bool& bookingType, const bool& tType,
		std::string& transID, const double& category,
		const double& amount, const std::string& description = "",
		const QDate& date = QDate(1,1,1)
	);
	
	transaction(const QJsonObject& json);
	transaction(const transaction& t);

	//json serializers / deserializers
	transaction* fromJSON(const QJsonObject& json) const;
	QJsonObject* toJSON() const;

	//comparisons
	bool operator==(const std::string& id) const;
	bool operator==(const double& cat) const;
	bool operator==(const filter& f) const;
};