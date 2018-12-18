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

vector<int> GetRegisters(const vector<string> & tokens)
{
  return {
    stoi(tokens[1]),
    stoi(tokens[2]),
    stoi(tokens[3]),
    stoi(tokens[4]),
  };
}

vector<int> GetInstruction(const vector<string> & tokens)
{
  return {
    stoi(tokens[0]),
    stoi(tokens[1]),
    stoi(tokens[2]),
    stoi(tokens[3]),
  };
}

vector<int> regsBefore;
vector<int> regsAfter;

enum Opcode
{
  addr,
  addi,
  mulr,
  muli,
  banr,
  bani,
  borr,
  bori,
  setr,
  set,
  gtir,
  gtri,
  gtrr,
  eqir,
  eqri,
  eqrr
};

vector<int> PerformInstructions(vector<int> registers, Opcode opcode, const vector<int> & instruction)
{
  int A = instruction[1];
  int B = instruction[2];
  int C = instruction[3];

  auto & r = registers;

  switch (opcode)
  {
    case addr: 
      r[C] = r[A] + r[B]; 
      break;
    case addi: 
      r[C] = r[A] + B; 
      break;
    case mulr:
      r[C] = r[A] * r[B];
      break;
    case muli: 
      r[C] = r[A] * B; 
      break;
    case banr:
      r[C] = r[A] & r[B];
      break;
    case bani:
      r[C] = r[A] & B;
      break;
    case borr:
      r[C] = r[A] | r[B];
      break;
    case bori:
      r[C] = r[A] | B;
      break;
    case setr: 
      r[C] = r[A]; 
      break;
    case set : 
      r[C] = A; 
      break;
    case gtir: 
      r[C] = A > r[B]; 
      break;
    case gtri: 
      r[C] = r[A] > B; 
      break;
    case gtrr: 
      r[C] = r[A] > r[B]; 
      break;
    case eqir: 
      r[C] = A == r[B]; 
      break;
    case eqri: 
      r[C] = r[A] == B; 
      break;
    case eqrr: 
      r[C] = r[A] == r[B]; 
      break;
    default:
      throw std::exception("case unknown");
      break;
  }

  return registers;
}

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  auto lines = Read();

  int samples = 0;
  map<int, unordered_set<Opcode>> matches;
  for (int i = 0; i < lines.size(); i+=4)
  {
    regsBefore = GetRegisters(Tokenize(lines[i], "[], ", true));
    regsAfter = GetRegisters(Tokenize(lines[i + 2], "[], ", true));

    auto instr = GetInstruction(Tokenize(lines[i + 1], " ", true));

    int matched = 0;
    for (int opcode = 0; opcode < 16; opcode++)
    {
      auto after = PerformInstructions(regsBefore, static_cast<Opcode>(opcode), instr);

      if (regsAfter == after)
        matches[instr[0]].insert(static_cast<Opcode>(opcode));
    }
  }

  vector<pair<int, unordered_set<Opcode>>> sortedMatches;
  copy(matches.begin(), matches.end(), back_inserter(sortedMatches));

  map<int, Opcode> opcodeMapping;
  while (opcodeMapping.size() < 16)
  {
    sort(sortedMatches.begin(), sortedMatches.end(), [](auto & first, auto & second) {
      return first.second.size() < second.second.size();
    });

    auto opcode = *sortedMatches.front().second.begin();

    opcodeMapping[sortedMatches.front().first] = opcode;

    for (auto &&[_, freq] : sortedMatches)
    {
      freq.erase(opcode);
    }

    sortedMatches.erase(
      remove_if(sortedMatches.begin(), sortedMatches.end(), [](auto & aFreq) { return aFreq.second.empty(); }),
      sortedMatches.end()
    );
  }

  // freopen("input2.txt", "r", stdin);

  ifstream f("input2.txt");
  lines.clear();
  string line;
  while (getline(f, line))
  {
    lines.push_back(move(line));
  }

  vector<int> regs(4, 0);
  for (auto && line : lines)
  {
    auto instruction = GetInstruction(Tokenize(line, " ", true));

    regs = PerformInstructions(regs, opcodeMapping[instruction[0]], instruction);
  }

  cout << regs[0];

  return 0;
}
