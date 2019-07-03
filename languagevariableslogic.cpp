#include "languagevariableslogic.h"
#include "languagevariablesgateway.h"

LanguageVariablesLogic::LanguageVariablesLogic()
{
    langVarG = new LanguageVariablesGateway();
    loadLangVarList();
}

QString LanguageVariablesLogic::getVariable(QString *locale, QString varName)
{
    bool finded = false;
    QVector<LanguageVariables>::iterator langI;
    for (langI = langVars.begin(); langI != langVars.end(); ++langI) {
        if (varName == langI->getVarName()) {
            finded = true;
            break;
        }
    }
    if (finded) {
        if (*locale == "en")
            return langI->getEn();
        else
            return langI->getRu();
    }
    else
        return "error";
}

void LanguageVariablesLogic::loadLangVarList()
{
    langVars = langVarG->findAll();
    if (langVars.size() == 0) {
        LanguageVariables newLangVar;
        newLangVar.setVarName("varName");
        newLangVar.setEn("en");
        newLangVar.setRu("ru");
        langVars.push_back(newLangVar);
    }
}
