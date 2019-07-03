#ifndef CRITERIA_H
#define	CRITERIA_H

#include <QString>

class Criteria {
public:
    Criteria();
    Criteria(const Criteria& orig);
    virtual ~Criteria();    

    void addCriteria(QString* field,
                     QString* value,
                     QString* sign);
    void addRelation(QString* relation);
    void pushBack(QString* value);
    void addDateBetween(QString* field,
                        QString* beginDate,
                        QString* endDate);
    QString getCriteria();
    void setCriteria(QString newCriteria);

private:
    QString criteriaStr;
    QString* field;
    QString* value;
    QString* sign;

};

#endif	/* CRITERIA_H */
