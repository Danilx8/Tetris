/*************************
*                        *
*     Lugovskih Danil    *
*        PI - 221        *
*        Tetris          *
*                        *
*************************/

#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <time.h>
using namespace std;


void colorize(int colorNumber) {
  HANDLE hConsole;
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, colorNumber);
}

void setCursorPosition(int x, int y) {
  static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  cout.flush();
  COORD coord = { (SHORT)x, (SHORT)y };
  SetConsoleCursorPosition(hOut, coord);
}

void hideCursor() {
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 100;
  info.bVisible = FALSE;
  SetConsoleCursorInfo(consoleHandle, &info);
}

/*string userName = "-";
int score = 0;

struct leaderboardLine {
  int place;
  string name = "-";
  int score;
} players[10];

void createLeaderboard(string currentPlayerName, int currentPlayerScore) {
  ofstream outfile ("leaderboard.txt");
  players[0].name = currentPlayerName;
  players[0].score = currentPlayerScore;

  outfile << players[0].place;
  outfile << ' ';
  outfile << players[0].name;
  outfile << ' ';
  outfile << players[0].score;
  outfile << '\n';
  outfile.close();
}

int fileParsing(fstream& file, string currentPlayerName, int currentPlayerScore ) {
  for (int playerIndex = 0; playerIndex < 10; ++playerIndex) {
    file >> players[playerIndex].place;
    file >> players[playerIndex].name;
    file >> players[playerIndex].score;
  }

  int worsePlayerScore;
  string worsePlayerName;
  for (int playerIndex = 0; playerIndex < 10; ++playerIndex) {
    if (currentPlayerScore > players[playerIndex].score) {
      for (int worsePlayerIndex = playerIndex; worsePlayerIndex < 10; ++worsePlayerIndex) {
        worsePlayerScore = players[worsePlayerIndex].score;
        worsePlayerName = players[worsePlayerIndex].name;
        players[worsePlayerIndex].score = currentPlayerScore;
        players[worsePlayerIndex].name = currentPlayerName;
        currentPlayerScore = worsePlayerScore;
        currentPlayerName = worsePlayerName;
      }
      return 1;
    }
  }
  return 0;
}

void leaderboardBuilder() {
  fstream output;
  output.open("leaderboard.txt", fstream::out | fstream::trunc);
  for (int playerIndex = 0; playerIndex < 10; ++playerIndex) {
    output << '\t';
    output << players[playerIndex].place;
    output << ' ';
    output << players[playerIndex].name;
    output << ' ';
    output << players[playerIndex].score;
    output << '\n';
  }
  output.close();;
}

void leaderboard(string name, int score) {
  fstream readingFile;
  for (int playerIndex = 0; playerIndex < 10; ++playerIndex) {
    players[playerIndex].place = playerIndex + 1;
  }
  readingFile.open("leaderboard.txt");
  if (readingFile.is_open()) {
    fileParsing(readingFile, name, score);
    readingFile.close();
  } else {
    createLeaderboard(name, score);
  }

  leaderboardBuilder();
  readingFile.open("leaderboard.txt");
  colorize(14);
  cout << "\n\tBEST SCORES\n";
  cout << readingFile.rdbuf();
  readingFile.close();
}

void getHighestScore() {
  fstream file;
  file.open("leaderboard.txt");
  fileParsing(file, "-", 0);
  file.close();
}
*/
struct coordinates {
  int rowIndex, columnIndex;
};

class field {
  private:
    static const int height, width;
    char ** currentField;

  public:
    field() {
      currentField = new char* [height];
      for(int rowIndex = 0; rowIndex < height; ++rowIndex) {
        currentField[rowIndex] = new char[width];
      }

      for (int rowIndex = 0; rowIndex < height ; ++rowIndex) {
        if (rowIndex == 0 || rowIndex == height - 1) {
          for (int columnIndex = 0; columnIndex < width; ++columnIndex) {
            currentField[rowIndex][columnIndex] = '#';
          }
        } else {
          currentField[rowIndex][0] = '#';
          currentField[rowIndex][width - 1] = '#';
        }
      }
    }

    void print(char** previousField) {
      for (int rowIndex = 1; rowIndex < height - 1; ++rowIndex) {
        for (int columnIndex = 1; columnIndex < width - 1; ++columnIndex) {
          if (currentField[rowIndex][columnIndex] == previousField[rowIndex][columnIndex]) {
            continue;
          }
          cout << currentField[rowIndex][columnIndex];
        }
        cout << endl;
      }
    }
    
    void clear() {
      setCursorPosition(0, 0);
      for(int rowIndex = 1; rowIndex < height - 1; ++rowIndex) {
        for(int columnIndex = 1; columnIndex < width - 1; ++columnIndex) {
          currentField[rowIndex][columnIndex] = ' ';
        }
      }
    }
} gameField;

class tetromino {
  public:
    coordinates mainSymbolPosition[100];
    int speed = 1;
    char symbol = '+';
    coordinates& mainSymbol;
    static int areaSize;
    char** tetrominoArea;
    enum {ABOVE, RSIDE, UNDER, LSIDE} state;
    enum {CHANGESTATE, LEFT, RIGHT, DOWN} direction;

    tetromino(field& fieldData):
      state(ABOVE),
      direction(DOWN),
      mainSymbol(mainSymbolPosition[0]) {
        tetrominoArea = new char*[areaSize];
        for (int rowIndex = 0; rowIndex < areaSize; ++rowIndex) {
          tetrominoArea[rowIndex] = new char[areaSize];
        }
       }

    void getInput(const field& currentField) {
      if (GetAsyncKeyState(VK_UP)) {
        direction = CHANGESTATE;
      }
      if (GetAsyncKeyState(VK_DOWN)) {
        direction = DOWN;
      }
      if (GetAsyncKeyState(VK_LEFT)) {
        direction = LEFT;
      }
      if (GetAsyncKeyState(VK_RIGHT)) {
        direction = RIGHT;
      }
    }

    void move(const field& currentField) {
      coordinates nextPosition = {0, 0};
      switch(direction) {
        case CHANGESTATE:
          switch (state) {
            case ABOVE:
              state = RSIDE;
              break;
            case RSIDE:
              state = UNDER;
              break;
            case UNDER:
              state = LSIDE;
              break;
            case LSIDE:
              state = ABOVE;
              break;
            default:
              break;
          }
          break;
        case DOWN:
          nextPosition.rowIndex += speed;
          break;
        case LEFT:
          nextPosition.columnIndex -= speed;
          break;
        case RIGHT:
          nextPosition.columnIndex += speed;
          break;
      }
      mainSymbol.rowIndex += nextPosition.rowIndex + 1;
      mainSymbol.columnIndex += nextPosition.columnIndex;
    }
};

const int field::height = 30;
const int field::width = 25;

int main() {
  hideCursor();
  cout << "hello world";
  return 0;
}
