#include "stdafx.h"
#include "settings.h"



bool settings::addMonthly(const double& amount, const std::string& description, const int& category, const monthly_type& type) {
	modified = true;
	transaction* p = nullptr;
	switch (type) {
	case budget:
		p = transaction::makeBudget(category, amount);
		m_budget.push_back(p);
		return true;
	case recurring:
		p = transaction::makeExpense(category, amount, description, { 1,1,1 });
		m_recurring.push_back(p);
		return true;
	case income:
		p = transaction::makeIncome(category, amount, description, { 1,1,1 });
		m_income.push_back(p);
		return true;
	default: return false;
	}
}

bool settings::removeMonthly(const std::string& id) {
	modified = true;
	if (id[0] == '0')
		for (auto i = 0; i < m_budget.size(); i++) {
			auto* b = m_budget[i];
			if (*b == id) {
				delete b;
				m_budget.erase(m_budget.begin() + i);
				return true;
			}
		}
	else if (id[0] == '1')
		for (auto i = 0; i < m_recurring.size(); i++) {
			auto* b = m_recurring[i];
			if (*b == id) {
				delete b;
				m_recurring.erase(m_recurring.begin() + i);
				return true;
			}
		}
	else if (id[0] == '2')
		for (auto i = 0; i < m_income.size(); i++) {
			auto* b = m_income[i];
			if (*b == id) {
				delete b;
				m_income.erase(m_income.begin() + i);
				return true;
			}
		}
	else
		return false;
	return false;
}

transaction* settings::editMonthly(const std::string& id) {
	modified = true;
	if (id[0] == '0')
		for (auto i = 0; i < m_budget.size(); i++) {
			auto* b = m_budget[i];
			if (*b == id) return b;
		}

	else if (id[0] == '1')
		for (auto i = 0; i < m_recurring.size(); i++) {
			auto* b = m_recurring[i];
			if (*b == id) return b;
		}
	else if (id[0] == '2')
		for (auto i = 0; i < m_income.size(); i++) {
			auto* b = m_income[i];
			if (*b == id) return b;
		}
	else
		return nullptr;
	return nullptr;
}

void settings::fillMonth(month* m) {
	for (auto* r : m_recurring)
		m->addTransaction(new transaction(*r));
	for (auto* b : m_budget)
		m->addTransaction(new transaction(*b));
	for (auto* i : m_income)
		m->addTransaction(new transaction(*i));
}