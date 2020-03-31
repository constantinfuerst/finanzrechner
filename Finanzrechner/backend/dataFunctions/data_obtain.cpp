#include "stdafx.h"

#include "data_obtain.h"

data_obtain::data_obtain(month_container* mc, const filter* f) {
	if (!f->f_enabled[filter::date]) return;
	m_mc = mc;
	m_f = f;
	m_current_date = f->f_date_range[0];
	m_current_tpos = 0;
}

transaction* data_obtain::getNext() {
	if (!m_current_date.isValid() || m_f == nullptr || m_mc == nullptr) return nullptr;
	//try to set the current month if unset, if setting fails return nullptr
	if (m_current_month == nullptr) m_current_month = m_mc->getMonth(m_current_date);
	if (m_current_month == nullptr)
		return nullptr;
	
	//obtain the next month if the current ones transactions have all been obtained
	//or if the current transaction container appears to be empty
	if (m_current_tpos >= m_current_month->p_transactions.size() ||
		m_current_month->p_transactions.empty()) {
		if (m_current_date >= m_f->f_date_range[1])
			return nullptr;
		m_current_date = m_current_date.addMonths(1);
		m_current_tpos = 0;
		m_current_month = m_mc->getMonth(m_current_date);
	}

	//now go through every transaction to be found starting with the incrementer (m_current_tpos)
	//return the first one that matches the supplied filter
	while (m_current_tpos < m_current_month->p_transactions.size()) {
		auto* ct = m_current_month->p_transactions[m_current_tpos];
		if (*ct == *m_f) {
			m_current_tpos++;
			return ct;
		}
		m_current_tpos++;
	}

	return getNext();
}
