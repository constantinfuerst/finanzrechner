#include "stdafx.h"
#include "finanzrechner.h"

#include "data/transaction.h"
#include "data/month.h"
#include "dataFunctions/m_container.h"
#include "dataFunctions/calc_evaluating.h"
#include "settings/settings.h"
#include "fileHandler/fh.h"
#include "fileHandler/plainFileHandler/plainFH.h"

#ifdef compileWithCrypt
#include "fileHandler/cryptFileHandler/cryptFH.h"
#endif

int qtstart(int argc, char* argv[]) {
	QApplication a(argc, argv);
	Finanzrechner w;
	w.show();
	return a.exec();
}

void addSettings(){
	settings::get().addMonthly(5000, "WORK", 0, settings::monthly_type::income);
	settings::get().addMonthly(30, "PIZZA", 0, settings::monthly_type::recurring);
}

void createMonth(month_container& mc) {
	auto* new_month = mc.getMonth({ 2020, 3, 1 });
	settings::get().fillMonth(new_month);
	
	new_month->addTransaction(transaction::INCOME, 2, 100, { 2020, 3, 9 }, "test1");
	new_month->addTransaction(transaction::EXPENSE, 5, 1000, { 2020, 3, 5 }, "test2");
	new_month->addTransaction(transaction::INCOME, 0, 10000, { 2020, 3, 1 }, "test3");
	new_month->addTransaction(transaction::EXPENSE, 6, 50, { 2020, 3, 12 }, "test4");

	new_month->addBudget(10, 100, "test5");
	new_month->addBudget(41, 199, "test6");
}

void storeSavings(month_container& mc) {
	settings::get().setBalance(100000);
	auto* m = mc.getMonth({ 2020, 3, 1 });
	const double monthlyBalance = evaluateMonth::calcBalance(*m);
	settings::get().addToBalance(monthlyBalance);
}

int main(int argc, char *argv[]) {
#ifndef compileWithCrypt
	auto* fh = new plainFileHandler;
#else
	auto* fh = new cryptFileHandler;
#endif
	month_container mc(fh);
	settings::init(fh);
	
	addSettings();
	createMonth(mc);
	storeSavings(mc);
	
	return qtstart(argc, argv);
}

//WORKING ON: TODO: Implement / fix the crypto file storage handler
//TODO: Create GUI with input options and graphic output options
//TODO: Create sctiptable sorting functions
//TODO: Create scriptable displaying functions
//TODO: Create scriptable evaluation functions