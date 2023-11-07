#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <windows.h>
#include <random>

void getRhyme(int i, int j,
              std::map<std::string, std::vector<int>>& rhymes,
              bool visited[],
              std::vector<int>& myPoem) {

  bool ok1 = false;
  bool ok2 = false;

  while (!ok1 || !ok2) {
    int randomRhymeIndex = rand() % rhymes.size();
    auto it = rhymes.begin();
    std::advance(it, randomRhymeIndex);

    if (it->second.size() <= 50) continue;

    if (!ok1) {
      auto rhymedLines = it->second;
      int randomLineIndex = rhymedLines[rand() % rhymedLines.size()];

      if (!visited[randomLineIndex]) {
        visited[randomLineIndex] = true;
        ok1 = true;

        myPoem[i - 1] = randomLineIndex;
        rhymedLines[rand() % rhymedLines.size()] = true;
      }
    }
    if (!ok2) {
      auto rhymedLines = it->second;
      int randomLineIndex = rhymedLines[rand() % rhymedLines.size()];

      if (!visited[randomLineIndex]) {
        visited[randomLineIndex] = true;
        ok2 = true;

        myPoem[j - 1] = randomLineIndex;
        rhymedLines[rand() % rhymedLines.size()] = true;
      }
    }
  }
}

int main() {
  // include Russian language
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);

  // our file
  std::ifstream file("Onegin.txt");

  // any errors or not
  if (file.fail() || !file.is_open()) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    std::cout << strerror(errno) << std::endl;

    return 1;
  } else {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    std::cout << strerror(errno) << std::endl;
  }
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

  // get size of file
  file.seekg(0, std::ios::end);
  int64_t fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  // other variables
  char* oneginPoem = new char[fileSize];
  file.read(oneginPoem, fileSize);
  file.close();

  const char* nextLine = "\n";
  char* text = nullptr;
  char* lines[10000];
  char* lineStep = strtok_s(oneginPoem, nextLine, &text);
  int lineCount = 0;

  while (lineStep != nullptr) {
    lines[lineCount] = lineStep;
    ++lineCount;
    lineStep = strtok_s(nullptr, nextLine, &text);
  }

  time_t randSeed = std::time(nullptr);
  std::srand(randSeed);

  // rhymes
  std::map<std::string, std::vector<int>> rhymes;
  bool visited[10000];
  for (int i = 0; i < lineCount; ++i) {
    const char* line = lines[i];
    std::string rhyme;

    for (int j = strlen(line) - 1; j >= 0; --j) {
      char letter = line[j];
      // skip english words
      if (letter >= 'a' && letter <= 'z' ||
          letter >='A' && letter <= 'Z') break;
      // bad symbols
      if (letter != ' ') {
        std::string temp = letter + rhyme;
        rhyme = temp;
      }

      if (rhyme.size() == 3) break;
    }

    rhymes[rhyme].push_back(i);

    visited[i] = false;
  }

  std::vector<int> myPoem(14);
  getRhyme(1, 3, rhymes, visited, myPoem);
  getRhyme(2, 4, rhymes, visited, myPoem);
  getRhyme(5, 6, rhymes, visited, myPoem);
  getRhyme(7, 8, rhymes, visited, myPoem);
  getRhyme(9, 12, rhymes, visited, myPoem);
  getRhyme(10, 11, rhymes, visited, myPoem);
  getRhyme(13, 14, rhymes, visited, myPoem);

  std::cout << std::endl;
  for (auto ind : myPoem) std::cout << lines[ind] << '\n';
  std::cout << std::endl;

  delete[] oneginPoem;

  return 0;
}
