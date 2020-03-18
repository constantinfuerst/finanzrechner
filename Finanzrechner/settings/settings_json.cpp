#include "stdafx.h"
#include "settings.h"

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
	(*json)["color"] = QJsonArray({ (double)color_r, double(color_b), double(color_g), double(color_a) });
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