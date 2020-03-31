#include <stdafx.h>
#include "calc_evaluating.h"

double evaluate_month::calcBalance(const month& month) {
	//loop every transaction
	double month_balance = 0;
	for (auto* t : month.p_transactions)
		//only evaluate transactions (not budgets)
		if (t->isTransaction()) {
			//add or subtract depending on type
			if (t->isExpense())
				month_balance -= t->p_amount;
			else
				month_balance += t->p_amount;
		}
	return month_balance;
}

double evaluate_month::calcFiltered(month_container* mc, const filter* f) {
	double budget = 0, balance = 0;
	mc->loadByFilter(f);
	data_obtain data(mc, f);
	auto* t = data.getNext();
	
	//loop every matching transaction
	while (t != nullptr) {
		if (*t == *f) {
			//transactions should affect budget variable
			if (t->isTransaction()) {
				if (t->isExpense())
					balance -= t->p_amount;
				else
					balance += t->p_amount;
			}
			//budget should affect budget only
			if (t->isBudget())
				budget += t->p_amount;
		}
		t = data.getNext();
	}
	
	//returns only balance if budget nonexistent or filtered out
	if (budget == 0) return balance;
	//returns only balance if balance exceeds budget
	if (balance >= budget) return balance;
	//returns budget result
	return budget + balance;
}