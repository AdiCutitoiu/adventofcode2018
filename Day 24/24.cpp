#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>
#include <list>
#include <numeric>
#include <functional>
#include <unordered_set>
#include <iterator>
#include <set>
#include <cstdlib>

#define I(pos) stoi(tokens[pos])
#define S(pos) tokens[pos]

#define G(t, p) get<p>(t)

using namespace std;

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

vector<string> ReadLines(bool aImmune)
{
  string fileName = "input.txt";
  if (!aImmune)
    fileName = "input2.txt";

  ifstream in(fileName);

  vector<string> result;

  string str;
  while (getline(in, str))
    result.push_back(str);

  return result;
}

class Group
{
public:
  Group(int aId, bool aImmune, int aUnits, int aUnitHp, int aUnitAttack, string aUnitAttackType, int aInitiative, set<string> aImmunities, set<string> aWeaknesses)
    : mId{ aId }
    , mImmune{ aImmune }
    , mUnits{ aUnits }
    , mUnitHp{ aUnitHp }
    , mUnitAttack{ aUnitAttack }
    , mUnitAttackType{ move(aUnitAttackType) }
    , mInitiative{ aInitiative }
    , mImmunities{ move(aImmunities) }
    , mWeaknesses{ move(aWeaknesses) }
  {
  }

  bool IsDead() const
  {
    return mUnits <= 0;
  }

  bool GetGroupType() const
  {
    return mImmune;
  }

  int GetGroupid() const
  {
    return mId;
  }

  int GetEffectivePower() const
  {
    return mUnitAttack * mUnits;
  }

  int GetInitiative() const
  {
    return mInitiative;
  }

  bool operator>(const Group & aOther) const
  {
    return GetEffectivePower() > aOther.GetEffectivePower() ||
           GetEffectivePower() == aOther.GetEffectivePower() && mInitiative > aOther.mInitiative;
  }

  int GetUnits() const
  {
    return mUnits;
  }

  void PickTarget(vector<Group*> & groups)
  {
    mTarget = nullptr;

    vector<Group*> candidates;
    vector<Group*> allies;
    for (auto && group : groups)
    {
      if (IsAlly(*group))
      {
        if (group != this)
          allies.push_back(group);
      }
      else
      {
        if(DamageTo(*group) != 0)
          candidates.push_back(group);
      }
    }

    auto isAlreadyPicked = [&](Group * aTarget) {
      return find_if(allies.begin(), allies.end(), [&](Group * aAlly) {
        return aAlly->mTarget == aTarget;
      }) != allies.end();
    };

    candidates.erase(
      remove_if(candidates.begin(), candidates.end(), isAlreadyPicked),
      candidates.end()
    );

    if (candidates.empty())
      return;

    sort(candidates.begin(), candidates.end(), [this](Group * c1, Group * c2) {
      auto criteria1 = make_tuple(DamageTo(*c1), c1->GetEffectivePower(), c1->mInitiative);
      auto criteria2 = make_tuple(DamageTo(*c2), c2->GetEffectivePower(), c2->mInitiative);

      return criteria1 > criteria2;
    });

    mTarget = candidates.front();

    string message = mImmune ? "Immune" : "Infection";
    message += " group " + to_string(mId) + " would deal defending group " + to_string(mTarget->mId);
    message += " " + to_string(DamageTo(*mTarget)) + " damage";
  }

  void Attack()
  {
    if (IsDead())
      return;

    if (!mTarget)
      return;

    if (mId == 1 && mTarget->mId == 1)
    {
      int x = 0;
      x;
    }

    auto damage = DamageTo(*mTarget);

    auto deadUnits = damage / mTarget->mUnitHp;
    if (deadUnits > mTarget->mUnits)
      deadUnits = mTarget->mUnits;

    string message = mImmune ? "Immune" : "Infection";
    message += " group " + to_string(mId) + " attacks defending group " + to_string(mTarget->mId);
    message += " killing " + to_string(deadUnits) + " units, dealing " + to_string(damage) + " damage";

    mTarget->mUnits -= deadUnits;
    if (mTarget->mUnits < 0)
      mTarget->mUnits = 0;
  }

  bool IsAlly(const Group & aOther) const
  {
    return mImmune == aOther.mImmune;
  }

  int DamageTo(const Group & aOther) const
  {
    if (aOther.mImmunities.find(mUnitAttackType) != aOther.mImmunities.end())
      return 0;

    auto power = GetEffectivePower();
    if (aOther.mWeaknesses.find(mUnitAttackType) != aOther.mWeaknesses.end())
      return 2 * power;

    return power;
  }

  void ResetTarget()
  {
    mTarget = nullptr;
  }

  void Boost(int aAmount)
  {
    mUnitAttack += aAmount;
  }

private:
  int mId;
  bool mImmune;
  int mUnits{};
  int mUnitHp{};
  int mUnitAttack{};
  string mUnitAttackType;
  int mInitiative;

  set<string> mImmunities;
  set<string> mWeaknesses;

  Group* mTarget{ nullptr };
};

