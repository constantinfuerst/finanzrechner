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