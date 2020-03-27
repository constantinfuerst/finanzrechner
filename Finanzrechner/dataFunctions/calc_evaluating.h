#pragma once
#include "stdafx.h"
#include "filter.h"
#include "../data/transaction.h"
#include "../data/month.h"

//class used to evaluate months based on total balance
//or calculating the result of filtered transactions
class evaluateMonth {
	//FUNCTIONS
public:
	static double calcFiltered(const month& month, const filter& f);
	static double calcBalance(const month& month);
};