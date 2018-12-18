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
#include <queue>
#include <cstdlib>
#include <set>

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

struct Rect
{
  int minX;
  int minY;
  int maxX;
  int maxY;
};

int top = INT_MAX;
int bottom = -INT_MAX;
int lleft = INT_MAX;
int rright = -INT_MAX;

set<pair<int, int>> clay;
map<pair<int, int>, bool> water;

bool IsClay(int line, int col)
{
  return clay.find(make_pair(line, col)) != clay.end();
}

bool IsStaticWater(int line, int col)
{
  auto found = water.find(make_pair(line, col));
  return found != water.end() && found->second;
}

bool IsWater(int line, int col)
{
  return water.find(make_pair(line, col)) != water.end();
}

bool IsOccupied(int line, int col)
{
  if (IsClay(line, col))
    return true;

  return IsStaticWater(line, col);
}

void Print(int Y, int X)
{
  for (int i = 0; i <= bottom; i++)
  {
    for (int j = lleft; j <= rright; j++)
    {
      if (i == Y && j == X)
      {
        cout << 'X';
      }
      else
        if (i == 0 && j == 500)
        {
          cout << '+';
        }
        else if (IsClay(i, j))
        {
          cout << '#';
        }
        else if (auto found = water.find(make_pair(i, j)); found != water.end())
        {
          cout << (found->second ? '~' : '|');
        }
        else
        {
          cout << '.';
        }
    }

    cout << '\n';
  }
  cout << '\n';
}

void CountWaterSquares(int Y, int X)
{
  if (Y > bottom)
    return;

  if (IsClay(Y, X) || IsWater(Y, X))
    return;

  water[make_pair(Y, X)] = false;

  bool retry = true;
  while (retry)
  {
    retry = false;

    if (IsOccupied(Y + 1, X))
    {
      auto left = X - 1;
      while (IsOccupied(Y + 1, left) && !IsOccupied(Y, left))
        left--;

      auto right = X + 1;
      while (IsOccupied(Y + 1, right) && !IsOccupied(Y, right))
        right++;

      bool staticWater = IsOccupied(Y + 1, left) && IsOccupied(Y + 1, right);
      for (int i = left + 1; i < right; i++)
        water[make_pair(Y, i)] = staticWater;

      if (!staticWater)
      {
        CountWaterSquares(Y, left);
        CountWaterSquares(Y, right);
      }
    }
    else
    {
      CountWaterSquares(Y + 1, X);

      if (IsStaticWater(Y + 1, X))
        retry = true;
    }
  }
}

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  auto lines = Read();


  vector<Rect> rects;
  for (auto && line : lines)
  {
    auto tokens = Tokenize(line, "=, .", true);

    Rect rect;
    if (tokens[0] == "x")
    {
      rect.minX = rect.maxX = I(1);
      rect.minY = I(3);
      rect.maxY = I(4);
    }
    else
    {
      rect.minY = rect.maxY = I(1);
      rect.minX = I(3);
      rect.maxX = I(4);
    }

    for (int i = rect.minY; i <= rect.maxY; i++)
    {
      for (int j = rect.minX; j <= rect.maxX; j++)
      {
        clay.insert(make_pair(i, j));
      }
    }

    top = min(top, rect.minY);
    lleft = min(lleft, rect.minX);
    bottom = max(bottom, rect.maxY);
    rright = max(rright, rect.maxX);
  }

  CountWaterSquares(0, 500);

  Print(0, 500);

  // part 1
  cout << count_if(water.begin(), water.end(), [](auto && water) {
    return water.first.first >= top && water.first.first <= bottom;
  }) << endl;

  // part 2
  cout << count_if(water.begin(), water.end(), [](auto && water) {
    return water.first.first >= top && water.first.first <= bottom && water.second;
  });

  return 0;
}
