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

  map<pair<int, int>, vector<pair<int, int>>> graph;

  map<int, pair<int, int>> positions;
  map<int, char> directions;
  map<int, int> turns;

  int carId = 0;
  for (int i = 0; i < lines.size(); i++)
  {
    for (int j = 0; j < lines[i].size(); j++)
    {
      if (lines[i][j] == '<' || lines[i][j] == '>')
      {
        positions[carId] = make_pair(i, j);
        directions[carId] = lines[i][j];
        turns[carId] = 0;
        carId++;

        lines[i][j] = '-';
      }
      else if (lines[i][j] == '^' || lines[i][j] == 'v')
      {
        positions[carId] = make_pair(i, j);
        directions[carId] = lines[i][j];
        turns[carId] = 0;
        carId++;

        lines[i][j] = '|';
      }
    }
  }

  auto nxt = [&lines](auto & i, auto & j, auto & direction, auto & turn) {
    if (direction == '>')
    {
      if (lines[i][j] == '-')
      {
        j++;
      }
      else if (lines[i][j] == '\\')
      {
        i++;
        direction = 'v';
      }
      else if (lines[i][j] == '/')
      {
        i--;
        direction = '^';
      }
      else if (lines[i][j] == '+')
      {
        if (turn == 0)
        {
          i--;
          direction = '^';
        }
        else if (turn == 1)
        {
          j++;
        }
        else if (turn == 2)
        {
          i++;
          direction = 'v';
        }

        turn = (turn + 1) % 3;
      }
    }
    else if (direction == '<')
    {
      if (lines[i][j] == '-')
      {
        j--;
      }
      else if (lines[i][j] == '\\')
      {
        i--;
        direction = '^';
      }
      else if (lines[i][j] == '/')
      {
        i++;
        direction = 'v';
      }
      else if (lines[i][j] == '+')
      {
        if (turn == 0)
        {
          i++;
          direction = 'v';
        }
        else if (turn == 1)
        {
          j--;
        }
        else if (turn == 2)
        {
          i--;
          direction = '^';
        }

        turn = (turn + 1) % 3;
      }

    }
    else if (direction == '^')
    {
      if (lines[i][j] == '|')
      {
        i--;
      }
      else if (lines[i][j] == '\\')
      {
        j--;
        direction = '<';
      }
      else if (lines[i][j] == '/')
      {
        j++;
        direction = '>';
      }
      else if (lines[i][j] == '+')
      {
        if (turn == 0)
        {
          j--;
          direction = '<';
        }
        else if (turn == 1)
        {
          i--;
        }
        else if (turn == 2)
        {
          j++;
          direction = '>';
        }

        turn = (turn + 1) % 3;
      }
    }
    else if (direction == 'v')
    {
      if (lines[i][j] == '|')
      {
        i++;
      }
      else if (lines[i][j] == '\\')
      {
        j++;
        direction = '>';
      }
      else if (lines[i][j] == '/')
      {
        j--;
        direction = '<';
      }
      else if (lines[i][j] == '+')
      {
        if (turn == 0)
        {
          j++;
          direction = '>';
        }
        else if (turn == 1)
        {
          i++;
        }
        else if (turn == 2)
        {
          j--;
          direction = '<';
        }

        turn = (turn + 1) % 3;
      }
    }
  };

  while (positions.size() != 1)
  {
    vector<int> ids;
    for (auto &&[id, _] : positions)
      ids.push_back(id);

    sort(ids.begin(), ids.end(), [&](int id1, int id2) {
      return positions[id1] < positions[id2];
    });

    for (auto id : ids)
    {
      if (positions.find(id) == positions.end())
        continue;

      auto &[i, j] = positions[id];
      auto &direction = directions[id];
      auto &turn = turns[id];

      nxt(i, j, direction, turn);

      auto isCrash = count_if(positions.begin(), positions.end(), [&](auto && pos) {
        return pos.second == make_pair(i, j);
      }) >= 2;

      if (isCrash)
      {
        auto x = i;
        auto y = j;

        positions.erase(positions.find(id));

        auto found = find_if(positions.begin(), positions.end(), [&](auto && pos) {
          return pos.second == make_pair(x, y);
        });
        positions.erase(found);
      }
    }
  }

  auto &&[_, last] = *positions.begin();
  cout << last.second << ',' << last.first;

  return 0;
}
