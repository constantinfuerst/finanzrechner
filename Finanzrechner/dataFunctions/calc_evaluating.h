#pragma once
#include "stdafx.h"
#include "../data/transaction.h"
#include "../data/month.h"

class evaluateMonth {
	//FUNCTIONS
public:
	static double calcCategory(const month& month, const double& catIdent);
	static double calcBalance(const month& month);
};