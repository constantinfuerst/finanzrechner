#include "stdafx.h"
#include "settings.h"

bool settings::addMonthly(const double& amount, const std::string& description, const int& category, const monthly_type& type) {
	modified = true;
	transaction* p = nullptr;
	std::string id = generateID(type, category);
	switch (type) {
	case budget:
		p = new transaction(
			transaction::BUDGET, transaction::EXPENSE, id, category, amount, description
		);
		m_budget.push_back(p);
		return true;
	case recurring:
		p = new transaction(
			transaction::TRANSACTION, transaction::EXPENSE, id, category, amount, description
		);
		m_recurring.push_back(p);
		return true;
	case income:
		p = new transaction(
			transaction::TRANSACTION, transaction::INCOME, id, category, amount, description
		);
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
		m->addT(new transaction(*r));
	for (auto* b : m_budget)
		m->addT(new transaction(*b));
	for (auto* i : m_income)
		m->addT(new transaction(*i));
}

settings& settings::get(fileHandler* fh_in) {
	static settings singleton(fh_in);
	return singleton;
}

void settings::init(fileHandler* fh_in) {
	get(fh_in);
}

std::string settings::generateID(const monthly_type& type, const int& category) {
	std::string num = std::to_string(m_idCounter);
	while (num.size() < 16)
		num = '0' + num;
	m_idCounter++;
	std::string id;
	switch (type) {
	case budget: id += '0'; break;
	case recurring: id += '1'; break;
	case income: id += '2'; break;
	default: id += '3'; break;
	}
	id += '-';
	std::string cat = std::to_string(category);
	while (cat.size() < 4)
		cat = '0' + cat;
	id += cat; id += '-'; id += num;
	return id;
}

settings::settings(fileHandler* fh_in) {
	if (fh_in != nullptr) fh = fh_in;
	m_idCounter = 0; m_catCounter = 0; m_current_balance = 0;
	readJSON();
}

settings::~settings() {
	for (auto* e : m_categories)
		delete e;
	for (auto* e : m_income)
		delete e;
	for (auto* e : m_budget)
		delete e;
	for (auto* e : m_recurring)
		delete e;
	m_categories.clear(); m_income.clear(); m_budget.clear(); m_recurring.clear();
}

void settings::clear() {
	if (modified)
		writeJSON();
	for (auto* e : m_categories)
		delete e;
	for (auto* e : m_income)
		delete e;
	for (auto* e : m_budget)
		delete e;
	for (auto* e : m_recurring)
		delete e;
	m_categories.clear(); m_income.clear(); m_budget.clear(); m_recurring.clear();
}

void settings::addCategory(const std::string& name, const QColor& color) {
	modified = true;
	m_categories.push_back(new category(m_catCounter, name, color));
	m_catCounter++;
}

bool settings::removeCategory(const double& id) {
	modified = true;
	for (auto i = 0; i < m_categories.size(); i++) {
		auto* c = m_categories[i];
		if (*c == id) {
			delete c;
			m_categories.erase(m_categories.begin() + i);
			return true;
		}
	}
	return false;
}

settings::category* settings::editCategory(const double& id) {
	modified = true;
	for (auto i = 0; i < m_categories.size(); i++) {
		auto* c = m_categories[i];
		if (*c == id) return c;
	}
	return nullptr;
}

void settings::addToBalance(const double& amount) {
	modified = true;
	m_current_balance += amount;
}

void settings::setBalance(const double& amount) {
	modified = true;
	m_current_balance = amount;
}

settings::category::category(const double& id, const std::string& name, const QColor& color) {
	identifier = id; category_name = name; category_color = color;
}

settings::category::category(const QJsonObject& json) {
	auto* c = fromJSON(json);
	identifier = c->identifier; category_name = c->category_name; category_color = c->category_color;
}

bool settings::category::operator==(const double& id) const {
	if (id == identifier)
		return true;
	return false;
}