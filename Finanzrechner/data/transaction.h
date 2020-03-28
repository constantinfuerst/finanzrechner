#pragma once
#include "stdafx.h"
#include "../dataFunctions/filter.h"

class transaction {
public:
	//DEFINITIONS

	//DATA
	double m_amount;

private:
	bool m_budget, m_transaction, m_expense, m_income;
	
	double m_category;
	QDate m_date;
	std::string m_description;

public:
	std::string m_transID;

	bool isBudget() const { return m_budget; }
	bool isTransaction() const { return m_transaction; }
	bool isExpense() const { return m_expense; }
	bool isIncome() const { return m_income; }
	
	//FUNCTIONS
	//constructors
	transaction(bool budget, bool transaction, bool expense, bool income,
		std::string& transID, const double& category,
		const double& amount, const std::string& description = "",
		const QDate& date = QDate(1, 1, 1)
	);

	transaction(const QJsonObject& json);
	transaction(const transaction& t);

	//json serializers / deserializers
	transaction* fromJSON(const QJsonObject& json) const;
	QJsonObject* toJSON() const;

	//comparisons
	bool operator==(const std::string& id) const;
	bool operator==(const filter& f) const;
};