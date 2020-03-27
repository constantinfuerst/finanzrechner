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

//reads transaction information out of a supplied json object
transaction* transaction::fromJSON(const QJsonObject& json) const{
	bool ttype = false, btype = false;
	std::string transID, description;
	double category = 0, amount = 0;
	QDate date;

	//check if data exists and set the appropriate values
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

//constructs a json object out of transaction data
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

//compares id, returns true if equal
bool transaction::operator==(const std::string& id) const {
	if (id == m_transID)
		return true;
	return false;
}

//implementation of filter sorting
bool transaction::operator==(const filter& f) const {
	if (f.fEnabled[filter::type]) {
		bool accepted = false;
		if (m_tType == EXPENSE)
			if (!f.fTypes_selected[0])
				accepted = true;
		if (m_tType == INCOME)
			if (!f.fTypes_selected[1])
				accepted = true;
		if (m_bookingType == BUDGET)
			if (!f.fTypes_selected[2])
				accepted = true;
		if (m_bookingType == TRANSACTION)
			if (!f.fTypes_selected[3])
				accepted = true;
		if (!accepted) return false;
	}
	if (f.fEnabled[filter::amount]) {
		if (m_amount < f.fAmount_range[0] || m_amount > f.fAmount_range[1])
			return false;
	}
	if (f.fEnabled[filter::date]) {
		if (m_date < f.fDate_range[0] || m_date > f.fDate_range[1])
			return false;
	}
	if (f.fEnabled[filter::category]) {
		bool found = false;
		for (auto& d : f.fCat_multiple)
			if (d == m_category)
				found = true;
		if (!found) return false;
	}
	if (f.fEnabled[filter::description]) {
		if (m_description != f.fDesc_match)
			return false;
	}

	return true;
}