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

  int recipesToDo = 11323081;

  vector<int> toSearch = { 3,2,3,0,8,1 };

  while (true)
  {
    auto recipe = *first + *second;

    if (recipe >= 10) {
      recipes.push_back(recipe / 10);
    }

    recipes.push_back(recipe % 10);

    first = Next(first, *first + 1);
    second = Next(second, *second + 1);

    if (recipes.size() > 10)
    {
      auto found = std::search(prev(recipes.end(), 10), recipes.end(), toSearch.begin(), toSearch.end());
      if (found != recipes.end())
      {
        auto dist = std::distance(recipes.begin(), found);
        cout << dist;
        return 0;
      }
    }
  }

  return 0;
}
