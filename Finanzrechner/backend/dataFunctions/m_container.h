#pragma once
#include "stdafx.h"

#include "../data/transaction.h"
#include "../data/month.h"
#include "../fileHandler/fh.h"
#include "filter.h"

//container class used to organize and group all loaded months and their data
class month_container {
	//DATA
	std::vector<month*> m_loaded_months;
	fileHandler* fh;

	//FUNCTIONS
	month* findLoadedMonth(const QDate& date);

	//guarded versions prevent duplicate month loading
	month* loadMonth_unguarded(const QDate& date);

public:
	month_container(fileHandler* fh);
	~month_container();
	month* getMonth(const QDate& date);
	void loadByFilter(const filter* f);
	bool removeLoadedMonth(const QDate& date);
};