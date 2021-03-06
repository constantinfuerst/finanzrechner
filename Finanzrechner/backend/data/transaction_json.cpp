#include "stdafx.h"
#include "transaction.h"

//reads transaction information out of a supplied json object
transaction* transaction::fromJSON(const QJsonObject& json) const {
	bool bud = false, trans = false, exp = false, inc = false, rec = false;
	std::string trans_id, description;
	double category = 0, amount = 0;
	QDate date;

	//check if data exists and set the appropriate values
	if (json.contains("bud") && json["bud"].isBool())
		bud = json["bud"].toBool();
	if (json.contains("trans") && json["trans"].isBool())
		trans = json["trans"].toBool();
	if (json.contains("exp") && json["exp"].isBool())
		exp = json["exp"].toBool();
	if (json.contains("inc") && json["inc"].isBool())
		inc = json["inc"].toBool();
	if (json.contains("category") && json["category"].isDouble())
		category = json["category"].toInt();
	if (json.contains("amount") && json["amount"].isDouble())
		amount = json["amount"].toDouble();
	if (json.contains("id") && json["id"].isString())
		trans_id = json["id"].toString().toStdString();
	if (json.contains("date") && json["date"].isString())
		date = QDate::fromString(json["date"].toString());
	if (json.contains("description") && json["description"].isString())
		description = json["description"].toString().toStdString();
	if (json.contains("recurring") && json["recurring"].isBool())
		rec = json["recurring"].toBool();

	return new transaction(bud, trans, exp, inc, category, amount, rec, description, date, trans_id);
}

//constructs a json object out of transaction data
QJsonObject* transaction::toJSON() const {
	auto* json = new QJsonObject;
	(*json)["id"] = QString::fromStdString(m_trans_id);
	(*json)["bud"] = m_budget;
	(*json)["trans"] = m_transaction;
	(*json)["exp"] = m_expense;
	(*json)["inc"] = m_income;
	(*json)["amount"] = p_amount;
	(*json)["category"] = m_category;
	(*json)["date"] = m_date.toString("dd-MM-yyyy");
	(*json)["description"] = QString::fromStdString(m_description);
	(*json)["recurring"] = m_recurring;
	return json;
}