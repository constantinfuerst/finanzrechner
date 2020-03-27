#pragma once
#include "stdafx.h"
#include "filter.h"
#include "../data/transaction.h"
#include "../data/month.h"

class evaluateMonth {
	//FUNCTIONS
public:
	static double calcFiltered(const month& month, const filter& f);
	static double calcBalance(const month& month);
};