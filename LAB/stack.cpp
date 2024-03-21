#include <iostream>
#include <vector>
#include <string>

using namespace std;

void printStack(const vector<char>& stack) {
    cout << "Stack: ";
    for (char c : stack) {
        cout << c << " ";
    }
    cout << endl;
}

bool isMatchingPair(char opening, char closing) {
    return (opening == '(' && closing == ')') ||
           (opening == '[' && closing == ']') ||
           (opening == '{' && closing == '}') ||
           (opening == '<' && closing == '>');
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " \"string\"" << endl;
        return 1;
    }

    string input(argv[1]);
    vector<char> stack;
    cout << "Stack: " << endl;

    for (char& c : input) {
        cout << "Character: " << c << endl;
        if (c == '(' || c == '[' || c == '{' || c == '<') {
            stack.push_back(c);
            cout << "Push" << endl;
            printStack(stack);
        } else if (c == ')' || c == ']' || c == '}' || c == '>') {
            if (stack.empty()) {
                cout << "Error: unmatched right symbol " << c << endl;
                return 1;
            } else if (!isMatchingPair(stack.back(), c)) {
                cout << "Error: mismatched pair " << stack.back() << " and " << c << endl;
                return 1;
            }
            cout << "Matching " << stack.back() << " and " << c << endl;
            cout << "Pop" << endl;
            stack.pop_back();
            printStack(stack);
        } else {
            cout << "Error: invalid character " << c << endl;
            return 1;
        }
    }

    if (!stack.empty()) {
        cout << "Reached end of string" << endl;
        cout << "Error: unmatched left symbol " << stack.back() << endl;
        return 1;
    }

    cout << "Reached end of string" << endl;
    cout << "String is properly balanced" << endl;
    return 0;
}