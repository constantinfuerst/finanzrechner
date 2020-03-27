#include "stdafx.h"
#include "month.h"

void month::addTransaction(const bool& ttype, const double& category, const double& amount,
	const QDate& date, const std::string& description)
{
	modified = true;
	std::string id = generateID(m_month, ttype, transaction::TRANSACTION);
	m_transactions.push_back(
		new transaction(
			transaction::TRANSACTION, ttype, id, category, amount, description, date
		)
	);
}

void month::addBudget(const double& category, const double& amount) {
	modified = true;
	std::string id = generateID(m_month, transaction::EXPENSE, transaction::BUDGET);
	m_transactions.push_back(
		new transaction(
			transaction::BUDGET, transaction::EXPENSE, id, category, amount
		)
	);
}

void month::addT(transaction* transaction) {
	m_transactions.push_back(transaction);
}

bool month::removeT(const std::string& id) {
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

transaction* month::modifyT(const std::string& id) {
	for (int i = 0; i < m_transactions.size(); i++) {
		auto* t = m_transactions[i];
		if (t->m_transID == id) {
			modified = true;
			return t;
		}
	}
	return nullptr;
}