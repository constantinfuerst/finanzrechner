#include "stdafx.h"
#include "transaction.h"

transaction::transaction(const bool& bookingType, const bool& tType, std::string& transID, const double& category, const double& amount, const std::string& description, const QDate& date) {
	m_transID = transID;  m_tType = tType, m_bookingType = bookingType; m_category = category; m_amount = amount; m_date = date; m_description = description;
}

transaction::transaction(const QJsonObject& json) {
	auto* t = fromJSON(json);
	m_bookingType = t->m_bookingType; m_tType = t->m_tType; m_transID = t->m_transID;  m_category = t->m_category; m_amount = t->m_amount; m_date = t->m_date;
	delete t;
}

transaction::transaction(const transaction& t) {
	m_amount = t.m_amount;
	m_tType = t.m_tType;
	m_bookingType = t.m_bookingType;
	m_category = t.m_category;
	m_date = t.m_date;
	m_description = t.m_description;
	m_transID = t.m_transID;
}

transaction* transaction::fromJSON(const QJsonObject& json) const{
	bool ttype = false, btype = false;
	std::string transID, description;
	double category = 0, amount = 0;
	QDate date;

	if (json.contains("btype") && json["btype"].isBool())
		btype = json["btype"].toBool();
	if (json.contains("ttype") && json["ttype"].isBool())
		ttype = json["ttype"].toBool();
	if (json.contains("category") && json["category"].isDouble())
		category = json["category"].toInt();
	if (json.contains("amount") && json["amount"].isDouble())
		amount = json["amount"].toDouble();
	if (json.contains("id") && json["id"].isString())
		transID = json["id"].toString().toStdString();
	if (json.contains("date") && json["date"].isString())
		date = QDate::fromString(json["date"].toString());
	if (json.contains("description") && json["description"].isString())
		description = json["description"].toString().toStdString();
	
	return new transaction(btype, ttype, transID, category, amount, description, date);
}

QJsonObject* transaction::toJSON() const {
	auto* json = new QJsonObject;
	(*json)["id"] = QString::fromStdString(m_transID);
	(*json)["btype"] = m_bookingType;
	(*json)["ttype"] = m_tType;
	(*json)["amount"] = m_amount;
	(*json)["category"] = m_category;
	(*json)["date"] = m_date.toString("dd-MM-yyyy");
	(*json)["description"] = QString::fromStdString(m_description);
	return json;
}

bool transaction::operator==(const std::string& id) const {
	if (id == m_transID)
		return true;
	return false;
}

bool transaction::operator==(const double& cat) const {
	if (cat == m_category)
		return true;
	return false;
}
