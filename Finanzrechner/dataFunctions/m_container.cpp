#include "stdafx.h"
#include "m_container.h"

month_container::month_container(fileHandler* fh_in) {
	fh = fh_in;
}

month_container::~month_container() {
	for (auto* m : m_loaded_months)
		delete m;
}

//searches for a month, if month not found try to load from file
//may return empty month with only date set
month* month_container::getMonth(const QDate& date) {
	auto* loaded = findLoadedMonth(date);
	if (loaded != nullptr) return loaded;
	return loadMonth_unguarded(date);
}

//searches for month in loaded vector
month* month_container::findLoadedMonth(const QDate& date) {
	for (auto* m : m_loaded_months)
		if (*m == date)
			return m;
	return nullptr;
}

//unloads a month from vector
//will trigger the months writing function through destructor
bool month_container::removeLoadedMonth(const QDate& date) {
	for (int i = 0; i < m_loaded_months.size(); i++) {
		auto* m = m_loaded_months[i];
		if (*m == date) {
			delete m;
			m_loaded_months.erase(m_loaded_months.begin() + i);
			return true;
		}
	}
	return false;
}

//loads a month into vector without checking for it already being loaded
//should be called after findLoadedMonth returns nullptr
month* month_container::loadMonth_unguarded(const QDate& date) {
	auto* m = new month(date, fh);
	m->readJSON();
	m_loaded_months.push_back(m);
	return m;
}