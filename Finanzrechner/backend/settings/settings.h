#pragma once
#include "stdafx.h"
#include "../data/transaction.h"
#include "../data/month.h"

//singleton class used to store monthly data such as:
//recurring payments, income, budgets and account balance
//also provides acces to the category structure used for grouping payments and budgets
class settings {
public:
	//SINGLETON ACCESS
	static settings& get(fileHandler* fh_in = nullptr);
	static void init(fileHandler* fh_in);

	//DEFINITIONS
	struct category {
		//data
		double identifier;
		std::string category_name;
		QColor category_color;
		//functions
		category(const double& id, const std::string& name, const QColor& color);
		category(const QJsonObject& json);
		bool operator==(const double& id) const;
		static category* fromJSON(const QJsonObject& json);
		[[nodiscard]] QJsonObject* toJSON() const;
	};
	enum monthly_type {
		budget, recurring, income
	};

private:
	//DATA
	bool m_modified = false;

	double m_id_counter;
	double m_cat_counter;

	fileHandler* m_fh = nullptr;

	std::vector<transaction*> m_monthly;
	std::vector<category*> m_categories;
	double m_current_balance;

	//FUNCTIONS
	//constructor / deconstructor
	~settings();
	settings(fileHandler* fh_in);
public:
	void clear();

	//json serializers / deserializers
	bool readJSON();
	bool writeJSON();

	//data manipulation
	void addCategory(const std::string& name, const QColor& color);
	bool removeCategory(const double& id);
	category* editCategory(const double& id);

	void addToBalance(const double& amount);
	void setBalance(const double& amount);

	bool addMonthly(const double& amount, const std::string& description, const int& category, const monthly_type& type);
	bool removeMonthly(const std::string& id);
	transaction* editMonthly(const std::string& id);

	void fillMonth(month* m);
};