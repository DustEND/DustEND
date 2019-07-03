#include "languagevariables.h"

LanguageVariables::LanguageVariables()
{
}

QString LanguageVariables::getVarName() const
{
    return varName;
}

QString LanguageVariables::getEn() const
{
    return en;
}

QString LanguageVariables::getRu() const
{
    return ru;
}

void LanguageVariables::setVarName(QString value)
{
    varName = value;
}

void LanguageVariables::setEn(QString value)
{
    en = value;
}

void LanguageVariables::setRu(QString value)
{
    ru = value;
}

LanguageVariables &LanguageVariables::operator =(const LanguageVariables &orig)
{
    this->varName = orig.varName;
    this->en = orig.en;
    this->ru = orig.ru;
    return *this;
}

bool LanguageVariables::operator !=(const LanguageVariables &orig)
{
    if (this->varName != orig.varName)
        if (this->en != orig.en)
            if (this->ru != orig.ru)
                return true;
            else
                return false;
        else
            return false;
    else
        return false;
}
