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

list<int> marbles(1, 0);

auto Next(list<int>::iterator aStart)
{
  for (int i = 0; i < 2; i++)
  {
    if (aStart == marbles.end())
      aStart = marbles.begin();

    aStart = next(aStart);
  }

  return aStart;
}

auto Prev(list<int>::iterator aStart)
{
  for (int i = 0; i < 7; i++)
  {
    if (aStart == marbles.begin())
      aStart = marbles.end();

    aStart = prev(aStart);
  }
  return aStart;
}

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  int p, m;
  cin >> p >> m;

  vector<size_t> players(p);
  auto currentPlayer = players.begin();

  auto currentMarble = marbles.begin();
  for (int marble = 1; marble < m; marble++) 
  {
    if (marble % 23 == 0) 
    {
      currentMarble = Prev(currentMarble);
      *currentPlayer += marble + *currentMarble;
      currentMarble = marbles.erase(currentMarble);
    }
    else 
    {
      currentMarble = marbles.insert(Next(currentMarble), marble);
    }

    currentPlayer++;
    if (currentPlayer == players.end())
      currentPlayer = players.begin();
  }

  cout << *max_element(players.begin(), players.end());

  return 0;
}
