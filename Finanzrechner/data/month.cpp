#include "stdafx.h"
#include "month.h"

month::month(const QDate& monthDate, fileHandler* fh_in) {
	m_month = monthDate;
	fh = fh_in;
}

month::~month() {
	if (modified)
		writeJSON();
	for (auto* e : m_transactions)
		delete e;
	m_transactions.clear();
}

//generate unique ID for identification in user interface
std::string month::generateID(const QDate& date, bool budget, bool transaction, bool expense, bool income) {
	std::string num = std::to_string(m_idCounter);
	while (num.size() < 16)
		num = '0' + num;
	std::string id = date.toString("MM-yyyy").toStdString();
	id += "-";
	id += (budget ? "1" : "0");
	id += (transaction ? "1" : "0");
	id += (expense ? "1" : "0");
	id += (income ? "1" : "0");
	id += "-";
	id += num;
	m_idCounter++;
	return id;
}

//used to search for a month by date
bool month::operator==(const QDate& date) const {
	if (date.month() == m_month.month() && date.year() == m_month.year())
		return true;
	return false;
}

//implementation of filter comparison
bool month::operator==(const filter& f) const {
	if (f.fEnabled[filter::date]) {
		if (
			m_month < f.fDate_range[0] ||	//test for lower than low range
			m_month > f.fDate_range[1]		//test for higher than high range
			)
			return false;
	}
	return true;
}