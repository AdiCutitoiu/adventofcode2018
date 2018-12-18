/*#include <iostream>
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

using namespace std;

template<typename T>
vector<T> Read(istream & aIstream)
{
  vector<T> res;

  copy(istream_iterator<T>(aIstream), istream_iterator<T>(), back_inserter(res));

  return res;
}

struct MyStruct
{
  int x, y;
};

istream & operator>>(istream & aIstream, MyStruct & aObj)
{
  return aIstream >> aObj.x >> aObj.y;
}

int main() {

  ifstream in("input.txt");

  auto r = Read<string>(in);

  return 0;
}*/

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

using namespace std;

template<typename T>
vector<T> Read(istream & aIstream)
{
  vector<T> res;

  copy(istream_iterator<T>(aIstream), istream_iterator<T>(), back_inserter(res));

  return res;
}

struct MyStruct
{
  string action;
  int minute;
  int guard;
};

int guard;
istream & operator>>(istream & aIstream, MyStruct & aObj)
{
  char sep;
  int hourPlaceholder;
  string stringPlaceHolder;
  aIstream 
    >> stringPlaceHolder
    >> hourPlaceholder
    >> sep
    >> aObj.minute
    >> sep;

  string action;
  aIstream >> action;

  if (action == "Guard")
  {
    aIstream >> sep;
    aIstream >> guard;
  }

  aObj.guard = guard;
  aObj.action = action;

  string str;
  getline(aIstream, str);
  return aIstream;
}

map<int, int> freq;
map<int, map<int, int>> guards;

void Task1()
{
  auto maxMinutes = std::max_element(freq.begin(), freq.end(), [](auto p1, auto p2) {
    return p1.second < p2.second;
  });

  auto g = maxMinutes->first;
  auto mostMin = std::max_element(guards[g].begin(), guards[g].end(), [](auto p1, auto p2) {
    return p1.second < p2.second;
  });

  cout << g * mostMin->first << '\n';
}

void Task2()
{
  int guardId;
  int minute;
  int maxMin = 0;

  for (auto & guard : guards)
  {
    for (auto & minutes : guard.second)
    {
      if (minutes.second > maxMin)
      {
        minute = minutes.first;
        guardId = guard.first;
        maxMin = minutes.second;
      }
    }
  }

  cout << minute * guardId;
}

int main() {

  // input is already sorted
  ifstream in("input.txt");

  auto r = Read<MyStruct>(in);

  for (int i = 0; i < r.size();)
  {
    while (i < r.size() && (r[i].action == "Guard"))
      i++;

    auto first = r[i++];
    auto second = r[i++];

    for (int j = first.minute; j < second.minute; j++)
    {
      freq[first.guard]++;
      guards[first.guard][j]++;
    }
  }

  Task1();
  Task2();

  return 0;
}

