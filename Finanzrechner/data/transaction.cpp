#include "stdafx.h"
#include "transaction.h"

transaction::transaction(bool budget, bool transaction, bool expense, bool income, std::string& transID, const double& category, const double& amount, const std::string& description, const QDate& date) {
	m_budget = budget, m_transaction = transaction, m_expense = expense, m_income = income, m_transID = transID; m_category = category; m_amount = amount; m_date = date; m_description = description;
}

transaction::transaction(const QJsonObject& json) {
	auto* t = fromJSON(json);
	m_budget = t->m_budget; m_transaction = t->m_transaction; m_expense = t->m_expense; m_income = t->m_income; m_transID = t->m_transID;  m_category = t->m_category; m_amount = t->m_amount; m_date = t->m_date;
	delete t;
}

transaction::transaction(const transaction& t) {
	m_amount = t.m_amount;
	m_budget = t.m_budget;
	m_transaction = t.m_transaction;
	m_expense = t.m_expense;
	m_income = t.m_income;
	m_category = t.m_category;
	m_date = t.m_date;
	m_description = t.m_description;
	m_transID = t.m_transID;
}

//reads transaction information out of a supplied json object
transaction* transaction::fromJSON(const QJsonObject& json) const {
	bool bud = false, trans = false, exp = false, inc = false;
	std::string transID, description;
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
		transID = json["id"].toString().toStdString();
	if (json.contains("date") && json["date"].isString())
		date = QDate::fromString(json["date"].toString());
	if (json.contains("description") && json["description"].isString())
		description = json["description"].toString().toStdString();

	return new transaction(bud, trans, exp, inc, transID, category, amount, description, date);
}

//constructs a json object out of transaction data
QJsonObject* transaction::toJSON() const {
	auto* json = new QJsonObject;
	(*json)["id"] = QString::fromStdString(m_transID);
	(*json)["bud"] = m_budget;
	(*json)["trans"] = m_transaction;
	(*json)["exp"] = m_expense;
	(*json)["inc"] = m_income;
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
		if (m_expense)
			if (f.fTypes_selected[0])
				accepted = true;
		if (m_income)
			if (f.fTypes_selected[1])
				accepted = true;
		if (m_budget)
			if (f.fTypes_selected[2])
				accepted = true;
		if (m_transaction)
			if (f.fTypes_selected[3])
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