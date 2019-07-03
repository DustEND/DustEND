#ifndef LANGUAGEVARIABLESLOGIC_H
#define LANGUAGEVARIABLESLOGIC_H

#include <QVector>
#include <QString>

class LanguageVariables;
class LanguageVariablesGateway;

class LanguageVariablesLogic
{
public:
    LanguageVariablesLogic();

    QString getVariable(QString *locale, QString varName);

private:
    LanguageVariablesGateway* langVarG;
    QVector< LanguageVariables > langVars;

    void loadLangVarList();
};

#endif // LANGUAGEVARIABLESLOGIC_H
