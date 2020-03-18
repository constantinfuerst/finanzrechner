#pragma once
#include "stdafx.h"
#include "../data/transaction.h"
#include "../data/month.h"

class settings {
public:
	//DEFINITIONS
	struct category {
		//data
		double identifier;
		QString category_name;
		QColor category_color;
		//functions
		category(const double& id, const QString& name, const QColor& color);
		category(const QJsonObject& json);
		bool operator==(const double& id) const ;
		category* fromJSON(const QJsonObject& json) const;
		QJsonObject* toJSON() const;
	};
	enum monthly_type {
		budget, recurring, income
	};
	
private:
	//DATA
	bool modified = false;
	
	double m_idCounter;
	double m_catCounter;
	
	QVector<transaction*> m_budget;
	QVector<transaction*> m_recurring;
	QVector<transaction*> m_income;
	QVector<category*> m_categories;
	double m_current_balance;
	
	//FUNCTIONS
	QString generateID(const monthly_type& type, const int& category);
	
public:
	//constructor / deconstructor
	settings();
	~settings();
	
	//json serializers / deserializers
	bool readJSON();
	bool writeJSON();

	//data manipulation
	void addCategory(const QString& name, const QColor& color);
	bool removeCategory(const double& id);
	category* editCategory(const double& id);
	
	void addToBalance(const double& amount);
	void setBalance(const double& amount);

	bool addMonthly(const double& amount, const QString& description, const int& category, const monthly_type& type);
	bool removeMonthly(const QString& id);
	transaction* editMonthly(const QString& id);

	void fillMonth(month* m);
};