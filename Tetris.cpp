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

enum {STICK, GAMMA, ALPHA, SQUARE, SLETTER, TRIANGLE, ZLETTER} tetrominoKind;
void tetrominoSpawn() {
  srand(time(0));
  int figurePicker = rand() % 8;
  switch (figurePicker) {
    case STICK:
      gameStick(gameArea, stickAreaSize)
      break;
      case GAMMA:

        break;
      case ALPHA:

        break;
      case SQUARE:

        break;
      case SLETTER:

        break;
      case TRIANGLE:

        break;
      case ZLETTER:

        break;
    default:
      cout << "Random doesn't work";
  }
}*/

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

    int getColumn() {
      return height;
    }

    int getWidth() {
      return width;
    }

    void print() {
      setCursorPosition(0,0);
      for (int rowIndex = 0; rowIndex < height; ++rowIndex) {
        for (int columnIndex = 0; columnIndex < width ; ++columnIndex) {
          cout << currentField[rowIndex][columnIndex];
        }
        cout << endl;
      }
    }

    void spawn(int rowIndex, int columnIndex, char character) {
      currentField[rowIndex][columnIndex] = character;
    }

    void clear() {
      setCursorPosition(0, 0);
      for(int rowIndex = 1; rowIndex < height - 1; ++rowIndex) {
        for(int columnIndex = 1; columnIndex < width - 1; ++columnIndex) {
          currentField[rowIndex][columnIndex] = ' ';
        }
      }
    }

    void clearLine(int doneLineIndex) {
      for (int lineIndex = doneLineIndex; lineIndex > 1; --lineIndex) {
        for (int elementIndex = 0; elementIndex < width; ++elementIndex) {
          currentField[lineIndex][elementIndex] = currentField[lineIndex - 1][elementIndex];
        }
      }
    }

    char getElement(int rowCoordinate, int columnCoordinate) {
      return currentField[rowCoordinate][columnCoordinate];
    }
} gameField;

const int field::height = 30;
const int field::width = 25;
const int stickAreaSize = 4;

class tetromino {
  public:
    coordinates mainSymbolPosition[100];
    coordinates secondarySymbolsPosition[100];
    int speed = 1;
    char symbol = '+';
    coordinates& mainSymbol;
    coordinates secondarySymbol[3];
    int areaSize;
    char** tetrominoArea;
    enum {ABOVE, RSIDE, UNDER, LSIDE} state;
    enum {CHANGESTATE, LEFT, RIGHT, DOWN} direction;

    tetromino(field& currentField, int figureAreaSize):
      mainSymbolPosition(),
      secondarySymbolsPosition(),
      state(ABOVE),
      direction(DOWN),
      mainSymbol(mainSymbolPosition[0]),
      areaSize(figureAreaSize) {
      tetrominoArea = new char*[areaSize];
      for (int rowIndex = 0; rowIndex < areaSize; ++rowIndex) {
        tetrominoArea[rowIndex] = new char[areaSize];
      }

      mainSymbolPosition[0].rowIndex = 3;
      mainSymbolPosition[0].columnIndex = currentField.getWidth() / 2;
    }

    int getColumnIndex() {
      return mainSymbol.columnIndex;
    }

    int getRowIndex() {
      return mainSymbol.rowIndex;
    }

    void tetrominoSpawn(field& currentField) {
      currentField.spawn(mainSymbol.rowIndex, mainSymbol.columnIndex, symbol);
      for (int secondarySymbolIndex = 0; secondarySymbolIndex < 3; ++secondarySymbolIndex) {
        currentField.spawn(secondarySymbol[secondarySymbolIndex].rowIndex,
                           secondarySymbol[secondarySymbolIndex].columnIndex, symbol);
      }
    }

    void stateChanger(field& currentField) {
      switch(state) {
        case ABOVE:
          mainSymbol.rowIndex += 1;
          mainSymbol.columnIndex += 1;
          state = RSIDE;
          break;
        case RSIDE:
          if (!(currentField.getElement(mainSymbol.rowIndex, mainSymbol.columnIndex - 3) == '#' ||
                currentField.getElement(mainSymbol.rowIndex, mainSymbol.columnIndex - 3) == '*')) {
            mainSymbol.rowIndex += 1;
            mainSymbol.columnIndex -= 1;
            state = UNDER;
          }
          break;
        case UNDER:
          if (!((mainSymbol.columnIndex > currentField.getWidth()) || (mainSymbol.columnIndex <= 3))) {
            mainSymbol.rowIndex -= 1;
            mainSymbol.columnIndex -= 1;
            state = LSIDE;
          }
          break;
        case LSIDE:
          mainSymbol.rowIndex -= 1;
          mainSymbol.columnIndex += 1;
          state = ABOVE;
          break;
        default:
          break;
      }
    }

