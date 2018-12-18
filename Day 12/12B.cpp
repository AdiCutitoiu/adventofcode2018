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
#include <deque>
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

  string istate = "#.##.###.#.##...##..#..##....#.#.#.#.##....##..#..####..###.####.##.#..#...#..######.#.....#..##...#";

  deque<bool> state;
  transform(istate.begin(), istate.end(), back_inserter(state), [](char ch) { return ch == '#'; });

  auto lines = Read();

  map<string, bool> rules;
  for (auto && line : lines)
  {
    auto tokens = Tokenize(line, " =>", true);
    rules[tokens[0]] = tokens[1] == "#";
  }

  int offset = 0;
  size_t to = 100;
  for (size_t i = 0; i < to; i++)
  {
    offset += 5;
    generate_n(front_inserter(state), 5, []() { return false; });
    generate_n(back_inserter(state), 5, []() { return false; });

    deque<bool> nextGen;

    copy(state.begin(), state.begin() + 2, back_inserter(nextGen));
    for (int j = 2; j < state.size() - 2; j++)
    {
      string current;
      for (int k = j - 2; k < j + 3; k++)
        current += state[k] ? '#' : '.';
      
      nextGen.push_back(rules.find(current) == rules.end() ? false : rules[current]);
    }
    copy(state.begin() + state.size() - 2, state.end(), back_inserter(nextGen));

    if (std::all_of(nextGen.begin(), nextGen.begin() + 5, [](auto plant) { return plant == false; }))
    {
      offset -= 5;
      nextGen.erase(nextGen.begin(), nextGen.begin() + 5);
    }

    if (std::all_of(nextGen.begin() + nextGen.size() - 5, nextGen.end(), [](auto plant) { return plant == false; }))
      nextGen.erase(nextGen.begin() + nextGen.size() - 5, nextGen.end());

    state = move(nextGen);
  }

  size_t sum = 0;
  for (int i = 0; i < state.size(); i++)
  {
    if (state[i])
      sum += i - offset + (50'000'000'000 - to);
  }

  cout << sum;

  return 0;
}
