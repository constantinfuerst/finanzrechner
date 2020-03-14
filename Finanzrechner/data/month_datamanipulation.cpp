#include "stdafx.h"
#include "month.h"

void month::addBudget(const bool& type, const double& category, const double& amount) {
	modified = true;
	m_budget.push_back(new transaction(type, generateID(m_month, type, true), category, amount));
}

void month::addTransaction(const bool& type, const double& category, const double& amount, const QDate& date){
	modified = true;
	m_transactions.push_back(new transaction(type, generateID(m_month, type, false), category, amount, date));
}

bool month::removeTransaction(const QString& id) {
	for (int i = 0; i < m_transactions.size(); i++) {
		auto* t = m_transactions[i];
		if (t->m_transID == id) {
			delete t;
			m_transactions.erase(m_transactions.begin() + i);
			modified = true;
			return true;
		}
	}
	return false;
}

transaction* month::modifyTransaction(const QString& id) {
	for (int i = 0; i < m_transactions.size(); i++) {
		auto* t = m_transactions[i];
		if (t->m_transID == id) {
			modified = true;
			return t;
		}
	}
	return nullptr;
}
