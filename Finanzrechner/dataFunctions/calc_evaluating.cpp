#include "stdafx.h"
#include "calc_evaluating.h"

double evaluateMonth::calcCategory(const month& month, const double& catIdent) {
	double budget = 0, spending = 0;
	for (auto* t : month.m_transactions)
		if (*t == catIdent)
			spending += t->m_amount;
	for (auto* b : month.m_budget)
		if (*b == catIdent)
			budget += b->m_amount;
	return budget - spending;
}

double evaluateMonth::calcBalance(const month& month) {
	double budget = 0, spending = 0;
	for (auto* t : month.m_transactions)
		spending += t->m_amount;
	for (auto* b : month.m_budget)
		budget += b->m_amount;
	return budget - spending;
}