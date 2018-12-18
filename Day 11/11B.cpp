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
#include <variant>
#include <iterator>
#include <optional>
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

  int rackID = 4842;

  int x[303][303] = { 0 };

  for (int i = 1; i <= 300; i++)
  {
    for (int j = 1; j <= 300; j++)
    {
      x[i][j] = ((((i + 10) * j + rackID) * (i + 10)) / 100) % 10 - 5;
    }
  }

  int m = 0;
  int maxX = 0;
  int maxY = 0;
  int s = 0;

  for (int i = 1; i <= 300; i++)
  {
    for (int j = 1; j <= 300; j++)
    {
      for (auto size = 1; size < 300 - (i > j ? i : j); size++)
      {
        auto sum = 0;
        for (auto coordX = i; coordX <= i + size - 1; coordX++)
        {
          for (auto coordY = j; coordY <= j + size - 1; coordY++)
          {
            sum += x[coordX][coordY];
          }
        }

        if (sum > m)
        {
          m = sum;
          maxX = i;
          maxY = j;
          s = size;
        }
      }
    }
  }

  cout << m << ' ' << maxX << ' ' << maxY << ' ' << s;

  return 0;
}
