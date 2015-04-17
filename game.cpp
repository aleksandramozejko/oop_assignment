#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
using namespace std;

#include "game.h"

bool charToDir(char c, Direction& k) {
  switch (c) {
    case 'R':
    case 'r': k = RIGHT; break;
    case 'D':
    case 'd': k = DOWN; break;
    case 'L':
    case 'l': k = LEFT; break;
    case 'U':
    case 'u': k = UP; break;
    default: return false;
  }
  return true;
}

int selectRand(int a, int b) {
  if (a == b) return a;
  return rand() % (b - a + 1) + a;
}

Game::Game(Board* board_):
  board(board_) {
  board->connectWithGame(this);
}

bool Game::computer() {
  actionPoints = pointsForRound;

  int cost;
  while ((cost = board->compMove(actionPoints)) > 0) {
    actionPoints -= cost;
  }
  printStatus();

  return true;
}

bool Game::player() {
  actionPoints = pointsForRound;

  char command[100];

  do {
    if (board->bilboIsDead() || board->demCaught())
      return false;

    printInteractions();

    if (!cin.getline(command, 100))
      return false;

  } while(handleCommand(string(command)));

  return true;
}

void Game::printInteractions() {
  board->printBoard(Board::SHOW_OPPONENTS);
  printStatus();
  cout << "> ";
}

bool Game::handleCommand(const string& command) {
  stringstream ss(command);

  string order;
  if (!(ss >> order)) {
    status << "Wrong order." << endl;
    return true;
  }

  if (order == "move") {
    int x, y;
    char kchar;
    if (!(ss >> x >> y >> kchar)) {
      status << "Wrong move parameters." << endl;
      return true;
    }
    Direction kier;
    if (!charToDir(kchar, kier)) {
      status << "Wrong direction." << endl;
      return true;
    }

    int cost;
    if ((cost = board->playerMove(x, y, kier, actionPoints)) > 0) {
      status << "You have moved!" << endl;
      status << "Move cost: " << cost << endl;
      actionPoints -= cost;
      status << "Action points left: " << actionPoints << endl;
    } else {
      status << "Sorry, you can't move." << endl;
    }
  } else if (order == "stat") {
    int x, y;
    if (!(ss >> x >> y)) {
      status << "Wrong stat parameters." << endl;
      return true;
    }
    int pkt;
    if (board->giveStats(x, y, pkt)) {
      status << "Strength points of a character (" << x << ", " << y << "): " << pkt << endl;
    } else {
      status << "No character on this field." << endl;
    }
  } else if (order == "end") {
    return false;
  } else {
    status << "Wrong order: " + order << endl;
  }
  return true;
}

int Game::whoWon(int a, int b) {
  /* przyjmujemy zalozenie, ze a < b */
  double ratio = a / ((double) (a + b));
  ratio = ratio * ratio;
  int sample = rand();
  return (sample <= (int) floor(ratio * RAND_MAX)) ? 1 : 2;
}

int Game::damage(Character* a, Character* b) {
  /* przyjmujemy zalozenie, ze a wygral */
  int aStrength = a->giveStrPoints();
  int bStrength = b->giveStrPoints();

  int pkt;
  if (aStrength < bStrength) {
    int q = aStrength*bStrength / (bStrength + 30);
    pkt = selectRand(q/2, q);
  } else {
    pkt = selectRand(bStrength/4, 3*bStrength/4);
  }
  a->damage(pkt);
  return pkt;
}

int Game::fight(Character* a, Character* b,
                         int x1, int y1, int x2, int y2) {
  int aStrength = a->giveStrPoints();
  int bStrength = b->giveStrPoints();

  if (aStrength > bStrength)
    return (3 - fight(b, a, x2, y2, x1, y1));

  int winner = whoWon(aStrength, bStrength);

  /* Przygotowywanie współrzędnych graczy. */
  stringstream ss;
  string playerW, playerP;
  ss << "(" << x1 << "," << y1 << ") ";
  ss << "(" << x2 << "," << y2 << ") ";
  /* ************************************* */

  int dam, left;
  if (winner == 1) {
    dam = damage(a, b);
    left = a->giveStrPoints();
    ss >> playerW >> playerP;
  } else {
    dam = damage(b, a);
    left = b->giveStrPoints();
    ss >> playerP >> playerW;
  }

  status << "There was a fight: " << playerW << " won with " << playerP << endl;
  status << "Character " << playerW << " lost " << dam
         << " strength points and has " << left << " points left." << endl;

  return winner;
}

void Game::printStatus() {
  // Niestety operator >> dla strumieni stringstream nie jest spójny z
  // operatorem >> strumieni plikowych - w przypadku stringstream nie można po
  // prostu napisać
  //
  // while (status >> c) { ... }
  //
  // ponieważ warunek "status >> c" ewaluuje się zawsze do false. Stąd
  // konieczne jest zastosowanie poniższej metody sprawdzania niepustości
  // strumienia.
  while (status.rdbuf()->in_avail() > 0) {
    char c;
    status.get(c);
    cout << c;
  }
}

