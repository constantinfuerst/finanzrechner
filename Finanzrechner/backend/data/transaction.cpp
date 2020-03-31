#include "stdafx.h"
#include "transaction.h"

transaction::transaction(const bool budget, const bool transaction, const bool expense, const bool income, const double& category,
                         const double& amount, const bool recurring, const std::string& description, const QDate& date, const std::string& id) {
	m_recurring = recurring;  m_budget = budget, m_transaction = transaction, m_expense = expense,
	m_income = income, m_category = category; p_amount = amount; m_date = date; m_description = description;
	if (id.empty()) m_trans_id = generateID(date, budget, transaction, expense, income);
	else m_trans_id = id;
}

transaction* transaction::makeBudget(const double& category, const double& amount) {
	return new transaction(true, false, true, false, category, amount, true);
}

transaction* transaction::makeIncome(const double& category, const double& amount, const std::string& description, const QDate& date, bool recurring) {
	return new transaction(false, true, false, true, category, amount, recurring, description, date);
}

transaction* transaction::makeExpense(const double& category, const double& amount, const std::string& description, const QDate& date, const bool recurring) {
	return new transaction(false, true, true, false, category, amount, recurring, description, date);
}

transaction::transaction(const QJsonObject& json) {
	auto* t = fromJSON(json);
	m_recurring = t->m_recurring;  m_budget = t->m_budget; m_transaction = t->m_transaction; m_expense = t->m_expense; m_income = t->m_income; m_trans_id = t->m_trans_id;  m_category = t->m_category; p_amount = t->p_amount; m_date = t->m_date;
	delete t;
}

//generate unique ID for identification in user interface - backend interaction
std::string transaction::generateID(const QDate& date, const bool budget, const bool transaction, const bool expense, const bool income) {
	static int idc = 0;
	const auto num = std::to_string(idc);
	const auto cdate = QDate::currentDate();
	std::string id = date.toString("MM-yyyy").toStdString();
	id += "-";
	id += cdate.toString("dd-MM-yyyy").toStdString();
	id += "-";
	id += budget ? "1" : "0";
	id += transaction ? "1" : "0";
	id += expense ? "1" : "0";
	id += income ? "1" : "0";
	id += "-";
	id += num;
	idc++;
	return id;
}

transaction::transaction(const transaction& t, const QDate& month) {
	m_recurring = t.m_recurring;
	p_amount = t.p_amount;
	m_budget = t.m_budget;
	m_transaction = t.m_transaction;
	m_expense = t.m_expense;
	m_income = t.m_income;
	m_category = t.m_category;
	m_date = month;
	m_description = t.m_description;
	m_trans_id = generateID(month, m_budget, m_transaction, m_expense, m_income);
}

//compares id, returns true if equal
bool transaction::operator==(const std::string& id) const {
	if (id == m_trans_id)
		return true;
	return false;
}

//implementation of filter sorting
//recurring transactions require explicit selection if "filter::type" enabled
bool transaction::operator==(const filter& f) const {
	if (f.f_enabled[filter::type]) {
		bool accepted = false;
		if (m_expense)
			if (f.f_types_selected[0])
				accepted = true;
		if (m_income)
			if (f.f_types_selected[1])
				accepted = true;
		if (m_budget)
			if (f.f_types_selected[2])
				accepted = true;
		if (m_transaction)
			if (f.f_types_selected[3])
				accepted = true;
		if (!accepted) return false;
		if (m_recurring && !f.f_types_selected[4]) return false;
	}
	if (f.f_enabled[filter::amount]) {
		if (p_amount < f.f_amount_range[0] || p_amount > f.f_amount_range[1])
			return false;
	}
	if (f.f_enabled[filter::date]) {
		//check that transaction is recurring as these have no correct date associated
		//as handling the month date is done by the appropriate filtering before handling transactions
		//this should not return anything outside of search scope, if recurring are explicitly selected then the user
		//will know they do not have a "date" as they are monthly recurring
		if (!m_recurring) {
			if (m_date < f.f_date_range[0] || m_date > f.f_date_range[1])
				return false;
		}
	}
	if (f.f_enabled[filter::category]) {
		bool found = false;
		for (auto& d : f.f_cat_multiple)
			if (d == m_category)
				found = true;
		if (!found) return false;
	}
	if (f.f_enabled[filter::description]) {
		if (m_description != f.f_desc_match)
			return false;
	}

	return true;
}