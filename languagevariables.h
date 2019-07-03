#ifndef LANGUAGEVARIABLES_H
#define LANGUAGEVARIABLES_H

#include <QString>

class LanguageVariables
{
public:
    LanguageVariables();

    QString getVarName() const;
    QString getEn() const;
    QString getRu() const;

    void setVarName(QString value);
    void setEn(QString value);
    void setRu(QString value);

    LanguageVariables& operator=(const LanguageVariables &orig);
    bool operator!=(const LanguageVariables &orig);

private:
    QString varName;
    QString en;
    QString ru;
};

#endif // LANGUAGEVARIABLES_H
