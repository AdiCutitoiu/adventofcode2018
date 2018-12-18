#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <regex>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>
#include <list>
#include <numeric>
#include <functional>
#include <iterator>
#include <cstdlib>
#include <type_traits>
#include <ctype.h>

#define I(pos) stoi(tokens[pos])
#define S(pos) tokens[pos]

#define G(t, p) get<p>(t)

using namespace std;

vector<string> Read()
{
  vector<string> result;

  string line;
  while (getline(cin, line))
  {
    result.push_back(line);
  }

  return result;
}

vector<string> Tokenize(const string & aText, const string & aSeparators, bool aDiscardEmptyStrings)
{
  vector<string> result;

  string current;
  for (char ch : aText)
  {
    if (aSeparators.find(ch) != string::npos)
    {
      result.push_back(current);
      current = "";
    }
    else
    {
      current += ch;
    }
  }

  result.push_back(current);

  if (aDiscardEmptyStrings)
    result.erase(remove(result.begin(), result.end(), string()), result.end());

  return result;
}

template<typename T>
void Write(const vector<T> & aVec)
{
}

int main()
{
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  string str;
  cin >> str;

  while (true)
  {
    string newStr;

    if (str.size() < 1)
      break;

    for (int i = 0; i < str.size() - 1; i++)
    {
      if (tolower(str[i + 1]) == tolower(str[i]) && str[i + 1] != str[i])
      {
        newStr = str.substr(0, i);

        if(i + 2 < str.size())
          newStr += str.substr(i + 2);

        break;
      }
    }

    if (newStr.empty() || newStr == str)
      break;

    str = newStr;
  }

  cout << str.size();

  return 0;
}
