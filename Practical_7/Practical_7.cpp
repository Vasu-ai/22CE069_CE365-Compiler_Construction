#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> firstSets;
map<char, set<char>> followSets;

// Function to compute First Set
void computeFirst(char nonTerminal)
{
  if (!firstSets[nonTerminal].empty())
    return;

  for (const string &production : grammar[nonTerminal])
  {
    bool epsilonPresent = true;

    for (char c : production)
    {
      if (isupper(c))
      {
        computeFirst(c);
        for (char first : firstSets[c])
        {
          if (first != 'ε')
            firstSets[nonTerminal].insert(first);
          else
            epsilonPresent = true;
        }
      }
      else
      {
        firstSets[nonTerminal].insert(c);
        epsilonPresent = false;
        break;
      }

      if (!epsilonPresent)
        break;
    }

    if (epsilonPresent)
      firstSets[nonTerminal].insert('ε');
  }
}

// Function to compute Follow Set
void computeFollow(char nonTerminal)
{
  if (!followSets[nonTerminal].empty())
    return;

  if (nonTerminal == 'S')
    followSets[nonTerminal].insert('$'); // Start symbol gets $

  for (const auto &pair : grammar) // Fix structured binding
  {
    char lhs = pair.first;
    const vector<string> &productions = pair.second;

    for (const string &production : productions)
    {
      for (size_t i = 0; i < production.length(); i++)
      {
        if (production[i] == nonTerminal)
        {
          bool epsilonFound = false;
          if (i + 1 < production.length())
          {
            char nextSymbol = production[i + 1];
            if (isupper(nextSymbol))
            {
              for (char first : firstSets[nextSymbol])
              {
                if (first != 'ε')
                  followSets[nonTerminal].insert(first);
                else
                  epsilonFound = true;
              }
            }
            else
            {
              followSets[nonTerminal].insert(nextSymbol);
            }
          }
          else
          {
            epsilonFound = true;
          }

          if (epsilonFound)
          {
            computeFollow(lhs);
            followSets[nonTerminal].insert(followSets[lhs].begin(), followSets[lhs].end());
          }
        }
      }
    }
  }
}

int main()
{
  int n;
  cout << "Enter number of productions: ";
  cin >> n;
  cin.ignore();

  cout << "Enter grammar (e.g., S=AB|a):\n";
  for (int i = 0; i < n; i++)
  {
    char lhs;
    string rhs;
    cin >> lhs;
    cin.ignore();
    getline(cin, rhs);

    size_t pos = 0;
    while ((pos = rhs.find('|')) != string::npos)
    {
      grammar[lhs].push_back(rhs.substr(0, pos));
      rhs.erase(0, pos + 1);
    }
    grammar[lhs].push_back(rhs);
  }

  for (const auto &pair : grammar) // Fix structured binding
    computeFirst(pair.first);
  for (const auto &pair : grammar) // Fix structured binding
    computeFollow(pair.first);

  // Output First Sets
  cout << "\nFirst Sets:\n";
  for (const auto &pair : firstSets)
  {
    cout << "First(" << pair.first << ") = { ";
    for (char first : pair.second)
      cout << first << " ";
    cout << "}\n";
  }

  // Output Follow Sets
  cout << "\nFollow Sets:\n";
  for (const auto &pair : followSets)
  {
    cout << "Follow(" << pair.first << ") = { ";
    for (char follow : pair.second)
      cout << follow << " ";
    cout << "}\n";
  }

  return 0;
}
