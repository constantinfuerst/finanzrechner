#include "stdafx.h"
#include "month.h"

//generates a unique filename for a months data file from the supplied date
//uses "savedir" macro to obtain root storage directory
std::string month::getFileName(const QDate& i_month) {
	return savedir + std::string("\\data\\") + i_month.toString("MM-yyyy").toStdString();
}

//serializes a months data into JSON using the supplied fileHandler
//this handler will then be delegated the job of transforming a json document into the file
bool month::writeJSON() {
	//if the fileHandler does not exist, return
	if (fh == nullptr) return false;
	//create an array to store the serialized transactions
	QJsonArray transactions;
	for (transaction* t : m_transactions) {
		QJsonObject tobj = *t->toJSON();
		transactions.append(tobj);
		delete t;
	} m_transactions.clear();

	//initialize the json data and append transaction data
	QJsonObject month;
	month["date"] = m_month.toString("MM-yyyy");
	month["transactions"] = transactions;

	//now create the document from the generated data and delegate the writing job
	auto* jdoc = new QJsonDocument(month);
	fh->writeJSON(jdoc, getFileName(m_month));
	delete jdoc;

	return true;
}

bool month::readJSON() {
	//if the fileHandler does not exist, return
	if (fh == nullptr) return false;

	//use the supplied fileHandler to try and read the json document of this month
	auto* jdoc = fh->readJSON(getFileName(m_month));
	//return if some error occurs
	if (jdoc == nullptr) return false;
	auto json = jdoc->object();
	delete jdoc;

	//check if the obtained json data contains valid json and the required data
	//then read the data into this month
	if (json.contains("transactions") && json["transactions"].isArray()) {
		QJsonArray transactionArray = json["transactions"].toArray();
		for (auto e : transactionArray)
			if (e.isObject())
				m_transactions.push_back(new transaction(e.toObject()));
	}
	else return false;

	return true;
}