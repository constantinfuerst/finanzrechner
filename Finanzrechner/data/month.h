#pragma once
#include "stdafx.h"
#include "transaction.h"
#include "../fileHandler/fh.h"
#include "../dataFunctions/filter.h"

/*
 * ID format:
 * month year type1 type2(bool: budget (true) OR transaction (false)) counter
 * mmyyyyttcccccccc
 *
 * every modifying function shall always update the "modified" status to true, indicating a save to the json file is necessary
 */

class month {
public:
	//DATA
	std::vector<transaction*> m_transactions;
private:
	fileHandler* fh;
	QDate m_month;
	bool modified = false;
	double m_idCounter = 0;
	
	//FUNCTIONS
	//helper functions
	std::string generateID(const QDate& date, const bool& expense_or_income, const bool& budget_or_transaction);
	static std::string getFileName(const QDate& i_month);
	
public:
	//constructors
	month(const QDate& monthDate, fileHandler* fh);
	~month();
	
	//json serialization / deserialization
	bool readJSON();
	bool writeJSON();
	
	//data manipulation
	//these functions only accept either budget or transaction
	void addTransaction(const bool& ttype, const double& category, const double& amount, const QDate& date, const std::string& description);
	void addBudget(const double& category, const double& amount);

	//"T" functions operate on both budget and transaction
	void addT(transaction* transaction);
	bool removeT(const std::string& id);
	transaction* modifyT(const std::string& id);

	//data interaction
	bool operator==(const QDate& date) const;
	bool operator==(const filter& f) const;
};