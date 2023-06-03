#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

using namespace std;

class Encoder {
public:
  string sanitize(string &s);
  void encode(string s, int iterations = 1);
  void printFlatTable();
  void printTable();
  string flatWord(char s);
  map<char, string> charTable;
  char key = 33;
};

void Encoder::encode(string s, int iterations) {
  while (s.length() > 2 || iterations-- > 0) {
    map<string, int> wordCount;
    for (int i = 0; i < s.length() - 1; ++i) {
      string w = s.substr(i, 2);
      for (int j = 0; j < s.length() - 1; ++j) {
        if (s.substr(j, 2) == w) {
          wordCount[w]++;
        }
      }
    }
    auto maxToken = max_element(
        wordCount.begin(), wordCount.end(),
        [](const pair<string, int> &p1, const pair<string, int> &p2) {
          return p1.second < p2.second;
        });

    string maxWord = maxToken->first;
    int maxCount = maxToken->second;

    if (maxCount < 2) {
      break;
    }

    this->charTable[this->key] = maxWord;
    for (int k = 0; k < s.length() - 1; ++k) {
      if (s.substr(k, 2) == maxWord) {
        s.replace(k, 2, string(1, this->key));
      };
    }
    this->key++;
  }
};

string Encoder::flatWord(char s) {
  if (this->charTable.find(s) == this->charTable.end()) {
    return string(1, s);
  }
  if (this->charTable[s].length() == 1) {
    return this->charTable[s];
  }
  string word = "";
  for (char c : this->charTable[s]) {
    word += flatWord(c);
  }
  return word;
}

string Encoder::sanitize(string &s) {
  string sanitized = "";
  for (char c : s) {
    if (c == ' ') {
      continue;
    }
    sanitized += c;
  }
  return sanitized;
}

void Encoder::printFlatTable() {
  for (auto const &x : this->charTable) {
    string word = "";
    for (char s : x.second) {
      word += this->flatWord(s);
    }
    cout << x.first << "| " << word << "\n";
  }
}

void Encoder::printTable() {
  for (auto const &x : this->charTable) {
    cout << x.first << "| " << x.second << "\n";
  }
}

int main(int argc, char const *argv[]) {
  string bigStr = "";

  if (argc > 1) {
    cout << "Reading from file: " << argv[1] << "\n";
    ifstream file(argv[1]);
    while (file.good()) {
      string line;
      file >> line;
      bigStr += line;
    }
  } else {
    cout << "Enter a string to encode: ";
    getline(cin, bigStr);
  }

  Encoder e = Encoder();

  string sanitized = e.sanitize(bigStr);

  cout << "sanitized input:" << sanitized << "\n";

  e.encode(sanitized, 100);
  printf("table:\n");
  e.printTable();
  printf("words:\n");
  e.printFlatTable();

  return 0;
}
