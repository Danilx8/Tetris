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
enum {EMPTINESS, BLOCK, WALL = 9} elementsCodes;

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
const int GAMESPEED = 20000;

vector<vector<vector<int>>> block_list = {
  {
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 }
  },
  {
    { 0, 0, 1, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 1, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 }
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
  char a;
  cout << " d888b   .d8b.  .88b  d88. d88888b    .d88b.  db    db d88888b d8888b. \n"
       "88' Y8b d8' `8b 88'YbdP`88 88'       .8P  Y8. 88    88 88'     88  `8D \n;"
       "88      88ooo88 88  88  88 88ooooo   88    88 Y8    8P 88ooooo 88oobY' \n"
       "88  ooo 88~~~88 88  88  88 88~~~~~   88    88 `8b  d8' 88~~~~~ 88`8b   \n"
       "88. ~8~ 88   88 88  88  88 88.       `8b  d8'  `8bd8'  88.     88 `88. \n"
       " Y888P  YP   YP YP  YP  YP Y88888P    `Y88P'     YP    Y88888P 88   YD \n";
  cout << "\nPlay again? press y for yes or n for no:\n";
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
  int delay = 1000;

  while (!gameover) {
    auto end = chrono::steady_clock::now();
    auto passedTime = chrono::duration_cast<chrono::milliseconds>(end - start);
    if (kbhit()) {
      userInput();
    }

    if (passedTime.count() >= delay) {
      spawnBlock();
      if (delay != 100) {
        delay -= 1;
      }
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
       "              _|      _|_|_|_|      _|      _|    _|  _|_|_|   _|_|_| \n"
       "\n\t1: Start Game\n\t2: Quit\n\n"
       "#==============================================================================#\n"
       "Choose >> ";
}

void display() {
  setCursorPosition(0, 0);

  for (int i = 0; i < FIELD_HEIGHT; i++) {
    cout << "\t\t";
    for (int j = 0; j < FIELD_WIDTH; j++) {
      switch (field[i][j]) {
        case EMPTINESS:
          cout << " ";
          break;
        case WALL:
          cout << "#";
          break;
        default:
          cout << "@";
          break;
      }
    }
    cout << endl;
  }

  cout << "\nA: left\tS: down\tD: right \t Rotation[Space]";

  if (gameover) {
    system("cls");
    gameOver();
  }
}

void initGame() {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if ((j == 0) || (j == FIELD_WIDTH - 1) || (i == FIELD_HEIGHT - 1)) {
        field[i][j] = stage[i][j] = 9;
      } else {
        field[i][j] = stage[i][j] = 0;
      }
    }
  }

  makeBlocks();

  display();
}

bool makeBlocks() {
  setFigureIntitalPosition();
  srand(time(0));

  int blockType = rand() % 7;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      block[i][j] = 0;
      block[i][j] = block_list[blockType][i][j];
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      field[i][j + 4] = stage[i][j + 4] + block[i][j];

      if (field[i][j + 4] > 1) {
        gameover = true;
        return true;
      }
    }
  }
  return false;
}

void moveBlock(int x2, int y2) {
  //Remove block
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      field[y + i][x + j] -= block[i][j];
    }
  }
  //Update coordinates
  x = x2;
  y = y2;

  // assign a block with the updated value
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      field[y + i][x + j] += block[i][j];
    }
  }

  display();
}

void collidable() {
  int sameElementsCounter;
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    sameElementsCounter = 0;
    for (int j = 0; j < FIELD_WIDTH; j++) {
      stage[i][j] = field[i][j];
      if (stage[i][j] == BLOCK) {
        ++sameElementsCounter;
      }
    }
    if (sameElementsCounter == 10) {
      cleanLine(i);
    }
  }
}

void cleanLine(int lineNumber) {
  for (int i = lineNumber; i > 2; --i) {
    for (int j = 0; j < FIELD_WIDTH; ++j) {
      field[i][j] = field[i-1][j];
      stage[i][j] = stage[i-1][j];
    }
  }
}

bool isCollide(int x2, int y2) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (block[i][j] && stage[y2 + i][x2 + j] != EMPTINESS) {
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

  for (int i = 0; i < 4; i++) {
    //Save temporarily block
    for (int j = 0; j < 4; j++) {
      tmp[i][j] = block[i][j];
    }
  }

  for (int i = 0; i < 4; i++) {
    //Rotate
    for (int j = 0; j < 4; j++) {
      block[i][j] = tmp[3 - j][i];
    }
  }

  if (isCollide(x, y)) {
    // And stop if it overlaps not be rotated
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        block[i][j] = tmp[i][j];
      }
    }
    return true;
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      field[y + i][x + j] -= tmp[i][j];
      field[y + i][x + j] += block[i][j];
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
