#include <stdafx.h>

#include "frontend/financecalc.h"
#include "backend/financeBackend.h"

int qtstart(int argc, char* argv[]) {
	QApplication a(argc, argv);
	financecalc w;
	w.show();
	return a.exec();
}

const QDate FEBRUARY = { 2020, 2, 1 };
const QDate MARCH = { 2020, 3, 1 };

void fillMonth(month_container* mc) {
	auto* month = mc->getMonth(FEBRUARY);
	month->addTransaction(transaction::makeExpense(1, 10, "ft1", FEBRUARY));
	month->addTransaction(transaction::makeIncome(2, 100, "ft2", FEBRUARY));
	month->addTransaction(transaction::makeBudget(1, 15));

	month = mc->getMonth(MARCH);
	month->addTransaction(transaction::makeExpense(1, 10, "mt1", MARCH));
	month->addTransaction(transaction::makeIncome(2, 100, "mt2", MARCH));
	month->addTransaction(transaction::makeBudget(1, 15));
}

void testBackend() {
	auto* mc = new month_container(nullptr);
	fillMonth(mc);

	auto* f_income = new filter;
	f_income->enableFilter(filter::date);
	f_income->selectDate(FEBRUARY, MARCH);
	f_income->enableFilter(filter::type);
	f_income->selectType(true, true, false, true, false);

	auto* f_budget = new filter;
	f_budget->enableFilter(filter::date);
	f_budget->selectDate(FEBRUARY, MARCH);
	f_budget->enableFilter(filter::category);
	f_budget->selectCategory({ 1 });

	const double income = evaluate_month::calcFiltered(mc, f_income);
	const double budget = evaluate_month::calcFiltered(mc, f_budget);

	DebugBreak();
}

int main(int argc, char *argv[]) {
	testBackend();
	return qtstart(argc, argv);
}

//TODO: Create GUI with input options and graphic output options