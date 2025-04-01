#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <algorithm>

using namespace std;

// Structure to represent a quadruple
struct Quadruple
{
  string op;
  string arg1;
  string arg2;
  string result;
};

// Function to get the precedence of an operator
int getPrecedence(const string &op)
{
  if (op == "*" || op == "/")
    return 2;
  if (op == "+" || op == "-")
    return 1;
  return 0;
}

// Function to convert infix expression to postfix using Shunting Yard Algorithm
vector<string> infixToPostfix(const string &expression)
{
  vector<string> postfix;
  stack<string> operators;
  stringstream ss(expression);
  string token;

  while (ss >> token)
  {
    if (isdigit(token[0]))
    {
      postfix.push_back(token);
    }
    else if (token == "(")
    {
      operators.push(token);
    }
    else if (token == ")")
    {
      while (!operators.empty() && operators.top() != "(")
      {
        postfix.push_back(operators.top());
        operators.pop();
      }
      operators.pop(); // Pop the "("
    }
    else if (token == "+" || token == "-" || token == "*" || token == "/")
    {
      while (!operators.empty() && getPrecedence(token) <= getPrecedence(operators.top()))
      {
        postfix.push_back(operators.top());
        operators.pop();
      }
      operators.push(token);
    }
  }

  while (!operators.empty())
  {
    postfix.push_back(operators.top());
    operators.pop();
  }

  return postfix;
}

// Function to generate quadruples from a postfix expression
vector<Quadruple> generateQuadruples(const vector<string> &postfix)
{
  vector<Quadruple> quadruples;
  stack<string> operands;
  int tempVarCount = 1;

  for (const string &token : postfix)
  {
    if (isdigit(token[0]))
    {
      operands.push(token);
    }
    else if (token == "+" || token == "-" || token == "*" || token == "/")
    {
      string operand2 = operands.top();
      operands.pop();
      string operand1 = operands.top();
      operands.pop();

      Quadruple quad;
      quad.op = token;
      quad.arg1 = operand1;
      quad.arg2 = operand2;
      quad.result = "t" + to_string(tempVarCount++);
      quadruples.push_back(quad);

      operands.push(quad.result);
    }
  }

  return quadruples;
}

int main()
{
  string expression;
  cout << "Enter an arithmetic expression: ";
  getline(cin, expression);

  // Convert infix to postfix
  vector<string> postfix = infixToPostfix(expression);

  // Generate quadruples from postfix
  vector<Quadruple> quadruples = generateQuadruples(postfix);

  // Print the quadruple table
  cout << "Operator\tOperand 1\tOperand 2\tResult\n";
  for (const auto &quad : quadruples)
  {
    cout << quad.op << "\t\t" << quad.arg1 << "\t\t" << quad.arg2 << "\t\t" << quad.result << endl;
  }

  return 0;
}