#include <stdafx.h>
#include "month.h"

//adds a transaction pointer to the t-storage, may be budget or transaction
void month::addTransaction(transaction* transaction) {
	p_transactions.push_back(transaction);
}

//adds a transaction pointer from the t-storage, may be budget or transaction
bool month::removeTransaction(const std::string& id) {
	for (auto i = 0; i < p_transactions.size(); i++) {
		auto* t = p_transactions[i];
		if (*t == id) {
			delete t;
			p_transactions.erase(p_transactions.begin() + i);
			m_modified = true;
			return true;
		}
	}
	return false;
}

//returns a transaction pointer from the t-storage for modification
//may be budget or transaction
transaction* month::modifyTransaction(const std::string& id) {
	for (auto* t : p_transactions)
		if (*t == id) {
			m_modified = true;
			return t;
		}
	return nullptr;
}