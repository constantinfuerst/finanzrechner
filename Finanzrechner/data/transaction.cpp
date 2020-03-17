#include "stdafx.h"
#include "transaction.h"

transaction::transaction(const bool& type, const QString& transID, const double& category, const double& amount, const QString& description, const QDate& date) {
	m_transID = transID;  m_type = type; m_category = category; m_amount = amount; m_date = date; m_description = description;
}

transaction::transaction(const QJsonObject& json) {
	auto* t = fromJSON(json);
	m_type = t->m_type; m_transID = t->m_transID;  m_category = t->m_category; m_amount = t->m_amount; m_date = t->m_date;
	delete t;
}

transaction* transaction::fromJSON(const QJsonObject& json) const{
	bool type = false; bool recurring = false; QString transID; QString description; double category = 0; double amount = 0; QDate date;
	
	if (json.contains("type") && json["type"].isBool())
		type = json["type"].toBool();
	if (json.contains("recurring") && json["recurring"].isBool())
		recurring = json["recurring"].toBool();
	if (json.contains("category") && json["category"].isDouble())
		category = json["category"].toInt();
	if (json.contains("amount") && json["amount"].isDouble())
		amount = json["amount"].toDouble();
	if (json.contains("id") && json["id"].isString())
		transID = json["id"].toString();
	if (json.contains("date") && json["date"].isString())
		date = QDate::fromString(json["date"].toString());
	if (json.contains("description") && json["description"].isString())
		description = json["description"].toString();
	
	return new transaction(type, transID, category, amount, description, date);
}

QJsonObject* transaction::toJSON() const {
	auto* json = new QJsonObject;
	(*json)["id"] = m_transID;
	(*json)["type"] = m_type;
	(*json)["amount"] = m_amount;
	(*json)["category"] = m_category;
	(*json)["date"] = m_date.toString("dd-MM-yyyy");
	(*json)["description"] = m_description;
	return json;
}

bool transaction::operator==(const QString& id) const {
	if (id == m_transID)
		return true;
	return false;
}

bool transaction::operator==(const double& cat) const {
	if (cat == m_category)
		return true;
	return false;
}
