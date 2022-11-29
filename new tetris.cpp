#include <time.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>

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
// coordinate
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
  cout << " #####     #    #     # ####### ####### #     # ####### ######\n" ;
  cout << "#     #   # #   ##   ## #       #     # #     # #       #     #\n";
  cout << "#        #   #  # # # # #       #     # #     # #       #     #\n";
  cout << "#  #### #     # #  #  # #####   #     # #     # #####   ######\n";
  cout << "#     # ####### #a     # #       #     #  #   #  #       #   #\n";
  cout << "#     # #     # #     # #       #     #   # #   #       #    #\n";
  cout << " #####  #     # #     # ####### #######    #    ####### #     #\n";
  cout << "\n\nPress any key and enter\n";
  cin >> a;
  return 0;
}

void gameLoop() {
  system("cls");
  hideCursor();
  time_t endwait;
  double seconds = 1;
  endwait = time(NULL) + seconds;
  initGame();

  while (!gameover) {
    while (time(NULL) < endwait) {
      if (kbhit()) {
        userInput();
      }
    }

    spawnBlock();
    endwait = time(NULL) + seconds;
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
      select_num = 0;
      main();
      break;
  }
  system("cls");
  return select_num;
}

void title() {
  system("cls");

  cout << "#==============================================================================#\n";

  cout << "####### ####### ####### ######    ###    #####\n";
  cout << "   #    #          #    #     #    #    #     #\n";
  cout << "   #    #          #    #     #    #    #\n";
  cout << "   #    #####      #    ######     #     #####\n";
  cout << "   #    #          #    #   #      #          #\n";
  cout << "   #    #          #    #    #     #    #     #\n";
  cout << "   #    #######    #    #     #   ###    #####\t\tmade for fun \n";
  cout << "\n\n\n\n";

  cout << "\t<Menu>\n";
  cout << "\t1: Start Game\n\t2: Quit\n\n";

  cout << "#==============================================================================#\n";
  cout << "Choose >> ";
}

void display() {
  setCursorPosition(0, 0);

  for (int i = 0; i < FIELD_HEIGHT; i++) {
    cout << "\t\t";
    for (int j = 0; j < FIELD_WIDTH; j++) {
      switch (field[i][j]) {
        case EMPTINESS:
          cout << " " << flush;
          break;
        case WALL:
          cout << "#" << flush;
          break;
        default:
          cout << "@" << flush;
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
        if (stage[i][j] == '@') {
          ++sameElementsCounter;
        }
    }
    if (sameElementsCounter == FIELD_WIDTH - 2) {
      cleanLine(i);
    }
    for (int j = 0; j < FIELD_WIDTH; j++) {
      stage[i][j] = field[i][j];
    }
  }
}

void cleanLine(int lineNumber) {
  for (int i = lineNumber; i > 1; --i) {
    for (int j = 0; j < FIELD_WIDTH; ++j) {
      stage[i][j] = stage[i+1][j];
    }
  }
}

bool isCollide(int x2, int y2) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (block[i][j] && stage[y2 + i][x2 + j] != 0) {
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
