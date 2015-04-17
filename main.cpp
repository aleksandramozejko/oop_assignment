#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;

#include "game.h"


int main(int argc, char* argv[]) {
  // Inicjalizacja generatora liczb pseudolosowych.
  srand(time(NULL));

  char* fil = NULL;

  // Parsowanie argumentów z linii poleceń.
  for (int i = 1; i < argc; i++) {
    if (string(argv[i]) == "--show_opponents")
      Board::SHOW_OPPONENTS = true;
    else if (fil != NULL) {
      cout << "Usage: " << argv[0] <<
        " [--show_opponents] board_file" << endl;
      return 1;
    } else {
      fil = argv[i];
    }
  }

  if (fil == NULL) {
      cout << "Usage: " << argv[0] <<
        " [--show_opponents] board_file" << endl;
      return 1;
  }

  ifstream fin(fil);
  if (!fin.is_open()) {
    cout << "No file." << endl;
    return 1;
  }

  Board board;
  if (!board.load(fin)) {
    cout << "Error .\n";
    return 1;
  }
  fin.close();

  if (!board.verify()) {
    return 1;
  }

  Game game(&board);

  // Właściwa pętla odpowiadająca za realizację gry.
  while (game.player() && game.computer());

  board.printBoard(true); //wypisz
  
  if (board.bilboIsDead())
    cout << "The end. You lost! :(" << endl;
  else if (board.demCaught())
    cout << "The end. You won! x)" << endl;
  else
    cout << "The end. Probably because of pressing CTRL+D." << endl;
}
