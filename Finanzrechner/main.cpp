#include "stdafx.h"
#include "finanzrechner.h"

#include "data/transaction.h"
#include "data/month.h"
#include "dataFunctions/m_container.h"
#include "dataFunctions/calc_evaluating.h"
#include "settings/settings.h"
#include "dataFunctions/filter.h"
#include "fileHandler/fh.h"

#ifdef compileWithCrypt
#include "fileHandler/cryptFileHandler/cryptFH.h"
#else
#include "fileHandler/plainFileHandler/plainFH.h"
#endif

int qtstart(int argc, char* argv[]) {
	QApplication a(argc, argv);
	Finanzrechner w;
	w.show();
	return a.exec();
}

int main(int argc, char *argv[]) {
	settings::init(nullptr);

	month_container mc(nullptr);
	auto* month = mc.getMonth({ 2020,3,1 });
	
	month->addTransaction(transaction::INCOME, 0, 100, { 2020, 3, 1 }, "");
	month->addTransaction(transaction::EXPENSE, 1, 50, { 2020, 3, 1 }, "");
	month->addBudget(1, 75);

	filter f;
	f.enableFilter(filter::category);
	f.selectCategory({1});
	
	const double balance = evaluateMonth::calcBalance(*month);
	const double budget = evaluateMonth::calcFiltered(*month, f);
	
	if (balance != 50)
		DebugBreak();
	if (budget != 25)
		DebugBreak();
	
	settings::get().clear();
	return 1;
}

//WORKING ON: TODO: Refactor, optimize and comment the existing backend
//TODO: Implement data readout functions for gui integration
//TODO: Create GUI with input options and graphic output options
//TODO: Create sctiptable sorting functions
//TODO: Create scriptable displaying functions
//TODO: Create scriptable evaluation functions