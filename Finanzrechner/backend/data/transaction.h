#pragma once
#include "stdafx.h"
#include "../dataFunctions/filter.h"

class transaction {
public:
	//DEFINITIONS

	//DATA
	double p_amount;

private:
	bool m_budget, m_transaction, m_expense, m_income, m_recurring;
	
	double m_category;
	QDate m_date;
	std::string m_description;
	std::string m_trans_id;

	//FUNCTIONS
	//helper
	static std::string generateID(const QDate& date, bool budget, bool transaction, bool expense, bool income);
	
	//constructors
	transaction(bool budget, bool transaction, bool expense, bool income, const double& category,
		const double& amount, bool recurring, const std::string& description = "",
		const QDate& date = QDate(1, 1, 1), const std::string& id = ""
	);
	
public:
	//intuitive constructors
	static transaction* makeBudget(const double& category, const double& amount);

	static transaction* makeIncome(
		const double& category, const double& amount,
		const std::string& description, const QDate& date, bool recurring = false);

	static transaction* makeExpense(
		const double& category, const double& amount,
		const std::string& description, const QDate& date, bool recurring = false);
	
	transaction(const QJsonObject& json);
	transaction(const transaction& t, const QDate& month);

	//information obtain
	[[nodiscard]] const bool& isBudget() const { return m_budget; }
	[[nodiscard]] const bool& isTransaction() const { return m_transaction; }
	[[nodiscard]] const bool& isExpense() const { return m_expense; }
	[[nodiscard]] const bool& isIncome() const { return m_income; }
	
	//json serializers / deserializers
	[[nodiscard]] transaction* fromJSON(const QJsonObject& json) const;
	[[nodiscard]] QJsonObject* toJSON() const;

	//comparisons
	bool operator==(const std::string& id) const;
	bool operator==(const filter& f) const;
};