#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <vector>
#include <istream>

#include "board.h"

using namespace std;

class Character {
  public:
    static Character* create(char* fieldDef);
    Character(char sign_, int strengthPoints_, int characterMoveCost_, bool isFriend_);

    // Sprawdza, czy collegue.
    bool collegue();
    // Sprawdza, czy dragon.
    bool dragon();
    // Sprawdza, czy Bilbo.
    bool hero();
    // Znak postaci.
    char giveChar();
    // Aktualne punkty siły.
    int giveStrPoints();

    // Sprawdza, czy można wykonać move tą postacią. Uwzględnia przekazany typ
    // pola i dostępną pulę punktów.
    // Zwraca true wtw można wykonać move.
    // Ustawia parametry: wykonajRuch - czy należy przemieścić postać
    // (zagadnienie Drwala), wykorzystanyKoszt - wykorzystana liczba punktów,
    // nowyType - typ, na jaki należy zmienić pole docelowe (zagadnienie Drwala i
    // Budowniczego). 
    bool move(Field::Type currFiledType, int pointsPool,
              bool& move, int& costUsed, Field::Type& newType);

    // Sprawdza, czy można przeprowadzić walkę inicjowaną przez this przeciwko
    // p.
    bool possibleFight(const Character& p);

    // Odejmuje pktObr od aktualnej liczby punktów siły.
    void damage(int damagePoints);

    // Sprawdza, czy postać może stać na danym polu. Do weryfikacji planszy.
    bool possibleField(Field::Type fieldType);

  protected:
    // Trochę stałych.
    static const int mountainsCost = 50;
    static const int bridgeCost = 100;
    static const int forestCost = 80;

    // Znak postaci do wyświetlenia.
    char sign;
    // Pola definiujące postać (zamiast właściwego polimorfizmu).
    int strengthPoints, characterMoveCost;
    bool isFriend, isDragon, isHero;
    bool isTroll;
    bool allowedFields[Field::ILE_TYP];
};

// Klasy pochodne. Wszystkie definiowane są poprzez wywołanie konstruktora klasy
// bazowej z odpowiednimi parametrami.
class Friend : public Character {
  public:
    Friend(char sign_, int strengthPoints_, int characterMoveCost_);
};

class Enemy : public Character {
  public:
    Enemy(char sign_, int strengthPoints_, int characterMoveCost_);
};

class Bilbo : public Friend {
  public:
    Bilbo();
};

class Dwarf : public Friend {
  public:
    Dwarf();
};

class Climber : public Friend {
  public:
    Climber();
};

class Builder : public Friend {
  public:
    Builder();
};

class Lumberjack : public Friend {
  public:
    Lumberjack();
};

class Dragon : public Enemy {
  public:
    Dragon();
};

class Troll : public Enemy {
  public:
    Troll();
};

class Ork : public Enemy {
  public:
    Ork();
};



#endif
