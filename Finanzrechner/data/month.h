#pragma once
#include "stdafx.h"
#include "transaction.h"

/*
 * ID format:
 * month year type1 type2(bool: budget OR transaction) counter
 * mmyyyyttcccccccc
 */

class month {
	//DATA
	QDate m_month;
	QVector<transaction*> m_transactions;
	QVector<transaction*> m_budget;
	
	//FUNCTIONS
	//constructors
	month(const QDate& monthDate);
	//json serialization / deserialization
	bool readJSON();
	bool writeJSON();
	//data manipulation
	bool addBudget(const bool& type, const double& category, const double& amount);
	bool addTransaction(const bool& type, const double& category, const double& amount, const QDate& date);
	bool removeTransaction(const QString& id);
	transaction& modifyTransaction(const QString& id);
	//helper functions
	QString generateID(const QDate& date, const bool& type1, const bool& type2);
	static QString getFileName(const QDate& i_month);
};

struct numberString {
public:
	QVector<int>* splitNumber(int number);
private:
	int recursiveNumberSplit(int number, QVector<int>* vec);
};