#pragma once
#include "stdafx.h"
#include "data/transaction.h"

class settings {
public:
	//DEFINITIONS
	struct category {
		double identifier;
		QString category_name;
		QColor category_color;
	};
	enum monthly_type {
		budget, recurring, income
	};
	
private:
	//DATA
	QVector<transaction*> m_budget;
	QVector<transaction*> m_recurring;
	QVector<transaction*> m_income;
	QVector<category*> m_categories;
	double m_current_balance;
	
	//FUNCTIONS
public:
	//json serializers / deserializers
	bool readJSON();
	bool writeJSON();

	//data manipulation
	bool addCategory(const category& new_category);
	bool removeCategory(const double& id);
	category* editCategory(const double& id);
	
	void addToBalance(const double& amount);
	void setBalance(const double& amount);

	bool addMonthly(const double& amount, const QString& description, const int& category, const monthly_type& type);
	bool removeMonthly(const QString& id);
	transaction* editMonthly(const QString& id);
};