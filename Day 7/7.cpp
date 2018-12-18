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
#include <queue>
#include <iterator>
#include <cstdlib>

#define TIME 0
#define WORKERS 5

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


map<string, set<string>> children;
set<string> visited;
vector<string> result;
map<int, vector<string>> levels;
map<string, vector<string>> parents;
map<char, int> dep;
map<char, bool> finished;

void DFS(const string & aStart)
{
  visited.insert(aStart);

  for (auto it = children[aStart].rbegin(); it != children[aStart].rend(); it++)
  {
    auto && child = *it;
    parents[child].push_back(aStart);
    dep[child[0]]++;
    if (visited.find(child) == visited.end())
      DFS(child);
  }

  result.push_back(aStart);
}

int GetTime(char aTask)
{
  return TIME + aTask - 'A' + 1;
}

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  auto lines = Read();

  auto elems = Transform(lines,
    [](const string & aLine) {
    auto tokens = Tokenize(aLine, " ", true);

    return make_tuple(
      S(1),
      S(7)
    );
  });

  for (auto && elem : elems)
  {
    children[G(elem, 0)].insert(G(elem, 1));
  }

  for (auto it = children.rbegin(); it != children.rend(); it++)
  {
    auto task = *it;
    if (visited.find(task.first) == visited.end())
      DFS(task.first);
  }

  reverse(result.begin(), result.end());

  int total = 0;
  vector<pair<int, char>> workers;
  bool done = false;
  do 
  {
    done = true;

    for (char ch = 'A'; ch <= 'Z'; ch++)
    {
      if (workers.size() >= WORKERS)
        break;

      if (children.find(string(1, ch)) == children.end())
        continue;

      auto found = find_if(workers.begin(), workers.end(), [ch](auto aWorker) {
        return ch == aWorker.second;
      });
      if (found != workers.end())
        continue;

      if (!dep[ch] && !finished[ch])
      {
        workers.push_back(make_pair(GetTime(ch), ch));
      }
    }

    auto m = min_element(workers.begin(), workers.end());
    total += m->first;

    int time = m->first;
    for (auto & worker : workers)
    {
      worker.first -= time;
      if (worker.first == 0)
      {
        string workerName = ""s + worker.second;
        for (auto child : children[workerName])
        {
          done = false;
          dep[child[0]]--;
        }
        finished[worker.second] = true;
      }
    }

    workers.erase(
      remove_if(workers.begin(), workers.end(), [](auto && aWorker) { return aWorker.first == 0; }),
      workers.end()
    );

  } while (!done || workers.size());

  cout << total;

  return 0;
}
