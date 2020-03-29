#include "stdafx.h"

#include "dataObtain.h"

dataObtain::dataObtain(month_container* mc, const filter* f) {
	if (!f->fEnabled[filter::date]) return;
	m_mc = mc;
	m_f = f;
	m_currentDate = f->fDate_range[0];
	m_currentTpos = 0;
}

transaction* dataObtain::getNext() {
	if (!m_currentDate.isValid() || m_f == nullptr || m_mc == nullptr) return nullptr;
	//try to set the current month if unset, if setting fails return nullptr
	if (m_currentMonth == nullptr) m_currentMonth = m_mc->getMonth(m_currentDate);
	if (m_currentMonth == nullptr)
		return nullptr;
	
	//obtain the next month if the current ones transactions have all been obtained
	//or if the current transaction container appears to be empty
	if (m_currentTpos >= m_currentMonth->m_transactions.size() ||
		m_currentMonth->m_transactions.empty()) {
		if (m_currentDate >= m_f->fDate_range[1])
			return nullptr;
		m_currentDate = m_currentDate.addMonths(1);
		m_currentTpos = 0;
		m_currentMonth = m_mc->getMonth(m_currentDate);
	}

	//now go through every transaction to be found starting with the incrementer (m_currentTpos)
	//return the first one that matches the supplied filter
	while (m_currentTpos < m_currentMonth->m_transactions.size()) {
		auto* ct = m_currentMonth->m_transactions[m_currentTpos];
		if (*ct == *m_f) {
			m_currentTpos++;
			return ct;
		}
		m_currentTpos++;
	}

	return getNext();
}
