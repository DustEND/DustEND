#ifndef PREDICATES_H
#define PREDICATES_H

#include <math.h>
#include "computer.h"

class ComputersEqualByIp{
    QString ipAddress;
public:
    ComputersEqualByIp (QString ipAddress){
        this->ipAddress = ipAddress;
    }
    bool operator ()(Computer& cmp){
        if (cmp.getIpAddress() == ipAddress){
            return true;
        }
        else{
            return false;
        }
    }
};

double roundDouble(double doValue, int nPrecision)
{
    static const double doBase = 10.0;
    double doComplete5, doComplete5i;

    doComplete5 = doValue * pow(doBase, (double) (nPrecision + 1));

    if (doValue < 0.0)
        doComplete5 -= 5.0;
    else
        doComplete5 += 5.0;

    doComplete5 /= doBase;
    modf(doComplete5, &doComplete5i);

    return doComplete5i / pow(doBase, (double) nPrecision);
}

#endif // PREDICATES_H
