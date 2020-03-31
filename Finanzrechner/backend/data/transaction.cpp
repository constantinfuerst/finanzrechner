#include "stdafx.h"
#include "transaction.h"

transaction::transaction(bool budget, bool transaction, bool expense, bool income, const double& category, const double& amount, const std::string& description, const QDate& date, const std::string& id) {
	m_budget = budget, m_transaction = transaction, m_expense = expense, m_income = income, m_category = category; m_amount = amount; m_date = date; m_description = description;
	if (id.empty()) m_transID = generateID(date, budget, transaction, expense, income);
	else m_transID = id;
}

transaction* transaction::makeBudget(const double& category, const double& amount) {
	return new transaction(true, false, true, false, category, amount);
}

transaction* transaction::makeIncome(const double& category, const double& amount, const std::string& description, const QDate& date) {
	return new transaction(false, true, false, true, category, amount, description, date);
}

transaction* transaction::makeExpense(const double& category, const double& amount, const std::string& description, const QDate& date) {
	return new transaction(false, true, true, false, category, amount, description, date);
}

transaction::transaction(const QJsonObject& json) {
	auto* t = fromJSON(json);
	m_budget = t->m_budget; m_transaction = t->m_transaction; m_expense = t->m_expense; m_income = t->m_income; m_transID = t->m_transID;  m_category = t->m_category; m_amount = t->m_amount; m_date = t->m_date;
	delete t;
}

//generate unique ID for identification in user interface - backend interaction
std::string transaction::generateID(const QDate& date, bool budget, bool transaction, bool expense, bool income) {
	static int idc = 0;
	std::string num = std::to_string(idc);
	const QDateTime cdate = QDateTime::currentDateTimeUtc();
	while (num.size() < 2)
		num = '0' + num;
	std::string id = date.toString("MM-yyyy").toStdString();
	id += "-";
	id += cdate.toString("ss-mm-hh-dd-MM-yyyy").toStdString();
	id += "-";
	id += (budget ? "1" : "0");
	id += (transaction ? "1" : "0");
	id += (expense ? "1" : "0");
	id += (income ? "1" : "0");
	id += "-";
	id += num;
	idc++;
	return id;
}

transaction::transaction(const transaction& t) {
	m_amount = t.m_amount;
	m_budget = t.m_budget;
	m_transaction = t.m_transaction;
	m_expense = t.m_expense;
	m_income = t.m_income;
	m_category = t.m_category;
	m_date = t.m_date;
	m_description = t.m_description;
	m_transID = t.m_transID;
}

//compares id, returns true if equal
bool transaction::operator==(const std::string& id) const {
	if (id == m_transID)
		return true;
	return false;
}

//implementation of filter sorting
bool transaction::operator==(const filter& f) const {
	if (f.fEnabled[filter::type]) {
		bool accepted = false;
		if (m_expense)
			if (f.fTypes_selected[0])
				accepted = true;
		if (m_income)
			if (f.fTypes_selected[1])
				accepted = true;
		if (m_budget)
			if (f.fTypes_selected[2])
				accepted = true;
		if (m_transaction)
			if (f.fTypes_selected[3])
				accepted = true;
		if (!accepted) return false;
	}
	if (f.fEnabled[filter::amount]) {
		if (m_amount < f.fAmount_range[0] || m_amount > f.fAmount_range[1])
			return false;
	}
	if (f.fEnabled[filter::date]) {
		//check that transaction is not budget as budgets have no correct date associated
		if (f.fEnabled[filter::type]) {
			if (!m_budget)
				if (m_date < f.fDate_range[0] || m_date > f.fDate_range[1])
					return false;
		}
		else {
			if (m_date < f.fDate_range[0] || m_date > f.fDate_range[1])
				return false;
		}
		
	}
	if (f.fEnabled[filter::category]) {
		bool found = false;
		for (auto& d : f.fCat_multiple)
			if (d == m_category)
				found = true;
		if (!found) return false;
	}
	if (f.fEnabled[filter::description]) {
		if (m_description != f.fDesc_match)
			return false;
	}

	return true;
}