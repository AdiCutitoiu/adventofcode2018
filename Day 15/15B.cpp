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

struct Unit
{
  pair<int, int> position;
  int hp;
  bool elf;

  bool operator<(const Unit & aOther)
  {
    return position < aOther.position;
  }
};

set<pair<int, int>> walls;
vector<Unit> units;
vector<Unit> readUnits;
set<Unit*> deleted;
int elfPower = 4;

void ReadMatrix()
{
  auto lines = Read();

  walls.clear();
  readUnits.clear();
  deleted.clear();

  for (size_t i = 0; i < lines.size(); i++)
  {
    for (size_t j = 0; j < lines[i].size(); j++)
    {
      if (lines[i][j] == '#')
      {
        walls.insert(make_pair(i, j));
      }
      else if (lines[i][j] == 'E')
      {
        readUnits.push_back(Unit{ make_pair(i, j), 200, true });
      }
      else if (lines[i][j] == 'G')
      {
        readUnits.push_back(Unit{ make_pair(i, j), 200, false });
      }
    }
  }
}

auto FindUnit(int x, int y, bool aElf)
{
  auto found = find_if(units.begin(), units.end(), [&](auto & aUnit) {
    return aUnit.position.first == x && aUnit.position.second == y && aUnit.elf == aElf;
  });

  return found != units.end() ? &(*found) : nullptr;
}

vector<Unit *> GetUnitsInRange(Unit & aSource)
{
  auto[x, y] = aSource.position;

  int l[] = { -1, 0, 1, 0 };
  int c[] = { 0 ,-1, 0, 1 };

  vector<Unit *> result;

  for (int i = 0; i < 4; i++)
  {
    auto unit = FindUnit(x + l[i], y + c[i], !aSource.elf);
    if (unit && deleted.find(unit) == deleted.end())
      result.push_back(unit);
  }

  return result;
}

bool Attack(Unit & unit)
{
  auto adjUnits = GetUnitsInRange(unit);

  if (!adjUnits.empty())
  {
    sort(adjUnits.begin(), adjUnits.end(), [](Unit * first, Unit * second) {
      return first->hp < second->hp || first->hp == second->hp && first->position < second->position;
    });

    adjUnits.front()->hp -= unit.elf ? elfPower : 3;

    if (adjUnits.front()->hp <= 0)
      deleted.insert(adjUnits.front());

    return true;
  }

  return false;
}

vector<pair<int, int>> GetEnemyUnitsAdjPositions(bool elf)
{
  int l[] = { -1, 0, 1, 0 };
  int c[] = { 0 ,-1, 0, 1 };

  vector<pair<int, int>> result;

  for (auto & unit : units)
  {
    if (deleted.find(&unit) != deleted.end())
      continue;

    if (unit.elf != elf)
    {
      auto pos = unit.position;
      for (int i = 0; i < 4; i++)
      {
        auto adjPos = unit.position;
        adjPos.first += l[i];
        adjPos.second += c[i];

        result.push_back(adjPos);
      }
    }
  }

  return result;
}

vector<Unit *> GetAllEnemyUnits(bool elf)
{
  vector<Unit *> result;

  for (auto & unit : units)
  {
    if (deleted.find(&unit) == deleted.end() && unit.elf == !elf)
      result.push_back(&unit);
  }

  return result;
}

bool IsOccupied(pair<int, int> position)
{
  if (walls.find(position) != walls.end())
    return true;

  auto foundUnit = find_if(units.begin(), units.end(), [&](Unit & unit) {
    return unit.position == position && unit.hp > 0;
  });

  return foundUnit != units.end();
}

bool IsWallOrAlly(pair<int, int> position, bool elf)
{
  if (walls.find(position) != walls.end())
    return true;

  auto foundUnit = find_if(units.begin(), units.end(), [&](auto & unit) {
    return unit.position == position && unit.elf == elf;
  });

  return foundUnit != units.end();
}

