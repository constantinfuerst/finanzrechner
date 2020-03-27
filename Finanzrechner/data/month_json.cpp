#include "stdafx.h"
#include "month.h"

std::string month::getFileName(const QDate& i_month) {
	return savedir + std::string("\\data\\") + i_month.toString("MM-yyyy").toStdString();
}

bool month::writeJSON() {
	if (fh == nullptr) return false;
	QJsonArray transactions;
	for (transaction* t : m_transactions) {
		QJsonObject tobj = *t->toJSON();
		transactions.append(tobj);
		delete t;
	} m_transactions.clear();

	QJsonObject month;
	month["date"] = m_month.toString("MM-yyyy");
	month["idCounter"] = m_idCounter;
	month["transactions"] = transactions;

	auto* jdoc = new QJsonDocument(month);
	fh->writeJSON(jdoc, getFileName(m_month));
	delete jdoc;
	
	return true;
}

bool month::readJSON() {
	if (fh == nullptr) return false;
	auto* jdoc = fh->readJSON(getFileName(m_month));
	if (jdoc == nullptr) return false;
	auto json = jdoc->object();
	delete jdoc;

	if (json.contains("transactions") && json["transactions"].isArray()) {
		QJsonArray transactionArray = json["transactions"].toArray();
		for (auto e : transactionArray)
			if (e.isObject())
				m_transactions.push_back(new transaction(e.toObject()));
	}
	else return false;

	if (json.contains("idCounter") && json["idCounter"].isDouble()) {
		m_idCounter = json["idCounter"].toDouble();
	}
	else return false;

	return true;
}
