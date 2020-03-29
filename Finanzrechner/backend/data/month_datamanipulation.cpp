#include "stdafx.h"
#include "month.h"

//adds a transaction pointer to the t-storage, may be budget or transaction
void month::addTransaction(transaction* transaction) {
	m_transactions.push_back(transaction);
}

//adds a transaction pointer from the t-storage, may be budget or transaction
bool month::removeTransaction(const std::string& id) {
	for (int i = 0; i < m_transactions.size(); i++) {
		auto* t = m_transactions[i];
		if (*t == id) {
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
transaction* month::modifyTransaction(const std::string& id) {
	for (auto* t : m_transactions)
		if (*t == id) {
			modified = true;
			return t;
		}
	return nullptr;
}