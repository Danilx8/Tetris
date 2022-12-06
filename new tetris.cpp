#include <time.h>
#include <iostream>
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

const int FIELD_HEIGHT = 22;
const int FIELD_WIDTH = 12;
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
const int WALLS_COLOR = 15;
const int BACKGROUND_COLOR = 0;

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
int score = 0;
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

int menu();
int gameOver();
void title();
void gameLoop();
void display();
bool makeBlocks();
void initGame();
void moveBlock(int, int);
void collidable();
bool isCollide(int, int);
void userInput();
bool rotateBlock();
void spawnBlock();
void setFigureIntitalPosition();
void cleanLine(int);

int main() {
  setFigureIntitalPosition();
  menu();
  return 0;
}

int gameOver() {
  cout << " d888b   .d8b.  .88b  d88. d88888b    .d88b.  db    db d88888b d8888b. \n"
       "88' Y8b d8' `8b 88'YbdP`88 88'       .8P  Y8. 88    88 88'     88  `8D \n"
       "88      88ooo88 88  88  88 88ooooo   88    88 Y8    8P 88ooooo 88oobY' \n"
       "88  ooo 88~~~88 88  88  88 88~~~~~   88    88 `8b  d8' 88~~~~~ 88`8b   \n"
       "88. ~8~ 88   88 88  88  88 88.       `8b  d8'  `8bd8'  88.     88 `88. \n"
       " Y888P  YP   YP YP  YP  YP Y88888P    `Y88P'     YP    Y88888P 88   YD \n";
  cout << "\nPlay again? press y for yes or n for no:\n";
  char a;
  cin >> a;
  if (a == 'y' || a == 'Y') {
    gameover = false;
    main();
  }
  return 0;
}

void gameLoop() {
  system("cls");
  hideCursor();
  initGame();
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
  x = FIELD_WIDTH / 3;
}

int menu() {
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
      break;
    default:
      cerr << "Choose 1~2" << endl;
      _getch();
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(),'\n');
      select_num = 0;
      main();
      break;
  }
  system("cls");
  return select_num;
}

void title() {
  system("cls");

  cout << "#==============================================================================#\n\n"
       "          _|_|_|_|_|  _|_|_|_|  _|_|_|_|_|  _|_|_|    _|_|_|    _|_|_| \n"
       "              _|      _|            _|      _|    _|    _|    _| \n"
       "              _|      _|_|_|        _|      _|_|_|      _|      _|_| \n"
       "              _|      _|            _|      _|    _|    _|          _| \n"
       "              _|      _|_|_|_|      _|      _|    _|  _|_|_|   _|_|_| \n\n"
       "                                1: Start Game\n"
       "                                2:  Quit\n\n"
       "Check your keyboard language and caps lock!\n"
       "#==============================================================================#\n"
       "Choose >> ";
}

