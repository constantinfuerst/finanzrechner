#include "stdafx.h"
#include "frontend/finanzrechner.h"
#include "backend/financeBackend.h"

int qtstart(int argc, char* argv[]) {
	QApplication a(argc, argv);
	Finanzrechner w;
	w.show();
	return a.exec();
}

const QDate february = { 2020, 2, 1 };
const QDate march = { 2020, 3, 1 };

void fillMonth(month_container* mc) {
	auto* month = mc->getMonth(february);
	month->addTransaction(transaction::makeExpense(1, 10, "ft1", february));
	month->addTransaction(transaction::makeIncome(2, 100, "ft2", february));
	month->addTransaction(transaction::makeBudget(1, 15));

	month = mc->getMonth(march);
	month->addTransaction(transaction::makeExpense(1, 10, "mt1", march));
	month->addTransaction(transaction::makeIncome(2, 100, "mt2", march));
	month->addTransaction(transaction::makeBudget(1, 15));
}

int main(int argc, char *argv[]) {
	auto* mc = new month_container(nullptr);
	fillMonth(mc);

	auto* fIncome = new filter;
	fIncome->enableFilter(filter::date);
	fIncome->selectDate(february, march);
	fIncome->enableFilter(filter::type);
	fIncome->selectType(true, true, false, true);

	auto* fBudget = new filter;
	fBudget->enableFilter(filter::date);
	fBudget->selectDate(february, march);
	fBudget->enableFilter(filter::category);
	fBudget->selectCategory({ 1 });

	double income = evaluateMonth::calcFiltered(mc, fIncome);
	double budget = evaluateMonth::calcFiltered(mc, fBudget);
	
	DebugBreak();
	return qtstart(argc, argv);
}

//WORKING ON: TODO: find any bugs, check for memory leaks and optimize backend as much as possible
//WORKING ON: TODO: repair calcFiltered with budget
//TODO: Create GUI with input options and graphic output options