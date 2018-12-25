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

using Coord = tuple<int, int, int, int>;

map<Coord, Coord> constellations;

Coord GetConstellationRepresentant(Coord aCoord)
{
  while (aCoord != constellations[aCoord])
  {
    aCoord = constellations[aCoord];
  }

  return aCoord;
}

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  auto lines = Read();

  auto points = Transform(lines,
    [](const string & aLine) {
    auto tokens = Tokenize(aLine, ",", true);

    return make_tuple(
      I(0),
      I(1),
      I(2),
      I(3)
    );
  });

  auto mDist = [](auto a, auto b) {
    return abs(G(a, 0) - G(b, 0)) +
           abs(G(a, 1) - G(b, 1)) +
           abs(G(a, 2) - G(b, 2)) +
           abs(G(a, 3) - G(b, 3));
  };

  for (auto && point : points)
  {
    constellations[point] = point;
  }

  for (auto p1 : points)
  {
    for (auto p2 : points)
    {
      auto first = GetConstellationRepresentant(p1);
      auto second = GetConstellationRepresentant(p2);

      if (first == second)
        continue;

      if (mDist(p1, p2) <= 3)
      {
        constellations[first] = second;
      }
    }
  }

  set<Coord> parents;
  for (auto point : points)
  {
    parents.insert(GetConstellationRepresentant(point));
  }

  cout << parents.size();

  return 0;
}
