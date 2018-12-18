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
#include <string>
#include <cstdlib>

using namespace std;

int sum = 0;
map<string, int> values;

void Recurse(const string & id)
{
  int children, md;
  cin >> children >> md;
  for (int i = 1; i <= children; i++)
  {
    Recurse(id + '.' + to_string(i));
  }
  
  for (int i = 1; i <= md; i++)
  {
    int val;
    cin >> val;
    sum += val;
    values[id] += children == 0 ? val : values[id + '.' + to_string(val)];
  }

}

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  Recurse("");

  cout << sum << ' ' << values[""];

  return 0;
}