void Move(Unit & unit)
{
  int l[] = { -1, 0, 1, 0 };
  int c[] = { 0 ,-1, 0, 1 };

  auto enemyUnits = GetAllEnemyUnits(unit.elf);
  sort(enemyUnits.begin(), enemyUnits.end(), [](auto first, auto second) {
    return *first < *second;
  });

  int minDist = INT_MAX;
  Unit * minEnemy = nullptr;
  vector<pair<int, pair<int, int>>> adjDist;

  for (auto enemy : enemyUnits)
  {
    map<pair<int, int>, int> distances;
    distances[enemy->position] = 0;

    queue<pair<int, int>> points;
    points.push(enemy->position);

    while (!points.empty())
    {
      auto current = points.front();
      points.pop();

      for (int i = 0; i < 4; i++)
      {
        auto nextPoint = current;
        nextPoint.first += l[i];
        nextPoint.second += c[i];

        if (nextPoint != unit.position && IsOccupied(nextPoint))
          continue;

        if (distances.find(nextPoint) == distances.end())
        {
          distances[nextPoint] = distances[current] + 1;
          points.push(nextPoint);
        }
      }
    }

    if (distances.find(unit.position) == distances.end())
      continue;

    if (distances[unit.position] < minDist)
    {
      minDist = distances[unit.position];
      minEnemy = enemy;

      adjDist.clear();
      for (int i = 0; i < 4; i++)
      {
        auto nextPoint = unit.position;
        nextPoint.first += l[i];
        nextPoint.second += c[i];

        if (auto found = distances.find(nextPoint); found != distances.end())
          adjDist.push_back(make_pair(found->second, nextPoint));
      }
    }
  }

  if (minDist == INT_MAX)
    return;

  sort(adjDist.begin(), adjDist.end());

  auto found = find_if(adjDist.begin(), adjDist.end(), [&](auto & p) {
    return p.first != adjDist.front().first;
  });

  adjDist.erase(found, adjDist.end());

  for (auto adj : adjDist)
  {
    if (!IsOccupied(adj.second))
    {
      unit.position = adj.second;
      return;
    }
  }
}

void Print()
{

  auto min = walls.begin();
  auto max = prev(walls.end());

  string line;
  for (int i = min->first; i <= max->first; i++)
  {
    for (int j = min->second; j <= max->second; j++)
    {
      if (walls.find(make_pair(i, j)) != walls.end())
      {
        line += '#';
      }
      else if (auto found = find_if(units.begin(), units.end(), [&](auto & unit) { return unit.position == make_pair(i, j); });
      found != units.end())
      {
        line += (found->elf ? 'E' : 'G');
      }
      else
      {
        line += '.';
      }
    }

    line += '\n';
  }
  line += '\n';

  sort(units.begin(), units.end());

  for (auto && unit : units)
  {
    line += (unit.elf ? 'E' : 'G');
    line += '(';
    line += to_string(unit.hp);
    line += ")";
    line += "\n";
  }

  cout << line << '\n';
}

bool TakeTurn()
{
  sort(units.begin(), units.end());

  for (auto & unit : units)
  {
    if (deleted.find(&unit) != deleted.end())
      continue;

    auto enemies = GetAllEnemyUnits(unit.elf);
    if (enemies.empty())
    {
      units.erase(
        remove_if(units.begin(), units.end(), [](auto & unit) { return unit.hp <= 0; }),
        units.end()
      );

      deleted.clear();
      return false;
    }

    Move(unit);

    Attack(unit);
  }

  units.erase(
    remove_if(units.begin(), units.end(), [](auto & unit) { return unit.hp <= 0; }),
    units.end()
  );

  deleted.clear();

  return true;
}

bool Check()
{
  bool foundElf = find_if(units.begin(), units.end(), [](auto & unit) { return unit.elf; }) != units.end();
  bool foundGoblin = find_if(units.begin(), units.end(), [](auto & unit) { return !unit.elf; }) != units.end();

  return foundElf && foundGoblin;
}

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  ReadMatrix();

  auto elvesCount = count_if(readUnits.begin(), readUnits.end(), [](Unit & unit) {return unit.elf; });

  while (true)
  {
    units = readUnits;

    auto turns = 0;
    while (Check())
    {
      if (!TakeTurn())
        turns--;

      ++turns;
    }

    auto elvesRemaining = count_if(units.begin(), units.end(), [](Unit & unit) {return unit.elf; });

    if (elvesRemaining == elvesCount)
    {
      auto hpSum = accumulate(units.begin(), units.end(), 0LL, [](auto sum, auto & unit) { return sum + unit.hp; });
      cout << 1LL * (turns)* hpSum;
      return 0;
    }

    elfPower++;
  }

  return 0;
}
