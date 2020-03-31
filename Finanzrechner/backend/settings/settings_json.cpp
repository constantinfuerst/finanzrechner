#include "stdafx.h"
#include "settings.h"

//deserializes a category for use in the settings json deserializer
settings::category* settings::category::fromJSON(const QJsonObject& json) {
	QColor color; std::string name; int id = 0;

	if (json.contains("id") && json["id"].isDouble())
		id = json["id"].toDouble();
	if (json.contains("name") && json["name"].isString())
		name = json["name"].toString().toStdString();
	if (json.contains("color") && json["color"].isArray())
		color.setRgb(json["color"][0].toInt(), json["color"][1].toInt(), json["color"][2].toInt(), json["color"][3].toInt());

	return new category(id, name, color);
}

//serializes a category for use in the settings json serializer
QJsonObject* settings::category::toJSON() const {
	int color_r, color_g, color_b, color_a;
	category_color.getRgb(&color_r, &color_g, &color_b, &color_a);
	auto* json = new QJsonObject;
	(*json)["id"] = static_cast<double>(identifier);
	(*json)["name"] = QString::fromStdString(category_name);
	(*json)["color"] = QJsonArray({ static_cast<double>(color_r), static_cast<double>(color_b), static_cast<double>(color_g), static_cast<double>(color_a) });
	return json;
}

//json deserializer using the fileHandler to obtain a json document
//reads all data into the settings singleton
bool settings::readJSON() {
	//return if fileHandler nonexistent
	if (fh == nullptr) return false;
	const std::string fname = std::string(savedir) + "settings";

	//obtain json document from filename and return if reading fails
	auto* jdoc = fh->readJSON(fname);
	if (jdoc == nullptr) return false;
	auto json = jdoc->object();
	delete jdoc;

	//check for valid json, set variables and return if encountering missing data
	if (json.contains("monthly") && json["monthly"].isArray()) {
		QJsonArray budgetArray = json["monthly"].toArray();
		for (auto e : budgetArray)
			if (e.isObject())
				m_monthly.push_back(new transaction(e.toObject()));
	}
	else return false;

	if (json.contains("categories") && json["categories"].isArray()) {
		QJsonArray budgetArray = json["categories"].toArray();
		for (auto e : budgetArray)
			if (e.isObject())
				m_categories.push_back(new category(e.toObject()));
	}
	else return false;

	if (json.contains("idCounter") && json["idCounter"].isDouble())
		m_idCounter = json["idCounter"].toDouble();
	else return false;

	if (json.contains("current_balance") && json["current_balance"].isDouble())
		m_current_balance = json["current_balance"].toDouble();
	else return false;

	if (json.contains("catCounter") && json["catCounter"].isDouble())
		m_catCounter = json["catCounter"].toDouble();
	else return false;

	return true;
}

//json serializer creating a json document of all the settings data
//delegating the write to the fileHandler function
bool settings::writeJSON() {
	//return if fileHandler nonexistent
	if (fh == nullptr) return false;
	//write transaction data into json arrays
	QJsonArray categories;
	for (auto* t : m_categories) {
		QJsonObject tobj = *t->toJSON();
		categories.append(tobj);
		delete t;
	} m_categories.clear();

	QJsonArray mt;
	for (auto* t : m_monthly) {
		QJsonObject tobj = *t->toJSON();
		mt.append(tobj);
		delete t;
	} m_monthly.clear();

	//create the object and add data to it
	QJsonObject settings;
	settings["idCounter"] = m_idCounter;
	settings["catCounter"] = m_catCounter;
	settings["monthly"] = mt;
	settings["categories"] = categories;
	settings["current_balance"] = m_current_balance;

	//call the write of fileHandler with "savedir" macro and appending 'settings'
	const std::string fname = std::string(savedir) + "settings";
	auto* jdoc = new QJsonDocument(settings);
	fh->writeJSON(jdoc, fname);
	delete jdoc;

	return true;
}