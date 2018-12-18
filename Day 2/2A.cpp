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

size_t CountLetters(const string & word, char ch)
{
  return count(word.begin(), word.end(), ch);
}

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
  
  auto words = Read();

  size_t twoTimes = 0;
  size_t threeTimes = 0;
  for (auto && word : words)
  {
    for (char ch = 'a'; ch <= 'z'; ch++)
    {
      if (CountLetters(word, ch) == 2)
      {
        twoTimes++;
        break;
      }
    }

    for (char ch = 'a'; ch <= 'z'; ch++)
    {
      if (CountLetters(word, ch) == 3)
      {
        threeTimes++;
        break;
      }
    }
  }

  cout << twoTimes * threeTimes;

  return 0;
}
