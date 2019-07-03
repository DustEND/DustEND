#include "Criteria.h"

Criteria::Criteria() {
}

Criteria::Criteria(const Criteria& orig) {
    this->criteriaStr = orig.criteriaStr;
    this->field = orig.field;
}

Criteria::~Criteria() {
}

void Criteria::addCriteria( QString* field,
                            QString* value,
                            QString* sign ){
	if (criteriaStr == "")
		criteriaStr = *field;
	else
		criteriaStr += *field;
	criteriaStr +=	" " 
					+ *sign 
					+ " '" 
					+ *value 
					+ "' ";
}

void Criteria::addRelation( QString* relation ){
	criteriaStr += *relation += " ";
}

void Criteria::addDateBetween( QString* field,
                               QString* beginDate,
                               QString* endDate ){
	if (criteriaStr == "")
		criteriaStr = *field;
	else
		criteriaStr += *field;
	criteriaStr += " between '" 
					+ *beginDate 
					+ "' and '" 
					+ *endDate 
					+ "' ";
}

QString Criteria::getCriteria(){
	return criteriaStr;
}

void Criteria::setCriteria( QString newCriteria ){
	this->criteriaStr = newCriteria;
}

void Criteria::pushBack( QString* value){
	criteriaStr += *value += " ";
}







