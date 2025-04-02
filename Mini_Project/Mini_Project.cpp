#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
using namespace std;

// Valid C data types
vector<string> validTypes = {"int", "float", "double", "char", "long", "short", "unsigned", "signed"};

// Function to check if a string is a valid variable name in C
bool isValidVariable(const string &var)
{
  regex varPattern("^[a-zA-Z_][a-zA-Z0-9_]*$");
  return regex_match(var, varPattern);
}

// Function to check if the given declaration is valid
bool checkDeclaration(const string &code)
{
  stringstream ss(code);
  string type, var;
  vector<string> variables;
  char lastChar = code.back();

  // Check if the statement ends with a semicolon
  if (lastChar != ';')
  {
    cout << "Error: Missing semicolon at the end of the statement." << endl;
    return false;
  }

  ss >> type;

  // Handle multi-word types (e.g., "unsigned int", "long int")
  string nextWord;
  while (ss >> nextWord)
  {
    if (find(validTypes.begin(), validTypes.end(), nextWord) != validTypes.end())
    {
      type += " " + nextWord;
    }
    else
    {
      ss.putback(' ');
      for (int i = nextWord.size() - 1; i >= 0; --i)
      {
        ss.putback(nextWord[i]);
      }
      break;
    }
  }

  // Check if type is valid
  if (find(validTypes.begin(), validTypes.end(), type) == validTypes.end() &&
      type != "unsigned int" && type != "long int")
  {
    cout << "Error: Invalid data type." << endl;
    return false;
  }

  // Extract variables
  while (getline(ss, var, ','))
  {
    var.erase(remove(var.begin(), var.end(), ';'), var.end()); // Remove semicolon if present
    var.erase(0, var.find_first_not_of(" "));                  // Trim leading spaces
    var.erase(var.find_last_not_of(" ") + 1);                  // Trim trailing spaces

    size_t equalPos = var.find('=');
    string varName = var;
    if (equalPos != string::npos)
    {
      varName = var.substr(0, equalPos);
      string value = var.substr(equalPos + 1);
      varName.erase(varName.find_last_not_of(" ") + 1); // Trim spaces from variable name
      value.erase(0, value.find_first_not_of(" "));     // Trim spaces from value

      // Check if the assigned value is valid (including hexadecimal, scientific notation, and character constants)
      if (!regex_match(value, regex("^[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)?$")) &&
          !regex_match(value, regex("^0[xX][0-9A-Fa-f]+$")) &&
          !regex_match(value, regex("^'.'$")))
      {
        cout << "Error: Invalid assignment value '" << value << "'." << endl;
        return false;
      }
    }

    // Check for arrays
    size_t bracketPos = varName.find('[');
    if (bracketPos != string::npos)
    {
      string arrayName = varName.substr(0, bracketPos);
      if (!isValidVariable(arrayName))
      {
        cout << "Error: Invalid array name '" << arrayName << "'." << endl;
        return false;
      }
    }
    else if (!isValidVariable(varName))
    {
      cout << "Error: Invalid variable name '" << varName << "'." << endl;
      return false;
    }

    if (find(variables.begin(), variables.end(), varName) != variables.end())
    {
      cout << "Error: Duplicate variable name '" << varName << "'." << endl;
      return false;
    }

    variables.push_back(varName);
  }

  cout << "Valid variable declaration." << endl;
  return true;
}

int main()
{
  string code;
  cout << "Enter a C variable declaration: ";
  getline(cin, code);
  checkDeclaration(code);
  return 0;
}