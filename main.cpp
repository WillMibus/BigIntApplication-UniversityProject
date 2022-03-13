#include <iostream>
#include <sstream>
#include "Bigint.h"

using namespace std;

class Stack {
    Bigint stack[4];

public :
    void push(Bigint a) {
        for (int i = 3; i > 0; i--) {
            stack[i] = stack[i - 1];
        }
        stack[0] = a;
    }

public :
    Bigint pop() {
        Bigint popped = stack[0];
        for (int i = 0; i < 3; i++) {
            stack[i] = stack[i + 1];
        }
        return popped;
    }

};

int main() {
    Bigint n1, n2, n3;
    Stack stack;
    Bigint token;

    string line;
    while (getline(cin, line)) {
        stringstream expression(line);
        while (expression >> token) {
            int firstCell = Bigint::firstCell(token);

            // if token is a number, store it
            if (firstCell >= 0) {
                stack.push(token);
            }
            // if operator is +
            else if (firstCell == -5) {
                n1 = stack.pop();
                n2 = stack.pop();
                n3 = n2 + n1;
                stack.push(n3);
            }
            // if operator is -
            else if (firstCell == -3) {
                n1 = stack.pop();
                n2 = stack.pop();
                n3 = n2 - n1;
                stack.push(n3);
            }
            // if operator is *
            else if (firstCell == -6) {
                n1 = stack.pop();
                n2 = stack.pop();
                n3 = n2 * n1;
                stack.push(n3);
            }
            // if operator is /
            else if (firstCell == -1) {
                n1 = stack.pop();
                n2 = stack.pop();
                n3 = n2 / n1;
                stack.push(n3);
            }
            Bigint::clearContainer(token);
        }
        cout << stack.pop() << endl;
    }

    return 0;
}

