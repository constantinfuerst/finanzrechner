#include "stdafx.h"
#include "finanzrechner.h"

#include "data/transaction.h"
#include "data/month.h"
#include "dataFunctions/m_container.h"

int qtstart(int argc, char* argv[]) {
	QApplication a(argc, argv);
	Finanzrechner w;
	w.show();
	return a.exec();
}

void createMonth(month_container& mc) {
	auto* new_month = mc.getMonth({ 2020, 3, 1 });

	new_month->addTransaction(transaction::INCOME, 2, 100, { 2020, 3, 9 });
	new_month->addTransaction(transaction::EXPENSE, 5, 1000, { 2020, 3, 5 });
	new_month->addTransaction(transaction::INCOME, 0, 10000, { 2020, 3, 1 });
	new_month->addTransaction(transaction::EXPENSE, 6, 50, { 2020, 3, 12 });

	new_month->addBudget(10, 100);
	new_month->addBudget(41, 199);
}

int main(int argc, char *argv[]) {
	month_container mc;
	
	createMonth(mc);
	
	return qtstart(argc, argv);
}

//TODO: Add settings storing savings, categories, monthly income and budget information
//TODO: Add evaluating functions (earnings or losses for a month)
//TODO: Add scripting support via qjs for functions like "calculate savings" etc
//TODO: Create GUI with input options and graphic output options
//TODO: Create sctiptable sorting functions
//TODO: Create scriptable displaying functions