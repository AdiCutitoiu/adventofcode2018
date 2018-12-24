#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <regex>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
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

const int neighbours[] = {
  -1, 0, 1
};

int main()
{
  ios::sync_with_stdio();

  freopen("input.txt", "r", stdin);
  // freopen("output.txt", "w", stdout);

  auto lines = Read();

  auto elems = Transform(lines,
    [](const string & aLine) {
    auto tokens = Tokenize(aLine, "pos=<,> r", true);

    return make_tuple(
      I(0),
      I(1),
      I(2),
      I(3)
    );
  });

  auto mDist = [](auto t1, auto t2) {
    return
      abs(G(t1, 0) - G(t2, 0)) +
      abs(G(t1, 1) - G(t2, 1)) +
      abs(G(t1, 2) - G(t2, 2));
  };

  auto strongest = std::max_element(elems.begin(), elems.end(), [](auto n1, auto n2) {
    return G(n1, 3) < G(n2, 3);
  });

  auto total = count_if(elems.begin(), elems.end(), [&](auto elem) {
    return mDist(elem, *strongest) <= G(*strongest, 3);
  });

  cout << total << endl;

  sort(elems.begin(), elems.end(), [](auto t1, auto t2) {
    return G(t1, 3) < G(t2, 3);
  });

  map<tuple<int, int, int, int>, size_t> freq;
  for (auto && current : elems)
  {
    freq[current] = count_if(elems.begin(), elems.end(), [&](auto elem) {
      return mDist(elem, current) <= G(elem, 3);
    });
  }

  sort(elems.begin(), elems.end(), [&](auto n1, auto n2) {
    return freq[n1] > freq[n2];
  });

  auto origin = make_tuple(0, 0, 0);

  auto maxElem = origin;
  auto maxCount = 0LL;
 
  bool retry = true;
  while (retry)
  {
    retry = false;

    auto lowCorner =
      make_tuple(
        1LL * G(maxElem, 0), 
        1LL * G(maxElem, 1), 
        1LL * G(maxElem, 2)  
      );
    auto highCorner =
      make_tuple(
        1LL * G(maxElem, 0), 
        1LL * G(maxElem, 1), 
        1LL * G(maxElem, 2)  
      );

    auto step = 100'000'000LL;

    auto add = [](auto & x, auto amount) {x += amount; };
    std::apply([&](auto && ...args) {(add(args, -step), ...); }, lowCorner);
    std::apply([&](auto && ...args) {(add(args, step), ...); }, highCorner);

    step /= 10;

    while (step)
    {
      auto[x1, y1, z1] = lowCorner;
      auto[x2, y2, z2] = highCorner;

      for (auto i = x1; i <= x2; i += step)
      {
        for (auto j = y1; j <= y2; j += step)
        {
          for (auto k = z1; k <= z2; k += step)
          {
            auto current = make_tuple(i, j, k);
            auto currentCount = count_if(elems.begin(), elems.end(), [&](auto elem) {
              return mDist(elem, current) <= G(elem, 3);
            });

            if (maxCount < currentCount)
            {
              retry = true;
              maxCount = currentCount;
              maxElem = current;
            }
          }
        }
      }

      lowCorner = maxElem;
      highCorner = maxElem;

      auto add = [](auto & x, auto amount) {x += amount; };
      std::apply([&](auto && ...args) {(add(args, -step), ...); }, lowCorner);
      std::apply([&](auto && ...args) {(add(args, step), ...); }, highCorner);

      step /= 10;
    }
  }

  cout << mDist(maxElem, origin);

  return 0;
}
