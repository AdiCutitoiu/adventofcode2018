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

vector<string> lines;
vector<string> newMatrix;

int x[] = { -1, 0, 1 };

void OpenAcre(int line, int col)
{
  int treecount = 0;

  for (int i : x)
  {
    for (int j : x)
    {
      if (i == 0 && j == 0)
        continue;

      if (i + line >= 0 && i + line < lines.size() && j + col < lines[0].size() && j + col >= 0)
      {
        if (lines[i + line][col + j] == '|')
          treecount++;
      }
    }
  }

  if (treecount >= 3)
    newMatrix[line][col] = '|';
  else
    newMatrix[line][col] = lines[line][col];
}

void TreeAcre(int line, int col)
{
  int lumberyardcount = 0;

  for (int i : x)
  {
    for (int j : x)
    {
      if (i == 0 && j == 0)
        continue;

      if (i + line >= 0 && i + line < lines.size() && j + col < lines[0].size() && j + col >= 0)
      {
        if (lines[i + line][col + j] == '#')
          lumberyardcount++;
      }
    }
  }

  if(lumberyardcount >= 3)
    newMatrix[line][col] = '#';
  else
    newMatrix[line][col] = lines[line][col];
}

void LumberyardAcre(int line, int col)
{
  int lumberyardcount = 0;
  int treecount = 0;

  for (int i : x)
  {
    for (int j : x)
    {
      if (i == 0 && j == 0)
        continue;

      if (i + line >= 0 && i + line < lines.size() && j + col < lines[0].size() && j + col >= 0)
      {
        if (lines[i + line][col + j] == '#')
          lumberyardcount++;
        if (lines[i + line][col + j] == '|')
          treecount++;

      }
    }
  }

  if (lumberyardcount && treecount)
    newMatrix[line][col] = '#';
  else
    newMatrix[line][col] = '.';
}

int GetResValue()
{
  int treeCount = 0;
  int lumberCount = 0;
  for (auto && line : lines)
  {
    for (auto && ch : line)
    {
      if (ch == '#')
        lumberCount++;
      else if (ch == '|')
        treeCount++;
    }
  }

  return treeCount * lumberCount;
}

void Next()
{
  newMatrix = vector<string>(lines.size(), string(lines[0].size(), ' '));

  for (int i = 0; i < lines.size(); i++)
  {
    for (int j = 0; j < lines[i].size(); j++)
    {
      if (lines[i][j] == '#')
      {
        LumberyardAcre(i, j);
      }
      else if (lines[i][j] == '.')
      {
        OpenAcre(i, j);
      }
      else
      {
        TreeAcre(i, j);
      }
    }
  }

  lines = move(newMatrix);
}

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  lines = Read();

  size_t cycleStart{};
  vector<int> values;
  map<string, size_t> forests;
  for (int min = 0;; min++)
  {
    Next();

    auto forest = reduce(lines.begin(), lines.end());

    if (auto [it, success] = forests.insert(make_pair(forest, values.size())); success)
    {
      values.push_back(GetResValue());
    }
    else
    {
      cycleStart = it->second;
      break;
    }
  }

  // part 1
  cout << values[9] << endl;

  // part 2
  auto cycleSize = forests.size() - cycleStart;
  auto lastValueIdx = (1'000'000'000 - 1 - cycleStart) % cycleSize + cycleStart;
  cout << values[lastValueIdx];

  return 0;
}
