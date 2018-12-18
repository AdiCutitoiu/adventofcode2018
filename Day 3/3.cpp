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

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  auto lines = Read();

  auto elems = Transform(lines,
    [](const string & aLine) {
    auto tokens = Tokenize(aLine, "#@ ,:x", true);

    return make_tuple(
      I(0),
      I(1),
      I(2),
      I(3),
      I(4)
    );
  });

  map<pair<int, int>, int> freq;

  for (auto &&[id, x, y, width, height] : elems)
  {
    for (int i = x; i < x + width; i++)
    {
      for (int j = y; j < y + height; j++)
      {
        freq[make_pair(i, j)]++;
      }
    }
  }

  // part 1
  cout << count_if(freq.begin(), freq.end(), [](auto && f) {
    return f.second > 1;
  }) << endl;

  // part 2
  auto checkRect = [&freq](auto && rect) {
    auto &&[id, x, y, width, height] = rect;

    for (int i = x; i < x + width; i++)
    {
      for (int j = y; j < y + height; j++)
      {
        if (freq[make_pair(i, j)] > 1)
          return false;
      }
    }

    return true;
  };

  for (auto && rect : elems)
  {
    if (checkRect(rect))
    {
      cout << G(rect, 0);
      return 0;
    }
  }

  return 0;
}
