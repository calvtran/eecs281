// Assignment Identifier: 1CAEF3A0FEDD0DEC26BA9808C69D4D22A9962768 
#include <iostream>
#include <stack>
#include <iterator>
using namespace std;

bool Paired(char open, char close)
{
    if (open == '(' && close == ')') 
        return true;
    else if (open == '[' && close == ']') 
        return true;
    else if (open == '{' && close == '}') 
        return true;
    else
        return false;
}

int main() {
    istream_iterator<char> read_in(cin);
    istream_iterator<char> end;
    bool balanced = true;

    stack<char> brackets;
    while (read_in != end && balanced) {
        // Only read in brackets
        if (*read_in == '(' || *read_in == '[' || *read_in == '{')
            brackets.push(*read_in);    // Add open brackets to stack
        else if (*read_in == ')' || *read_in == ']' || *read_in == '}') 
        {
            if (brackets.empty() || !Paired(brackets.top(), *read_in))
                // false if closing bracket is detected but 
                // was not paired with correct corresponding open bracket
                // or if stack is empty
                balanced = false;
            else
                brackets.pop();     // pop stack if brackets were paired
        }
        ++read_in;
    }

    if (brackets.empty() && balanced)
        balanced = true;
    else
        balanced = false;

    if (balanced)
        cout << "Balanced\n";
    else
        cout << "Not balanced\n";

    return 0;
}