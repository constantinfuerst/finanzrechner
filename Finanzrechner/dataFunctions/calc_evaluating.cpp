#include "stdafx.h"
#include "calc_evaluating.h"

double evaluateMonth::calcBalance(const month& month) {
	//loop every transaction
	double month_balance = 0;
	for (auto* t : month.m_transactions)
		//only evaluate transactions (not budgets)
		if (t->isTransaction())
			//add or subtract depending on type
			if (t->isExpense())
				month_balance -= t->m_amount;
			else
				month_balance += t->m_amount;
	return month_balance;
}

double evaluateMonth::calcFiltered(const month& month, const filter& f) {
	double budget = 0, balance = 0;
	//loop every transaction
	for (auto* t : month.m_transactions)
		if (*t == f) {
			//transactions should affect budget variable
			if (t->isTransaction())
				if (t->isExpense())
					balance -= t->m_amount;
				else
					balance += t->m_amount;
			//budget should affect budget only
			if (t->isBudget())
				budget += t->m_amount;
		}
	//returns only balance if budget nonexistent or filtered out
	if (budget == 0) return balance;
	//returns only balance if balance exceeds budget
	if (balance >= budget) return balance;
	//returns budget result
	return budget + balance;
}