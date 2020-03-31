#pragma once
#include "stdafx.h"

// TEMPLATE FOR MATCHING FUNCTION
//	bool class::operator==(const filter& f) const {
//		if (f.f_enabled[filter::type]) {
//
//		}
//		if (f.f_enabled[filter::amount]) {
//
//		}
//		if (f.f_enabled[filter::date]) {
//
//		}
//		if (f.f_enabled[filter::category]) {
//
//		}
//		if (f.f_enabled[filter::description]) {
//
//		}
//
//		return true;
//	}

struct filter {
public:
	//FILTER DEFINITIONS
	enum filter_type {
		type, amount, date, category, description
	};
	//element types: 0 = expense, 1 = income, 2 = budget, 3 = transaction, 4 = recurring

//PLEASE TREAT THESE MEMBERS AS READ ONLY
	//ENABLED FILTERS
	//uses index defined by filter_type enumerator to select match of type if true
	std::array<bool, 5> f_enabled = { false };
	//FILTER DATA
	//uses index defined by comment on LINE 10 to select match of type if true
	std::array<bool, 5> f_types_selected = { false };
	//index 0 -> low range, index 1 -> high range, for single value set i0=i1
	std::array<double, 2> f_amount_range = { 0 };
	std::array<QDate, 2> f_date_range;
	//single elements
	std::vector<double> f_cat_multiple;
	std::string f_desc_match;
	//AND USE FOLLOWING FUNCTIONS FOR SETTING THEM

		//FILTER MANIPULATORS
		//behavioral manipulation
	inline void enableFilter(filter_type ft);
	inline void disableFilter(filter_type ft);

	//filter data manipulation
	inline void selectType(bool expense, bool income, bool budget, bool transaction, bool recurring);
	inline void selectAmount(const double& low, const double& high);
	inline void selectDate(const QDate& low, const QDate& high);
	inline void selectCategory(const std::vector<double>& categories);
	inline void selectDescription(const std::string& descr);
};

inline void filter::enableFilter(const filter_type ft) {
	f_enabled[ft] = true;
}

inline void filter::disableFilter(const filter_type ft) {
	f_enabled[ft] = false;
}

inline void filter::selectType(const bool expense, const bool income, const bool budget, const bool transaction, const bool recurring) {
	f_types_selected[0] = expense; f_types_selected[1] = income; f_types_selected[2] = budget; f_types_selected[3] = transaction; f_types_selected[4] = recurring;
}

inline void filter::selectAmount(const double& low, const double& high) {
	f_amount_range[0] = low; f_amount_range[1] = high;
}

inline void filter::selectDate(const QDate& low, const QDate& high) {
	f_date_range[0] = low; f_date_range[1] = high;
}

inline void filter::selectCategory(const std::vector<double>& categories) {
	f_cat_multiple = categories;
}

inline void filter::selectDescription(const std::string& descr) {
	f_desc_match = descr;
}
