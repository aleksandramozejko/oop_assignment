#include <iostream>
#include <cstdlib>
using namespace std;

#include "characters.h"
#include "board.h"
#include "game.h"

bool Board::SHOW_OPPONENTS = false;

bool charInString(char c, const char* s) {
  for (int i = 0; s[i] != '\0'; i++)
    if (c == s[i])
      return true;
  return false;
}

//////////////////////////////////////////////////
// Field

void Field::print() {
  if (!seen) {
    cout << '#';
    return;
  }
  switch (typ) {
    case FLAT:
      cout << 'p';
      break;
    case FOREST:
      cout << 'l';
      break;
    case ROAD:
      cout << 'd';
      break;
    case MOUNTAINS:
      cout << 's';
      break;
    case RIVER:
      cout << 'r';
      break;
    case BRIDGE:
      cout << 'm';
      break;
    case OCEAN:
      cout << 'o';
      break;
    case ILE_TYP:
      break;
  }
}

Field::Field():
  seen(false) {}

bool Field::visible() {
  return seen;
}

void Field::see() {
  seen = true;
}

void Field::initialize(char* fieldDef) {
  switch (fieldDef[0]) {
    case 'p': typ = FLAT; break;
    case 'l': typ = FOREST; break;
    case 'd': typ = ROAD; break;
    case 's': typ = MOUNTAINS; break;
    case 'r': typ = RIVER; break;
    case 'm': typ = BRIDGE; break;
    case 'o': typ = OCEAN; break;
  }
  seen = false;
}

Field::Type Field::giveType() {
  return typ;
}

void Field::setType(Type typ_) {
  typ = typ_;
}

//////////////////////////////////////////////////
// Board

Board::Board() {}

Board::~Board() {
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      if (characters[y][x])
        delete characters[y][x];
}

bool Board::load(istream& in) {
  if (!(in >> height >> width))
    return false;

  fields = vector< vector<Field> >(height, vector<Field>(width));
  characters = vector< vector<Character*> >(height, vector<Character*>(width, (Character*) NULL));

  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++) {
      char fieldDef[2];
      if (!loadField(in, fieldDef))
        return false;
      fields[y][x].initialize(fieldDef);
      characters[y][x] = Character::create(fieldDef);
    }

  return true;
}

bool Board::loadField(istream& in, char* fieldDef) {
  char c;

  while (true) {
    if (!in.get(c))
      return false;
    if (charInString(c, FIELD_CHARS)) {
      fieldDef[0] = c;
      break;
    } else if (!(c == ' ' || c == '\n')) {
      return false;
    }
  }

  if (!in.get(c))
    return false;
  if (charInString(c, CHARACTER_CHARS)) {
    fieldDef[1] = c;
  } else {
    return false;
  }

  return true;
}

bool Board::verify() {
  bool isBilbo, isDragon;
  isBilbo = isDragon = false;

  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      if (characters[y][x]) {
        if (characters[y][x]->hero())
          isBilbo = true;
        if (characters[y][x]->dragon())
          isDragon = true;

        // Weryfikacja, czy na na polu o danym terenie może stać analizowana
        // postać.
        Field::Type fieldType = fields[y][x].giveType();
        if (!(characters[y][x]->dragon() || (
                (fieldType == Field::FLAT ||
                 fieldType == Field::ROAD ||
                 fieldType == Field::MOUNTAINS) &&
                characters[y][x]->possibleField(fieldType)))) {
          cout << "Character (" << x << ", " << y << ") stands on a " <<
            "wrong field type." << endl;
          return false;
        }
      }

  if (!isBilbo)
    cout << "No hero on board." << endl;
  if (!isDragon)
    cout << "No dragon on board." << endl;

  return isBilbo && isDragon;
}

void Board::printChar(int n, char c) {
  cout << string(n, c);
}

void Board::printNum(int x) {
  cout << x;
  if (x < 10) cout << " ";
}

void Board::printHorizontalLine() {
  cout << "  +";
  printChar(width*2, '-');
  cout << "+\n";
}

void Board::printCharacter(int x, int y) {
  if (characters[y][x] == NULL)
    cout << " ";
  else
    cout << characters[y][x]->giveChar();
}

void Board::genVisibleFields(vector< vector<bool> >& visibleFields) {
  visibleFields = vector< vector<bool> >(height, vector<bool>(width, false));
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      if (characters[y][x] && characters[y][x]->collegue())
        for (int yn = y-1; yn <= y+1; yn++)
          if (yn>=0 && yn<height)
            for (int xn = x-1; xn <= x+1; xn++)
              if (xn>=0 && xn<width) {
                visibleFields[yn][xn] = true;
                fields[yn][xn].see();
              }
}

