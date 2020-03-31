#pragma once
#include "stdafx.h"

#include "m_container.h"
#include "filter.h"
#include "../data/transaction.h"
#include "../data/month.h"

class data_obtain {
private:
	QDate m_current_date;
	size_t m_current_tpos;
	
	month* m_current_month = nullptr;
	month_container* m_mc = nullptr;
	const filter* m_f = nullptr;
	
public:
	//filter must have "date" filtering enabled and correctly set
	//in order to determine search space
	data_obtain(month_container* mc, const filter* f);
	//operation vulnerable to deletion of month or transaction data during runtime
	//should be used with a while loop while taking care to not cause destruction or deletion
	//of elements in the months or transactions in question
	//function has guards in place to prevent crashes caused by accidental deletion
	transaction* getNext();
};