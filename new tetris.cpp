/*************************
*                        *
*     Lugovskih Danil    *
*        PI - 221        *
*         Tetris         *
*                        *
*************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <limits>
#include <chrono>

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

const int PLAYERS_AMOUNT = 5;
const int FIELD_HEIGHT = 22;
const int FIELD_WIDTH = 12;
const int MATRIX_SIZE = 4;

enum {
  EMPTINESS = 0,
  I_BLOCK,
  J_BLOCK,
  Z_BLOCK,
  S_BLOCK,
  T_BLOCK,
  O_BLOCK,
  L_BLOCK,
  WALL = 9
} elementsCodes;

const int I_COLOR = 11;
const int J_COLOR = 1;
const int Z_COLOR = 4;
const int S_COLOR = 10;
const int T_COLOR = 13;
const int O_COLOR = 14;
const int L_COLOR = 6;
const int REGULAR_TEXT_COLOR = 7;
const int WALLS_COLOR = 15;
const int BACKGROUND_COLOR = 0;
const int FAILURE_COLOR = 64;
const int RESULTS_COLOR = 246;
const int LOGO_COLOR = 244;
const int FIRST_SCREEN_INPUT_COLOR = 4;

vector<vector<int>> stage(FIELD_HEIGHT, vector<int>(FIELD_WIDTH, EMPTINESS));
vector<vector<int>> block = {
  { 0, 0, 0, 0 },
  { 0, 0, 0, 0 },
  { 0, 0, 0, 0 },
  { 0, 0, 0, 0 }
};

vector<vector<int>> field(FIELD_HEIGHT, vector<int>(FIELD_WIDTH, EMPTINESS));
int y;
int x;

bool gameover = false;
int GAMESPEED = 1000;
int level = 0;
string username;
int userscore = 0;
int highestScore = 0;
int blockType;

vector<vector<vector<int>>> block_list = {
  {
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 2, 2, 0 },
    { 0, 2, 0, 0 },
    { 0, 2, 0, 0 }
  },
  {
    { 0, 0, 3, 0 },
    { 0, 3, 3, 0 },
    { 0, 3, 0, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 4, 0, 0 },
    { 0, 4, 4, 0 },
    { 0, 0, 4, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 5, 0, 0 },
    { 5, 5, 5, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 6, 6, 0 },
    { 0, 6, 6, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 7, 7, 0 },
    { 0, 0, 7, 0 },
    { 0, 0, 7, 0 }
  }
};

struct leaderboardLine {
  int place;
  string name = "-";
  int score;
} players[PLAYERS_AMOUNT];

int menu();
void openLeaderboard(string, int);
void getHighestScore(fstream&);
void createLeaderboard();
int saveResults(string, int);
void showLeaderboard();
int gameOver();
void title();
void gameLoop();
void display();
bool makeBlocks();
void initializeGame();
void moveBlock(int, int);
void placeOnGround();
bool isOnGround(int, int);
void userInput();
bool rotateBlock();
void spawnBlock();
void setFigureIntitalPosition();
void cleanLine(int);

int main() {
  setFigureIntitalPosition();
  colorize(REGULAR_TEXT_COLOR);
  cout << "Enter your name: ";
  cin >> username;
  menu();
  return 0;
}

void openLeaderboard (string name, int score) {
  fstream readingFile;
  readingFile.open("leaderboard.txt");
  if (readingFile.is_open()) {
    getHighestScore(readingFile);
    readingFile.close();
  } else {
    createLeaderboard();
  }
}

void createLeaderboard() {
  ofstream outfile ("leaderboard.txt");
  for (int playerIndex = 0; playerIndex < PLAYERS_AMOUNT; ++playerIndex) {
    players[playerIndex].place = playerIndex + 1;
    outfile << players[playerIndex].place;
    outfile << ' ';
    outfile << players[playerIndex].name;
    outfile << ' ';
    outfile << players[playerIndex].score;
    outfile << '\n';
  }

  outfile.close();
}

void getHighestScore(fstream& file) {
  int temporaryPlaceVariable;
  string temporaryNameVariable;

  file >> temporaryPlaceVariable;
  file >> temporaryNameVariable;
  file >> highestScore;
}

int saveResults(string currentPlayerName, int currentPlayerScore ) {
  fstream file ("leaderboard.txt");
  for (int playerIndex = 0; playerIndex < PLAYERS_AMOUNT; ++playerIndex) {
    file >> players[playerIndex].place;
    file >> players[playerIndex].name;
    file >> players[playerIndex].score;
  }

  int worsePlayerScore;
  string worsePlayerName;

  for (int playerIndex = 0; playerIndex < PLAYERS_AMOUNT; ++playerIndex) {
    if (currentPlayerScore > players[playerIndex].score) {
      for (int worsePlayerIndex = playerIndex; worsePlayerIndex < PLAYERS_AMOUNT; ++worsePlayerIndex) {
        worsePlayerScore = players[worsePlayerIndex].score;
        worsePlayerName = players[worsePlayerIndex].name;
        players[worsePlayerIndex].score = currentPlayerScore;
        players[worsePlayerIndex].name = currentPlayerName;
        currentPlayerScore = worsePlayerScore;
        currentPlayerName = worsePlayerName;
      }
      file.close();

      fstream input;
      input.open("leaderboard.txt", fstream::out | fstream::trunc);
      for (int playerIndex = 0; playerIndex < PLAYERS_AMOUNT; ++playerIndex) {
        input << '\t';
        input << players[playerIndex].place;
        input << ' ';
        input << players[playerIndex].name;
        input << ' ';
        input << players[playerIndex].score;
        input << '\n';
      }
      input.close();;
      return 1;
    }
  }
  return 0;
}

void showLeaderboard() {
  colorize(RESULTS_COLOR);
  cout << "BEST RESULTS:\n";
  
  for (int playerIndex = 0; playerIndex < PLAYERS_AMOUNT; ++playerIndex) {
    cout << players[playerIndex].place << ' ' << players[playerIndex].name << ' '
         << players[playerIndex].score << endl;
  }
}

int gameOver() {
  saveResults(username, userscore);
  
  colorize(FAILURE_COLOR);
  cout << " d888b   .d8b.  .88b  d88. d88888b    .d88b.  db    db d88888b d8888b. \n"
       "88' Y8b d8' `8b 88'YbdP`88 88'       .8P  Y8. 88    88 88'     88  `8D \n"
       "88      88ooo88 88  88  88 88ooooo   88    88 Y8    8P 88ooooo 88oobY' \n"
       "88  ooo 88~~~88 88  88  88 88~~~~~   88    88 `8b  d8' 88~~~~~ 88`8b   \n"
       "88. ~8~ 88   88 88  88  88 88.       `8b  d8'  `8bd8'  88.     88 `88. \n"
       " Y888P  YP   YP YP  YP  YP Y88888P    `Y88P'     YP    Y88888P 88   YD \n\n";
       
  showLeaderboard();
  cout << "\nPlay again? press y for yes or n for no:\n";
  
  char answer;
  cin >> answer;
  if (answer == 'y' || answer == 'Y') {
    gameover = false;
    userscore = 0;
    system("cls");
    colorize(BACKGROUND_COLOR);
    menu();
  }
  return 0;
}

void gameLoop() {
  system("cls");
  hideCursor();
  initializeGame();
  auto start = chrono::steady_clock::now();

  while (!gameover) {
    auto end = chrono::steady_clock::now();
    auto passedTime = chrono::duration_cast<chrono::milliseconds>(end - start);
    if (kbhit()) {
      userInput();
    }

    if (passedTime.count() >= GAMESPEED) {
      spawnBlock();
      start = chrono::steady_clock::now();
    }
  }
}

void setFigureIntitalPosition() {
  y = 0;
  x = FIELD_WIDTH / 2 - 1;
}

int menu() {
  openLeaderboard(username, userscore);
  title();

  int select_num = 0;

  cin >> select_num;

  switch (select_num) {
    case 1:
      gameLoop();
      break;
    case 2:
      break;
    case 3:
      cout << "See you later!";
      _getch();
      break;
    default:
      cerr << "Choose 1~2~3" << endl;
      _getch();
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(),'\n');
      select_num = 0;
      menu();
      break;
  }
  return 0;
}

void title() {
  system("cls");
  colorize(LOGO_COLOR);
  cout << "#================================================================================#\n"
       "                                                                                  \n"
       "                                                                                  \n"
       "          _|_|_|_|_|  _|_|_|_|  _|_|_|_|_|  _|_|_|    _|_|_|    _|_|_|            \n"
       "              _|      _|            _|      _|    _|    _|    _|                  \n"
       "              _|      _|_|_|        _|      _|_|_|      _|      _|_|              \n"
       "              _|      _|            _|      _|    _|    _|          _|            \n"
       "              _|      _|_|_|_|      _|      _|    _|  _|_|_|   _|_|_|             \n"
       "                                                                                  \n"
       "                                1: Start Game                                     \n"
       "                                2:  Quit                                          \n"
       "                                                                                  \n"
       "Check your keyboard language and caps lock!                                       \n"
       "                                                                                  \n"
       "#================================================================================#\n";
  colorize(FIRST_SCREEN_INPUT_COLOR);
  cout << endl << "Choose >> ";
}

void display() {
  setCursorPosition(0, 0);

  for (int rowIndex = 0; rowIndex < FIELD_HEIGHT; ++rowIndex) {
    cout << "\t\t";
    for (int columnIndex = 0; columnIndex < FIELD_WIDTH; ++columnIndex) {
      switch (field[rowIndex][columnIndex]) {
        case EMPTINESS:
          colorize(BACKGROUND_COLOR);
          cout << " ";
          break;
        case WALL:
          colorize(WALLS_COLOR);
          cout << "#";
          break;
        case I_BLOCK:
          colorize(I_COLOR);
          cout << "@";
          break;
        case J_BLOCK:
          colorize(J_COLOR);
          cout << "@";
          break;
        case Z_BLOCK:
          colorize(Z_COLOR);
          cout << "@";
          break;
        case S_BLOCK:
          colorize(S_COLOR);
          cout << "@";
          break;
        case T_BLOCK:
          colorize(T_COLOR);
          cout << "@";
          break;
        case O_BLOCK:
          colorize(O_COLOR);
          cout << "@";
          break;
        case L_BLOCK:
          colorize(L_COLOR);
          cout << "@";
          break;
        default:
          break;
      }
    }
    cout << endl;
  }

  cout << "\nA: left      S: down     D: right     W: Rotate\n\n";

  cout << "Your score: " << userscore << endl << "Current level: " << level;
  cout << "\n\nHighest score: " << highestScore;

  if (gameover) {
    system("cls");
    gameOver();
  }
}

void initializeGame() {
  for (int rowIndex = 0; rowIndex < FIELD_HEIGHT; ++rowIndex) {
    for (int columnIndex = 0; columnIndex < FIELD_WIDTH; ++columnIndex) {
      if ((columnIndex == 0) || (columnIndex == FIELD_WIDTH - 1) || (rowIndex == FIELD_HEIGHT - 1)) {
        field[rowIndex][columnIndex] = stage[rowIndex][columnIndex] = WALL;
      } else {
        field[rowIndex][columnIndex] = stage[rowIndex][columnIndex] = EMPTINESS;
      }
    }
  }

  makeBlocks();

  display();
}

bool makeBlocks() {
  setFigureIntitalPosition();
  srand(time(0));

  blockType = rand() % 7;

  for (int rowIndex = 0; rowIndex < MATRIX_SIZE; ++rowIndex) {
    for (int columnIndex = 0; columnIndex < MATRIX_SIZE; ++columnIndex) {
      block[rowIndex][columnIndex] = 0;
      block[rowIndex][columnIndex] = block_list[blockType][rowIndex][columnIndex];
    }
  }

  for (int rowIndex = 0; rowIndex < MATRIX_SIZE; ++rowIndex) {
    for (int columnIndex = 0; columnIndex < MATRIX_SIZE; ++columnIndex) {
      field[y + rowIndex][x + columnIndex] = stage[y + rowIndex][x + columnIndex] + block[rowIndex][columnIndex];

      if (isOnGround(x, y)) {
        gameover = true;
        return true;
      }
    }
  }
  return false;
}

void moveBlock(int x2, int y2) {
  for (int rowIndex = 0; rowIndex < MATRIX_SIZE; ++rowIndex) {
    for (int columnIndex = 0; columnIndex < MATRIX_SIZE; ++columnIndex) {
      field[y + rowIndex][x + columnIndex] -= block[rowIndex][columnIndex];
    }
  }

  x = x2;
  y = y2;

  for (int rowIndex = 0; rowIndex < MATRIX_SIZE; ++rowIndex) {
    for (int columnIndex = 0; columnIndex < MATRIX_SIZE; ++columnIndex) {
      field[y + rowIndex][x + columnIndex] += block[rowIndex][columnIndex];
    }
  }
  display();
}

void placeOnGround() {
  int sameElementsCounter;
  for (int rowIndex = 0; rowIndex < FIELD_HEIGHT; ++rowIndex) {
    sameElementsCounter = 0;
    for (int columnIndex = 0; columnIndex < FIELD_WIDTH; ++columnIndex) {
      stage[rowIndex][columnIndex] = field[rowIndex][columnIndex];
      if (stage[rowIndex][columnIndex] != EMPTINESS && stage[rowIndex][columnIndex] != WALL) {
        ++sameElementsCounter;
      }
    }
    if (sameElementsCounter == FIELD_WIDTH - 2) {
      cleanLine(rowIndex);
    }
  }
}

void cleanLine(int lineNumber) {
  for (int rowIndex = lineNumber; rowIndex > 2; --rowIndex) {
    for (int columnIndex = 1; columnIndex < FIELD_WIDTH - 1; ++columnIndex) {
      field[rowIndex][columnIndex] = field[rowIndex-1][columnIndex];
      stage[rowIndex][columnIndex] = stage[rowIndex-1][columnIndex];
    }
  }
  userscore += 100;
  if (userscore % 1000 == 0 && GAMESPEED != 100) {
    ++level;
    GAMESPEED -= 200;
  }
}

bool isOnGround(int x2, int y2) {
  for (int rowIndex = 0; rowIndex < 4; ++rowIndex) {
    for (int columnIndex = 0; columnIndex < 4; ++columnIndex) {
      if (block[rowIndex][columnIndex] && stage[y2 + rowIndex][x2 + columnIndex] != EMPTINESS) {
        return true;
      }
    }
  }
  return false;
}

void userInput() {
  switch (_getch()) {
    case 'd':
      if (!isOnGround(x + 1, y)) {
        moveBlock(x + 1, y);
      }
      break;
    case 'a':
      if (!isOnGround(x - 1, y)) {
        moveBlock(x - 1, y);
      }
      break;
    case 's':
      if (!isOnGround(x, y + 1)) {
        moveBlock(x, y + 1);
      }
      break;
    case 'w':
      rotateBlock();
  }
}

bool rotateBlock() {
  vector<vector<int>> temporaryMatrix(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));

  for (int rowIndex = 0; rowIndex < MATRIX_SIZE; ++rowIndex) {
    for (int columnIndex = 0; columnIndex < MATRIX_SIZE; ++columnIndex) {
      temporaryMatrix[rowIndex][columnIndex] = block[rowIndex][columnIndex];
    }
  }

  for (int rowIndex = 0; rowIndex < MATRIX_SIZE; ++rowIndex) {
    for (int columnIndex = 0; columnIndex < MATRIX_SIZE; ++columnIndex) {
      block[rowIndex][columnIndex] = temporaryMatrix[3 - columnIndex][rowIndex];
    }
  }

  if (isOnGround(x, y)) {
    for (int rowIndex = 0; rowIndex < MATRIX_SIZE; ++rowIndex) {
      for (int columnIndex = 0; columnIndex < MATRIX_SIZE; ++columnIndex) {
        block[rowIndex][columnIndex] = temporaryMatrix[rowIndex][columnIndex];
      }
    }
    return true;
  }

  for (int rowIndex = 0; rowIndex < MATRIX_SIZE; ++rowIndex) {
    for (int columnIndex = 0; columnIndex < MATRIX_SIZE; ++columnIndex) {
      field[y + rowIndex][x + columnIndex] -= temporaryMatrix[rowIndex][columnIndex];
      field[y + rowIndex][x + columnIndex] += block[rowIndex][columnIndex];
    }
  }

  display();

  return false;
}

void spawnBlock() {
  if (!isOnGround(x, y + 1)) {
    moveBlock(x, y + 1);
  } else {
    placeOnGround();
    makeBlocks();
    display();
  }
}
