#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

class Parser {
private:
    istringstream input;
    char currentChar;

    void consume() {
        input.get(currentChar);
    }

    void expect(char expected) {
        if (currentChar == expected) {
            consume();
        } else {
            throw runtime_error("Syntax Error: Expected " + string(1, expected));
        }
    }

    double number() {
        double value;
        input.putback(currentChar);
        input >> value;
        consume();
        return value;
    }

    double factor() {
        if (isdigit(currentChar)) {
            return number();
        } else if (currentChar == '(') {
            consume();
            double result = expression();
            expect(')');
            return result;
        } else {
            throw runtime_error("Syntax Error: Unexpected character");
        }
    }

    double term() {
        double result = factor();
        while (currentChar == '*' || currentChar == '/') {
            char op = currentChar;
            consume();
            if (op == '*') {
                result *= factor();
            } else {
                double divisor = factor();
                if (divisor == 0) throw runtime_error("Math Error: Division by zero");
                result /= divisor;
            }
        }
        return result;
    }

    double expression() {
        double result = term();
        while (currentChar == '+' || currentChar == '-') {
            char op = currentChar;
            consume();
            if (op == '+') {
                result += term();
            } else {
                result -= term();
            }
        }
        return result;
    }

public:
    double parse(const string& inputStr) {
        input = istringstream(inputStr);
        consume();
        double result = expression();
        if (!input.eof()) {
            throw runtime_error("Syntax Error: Unexpected input");
        }
        return result;
    }
};

int main() {
    Parser parser;
    string input;
    
    cout << "Enter an arithmetic expression (or type 'exit' to quit):\n";
    while (true) {
        cout << ">> ";
        getline(cin, input);
        if (input == "exit") break;
        try {
            double result = parser.parse(input);
            cout << "Result: " << result << "\n";
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}