#include "stdafx.h"
#include "month.h"

QString month::getFileName(const QDate& i_month) {
	return savedir + QString("\\data\\") + i_month.toString("MM-yyyy");
}

bool month::writeJSON() {
	QJsonArray transactions;
	for (transaction* t : m_transactions) {
		QJsonObject tobj = *t->toJSON();
		transactions.append(tobj);
		delete t;
	} m_transactions.clear();
	
	QJsonArray budget;
	for (transaction* t : m_budget) {
		QJsonObject tobj = *t->toJSON();
		budget.append(tobj);
		delete t;
	} m_budget.clear();

	QJsonObject month;
	month["date"] = m_month.toString("MM-yyyy");
	month["idCounter"] = m_idCounter;
	month["transactions"] = transactions;
	month["budget"] = budget;

	auto* jdoc = new QJsonDocument(month);
	fh->writeJSON(jdoc, getFileName(m_month));
	delete jdoc;
	
	return true;
}

bool month::readJSON() {
	auto* jdoc = fh->readJSON(getFileName(m_month));
	auto json = jdoc->object();
	delete jdoc;

	if (json.contains("budget") && json["budget"].isArray()) {
		QJsonArray budgetArray = json["budget"].toArray();
		for (auto e : budgetArray)
			if (e.isObject())
				m_budget.append(new transaction(e.toObject()));
	}
	else {
		qWarning("Did not find budget listing, possible corrupt file.");
		return false;
	}

	if (json.contains("transactions") && json["transactions"].isArray()) {
		QJsonArray transactionArray = json["transactions"].toArray();
		for (auto e : transactionArray)
			if (e.isObject())
				m_transactions.append(new transaction(e.toObject()));
	}
	else {
		qWarning("Did not find transactions listing, possible corrupt file.");
		return false;
	}

	if (json.contains("idCounter") && json["idCounter"].isDouble()) {
		m_idCounter = json["idCounter"].toDouble();
	}
	else {
		qWarning("Did not find id listing, possible corrupt file.");
		return false;
	}

	return true;
}
