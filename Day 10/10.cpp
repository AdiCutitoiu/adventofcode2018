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
#include <queue>
#include <numeric>
#include <functional>
#include <iterator>
#include <cstdlib>

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
    auto tokens = Tokenize(aLine, "=< ,", true);

    return make_tuple(
      I(1),
      I(2),
      I(4),
      I(5)
    );
  });

  auto print = [&]() {
    auto left = *min_element(elems.begin(), elems.end(), [](auto && t1, auto && t2) { return G(t1, 0) < G(t2, 0); });
    auto right = *max_element(elems.begin(), elems.end(), [](auto && t1, auto && t2) { return G(t1, 0) < G(t2, 0); });

    auto top = *min_element(elems.begin(), elems.end(), [](auto && t1, auto && t2) { return G(t1, 1) < G(t2, 1); });
    auto bottom = *max_element(elems.begin(), elems.end(), [](auto && t1, auto && t2) { return G(t1, 1) < G(t2, 1); });

    set<pair<int, int>> points;
    for (auto && elem : elems)
    {
      points.insert(make_pair(G(elem, 0), G(elem, 1)));
    }

    for (int j = G(top, 1); j <= G(bottom, 1); j++)
    {
      for (int i = G(left, 0); i <= G(right, 0); i++)
      {
        if (points.find(make_pair(i, j)) == points.end())
        {
          cout << '.';
        }
        else
        {
          cout << '#';
        }
      }
      cout << '\n';
    }
  };

  auto cont = [&]() mutable {

    set<pair<int, int>> points;
    for (auto && elem : elems)
    {
      points.insert(make_pair(G(elem, 0), G(elem, 1)));
    }

    auto f = [&](int offset, auto p) {
      p.second += offset;
      return points.find(p) != points.end();
    };

    for (auto && p: points)
    {
      if (f(1, p) &&
          f(2, p) &&
          f(3, p) &&
          f(4, p) &&
          f(5, p) &&
          f(6, p) &&
          f(7, p))
      {
        return false;
      }
    }

    return true;
  };

  int i = 0;
  while(cont())
  {
    for (auto && elem : elems)
    {
      G(elem, 0) += G(elem, 2);
      G(elem, 1) += G(elem, 3);
    }
    i++;

    cout << i << '\n';
  }

  print();

  return 0;
}
