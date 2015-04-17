#include <algorithm>
using namespace std;

#include "characters.h"
#include "board.h"

Character* Character::create(char *defPola) {
  switch (defPola[1]) {
    case 'B': return new Bilbo();
    case 'K': return new Dwarf();
    case 'W': return new Climber();
    case 'M': return new Builder();
    case 'S': return new Dragon();
    case 'T': return new Troll();
    case 'O': return new Ork();
    case 'D': return new Lumberjack();
  }
  return NULL;
}

Character::Character(char char_, int strengthPoints_, int characterMoveCost_, bool isFriend_):
    sign(char_), strengthPoints(strengthPoints_), characterMoveCost(characterMoveCost_), isFriend(isFriend_) {
  allowedFields[Field::FLAT] = true;
  allowedFields[Field::FOREST] = false;
  allowedFields[Field::ROAD] = true;
  allowedFields[Field::MOUNTAINS] = false;
  allowedFields[Field::RIVER] = false;
  allowedFields[Field::BRIDGE] = true;
  allowedFields[Field::OCEAN] = false;

  isDragon = false;
  isHero = false;
  isTroll = false;
}

bool Character::move(Field::Type currFieldType, int pointsPool,
                  bool& move, int& costUsed, Field::Type& newType) {
  if (!allowedFields[currFieldType])
    return false;

  costUsed = characterMoveCost;
  move = true;
  newType = currFieldType;
  switch (currFieldType) {
    case Field::FOREST:
      move = false;
      costUsed = forestCost;
      newType = Field::ROAD;
      break;
    case Field::MOUNTAINS:
      costUsed = mountainsCost;
      break;
    case Field::RIVER:
      costUsed = bridgeCost;
      newType = Field::BRIDGE;
      break;
    default:
      break;
  }

  return costUsed <= pointsPool;
}

bool Character::possibleField(Field::Type fieldType) {
  return isDragon || allowedFields[fieldType];
}

bool Character::possibleFight(const Character& p) {
  return
    !p.isDragon && !isFriend && (
        (isFriend != p.isFriend) ||
        (isTroll && !p.isTroll));
}

char Character::giveChar() {
  return sign;
}

bool Character::collegue() {
  return isFriend;
}

bool Character::dragon() {
  return isDragon;
}

bool Character::hero() {
  return isHero;
}

int Character::giveStrPoints() {
  return strengthPoints;
}

void Character::damage(int damagePoints) {
  strengthPoints = max(strengthPoints - damagePoints, 1);
}

Friend::Friend(char char_, int strengthPoints_, int characterMoveCost_):
  Character(char_, strengthPoints_, characterMoveCost_, true) {}

Enemy::Enemy(char char_, int strengthPoints_, int characterMoveCost_):
  Character(char_, strengthPoints_, characterMoveCost_, false) {}

Bilbo::Bilbo():
  Friend('B', 20, 45) {
  isHero = true;
}

Dwarf::Dwarf():
  Friend('K', 30, 35) {}

Climber::Climber():
  Friend('W', 32, 28) {
  allowedFields[Field::MOUNTAINS] = true;
}

Builder::Builder():
  Friend('M', 35, 40) {
  allowedFields[Field::RIVER] = true;
}

Lumberjack::Lumberjack():
  Friend('D', 38, 40) {
  allowedFields[Field::FOREST] = true;
}

Dragon::Dragon():
  Enemy('S', 0, 1000000) {
  isDragon = true;
}

Troll::Troll():
  Enemy('T', 70, 55) {}

Ork::Ork():
  Enemy('O', 30, 35) {
  allowedFields[Field::MOUNTAINS] = true;
}

