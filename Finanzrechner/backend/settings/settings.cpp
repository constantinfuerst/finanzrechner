#include "stdafx.h"
#include "settings.h"

settings& settings::get(fileHandler* fh_in) {
	static settings singleton(fh_in);
	return singleton;
}

void settings::init(fileHandler* fh_in) {
	get(fh_in);
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