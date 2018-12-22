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
#include <queue>

using namespace std;

#define NEITHER 0
#define TORCH 1
#define CLIMBING_GEAR 2

#define ROCKY 0
#define WET 1
#define NARROW 2

const int tools[] = {
  NEITHER,
  TORCH,
  CLIMBING_GEAR
};

const bool allowRegionTools[3][3]{
  { false, true,  true},
  { true,  false, true},
  { true,  true,  false},
};

const pair<int, int> neighbours[] = {
  { -1, 0 },
  { 0, -1 },
  { 1,  0 },
  { 0,  1 },
};

struct node
{
  int x{};
  int y{};
  int tool{};
  int totalCost{};
};

struct cost
{
  long long tools[3] = {
    INT_MAX,
    INT_MAX,
    INT_MAX
  };
};

int depth = 9465;
int offset = 50;
pair<int, int> target = make_pair(704, 13);
int lines = target.first + offset;
int cols = target.second + offset;

void Read()
{
  cin.ignore(7);
  cin >> depth;
  cin.ignore(1);

  cin.ignore(8);
  cin >> target.second;

  cin.ignore(1);
  cin >> target.first;
  
  lines = target.first + offset;
  cols = target.second + offset;
}

vector<vector<unsigned long long>> erosionLevel;
vector<vector<short>> type;

void Build()
{
  erosionLevel = vector<vector<unsigned long long>>( lines, vector<unsigned long long>(cols) );
  type = vector<vector<short>>( lines, vector<short>(cols) );
  
  for (int i = 0; i < erosionLevel.size(); i++)
  {
    for (int j = 0; j < erosionLevel[i].size(); j++)
    {
      if (i == 0 && j == 0)
      {
        erosionLevel[i][j] = depth % 20183;
        continue;
      }

      if (i == target.first && j == target.second)
      {
        erosionLevel[i][j] = depth % 20183;
        continue;
      }

      if (i == 0)
      {
        erosionLevel[i][j] = (j * 16807 + depth) % 20183;
        continue;
      }

      if (j == 0)
      {
        erosionLevel[i][j] = (i * 48271 + depth) % 20183;
        continue;
      }

      erosionLevel[i][j] = (erosionLevel[i - 1][j] * erosionLevel[i][j - 1] + depth) % 20183;
    }
  }

  for (int i = 0; i < type.size(); i++)
  {
    for (int j = 0; j < type[i].size(); j++)
    {
      type[i][j] = erosionLevel[i][j] % 3;;
    }
  }
}

int GetRiskLevel()
{
  int sum = 0;

  for (int i = 0; i <= target.first; i++)
  {
    for (int j = 0; j <= target.second; j++)
    {
      sum += type[i][j];
    }
  }

  return sum;
}

int GetMinTime()
{
  vector<vector<cost>> minCosts = vector<vector<cost>>(lines, vector<cost>(cols, cost{}));

  minCosts[0][0] = cost{ { INT_MAX, 0, 0 } };

  bool retry = true;
  while (retry)
  {
    retry = false;

    for (int i = 0; i < minCosts.size(); i++)
    {
      for (int j = 0; j < minCosts[i].size(); j++)
      {
        for (auto neigh : neighbours)
        {
          int x = i + neigh.first;
          int y = j + neigh.second;

          if (x >= 0 && y >= 0 && x < minCosts.size() && y < minCosts[x].size())
          {
            for (auto prevTool : tools)
            {
              // can't reach region i j with tool1
              if (!allowRegionTools[type[i][j]][prevTool])
                continue;

              auto initialCost = minCosts[x][y].tools[prevTool] + 1;
              for (auto currentTool : tools)
              {
                // can't change to tool2 after reaching region i j
                if (!allowRegionTools[type[i][j]][currentTool])
                  continue;

                auto cost = initialCost;
                if (prevTool != currentTool)
                  cost += 7;

                if (cost < minCosts[i][j].tools[currentTool])
                {
                  retry = true;
                  minCosts[i][j].tools[currentTool] = cost;
                }
              }
            }
          }
        }
      }
    }
  }

  auto & m = minCosts[target.first][target.second];
  m.tools[CLIMBING_GEAR] += 7;

  return *min_element(begin(m.tools), end(m.tools));
}

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  Read();
  Build();

  cout << GetRiskLevel() << endl;

  cout << GetMinTime() << endl;

  return 0;
}
