#include "cooler.h"

Cooler::Cooler()
{
    this->diameter = 120;
    this->amount = 0;
}

Cooler::~Cooler()
{
}

Cooler &Cooler::operator =(const Cooler &orig)
{
    this->diameter = orig.diameter;
    this->amount = orig.amount;
    return *this;
}

bool Cooler::operator !=(const Cooler &orig)
{
    if (this->diameter != orig.diameter)
        if (this->amount != orig.amount)
            return true;
        else
            return false;
    else
        return false;
}

unsigned short Cooler::getDiameter() const
{
    return this->diameter;
}

unsigned short Cooler::getAmount() const
{
    return this->amount;
}

void Cooler::setDiameter(unsigned short *newDiameter)
{
    this->diameter = *newDiameter;
}

void Cooler::setAmount(unsigned short *newAmount)
{
    this->amount = *newAmount;
}
