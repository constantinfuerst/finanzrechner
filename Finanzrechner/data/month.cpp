#include "stdafx.h"
#include "month.h"

month::month(const QDate& monthDate) {
	m_month = monthDate;
}

QString month::generateID(const QDate& date, const bool& type1, const bool& type2) {
	static int idCounter = 0;
	QString id;
	QString m = date.month(); if (m.size() == 1) m = '0' + m;
	id += m;
	id += date.year();
	id += int(type1);
	id += int(type2);
	id += idCounter;
	return id;
}

QVector<int>* numberString::splitNumber(int number) {
	auto* vec = new QVector<int>;
	recursiveNumberSplit(number, vec);
	vec->push_back(number - (number / 10) * 10);
	return vec;
}

int numberString::recursiveNumberSplit(int number, QVector<int>* vec) {
	int localN = number / 10;
	if (localN < 10) {
		vec->append(localN);
		return localN;
	}
	const int split = recursiveNumberSplit(localN, vec) * 10;
	const int calc = localN - split;
	vec->push_back(calc);
	return calc + split;
}

QString month::getFileName(const QDate& i_month) {
	return i_month.toString("MM.yyyy") + '.json';
}

bool month::writeJSON() {
	QFile saveFile(getFileName(m_month));
	
	if (!saveFile.open(QIODevice::WriteOnly)) {
		qWarning("Couldn't open save file.");
		return false;
	}

	QJsonArray transactions;
	for (transaction* t : m_transactions) {
		transactions.push_back(t->toJSON());
		delete t;
	}
	QJsonArray budget;
	for (transaction* t : m_budget) {
		budget.push_back(t->toJSON());
		delete t;
	}

	QJsonObject month;
	month["transactions"] = transactions;
	month["budget"] = budget;
	month["date"] = m_month.toString("MM.yyyy");
	
	QJsonDocument saveDoc(month);
	saveFile.write(saveDoc.toJson());

	return true;
}

bool month::readJSON() {
	QFile loadFile(getFileName(m_month));

	if (!loadFile.open(QIODevice::ReadOnly)) {
		qWarning("Couldn't open save file.");
		return false;
	}

	QByteArray data = loadFile.readAll();
	QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
	auto json = jsonDoc.object();
	
	if (json.contains("budget") && json["budget"].isArray()) {
		QJsonArray budgetArray = json["budget"].toArray();
		for (auto e : budgetArray)
			if (e.isObject())
				m_budget.push_back(new transaction(e.toObject()));
	}
	else {
		qWarning("Did not find budget listing, possible corrupt file.");
		return false;
	}
	
	if (json.contains("transactions") && json["transactions"].isArray()) {
		QJsonArray transactionArray = json["transactions"].toArray();
		for (auto e : transactionArray)
			if (e.isObject())
				m_transactions.push_back(new transaction(e.toObject()));
	}
	else {
		qWarning("Did not find transactions listing, possible corrupt file.");
		return false;
	}
	
	return true;
}