void display() {
  setCursorPosition(0, 0);

  for (int rowIndex = 0; rowIndex < FIELD_HEIGHT; rowIndex++) {
    cout << "\t\t";
    for (int columnIndex = 0; columnIndex < FIELD_WIDTH; columnIndex++) {
      switch (field[rowIndex][columnIndex]) {
        case EMPTINESS:
          /*if (rowIndex > y + 1 && columnIndex == x + 1) {
            colorize(119);
          } else {
            colorize(BACKGROUND_COLOR);
          }*/
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

  cout << "\nA: left      S: down     D: right     W: Rotate";

  if (gameover) {
    system("cls");
    gameOver();
  }
}

void initGame() {
  for (int rowIndex = 0; rowIndex < FIELD_HEIGHT; rowIndex++) {
    for (int columnIndex = 0; columnIndex < FIELD_WIDTH; columnIndex++) {
      if ((columnIndex == 0) || (columnIndex == FIELD_WIDTH - 1) || (rowIndex == FIELD_HEIGHT - 1)) {
        field[rowIndex][columnIndex] = stage[rowIndex][columnIndex] = 9;
      } else {
        field[rowIndex][columnIndex] = stage[rowIndex][columnIndex] = 0;
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

  for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
    for (int columnIndex = 0; columnIndex < 4; columnIndex++) {
      block[rowIndex][columnIndex] = 0;
      block[rowIndex][columnIndex] = block_list[blockType][rowIndex][columnIndex];
    }
  }

  for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
    for (int columnIndex = 0; columnIndex < 4; columnIndex++) {
      field[rowIndex][columnIndex + 4] = stage[rowIndex][columnIndex + 4] + block[rowIndex][columnIndex];

      if (field[rowIndex][columnIndex + 4] > 6) {
        gameover = true;
        return true;
      }
    }
  }
  return false;
}

void moveBlock(int x2, int y2) {
  //Remove block
  for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
    for (int columnIndex = 0; columnIndex < 4; columnIndex++) {
      field[y + rowIndex][x + columnIndex] -= block[rowIndex][columnIndex];
    }
  }
  //Update coordinates
  x = x2;
  y = y2;

  // assign a block with the updated value
  for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
    for (int columnIndex = 0; columnIndex < 4; columnIndex++) {
      field[y + rowIndex][x + columnIndex] += block[rowIndex][columnIndex];
    }
  }

  display();
}

void collidable() {
  int sameElementsCounter;
  for (int rowIndex = 0; rowIndex < FIELD_HEIGHT; rowIndex++) {
    sameElementsCounter = 0;
    for (int columnIndex = 0; columnIndex < FIELD_WIDTH; columnIndex++) {
      stage[rowIndex][columnIndex] = field[rowIndex][columnIndex];
      if (stage[rowIndex][columnIndex] != EMPTINESS && stage[rowIndex][columnIndex] != WALL) {
        ++sameElementsCounter;
      }
    }
    if (sameElementsCounter == 10) {
      cleanLine(rowIndex);
    }
  }
}

void cleanLine(int lineNumber) {
  for (int rowIndex = lineNumber; rowIndex > 2; --rowIndex) {
    for (int columnIndex = 0; columnIndex < FIELD_WIDTH; ++columnIndex) {
      field[rowIndex][columnIndex] = field[rowIndex-1][columnIndex];
      stage[rowIndex][columnIndex] = stage[rowIndex-1][columnIndex];
    }
  }
  score += 10;
  if (/*score % 100 == 0 &&*/ GAMESPEED != 100) {
    ++level;
    GAMESPEED -= 50;
  }
}

bool isCollide(int x2, int y2) {
  for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
    for (int columnIndex = 0; columnIndex < 4; columnIndex++) {
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
      if (!isCollide(x + 1, y)) {
        moveBlock(x + 1, y);
      }
      break;
    case 'a':
      if (!isCollide(x - 1, y)) {
        moveBlock(x - 1, y);
      }
      break;
    case 's':
      if (!isCollide(x, y + 1)) {
        moveBlock(x, y + 1);
      }
      break;
    case 'w':
      rotateBlock();
  }
}

bool rotateBlock() {
  vector<vector<int>> tmp(4, vector<int>(4, 0));

  for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
    //Save temporarily block
    for (int columnIndex = 0; columnIndex < 4; columnIndex++) {
      tmp[rowIndex][columnIndex] = block[rowIndex][columnIndex];
    }
  }

  for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
    //Rotate
    for (int columnIndex = 0; columnIndex < 4; columnIndex++) {
      block[rowIndex][columnIndex] = tmp[3 - columnIndex][rowIndex];
    }
  }

  if (isCollide(x, y)) {
    // And stop if it overlaps not be rotated
    for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
      for (int columnIndex = 0; columnIndex < 4; columnIndex++) {
        block[rowIndex][columnIndex] = tmp[rowIndex][columnIndex];
      }
    }
    return true;
  }

  for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
    for (int columnIndex = 0; columnIndex < 4; columnIndex++) {
      field[y + rowIndex][x + columnIndex] -= tmp[rowIndex][columnIndex];
      field[y + rowIndex][x + columnIndex] += block[rowIndex][columnIndex];
    }
  }

  display();

  return false;
}

void spawnBlock() {
  if (!isCollide(x, y + 1)) {
    moveBlock(x, y + 1);
  } else {
    collidable();
    makeBlocks();
    display();
  }
}
