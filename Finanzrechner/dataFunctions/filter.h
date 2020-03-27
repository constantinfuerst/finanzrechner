#pragma once
#include "stdafx.h"

// TEMPLATE FOR MATCHING FUNCTION
//	bool class::operator==(const filter& f) const {
//		bool match = true;
//		
//		if (f.fEnabled[filter::type]) {
//			
//		}
//		if (f.fEnabled[filter::amount]) {
//
//		}
//		if (f.fEnabled[filter::date]) {
//
//		}
//		if (f.fEnabled[filter::category]) {
//
//		}
//		if (f.fEnabled[filter::description]) {
//
//		}
//		
//		return match;
//	}

struct filter {
public:
	//FILTER DEFINITIONS
	enum filterType {
		type, amount, date, category, description
	};
	//element types: 0 = expense, 1 = income, 2 = budget, 3 = transaction
	
	//ENABLED FILTERS
	//uses index defined by filterType enumerator to select match of type if true
	std::array<bool, 5> fEnabled = {false};
	//FILTER DATA
	//uses index defined by comment on LINE 10 to select match of type if true
	std::array<bool, 4> fTypes_selected = {false};
	//index 0 -> low range, index 1 -> high range, for single value set i0=i1
	std::array<double, 2> fAmount_range = { 0 };
	std::array<QDate, 2> fDate_range;
	//single elements
	std::vector<double> fCat_multiple;
	std::string fDesc_match;
	
	//FILTER MANIPULATORS
	//behavioral manipulation
	inline void enableFilter(filterType ft);
	inline void disableFilter(filterType ft);

	//filter data manipulation
	inline void selectType(bool expense, bool income, bool budget, bool transaction);
	inline void selectAmount(const double& low, const double& high);
	inline void selectDate(const QDate& low, const QDate& high);
	inline void selectCategory(const std::vector<double>& categories);
	inline void selectDescription(const std::string& descr);
};

inline void filter::enableFilter(filterType ft) {
	fEnabled[ft] = true;
}

inline void filter::disableFilter(filterType ft) {
	fEnabled[ft] = false;
}

inline void filter::selectType(bool expense, bool income, bool budget, bool transaction) {
	fTypes_selected[0] = expense; fTypes_selected[1] = income; fTypes_selected[2] = budget; fTypes_selected[3] = transaction;
}

inline void filter::selectAmount(const double& low, const double& high) {
	fAmount_range[0] = low; fAmount_range[1] = high;
}

inline void filter::selectDate(const QDate& low, const QDate& high) {
	fDate_range[0] = low; fDate_range[1] = high;
}

inline void filter::selectCategory(const std::vector<double>& categories) {
	fCat_multiple = categories;
}

inline void filter::selectDescription(const std::string& descr) {
	fDesc_match = descr;
}
