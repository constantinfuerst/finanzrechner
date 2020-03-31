#include <stdafx.h>
#include "settings.h"

//adds a monthly transaction of type budget, expense (recurring charges) or income (monthly earnings)
bool settings::addMonthly(const double& amount, const std::string& description, const int& category, const monthly_type& type) {
	m_modified = true;
	transaction* p = nullptr;
	
	switch (type) {
	case budget:
		p = transaction::makeBudget(category, amount); break;
	case recurring:
		p = transaction::makeExpense(category, amount, description, { 1,1,1 }); break;
	case income:
		p = transaction::makeIncome(category, amount, description, { 1,1,1 }); break;
	default: return false;
	}

	m_monthly.push_back(p);
	return true;
}

//erases a monthly transaction from the settings pool, not removing it from past months
bool settings::removeMonthly(const std::string& id) {
	m_modified = true;

	for (auto i = 0; i < m_monthly.size(); i++) {
		auto* b = m_monthly[i];
		if (*b == id) {
			delete b;
			m_monthly.erase(m_monthly.begin() + i);
			return true;
		}
	}

	return false;
}

//may edit monthly transactions, affecting only coming months still to be filled by fillMonth()
transaction* settings::editMonthly(const std::string& id) {
	m_modified = true;
	for (auto t : m_monthly)
		if (*t == id) return t;
	return nullptr;
}

//adds all monthly transactions stored to a specified month
//calling multiple times on the same month will result in duplicate transactions
void settings::fillMonth(month* m) {
	for (auto* t : m_monthly)
		m->addTransaction(new transaction(*t, m->p_month));
}