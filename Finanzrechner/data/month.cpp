#include "stdafx.h"
#include "month.h"

month::month(const QDate& monthDate, fileHandler* fh_in) {
	m_month = monthDate;
	fh = fh_in;
}

month::~month() {
	if (modified)
		writeJSON();
	for (auto* e : m_transactions)
		delete e;
	m_transactions.clear();
}

std::string month::generateID(const QDate& date, const bool& expense_or_income, const bool& budget_or_transaction) {
	std::string num = std::to_string(m_idCounter);
	while (num.size() < 16)
		num = '0' + num;
	std::string id = date.toString("MM-yyyy").toStdString();
	id += "-";
	id += (expense_or_income ? "1" : "0");
	id += "-";
	id += (budget_or_transaction ? "1" : "0");
	id += "-";
	id += num;
	m_idCounter++;
	return id;
}

bool month::operator==(const QDate& date) const {
	if (date.month() == m_month.month() && date.year() == m_month.year())
		return true;
	return false;
}

bool month::operator==(const filter& f) const {
	bool match = true;
	
	return match;
}