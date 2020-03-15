#pragma once
#include "stdafx.h"
#include "../data/transaction.h"
#include "../data/month.h"

class month_container {
	//DATA
	QVector<month*> m_loaded_months;
	
	//FUNCTIONS
private:
	month* findLoadedMonth(const QDate& date);
	
	//guarded versions prevent duplicate month loading
	month* loadMonth_unguarded(const QDate& date);
	
public:
	month_container() = default;
	~month_container();
	month* getMonth(const QDate& date);
	bool removeLoadedMonth(const QDate& date);
};