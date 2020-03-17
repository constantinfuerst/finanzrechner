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
public:
	//DEFINITIONS
	static const bool BUDGET = false;
	static const bool TRANSACTION = true;

	//DATA
	QVector<transaction*> m_transactions;
	QVector<transaction*> m_budget;
private:
	QDate m_month;
	bool modified = false;
	double m_idCounter = 0;
	
	//FUNCTIONS
	//helper functions
	QString generateID(const QDate& date, const bool& expense_or_income, const bool& budget_or_transaction);
	static QString getFileName(const QDate& i_month);
	
public:
	//constructors
	month(const QDate& monthDate);
	~month();
	
	//json serialization / deserialization
	bool readJSON();
	bool writeJSON();
	
	//data manipulation
	void addBudget(const double& category, const double& amount, const QString& description);
	void addTransaction(const bool& type, const double& category, const double& amount, const QDate& date, const QString& description);
	bool removeTransaction(const QString& id);
	transaction* modifyTransaction(const QString& id);

	//data interaction
	bool operator==(const QDate& date) const;
};