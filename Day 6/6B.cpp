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
#include <queue>

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

template<typename LineParser>
auto Transform(const vector<string> & aLines, LineParser && aLineParser)
{
  vector<invoke_result_t<LineParser, string>> items;

  transform(aLines.begin(), aLines.end(), back_inserter(items), aLineParser);

  return items;
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

string Replace(string aString, const string & aNeedle, const string & aReplaceable)
{
  if (!aNeedle.empty())
  {
    size_t pos = 0;

    while ((pos = aString.find(aNeedle, pos)) != std::string::npos)
    {
      aString.replace(pos, aNeedle.size(), aReplaceable);
      pos += aReplaceable.size();
    }
  }

  return aString;
}

string Remove(const string &aString, const string & aNeedle)
{
  return Replace(aString, aNeedle, "");
}

template<typename T>
void Dump(T & aElems)
{
  auto p = [](auto && arg) { cout << arg << ' '; };

  for (auto && elem : aElems)
  {

    std::apply([&p](auto && ...args) {
      (p(args), ...);
    }, elem);

    cout << '\n';
  }
}

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  auto lines = Read();

  auto elems = Transform(lines,
    [](const string & aLine) {
    auto tokens = Tokenize(aLine, " ,", true);

    return make_pair(
      I(0),
      I(1)
    );
  });

  // Dump(elems);

  auto left = std::min_element(begin(elems), end(elems), [](auto p1, auto p2) { return p1.first < p2.first; })->first;
  auto right = std::max_element(begin(elems), end(elems), [](auto p1, auto p2) { return p1.first < p2.first; })->first;

  auto top = std::min_element(begin(elems), end(elems), [](auto p1, auto p2) { return p1.second < p2.second; })->second;
  auto bottom = std::max_element(begin(elems), end(elems), [](auto p1, auto p2) { return p1.second < p2.second; })->second;

  auto mDist = [](auto p1, auto p2) {
    return abs(p1.first - p2.first) + abs(p1.second - p2.second);
  };

  int count = 0;
  for (int i = left; i <= right; i++)
  {
    for (int j = top; j <= bottom; j++)
    {
      auto s = 0;
      for (auto && p : elems)
      {
        s += mDist(p, make_pair(i, j));
      }
      if (s < 10000)
        count++;
    }
  }

  cout << count;

  return 0;
}
