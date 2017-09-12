//
//  RationalNumber.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/22.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef RationalNumber_hpp
#define RationalNumber_hpp

#include <stdio.h>
#include "shared_ptr.h"

class RationalNumber
{
    public:
    long Numerator;
    long Denominator;
    static shared_ptr<RationalNumber> create(long numerator, long denominator) {
        if (denominator == 0) {
            return 0;
        }
        return new RationalNumber(numerator, denominator);
    }
    private:
    long GCD(long a, long b) {
        if (a < 0) {
            a = -a;
        }
        if (b < 0) {
            b = -b;
        }
        while (a != 0 && b != 0) {
            if (a > b) {
                a = a % b;
            } else {
                b = b % a;
            }
        }
        return a + b;
    }
    RationalNumber(long numerator, long denominator) {
        long gcd = GCD(numerator, denominator);
        if (gcd > 1) {
            numerator /= gcd;
            denominator /= gcd;
        }
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
        Numerator = numerator;
        Denominator = denominator;
    }
    bool equals(shared_ptr<RationalNumber> otherNumber) {
        
        return otherNumber->Numerator == Numerator && otherNumber->Denominator == Denominator;
    }
};
#endif /* RationalNumber_hpp */
