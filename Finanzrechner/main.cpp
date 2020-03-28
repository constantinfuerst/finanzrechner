#include "stdafx.h"
#include "finanzrechner.h"

#include "data/transaction.h"
#include "data/month.h"
#include "dataFunctions/m_container.h"
#include "dataFunctions/calc_evaluating.h"
#include "dataFunctions/filter.h"
#include "dataFunctions/dataObtain.h"
#include "settings/settings.h"
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

void fillMonth(month_container* mc) {
	auto* month = mc->getMonth({ 2020,3,1 });
	month->addTransaction(false, true, 0, 100, { 2020, 3, 1 }, "m1t1");
	month->addTransaction(true, false, 1, 50, { 2020, 3, 1 }, "m1t2");
	month->addBudget(1, 75);

	month = mc->getMonth({ 2020,2,1 });
	month->addTransaction(false, true, 0, 100, { 2020, 3, 1 }, "m2t1");
	month->addTransaction(true, false, 1, 50, { 2020, 3, 1 }, "m2t2");
	month->addBudget(1, 75);
}

int main(int argc, char *argv[]) {
	month_container mc(nullptr);
	fillMonth(&mc);

	filter f;
	f.enableFilter(filter::date);
	f.enableFilter(filter::type);
	f.selectDate({ 2020, 2, 1 }, { 2020, 3, 31 });
	f.selectType(true, true, false, true);

	double amount = 0;
	
	dataObtain data(&mc, &f);
	transaction* t = data.getNext();
	while (t != nullptr) {
		if (t->isIncome())
			amount += t->m_amount;
		else
			amount -= t->m_amount;
		t = data.getNext();
	}
	
	return 1;
}

//TODO: find a way to make transaction constructor more intuitive and less prone to programmer error
//TODO: Create GUI with input options and graphic output options