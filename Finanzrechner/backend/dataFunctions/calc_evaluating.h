#pragma once
#include "stdafx.h"
#include "filter.h"
#include "../data/transaction.h"
#include "../data/month.h"
#include "m_container.h"
#include "data_obtain.h"

//class used to evaluate months based on total balance
//or calculating the result of filtered transactions
class evaluate_month {
	//FUNCTIONS
public:
	//uses a filter in conjunction with data_obtain to calculate selected values
	//subject to same requirements as data_obtain
	//meaning: filter must have valid and enabled date filtering
	static double calcFiltered(month_container* mc, const filter* f);
	static double calcBalance(const month& month);
};