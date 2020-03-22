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

void fillMonth(month_container& mc) {
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

void accessMonth(month_container& mc) {
	auto* m = mc.getMonth({ 2020, 3, 1 });
}

int main(int argc, char *argv[]) {
	auto* fh = new cryptFileHandler;
	fh->setKEY("testpassword");
	if (!fh->checkKEY())
		return false;
	
	{
		month_container mc(fh);
		settings::init(fh);

		const bool fill = false;

		if (fill) {
			addSettings();
			fillMonth(mc);
			storeSavings(mc);
		}
		else {
			accessMonth(mc);
		}
	}
	
	fh->eraseKEY();
	
	return true;
}

//TODO: Implement salting and csprng for iv to make encryption actually safe
//TODO: Create sctiptable sorting functions
//TODO: Create scriptable displaying functions
//TODO: Create scriptable evaluation functions
//TODO: Create GUI with input options and graphic output options
//TODO: Implement a "update password" function that decrypts every file in the database one by one into memory and reencrypts with the new key