// Logika gry.
#ifndef GAME_H
#define GAME_H

#include <sstream>
#include "characters.h"

bool charToDir(char c, Direction& k);
// Losuje z przedziału [a, b].
int selectRand(int a, int b);

// Klasa obsługująca mechanizm rozgrywki.
class Game {
  public:
    Game(Board* board_);
    // Tura gracza. Wywołanie metody kończy się, gdy zostanie odebrana komenda
    // "koniec". Wartość zwracana oznaczy, czy należy kontynuować grę.
    bool player();
    // Tura gracza. Wywołanie metody kończy się, gdy nie można już wykonać ruchu.
    // Wartość zwracana oznaczy, czy należy kontynuować grę.
    bool computer();
    // Realizacja walki pomiędzy postaciami a i b. Współrzędne (x1, y1) i (x2,
    // y2) odpowiadają współrzędnych postaci a i b odpowiednio. Zwracana jest
    // wartość 1 lub 2 - nr gracza, który wygrał. Metoda ta również zmieniejsza
    // punkty siły wygranego o odpowiednią wartość obrażeń.
    int fight(Character* a, Character* b,
                         int x1, int y1, int x2, int y2);

  private:
    // Liczba punktów, jakie gracz/komputer mają przydzielone na turę.
    static const int pointsForRound = 120;

    // Wypisuje planszę i status gry (zebrane komunikaty).
    void printInteractions();
    // Komenda - wczytany input od użytkownika,
    // Zwraca true wtw komenda obsłużona poprawnie.
    bool handleCommand(const string& command);
    // Losuje obrażenia i odejmuje je od siły pierwszego gracza.
    // Założenie: wygrał gracz a.
    int damage(Character* a, Character* b);
    // Losuje, kto wygrał dla wartości sił a oraz b. Zwraca 1 lub 2 odpowiednio.
    int whoWon(int a, int b);
    // Wypisuje zebrane komunikaty (zawartość zmiennej status). Czyści zebrane
    // komunikaty.
    void printStatus();

    Board* board;
    // Zebrane komunikaty.
    stringstream status;
    // Pozostałe punkty akcji dla bieżącej tury (gracza lub komputera).
    int actionPoints;
};

#endif
