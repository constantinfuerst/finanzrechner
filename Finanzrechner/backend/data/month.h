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
 * every modifying function shall always update the "m_modified" status to true, indicating a save to the json file is necessary
 */

class month {
public:
	//DATA
	std::vector<transaction*> p_transactions;
	QDate p_month;
	
private:
	fileHandler* m_fh;
	bool m_modified = false;

	//FUNCTIONS
	//helper functions
	static std::string getFileName(const QDate& i_month);

public:
	//constructors
	month(const QDate& month_date, fileHandler* fh_in);
	~month();

	//json serialization / deserialization
	bool readJSON();
	bool writeJSON();

	//"T" functions operate on both budget and transaction
	void addTransaction(transaction* transaction);
	bool removeTransaction(const std::string& id);
	transaction* modifyTransaction(const std::string& id);

	//data interaction
	bool operator==(const QDate& date) const;
	bool operator==(const filter& f) const;
};