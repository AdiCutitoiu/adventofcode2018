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

int main()
{
  freopen("input.txt", "r", stdin);

  string org;
  cin >> org;

  auto m = INT_MAX;

  for(int it = 'a'; it <= 'z'; it++)
  {
    string str;
    for (auto ch : org)
    {
      if (tolower(ch) == it)
        continue;
      str += ch;
    }

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

          if (i + 2 < str.size())
            newStr += str.substr(i + 2);

          break;
        }
      }

      if (newStr.empty() || newStr == str)
        break;

      str = newStr;
    }

    m = str.size() < m ? str.size() : m;
  }

  cout << m;

  return 0;
}
