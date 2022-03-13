//
// Created by Will Mibus on 11/10/21.
//

#ifndef BIGINT_BIGINT_H
#define BIGINT_BIGINT_H

#include <iostream>

class Bigint {
public:
    Bigint();

    static Bigint clearContainer(Bigint &n);
    static Bigint numberToContainer(int number);
    static int containerLength(const Bigint& n);
    static Bigint insertZeros(const Bigint &n, int number);
    static int firstCell(const Bigint &n);

    friend Bigint operator+ (const Bigint& n1, const Bigint& n2);
    friend Bigint operator- (const Bigint& n1, const Bigint& n2);
    friend Bigint operator* (const Bigint& n1, const Bigint& n2);
    friend Bigint operator/ (const Bigint& n1, const Bigint& n2);

    friend std::ostream& operator<< (std::ostream& out, const Bigint& n);
    friend std::istream& operator>> (std::istream& in, Bigint& n);

private:
    std::string number;
    int container[256] = { 0 };
};


#endif //BIGINT_BIGINT_H
