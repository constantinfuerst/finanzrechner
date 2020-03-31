#include "stdafx.h"
#include "month.h"

//generates a unique filename for a months data file from the supplied date
//uses "SAVEDIR" macro to obtain root storage directory
std::string month::getFileName(const QDate& i_month) {
	return std::string(SAVEDIR) + "\\data\\" + i_month.toString("MM-yyyy").toStdString();
}

//serializes a months data into JSON using the supplied fileHandler
//this handler will then be delegated the job of transforming a json document into the file
bool month::writeJSON() {
	//if the fileHandler does not exist, return
	if (m_fh == nullptr) return false;
	//create an array to store the serialized transactions
	QJsonArray transactions;
	for (auto* t : p_transactions) {
		auto tobj = *t->toJSON();
		transactions.append(tobj);
		delete t;
	} p_transactions.clear();

	//initialize the json data and append transaction data
	QJsonObject month;
	month["date"] = p_month.toString("MM-yyyy");
	month["transactions"] = transactions;

	//now create the document from the generated data and delegate the writing job
	auto* jdoc = new QJsonDocument(month);
	m_fh->writeJSON(jdoc, getFileName(p_month));
	delete jdoc;

	return true;
}

bool month::readJSON() {
	//if the fileHandler does not exist, return
	if (m_fh == nullptr) return false;

	//use the supplied fileHandler to try and read the json document of this month
	auto* jdoc = m_fh->readJSON(getFileName(p_month));
	//return if some error occurs
	if (jdoc == nullptr) return false;
	auto json = jdoc->object();
	delete jdoc;

	//check if the obtained json data contains valid json and the required data
	//then read the data into this month
	if (json.contains("transactions") && json["transactions"].isArray()) {
		auto transaction_array = json["transactions"].toArray();
		for (auto e : transaction_array)
			if (e.isObject())
				p_transactions.push_back(new transaction(e.toObject()));
	}
	else return false;

	return true;
}