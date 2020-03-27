#include "stdafx.h"
#include "calc_evaluating.h"

double evaluateMonth::calcCategory(const month& month, const double& catIdent) {
	double budget = 0, spending = 0;
	for (auto* t : month.m_transactions)
		if (t->m_bookingType == transaction::TRANSACTION)
			if (*t == catIdent)
				spending += t->m_amount;
	for (auto* b : month.m_transactions)
		if (b->m_bookingType == transaction::BUDGET)
			if (*b == catIdent)
				budget += b->m_amount;
	return budget - spending;
}

double evaluateMonth::calcBalance(const month& month) {
	double month_balance = 0;
	for (auto* t : month.m_transactions)
		if (t->m_bookingType == transaction::TRANSACTION)
			if (t->m_tType == transaction::EXPENSE)
				month_balance -= t->m_amount;
			else
				month_balance += t->m_amount;
	return month_balance;
}