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
	for (auto* e : m_monthly)
		delete e;
	m_categories.clear(); m_monthly.clear();
}

void settings::clear() {
	if (modified)
		writeJSON();
	for (auto* e : m_categories)
		delete e;
	for (auto* e : m_monthly)
		delete e;
	m_categories.clear(); m_monthly.clear();
}

//creates a new category with name and specified color supplied
//category is ready to use system-wide (may be added to existing months by editing transactions)
void settings::addCategory(const std::string& name, const QColor& color) {
	modified = true;
	m_categories.push_back(new category(m_catCounter, name, color));
	m_catCounter++;
}

//deletes a category, does not however remove references to said category
//this should be used carefully and a corrective statement in the ui of type "category missing"
//should be implemented
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

//allows editing a category by its id, editing the id is however not recommended
//reasons for this are stated in the documentation for removeCategory
settings::category* settings::editCategory(const double& id) {
	modified = true;
	for (auto i = 0; i < m_categories.size(); i++) {
		auto* c = m_categories[i];
		if (*c == id) return c;
	}
	return nullptr;
}

//adds to currently stored balance, amount may obviously be negative
void settings::addToBalance(const double& amount) {
	modified = true;
	m_current_balance += amount;
}

//sets the current balance level, must be treated with caution, implement a warning in UI
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