vector<Group> GetGroups(bool aImmune)
{
  vector<Group> result;

  vector<string> lines = ReadLines(aImmune);

  static int id;
  for (int i = 0; i < lines.size(); i++)
  {
    auto line = lines[i];

    auto tokens = Tokenize(line, " ", false);

    int units = I(0);
    int unitHp = I(4);
    int unitAttack = I(tokens.size() - 6);
    string unitAttackType = S(tokens.size() - 5);
    int initiative = I(tokens.size() - 1);

    set<string> immunities;
    set<string> weaknesses;
    {
      if (Tokenize(line, "()", false).size() >= 2)
      {
        auto misc = Tokenize(line, "()", false)[1];
        auto types = Tokenize(misc, ";", true);

        for (auto && type : types)
        {
          auto tokens = Tokenize(type, ", ", true);

          auto &toInsertIn = tokens.front() == "immune" ? immunities : weaknesses;

          for (int i = 2; i < tokens.size(); i++)
          {
            toInsertIn.insert(tokens[i]);
          }
        }
      }
    }

    result.emplace_back(id++, aImmune, units, unitHp, unitAttack, move(unitAttackType), initiative, 
      move(immunities), move(weaknesses));
  }

  return result;
}

vector<Group> GetAllGroups()
{
  auto immuneGroups = GetGroups(true);
  auto infectionGroups = GetGroups(false);

  vector<Group> groups;
  for (auto && g : immuneGroups)
    groups.push_back(move(g));

  for (auto && g : infectionGroups)
    groups.push_back(move(g));

  return groups;
}

bool CanContinue(vector<Group> & groups)
{
  auto c = count_if(groups.begin(), groups.end(), [](auto & group) { return group.GetGroupType(); });

  return c != 0 && c != groups.size();
}

void TakeTurn(vector<Group *> groups)
{
  for (auto && group : groups)
    group->ResetTarget();

  sort(groups.begin(), groups.end(), [](const Group * first, const Group * second) {
    return *first > *second;
  });

  for (auto && group : groups)
  {
    group->PickTarget(groups);
  }

  sort(groups.begin(), groups.end(), [](const Group * first, const Group * second) {
    return first->GetInitiative() > second->GetInitiative();
  });

  for (auto && group : groups)
  {
    group->Attack();
  }
}

void RemoveDeadGroups(vector<Group> & groups)
{
  groups.erase(
    remove_if(groups.begin(), groups.end(), [](Group & aGroup) { return aGroup.IsDead(); }),
    groups.end()
  );
}

void Print(vector<Group> & groups)
{
  vector<const Group *> immune;
  for (auto & group : groups)
  {
    if (group.GetGroupType())
      immune.push_back(&group);
  }

  vector<const Group *> infectious;
  for (auto & group : groups)
  {
    if (!group.GetGroupType())
      infectious.push_back(&group);
  }

  auto printGroups = [](vector<const Group *> groups) {
    sort(groups.begin(), groups.end(), [](const Group * g1, const Group * g2) {
      return g1->GetGroupid() < g2->GetGroupid();
    });

    for (auto group : groups)
    {
      cout << "Group " << group->GetGroupid() << " contains " << group->GetUnits() << " units\n";
    }
  };

  cout << "Immune System: " << immune.size() << endl;
  printGroups(immune);

  cout << "Infection:" << infectious.size() << endl;
  printGroups(infectious);
  cout << endl;
}

void Boost(vector<Group> & groups, int aAmount)
{
  for (auto && group : groups)
  {
    if (group.GetGroupType())
      group.Boost(aAmount);
  }
}

bool SimulateGame(vector<Group> & groups)
{
  int turn = 1;
  while (CanContinue(groups))
  {
    map<Group*, int> before;
    for (auto & group : groups)
      before.insert(make_pair(&group, group.GetUnits()));

    vector<Group*> groupPointers;
    for (auto & group : groups)
    {
      groupPointers.push_back(&group);
    }

    TakeTurn(groupPointers);

    RemoveDeadGroups(groups);

    if (groups.size() == before.size())
    {
      auto isInfinite = all_of(before.begin(), before.end(), [](const pair<Group*, int> aPair) {
        return aPair.first->GetUnits() == aPair.second;
      });
      if (isInfinite)
        return false;
    }
  }

  return true;
}

int main()
{
  ios::sync_with_stdio();

  auto groups = GetAllGroups();

  int power = 0;
  while (true)
  {
    auto cpy = groups;

    for (auto & group : cpy)
    {
      if (group.GetGroupType())
        group.Boost(power);
    }

    cout << "[" << power << "] ";
    if (SimulateGame(cpy))
    {
      cout << accumulate(cpy.begin(), cpy.end(), 0,
                         [](int aPrev, Group & group) { return aPrev + group.GetUnits();  });
      
      if (power == 0)
      {
        cout << " <------ ANSWER PART 1\n";
      }

      if (cpy.front().GetGroupType())
      {
        cout << " <------ ANSWER PART 2\n";
        break;
      }

      cout << endl;
    }
    else
    {
      cout << "Infinite" << endl;
    }

    power++;
  }

  return 0;
}
