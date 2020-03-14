#pragma once
#include "stdafx.h"
#include "transaction.h"

/*
 * ID format:
 * month year type1 type2(bool: budget (true) OR transaction (false)) counter
 * mmyyyyttcccccccc
 *
 * every modifying function shall always update the "modified" status to true, indicating a save to the json file is necessary
 */

class month {
	//DATA
	QDate m_month;
	QVector<transaction*> m_transactions;
	QVector<transaction*> m_budget;
	bool modified = false;
	
	//FUNCTIONS
	//constructors
	month(const QDate& monthDate);
	
	//json serialization / deserialization
	bool readJSON();
	bool writeJSON();
	
	//data manipulation
	void addBudget(const bool& type, const double& category, const double& amount);
	void addTransaction(const bool& type, const double& category, const double& amount, const QDate& date);
	bool removeTransaction(const QString& id);
	transaction* modifyTransaction(const QString& id);
	
	//helper functions
	QString generateID(const QDate& date, const bool& type1, const bool& type2);
	static QString getFileName(const QDate& i_month);
};