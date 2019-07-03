#ifndef COOLER_H
#define COOLER_H

class Cooler
{
public:
    Cooler();
    ~Cooler();
    Cooler& operator = (const Cooler& orig);
    bool operator != (const Cooler& orig);

    unsigned short getDiameter() const;
    unsigned short getAmount() const;
    void setDiameter(unsigned short *newDiameter);
    void setAmount(unsigned short *newAmount);

private:
    unsigned short diameter;
    unsigned short amount;
};

#endif // COOLER_H
