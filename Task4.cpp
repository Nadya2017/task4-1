#include <iostream>
#include <fstream>
#include <string>

using namespace std;

template<typename T>
struct Stack {
    T value;
    Stack* next;
};

template<typename T>
void push(Stack<T>*& stack, T value) {
    Stack<T>* new_node = new Stack<T>;
    new_node->value = value;
    new_node->next = stack;
    stack = new_node;
}

template<typename T>
T pop(Stack<T>*& stack) {
    T val = stack->value;
    Stack<T>* next = stack->next;
    delete stack;
    stack = next;
    return val;
}

int priority(string ch) {
    if (ch == "=") return 1;
    if (ch == "(") return 2;
    if (ch == "-" || ch == "+") return 3;
    if (ch == "*" || ch == "/") return 4;
    return -1;
}

string getString(char x) {
    string s(1, x);
    return s;
}

int main() {
    ifstream file("input.txt");
    string input;
    file >> input;

    cout << "Input: " << input << endl;

    Stack<string>* first = nullptr;
    Stack<string>* second = nullptr;

    bool read_num = false;
    string num_string;
    int i = 0;
    if (input[0] == '+' || input[0] == '-') {
        num_string += input[0];
        read_num = true;
        i++;
    }
    while (i < input.size()) {
        if (read_num) {
            if (input[i] == 'e') {
                num_string += input[i];
                num_string += input[i + 1];
                i += 2;
            }
            else if (isalnum(input[i]) || input[i] == '.') {
                num_string += input[i];
                i++;
            }
            else {
                push(first, num_string);
                num_string = "";
                read_num = false;
            }
        }
        else {
            if (input[i] == '(') {
                push(second, getString(input[i]));
                if (input[i + 1] == '+' || input[i + 1] == '-') {
                    read_num = true;
                    num_string += input[i + 1];
                    i++;
                }
                i++;
            }
            else if (input[i] == ')') {
                string to_copy = pop(second);
                while (to_copy != "(") {
                    push(first, to_copy);
                    to_copy = pop(second);
                }
                i++;
            }
            else {
                if (isalnum(input[i])) {
                    read_num = true;
                    num_string += input[i];
                }
                else if (second == nullptr) {
                    push(second, getString(input[i]));
                }
                else if (priority(second->value) < priority(getString(input[i]))) {
                    push(second, getString(input[i]));
                }
                else {
                    string to_copy = pop(second);
                    push(first, to_copy);
                    i--;
                }
                i++;
            }
        }
    }
    push(first, pop(second)); // Перенос '='

    Stack<string>* third = nullptr;
    string polish;
    while (first != nullptr) {
        string to_copy = pop(first);
        push(third, to_copy);
        polish = to_copy + " " + polish;
    }
    cout << "Reverse Polish notation: " << polish << endl;

    Stack<double>* fourth = nullptr;

    while (third != nullptr) {
        string new_val = pop(third);
        if (new_val == "+") {
            double right = pop(fourth);
            double left = pop(fourth);
            push(fourth, left + right);
        }
        else if (new_val == "-") {
            double right = pop(fourth);
            double left = pop(fourth);
            push(fourth, left - right);
        }
        else if (new_val == "*") {
            double right = pop(fourth);
            double left = pop(fourth);
            push(fourth, left * right);
        }
        else if (new_val == "/") {
            double right = pop(fourth);
            double left = pop(fourth);
            if (right == 0) {
                cout << "Division by zero";
                return 0;
            }
            push(fourth, left / right);
        }
        else if (new_val == "=") {
            break;
        }
        else {
            push(fourth, stod(new_val));
        }
    }
    double ans = pop(fourth);
    cout << "Answer: " << ans << endl;
    return 0;
}
