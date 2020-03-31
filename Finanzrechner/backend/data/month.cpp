#include "stdafx.h"
#include "month.h"

month::month(const QDate& month_date, fileHandler* fh_in) {
	p_month = month_date;
	m_fh = fh_in;
}

month::~month() {
	if (m_modified)
		writeJSON();
	for (auto* e : p_transactions)
		delete e;
	p_transactions.clear();
}

//used to search for a month by date
bool month::operator==(const QDate& date) const {
	if (date.month() == p_month.month() && date.year() == p_month.year())
		return true;
	return false;
}

//implementation of filter comparison
bool month::operator==(const filter& f) const {
	if (f.f_enabled[filter::date]) {
		if (
			p_month < f.f_date_range[0] ||	//test for lower than low range
			p_month > f.f_date_range[1]		//test for higher than high range
			)
			return false;
	}
	return true;
}