#include "stdafx.h"
#include "month.h"

void month::addBudget(const double& category, const double& amount) {
	modified = true;
	m_budget.append(new transaction(transaction::EXPENSE, generateID(m_month, transaction::EXPENSE, BUDGET), category, amount));
}

void month::addTransaction(const bool& type, const double& category, const double& amount, const QDate& date){
	modified = true;
	m_transactions.append(new transaction(type, generateID(m_month, type, TRANSACTION), category, amount, date));
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