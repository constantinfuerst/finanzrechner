#include "stdafx.h"
#include "month.h"

month::month(const QDate& monthDate) {
	m_month = monthDate;
}

month::~month() {
	if (modified)
		writeJSON();
	for (auto* e : m_transactions)
		delete e;
	for (auto* e : m_budget)
		delete e;
	m_transactions.clear(); m_budget.clear();
}

QString month::generateID(const QDate& date, const bool& expense_or_income, const bool& budget_or_transaction) {
	QString num = QString::number(m_idCounter);
	while (num.size() < 16)
		num = '0' + num;
	QString id = date.toString("MM-yyyy");
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