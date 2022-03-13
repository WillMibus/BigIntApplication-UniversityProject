//
// Created by Will Mibus on 11/10/21.
//

#include "Bigint.h"
#include <iostream>

Bigint::Bigint() {}

Bigint Bigint::clearContainer(Bigint &n) {
    std::fill_n(n.container, 255, 0);
    return n;
}

Bigint Bigint::numberToContainer(int number) {
    Bigint resultInt;
    std::string numberString = std::to_string(number);
    int numberStringIndex = numberString.length() - 1;

    // store number
    for (int i = 255; i >= 256 - numberString.length(); i--) {
        resultInt.container[i] = numberString.at(numberStringIndex) - 48;
        numberStringIndex--;
    }

    return resultInt;
}

// returns index of left most value in array
// e.g. if container holds {1,2,3}, the function would return 253, the position of {1}
int Bigint::containerLength(const Bigint &n) {
    int index;
    for (index = 0; index < 256; index++) {
        if (n.container[index] != 0) {
            break;
        }
    }
    return index;
}

Bigint Bigint::insertZeros(const Bigint &n, int number) {
    Bigint resultInt = n;

    // move up until first digit of number
    for (int i = 0; i <= 255 - number; i++) {
        resultInt.container[i] = resultInt.container[i + number];
    }

    // fill the remaining with zeros
    for (int i = 255; i > 255 - number; i--) {
        resultInt.container[i] = 0;
    }
    return resultInt;
}

int Bigint::firstCell(const Bigint &n) {
    return n.container[255];
}

Bigint operator+(const Bigint &n1, const Bigint &n2) {
    Bigint resultInt;
    int result;
    int carry = 0;
    std::string checkCarry;

    for (int i = 255; i >= 0; i--) {
        // check for carry from previous iteration, if set, add 1
        if (carry == 1) {
            result = n1.container[i] + n2.container[i] + carry;
            carry = 0;
        } else {
            result = n1.container[i] + n2.container[i];
        }

        // check if carry, if there is, set it for the next iteration
        checkCarry = std::to_string(result);
        if (result == 10) {                     // special case if result is 10
            carry = 1;
            resultInt.container[i] = 0;
        } else if (checkCarry.length() > 1) {  // if result is > 10
            carry = 1;
            result = result - 10;
            resultInt.container[i] = result;
        } else {                                // if result < 10
            resultInt.container[i] = result;
        }
    }
    return resultInt;
}

Bigint operator-(const Bigint &n1, const Bigint &n2) {
    Bigint resultInt;
    int num1, num2, result, borrow = 0;

    for (int i = 255; i >= 0; i--) {
        num1 = (borrow == 1) ? n1.container[i] - 1 : n1.container[i];
        num2 = n2.container[i];

        borrow = 0;

        if (num1 - num2 < 0) {
            borrow = 1;
            result = (num1 + 10) - num2;
        } else {
            result = num1 - num2;
        }

        resultInt.container[i] = result;
    }

    return resultInt;
}

Bigint operator*(const Bigint &n1, const Bigint &n2) {
    Bigint resultInt;
    int n2Length = Bigint::containerLength(n2);     // for {1, 2, 3} returns 253

    for (int i = 255; i >= n2Length; i--) {
        Bigint temp;

        // add n1 the amount of times specified at n[i]
        for (int j = 0; j < n2.container[i]; j++) {
            temp = temp + n1;
        }

        // check if trailing 0s need to be added
        int index = 255 - i;
        if (index > 0) {
            temp = Bigint::insertZeros(temp, index);
        }

        resultInt = resultInt + temp;
    }

    return resultInt;
}

Bigint operator/(const Bigint &n1, const Bigint &n2) {
    /*
     * BASIC STRUCTURE
     * 1 - instantiate variables
     * LOOP:
     *  2 - compare current value of n1 to n2
     *  3A - if n1 == n2 -> FINISHED
     *  3B - if n1 == 0 OR n1 < n2 -> FINISHED
     *  3C - if n1 > n2 -> DIVIDE
     * 4 - return result
     *
     */

    // 1 - instantiate variables
    Bigint n1Variable = n1;
    Bigint n2Variable = n2;
    Bigint result;
    bool finished = false;
    int n2Length = Bigint::containerLength(n2Variable);

    while (!finished) {
        // 2 - compare current value of n1 to n2
        bool isSame = true;
        bool isZero = true;
        bool isBigger = false;
        bool isSmaller = false;
        for (int i = 0; i < 256; i++) {
            if (n1Variable.container[i] != n2.container[i]) {
                isSame = false;
            }
            if (n1Variable.container[i] != 0) {
                isZero = false;
            }
            if (n1Variable.container[i] > n2.container[i] && !isBigger) {
                isBigger = true;
                isSmaller = false;
            }
            if (n1Variable.container[i] < n2.container[i] && !isBigger) {
                isSmaller = true;
                break;
            }
        }

        // 3A - if n1 == n2
        if (isSame) {
            Bigint one = Bigint::numberToContainer(1);
            result = result + one;
            finished = true;
        }
            // 3B - if n1 == 0 OR n1 < n2
        else if (isZero || isSmaller) {
            finished = true;
        }
            // 3C - if n1 > n2 -> DIVIDE
        else {
            // find length of current n1
            int n1Length = Bigint::containerLength(n1Variable);     // eg 251

            // find smallest n1 substring that n2 divides into
            int n1Index = n1Length + (255 - n2Length);         // index that the n1 substring goes to, eg 253
            if (n1Variable.container[n1Length] <= n2Variable.container[n2Length]) {
                n1Index++;
            }

            // calculate substring and store
            Bigint n1Substring;
            int substringSize = n1Index - n1Length;
            for (int i = n1Length; i <= n1Index; i++) {
                n1Substring.container[255 - substringSize] = n1Variable.container[i];
                substringSize--;
            }

            // divide substring by n2 and store result
            Bigint tempResult;
            int count;
            for (count = 0; n1Substring.container[0] != 9; count++) {
                n1Substring = n1Substring - n2;
            }
            count--;

            // add trailing zeros to result
            tempResult = Bigint::numberToContainer(count);
            int trailingZeros = 255 - n1Index;
            if (trailingZeros > 0) {
                tempResult = Bigint::insertZeros(tempResult, trailingZeros);
            }

            // store result and update variables for next iteration
            result = result + tempResult;       // add result from this iteration to running total
            tempResult = tempResult * n2;       // calculate the amount to take from the current n1 value
            n1Variable = n1Variable - tempResult;   // take the amount from current n1 value
        }
    }
    return result;
}

std::istream &operator>>(std::istream &in, Bigint &n) {
    in >> n.number;
    int numberIndex = n.number.length() - 1;
    for (int i = 255; i >= 256 - n.number.length(); i--) {
        n.container[i] = n.number.at(numberIndex) - 48;
        numberIndex--;
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const Bigint &n) {
    bool isNumStarted = false;
    for (int i = 0; i < 256; i++) {
        if (n.container[i] != 0) {
            isNumStarted = true;
        }

        if (isNumStarted) {
            std::cout << n.container[i];
        }
    }

    // special condition where the result is 0
    if (!isNumStarted) {
        std::cout << n.container[0];
    }

    return out;
}


















