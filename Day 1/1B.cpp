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

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  vector<int> numbers{ istream_iterator<int>(cin), istream_iterator<int>() };
  
  int sum = 0;
  set<int> freqs;
  freqs.insert(0);

  while (true)
  {
    for (auto number : numbers)
    {
      sum += number;
      if (freqs.find(sum) != freqs.end())
      {
        cout << sum;
        return 0;
      }

      freqs.insert(sum);
    }
  }

  return 0;
}
