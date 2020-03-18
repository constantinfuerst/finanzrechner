#include "stdafx.h"
#include "settings.h"

bool settings::addMonthly(const double& amount, const QString& description, const int& category, const monthly_type& type) {
	modified = true;
	switch (type) {
	case budget:
		m_budget.append(new transaction(transaction::EXPENSE, generateID(type, category), category, amount, description));
		return true;
	case recurring:
		m_recurring.append(new transaction(transaction::EXPENSE, generateID(type, category), category, amount, description));
		return true;
	case income:
		m_income.append(new transaction(transaction::INCOME, generateID(type, category), category, amount, description));
		return true;
	default: return false;
	}
}

bool settings::removeMonthly(const QString& id) {
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

transaction* settings::editMonthly(const QString& id) {
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
		m->addTransaction(r);
	for (auto* b : m_budget)
		m->addBudget(b);
	for (auto* i : m_income)
		m->addTransaction(i);
}

QString settings::generateID(const monthly_type& type, const int& category) {
	QString num = QString::number(m_idCounter);
	while (num.size() < 16)
		num = '0' + num;
	m_idCounter++;
	QString id;
	switch (type) {
	case budget: id += '0'; break;
	case recurring: id += '1'; break;
	case income: id += '2'; break;
	default: id += '3'; break;
	}
	id += '-';
	QString cat = QString::number(category);
	while (cat.size() < 4)
		cat = '0' + num;
	id += cat; id += '-'; id += num;
	return id;
}

settings::settings() {
	m_idCounter = 0; m_catCounter = 0; m_current_balance = 0;
	readJSON();
}

settings::~settings() {
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

void settings::addCategory(const QString& name, const QColor& color) {
	modified = true;
	m_categories.append(new category(m_catCounter, name, color));
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

settings::category::category(const double& id, const QString& name, const QColor& color) {
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

settings::category* settings::category::fromJSON(const QJsonObject& json) const {
	QColor color; QString name; int id = 0;
	
	if (json.contains("id") && json["id"].isDouble())
		id = json["id"].toDouble();
	if (json.contains("name") && json["name"].isString())
		name = json["name"].isString();
	if (json.contains("color") && json["color"].isArray())
		color.setRgb(json["color"][0].toInt(), json["color"][1].toInt(), json["color"][2].toInt(), json["color"][3].toInt());

	return new category(id, name, color);
}

QJsonObject* settings::category::toJSON() const {
	int color_r, color_g, color_b, color_a;
	category_color.getRgb(&color_r, &color_g, &color_b, &color_a);
	auto* json = new QJsonObject;
	(*json)["id"] = (double)identifier;
	(*json)["name"] = category_name;
	(*json)["color"] = QJsonArray({(double)color_r, double(color_b), double(color_g), double(color_a)});
	return json;
}

bool settings::readJSON() {
	QString fname = QString(savedir) + "settings.json";
	QFile loadFile(fname);

	if (!loadFile.open(QIODevice::ReadOnly)) {
		qWarning("Couldn't open settings file.");
		return false;
	}

	QByteArray data = loadFile.readAll();
	QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
	auto json = jsonDoc.object();

	if (json.contains("monthly_income") && json["monthly_income"].isArray()) {
		QJsonArray budgetArray = json["monthly_income"].toArray();
		for (auto e : budgetArray)
			if (e.isObject())
				m_income.append(new transaction(e.toObject()));
	}
	else {
		qWarning("Did not find income listing, possible corrupt settings file.");
		return false;
	}

	if (json.contains("monthly_budget") && json["monthly_budget"].isArray()) {
		QJsonArray budgetArray = json["monthly_budget"].toArray();
		for (auto e : budgetArray)
			if (e.isObject())
				m_budget.append(new transaction(e.toObject()));
	}
	else {
		qWarning("Did not find budget listing, possible corrupt settings file.");
		return false;
	}

	if (json.contains("monthly_recurring") && json["monthly_recurring"].isArray()) {
		QJsonArray budgetArray = json["monthly_recurring"].toArray();
		for (auto e : budgetArray)
			if (e.isObject())
				m_recurring.append(new transaction(e.toObject()));
	}
	else {
		qWarning("Did not find recurring listing, possible corrupt settings file.");
		return false;
	}

	if (json.contains("categories") && json["categories"].isArray()) {
		QJsonArray budgetArray = json["categories"].toArray();
		for (auto e : budgetArray)
			if (e.isObject())
				m_categories.append(new category(e.toObject()));
	}
	else {
		qWarning("Did not find recurring listing, possible corrupt settings file.");
		return false;
	}

	if (json.contains("idCounter") && json["idCounter"].isDouble())
		m_idCounter = json["idCounter"].toDouble();
	else {
		qWarning("Did not find recurring listing, possible corrupt settings file.");
		return false;
	}
	if (json.contains("current_balance") && json["current_balance"].isDouble())
		m_current_balance = json["current_balance"].toDouble();
	else {
		qWarning("Did not find recurring listing, possible corrupt settings file.");
		return false;
	}
	if (json.contains("catCounter") && json["catCounter"].isDouble())
		m_catCounter = json["catCounter"].toDouble();
	else {
		qWarning("Did not find recurring listing, possible corrupt settings file.");
		return false;
	}
	
	return true;
}

bool settings::writeJSON() {
	QString fname = QString(savedir) + "settings.json";
	QFile saveFile(fname);

	if (!saveFile.open(QIODevice::WriteOnly)) {
		qWarning("Couldn't open settings file.");
		return false;
	}

	QJsonArray categories;
	for (auto* t : m_categories) {
		QJsonObject tobj = *t->toJSON();
		categories.append(tobj);
		delete t;
	} m_categories.clear();

	QJsonArray income;
	for (auto* t : m_income) {
		QJsonObject tobj = *t->toJSON();
		income.append(tobj);
		delete t;
	} m_income.clear();

	QJsonArray budget;
	for (auto* t : m_budget) {
		QJsonObject tobj = *t->toJSON();
		budget.append(tobj);
		delete t;
	} m_budget.clear();

	QJsonArray recurring;
	for (auto* t : m_recurring) {
		QJsonObject tobj = *t->toJSON();
		recurring.append(tobj);
		delete t;
	} m_recurring.clear();

	QJsonObject settings;
	settings["idCounter"] = m_idCounter;
	settings["catCounter"] = m_catCounter;
	settings["monthly_budget"] = budget;
	settings["monthly_income"] = income;
	settings["monthly_recurring"] = recurring;
	settings["categories"] = categories;
	settings["current_balance"] = m_current_balance;

	QJsonDocument saveDoc(settings);
	saveFile.write(saveDoc.toJson(QJsonDocument::Compact));

	saveFile.close();
	return true;
}