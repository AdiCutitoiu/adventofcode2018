#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <regex>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>
#include <queue>
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

string reg;
map <pair<int, int>, set<pair<int, int>>> graph;

set<pair<int, int>> BuildGraph(int & index, set<pair<int, int>> points)
{
  vector<set<pair<int, int>>> pointSets;
  pointSets.emplace_back(points);

  while (true)
  {
    ++index;
    if (index >= reg.size() || reg[index] == ')')
      break;

    if (reg[index] == '(')
    {
      BuildGraph(index, pointSets.back());
    }
    else if (reg[index] == '|')
    {
      pointSets.emplace_back(points);
    }
    else
    {
      set<pair<int, int>> newSet;

      for (auto &&[prevX, prevY] : pointSets.back())
      {
        auto x = prevX;
        auto y = prevY;
        switch (reg[index])
        {
        case 'W':
          y--;
          break;
        case 'E':
          y++;
          break;
        case 'N':
          x++;
          break;
        case 'S':
          x--;
          break;
        }

        graph[make_pair(x, y)].insert(make_pair(prevX, prevY));
        graph[make_pair(prevX, prevY)].insert(make_pair(x, y));

        newSet.insert(make_pair(x, y));
      }

      pointSets.back() = move(newSet);
    }
  }

  set<pair<int, int>> result = move(pointSets.front());

  for (auto it = next(pointSets.begin()); it != pointSets.end(); it++)
    copy(it->begin(), it->end(), inserter(result, result.begin()));

  return result;
}

map<pair<int, int>, int> distances;
set<pair<int, int>> visited;
void BFS(pair<int, int> start)
{
  distances.clear();
  visited.clear();

  queue<pair<int, int>> q;
  q.push(start);
  distances[start] = 0;
  visited.insert(start);
  while (!q.empty())
  {
    auto current = q.front();
    q.pop();

    auto dist = distances[current];
    for (auto child : graph[current])
    {
      if (visited.find(child) == visited.end())
      {
        visited.insert(child);
        distances[child] = dist + 1;
        q.push(child);
      }
    }
  }
}

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  cin.ignore(1);
  cin >> reg;
  reg.pop_back();

  int index = -1;
  BuildGraph(index, { make_pair(0, 0) });
  BFS(make_pair(0, 0));

  cout << std::max_element(distances.begin(), distances.end(), [](auto && p1, auto && p2) {
    return p1.second < p2.second;
  })->second << endl;

  cout << std::count_if(distances.begin(), distances.end(), [](auto && p) {
    return p.second >= 1000;
  });

  return 0;
}
