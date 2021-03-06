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

#define I(pos) stoi(tokens[pos])
#define S(pos) tokens[pos]

#define G(t, p) get<p>(t)

using namespace std;

list<int> recipes;

auto Next(list<int>::iterator aStart, int times)
{
  while(times--)
  {
    aStart = next(aStart);

    if (aStart == recipes.end())
      aStart = recipes.begin();
  }

  return aStart;
}

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  recipes.push_back(3);
  recipes.push_back(7);

  auto first = recipes.begin();
  auto second = Next(recipes.begin(), 1);

  int recipesToDo = 323081;

  for (int i = 0; i < recipesToDo + 10; i++)
  {
    auto recipe = *first + *second;
    
    if (recipe >= 10) {
      recipes.push_back(recipe / 10);
    }

    recipes.push_back(recipe % 10);

    first = Next(first, *first + 1);
    second = Next(second, *second + 1);
  }

  auto start = next(recipes.begin(), recipesToDo);
  for (int i = 0; i < 10; i++)
  {
    cout << *start;
    start++;
  }

  return 0;
}
