#include "stdafx.h"
#include "month.h"

//creates a new transaction(transaction) from the supplied data
//which is then pushed to the transaction storage
void month::addTransaction(bool expense, bool income, const double& category, const double& amount,
	const QDate& date, const std::string& description)
{
	modified = true;
	std::string id = generateID(m_month, false, true, expense, income);
	m_transactions.push_back(
		new transaction(
			false, true, expense, income, id, category, amount, description, date
		)
	);
}

//creates a new transaction(budget) from the supplied data
//which is then pushed to the transaction storage
void month::addBudget(const double& category, const double& amount) {
	modified = true;
	std::string id = generateID(m_month, true, false, true, false);
	m_transactions.push_back(
		new transaction(
			true, false, true, false, id, category, amount
		)
	);
}

//adds a transaction pointer to the t-storage, may be budget or transaction
void month::addT(transaction* transaction) {
	m_transactions.push_back(transaction);
}

//adds a transaction pointer from the t-storage, may be budget or transaction
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

//returns a transaction pointer from the t-storage for modification
//may be budget or transaction
transaction* month::modifyT(const std::string& id) {
	for (auto* t : m_transactions)
		if (t->m_transID == id) {
			modified = true;
			return t;
		}
	return nullptr;
}