void Board::printBoard(bool showOpponents) {
  vector< vector<bool> > visibleFields;
  genVisibleFields(visibleFields);

  printChar(3, ' ');
  for (int i = 0; i < width; i++)
    printNum(i);
  cout << "\n";

  printHorizontalLine();

  for (int y = 0; y < height; y++) {
    printNum(y);
    cout << "|";
    for (int x = 0; x < width; x++) {
      fields[y][x].print();
      if (visibleFields[y][x] || showOpponents)
        printCharacter(x, y);
      else
        cout << "#";
    }
    cout << "|\n";
  }

  printHorizontalLine();
}


bool Board::pointInDir(int x, int y, Direction k, int &nx, int &ny) {
  nx = x;
  ny = y;
  switch (k) {
    case RIGHT: nx++; break;
    case DOWN: ny++; break;
    case LEFT: nx--; break;
    case UP: ny--; break;
  }
  return nx>=0 && ny>=0 && nx<width && ny<height;
}

void Board::doFight(int x, int y, int nx, int ny) {
  int result = game->fight(characters[y][x], characters[ny][nx],
                                    x, y, nx, ny);
  if (result == 1) {
    delete characters[ny][nx];
    characters[ny][nx] = NULL;
  } else {
    delete characters[y][x];
    characters[y][x] = NULL;
  }
}

int Board::movePossible(int x, int y, Direction k, int pointsPool,
                          int &nx, int &ny, Field::Type& newType,
                          bool& possibleMove) {
  if (!characters[y][x] || !pointInDir(x, y, k, nx, ny) ||
      characters[y][x]->dragon()) {
    return -1;
  }

  if (characters[ny][nx]) {
    if (characters[y][x]->possibleFight(*characters[ny][nx])) {
      doFight(x, y, nx, ny);
      if (!characters[y][x])
        return 0;
    } else {
      return -1;
    }
  }

  int cost;
  Character* character = characters[y][x];
  if (!character->move(fields[ny][nx].giveType(), pointsPool,
                    possibleMove, cost, newType)) {
    return -1;
  }

  return cost;
}
                          
int Board::playerMove(int x, int y, Direction k, int pointsPool) {
  if (!characters[y][x] || !characters[y][x]->collegue())
    return -1;
  return doMove(x, y, k, pointsPool);
}

int Board::doMove(int x, int y, Direction k, int pointsPool) {
  int nx, ny;
  Field::Type newType;
  bool possibleMove;
  int cost;

  if ((cost = movePossible(x, y, k, pointsPool, nx, ny,
                           newType, possibleMove)) < 0)
    return cost;

  fields[ny][nx].setType(newType);
  if (possibleMove) {
    characters[ny][nx] = characters[y][x];
    characters[y][x] = NULL;
  }
  return cost;
}

bool Board::giveStats(int x, int y, int &strengthPoints) {
  if (!(x>=0 && x<width && y>=0 && y<height))
    return false;
  if (characters[y][x] == NULL)
    return false;
  strengthPoints = characters[y][x]->giveStrPoints();
  return true;
}

void Board::connectWithGame(Game* game_) {
  game = game_;
}

void Board::addPossibleMoves(int x, int y, vector<PossibleMove>& moves,
                                int pointsPool) {
  int nx, ny;
  Field::Type newType;
  bool possibleMove;
  for (int i = 0; i < 4; i++)
    if (movePossible(x, y, (Direction) i, pointsPool,
                    nx, ny, newType, possibleMove) >= 0)
      moves.push_back(PossibleMove(x, y, (Direction) i));
}

int Board::compMove(int pointsPool) {
  vector<PossibleMove> moves;
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      if (characters[y][x] != NULL && !characters[y][x]->collegue())
        addPossibleMoves(x, y, moves, pointsPool);
  if (moves.empty()) {
    return -1;
  }

  int ind = rand() % moves.size();
  PossibleMove& move = moves[ind];
  return doMove(move.x, move.y, move.k, pointsPool);
}

bool Board::bilboIsDead() {
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      if (characters[y][x] && characters[y][x]->hero())
        return false;
  return true;
}

bool Board::demCaught() {
  int dx, dy;
  // Szukanie demotywatora.
  dx = dy = -1;
  for (int y = 0; dx == -1 && y < height; y++)
    for (int x = 0; dx == -1 && x < width; x++)
      if (characters[y][x] && characters[y][x]->dragon()) {
        dx = x;
        dy = y;
      }
  if (dx == -1)
    return false;
  // Sprawdzanie, czy demotywator otoczony przez min. dwie characters.
  int s = 0;
  for (int y = dy-1; y<=dy+1; y++)
    if (y>=0 && y<height)
      for (int x = dx-1; x<=dx+1; x++)
        if (x>=0 && x<width)
          if (characters[y][x] && characters[y][x]->collegue())
            s++;
  return s>=2;
}
