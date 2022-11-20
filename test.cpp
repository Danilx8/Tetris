#include <iostream>
using namespace std;

int main() {
  typedef enum Directions {UP, LEFT, DOWN};
  Directions dir;
  dir = Directions::UP;
  cout << dir;
  return 0;
}
