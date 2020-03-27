#include "stdafx.h"
#include "calc_evaluating.h"

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

double evaluateMonth::calcFiltered(const month& month, const filter& f) {
	double budget = 0, balance = 0;
	for (auto* t : month.m_transactions)
		if (*t == f) {
			if (t->m_bookingType == transaction::TRANSACTION)
				if (t->m_tType == transaction::EXPENSE)
					balance -= t->m_amount;
				else
					balance += t->m_amount;
			if (t->m_bookingType == transaction::BUDGET)
				budget += t->m_amount;
		}
	//returns only balance if budget nonexistent or filtered out
	if (budget == 0) return balance;
	//returns only balance if balance exceeds budget
	if (balance >= budget) return balance;
	//returns budget result
	return budget + balance;
}