    void move(field& currentField) {
      coordinates nextPosition = {0, 0};
      if (_kbhit()) {
        switch (_getch()) {
          case 'w':
            direction = CHANGESTATE;
            break;
          case 's':
            direction = DOWN;
            break;
          case 'a':
            direction = LEFT;
            break;
          case 'd':
            direction = RIGHT;
            break;
        }
        switch(direction) {
          case CHANGESTATE:
            stateChanger(gameField);
            break;
          case DOWN:
            nextPosition.rowIndex += speed;
            for (int secondarySymbolsIndex = 0; secondarySymbolsIndex < 3; ++secondarySymbolsIndex) {
              secondarySymbol[secondarySymbolsIndex].rowIndex += speed;
            }
            break;
          case LEFT:
            if (!(currentField.getElement(secondarySymbol[2].rowIndex, secondarySymbol[2].columnIndex - 1) == '#' ||
                  currentField.getElement(secondarySymbol[2].rowIndex, secondarySymbol[2].columnIndex - 1) == '*') ) {
              nextPosition.columnIndex -= speed;
              for (int secondarySymbolsIndex = 0; secondarySymbolsIndex < 3; ++secondarySymbolsIndex) {
                secondarySymbol[secondarySymbolsIndex].columnIndex -= speed;
              }
            }
            break;
          case RIGHT:
            if (!(currentField.getElement(secondarySymbol[0].rowIndex, secondarySymbol[0].columnIndex + 1) == '#' ||
                  currentField.getElement(secondarySymbol[2].rowIndex, secondarySymbol[2].columnIndex + 1) == '*') ) {
              nextPosition.columnIndex += speed;
              for (int secondarySymbolsIndex = 0; secondarySymbolsIndex < 3; ++secondarySymbolsIndex) {
                secondarySymbol[secondarySymbolsIndex].columnIndex += speed;
              }
            }
            break;
          default:
            break;
        }

        mainSymbol.columnIndex += nextPosition.columnIndex;
        mainSymbol.rowIndex += nextPosition.rowIndex;
      }
    }

    void tetrominoSink() {
      mainSymbol.rowIndex += 1;
      for (int secondarySymbolIndex = 0; secondarySymbolIndex < 3; ++secondarySymbolIndex) {
        ++secondarySymbol[secondarySymbolIndex].rowIndex;
      }
    }
};

class stick: public tetromino {
  public:
    stick(field& currentField, const int areaSize): tetromino(currentField, areaSize) {}

    void secondarySymbolsConcatenation () {
      switch(state) {
        case ABOVE:
          for (int secondarySymbolIndex = 0; secondarySymbolIndex < 3; ++secondarySymbolIndex) {
            secondarySymbol[secondarySymbolIndex].rowIndex = mainSymbol.rowIndex;
          }
          secondarySymbol[0].columnIndex = mainSymbol.columnIndex + 1;
          secondarySymbol[1].columnIndex = mainSymbol.columnIndex - 1;
          secondarySymbol[2].columnIndex = mainSymbol.columnIndex - 2;
          break;
        case RSIDE:
          for (int secondarySymbolIndex = 0; secondarySymbolIndex < 3; ++secondarySymbolIndex) {
            secondarySymbol[secondarySymbolIndex].columnIndex = mainSymbol.columnIndex;
          }
          secondarySymbol[0].rowIndex = mainSymbol.rowIndex + 1;
          secondarySymbol[1].rowIndex = mainSymbol.rowIndex - 1;
          secondarySymbol[2].rowIndex = mainSymbol.rowIndex - 2;
          break;
        case UNDER:
          for (int secondarySymbolIndex = 0; secondarySymbolIndex < 3; ++secondarySymbolIndex) {
            secondarySymbol[secondarySymbolIndex].rowIndex = mainSymbol.rowIndex;
          }
          secondarySymbol[0].columnIndex = mainSymbol.columnIndex + 1;
          secondarySymbol[1].columnIndex = mainSymbol.columnIndex - 1;
          secondarySymbol[2].columnIndex = mainSymbol.columnIndex - 2;
          break;
        case LSIDE:
          for (int secondarySymbolIndex = 0; secondarySymbolIndex < 3; ++secondarySymbolIndex) {
            secondarySymbol[secondarySymbolIndex].columnIndex = mainSymbol.columnIndex;
          }
          secondarySymbol[0].rowIndex = mainSymbol.rowIndex + 1;
          secondarySymbol[1].rowIndex = mainSymbol.rowIndex - 1;
          secondarySymbol[2].rowIndex = mainSymbol.rowIndex - 2;
          break;
      }
    }
} gameStick(gameField, stickAreaSize);
/*
class stick: public tetromino {
  public:
    stick(field& currentField, const int areaSize): tetromino(currentField, areaSize) {
    }
} gameStick(gameField, stickAreaSize);

class stick: public tetromino {
  public:
    stick(field& currentField, const int areaSize): tetromino(currentField, areaSize) {
    }
} gameStick(gameField, stickAreaSize);

class stick: public tetromino {
  public:
    stick(field& currentField, const int areaSize): tetromino(currentField, areaSize) {
    }
} gameStick(gameField, stickAreaSize);

class stick: public tetromino {
  public:
    stick(field& currentField, const int areaSize): tetromino(currentField, areaSize) {
    }
} gameStick(gameField, stickAreaSize);

class stick: public tetromino {
  public:
    stick(field& currentField, const int areaSize): tetromino(currentField, areaSize) {
    }
} gameStick(gameField, stickAreaSize);

class stick: public tetromino {
  public:
    stick(field& currentField, const int areaSize): tetromino(currentField, areaSize) {
    }
} gameStick(gameField, stickAreaSize);
*/

int main() {
  hideCursor();
  while (true) {
    gameField.print();

    time_t endwait;
    int seconds = 2;
    endwait = time(NULL) + seconds;
    while (time(NULL) < endwait) {
      gameStick.move(gameField);
      gameStick.secondarySymbolsConcatenation();
      gameStick.tetrominoSpawn(gameField);
      gameField.print();
      gameField.clear();
    }

    //gameStick.tetrominoSink();
  }
  return 0;
}
