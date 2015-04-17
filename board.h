#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <istream>

using namespace std;

enum Direction {
  RIGHT,
  DOWN,
  LEFT,
  UP
};

// Możliwe ruchy do wykonania. Konieczne przy zbieraniu możliwych ruchów
// komputera, z których możemy sobie jakiś wylosować.
struct PossibleMove {
  PossibleMove() {}
  PossibleMove(int x_, int y_, Direction k_):
    x(x_), y(y_), k(k_) {}
  int x, y;
  Direction k;
};

// Możliwe znaki definiujące teren. Dla weryfikacji planszy.
const char FIELD_CHARS[] = "pldsrmo";
// Możliwe znaki definiujące postać. Dla weryfikacji planszy.
const char CHARACTER_CHARS[] = " KWDBOTSM";

// Sprawdza czy c \in s.
bool charInString(char c, const char* s);

class Character;
class Game;

class Field {
  public:
    // Typ terenu.
    enum Type {
      FLAT,
      FOREST,
      ROAD,
      MOUNTAINS,
      RIVER,
      BRIDGE,
      OCEAN,
      ILE_TYP
    };
    Field();
    // Inicjuje strukturę.
    // defPola jest poprawnym dwuznakową definicją pola wziętą z opisu planszy.
    // Ma format:
    // <typ terenu><typ postaci>
    void initialize(char* fieldDef);
    // Wypisuje typ terenu. W przypadku, gdy czyWidziane == false, to wypisywany
    // jest symbol #.
    void print();
    // getter
    Type giveType();
    // setter
    void setType(Type typ_);
    // getter
    bool visible();
    // Ustawia czyWidziane na true.
    void see();

  private:
    Type typ;
    bool seen;
};

class Board {
  public:
    Board();
    // Destruktor, bo trzeba skasować zaalokowane postacie.
    ~Board();
    bool load(istream& in);
    // W przypadku, gdy pokazPrzeciwnikow == true, to wypisywani są również
    // przeciwnicy niewidoczni. Dla debuggingu.
    void printBoard(bool showOpponents);
    // Realizuje wykonanie ruchu postaci (x, y) w kierunku k przy dostępnej puli
    // punktów pulaPkt. Zwraca wykorzystaną liczbę punktów lub -1, gdy się to
    // nie powiodło.
    int playerMove(int x, int y, Direction k, int pointsPool);
    // Zapisuje liczbę punktów postaci (x, y) w parametrze pktSily. Zwraca true
    // wtw się udało.
    bool giveStats(int x, int y, int &strengthPoints);
    void connectWithGame(Game* game_);
    // Losuje i wykonuje ruch komputera. Zwraca wykorzystane punkty ruchu, lub
    // -1 w przypadku braku ruchu.
    int compMove(int pointsPool);
    bool bilboIsDead();
    bool demCaught();
    bool verify();

    // Jeśli POKAZ_PRZECIWNIKOW == true, to wypisywani są także niewidzialni
    // przeciwnicy. Dla debuggingu.
    static bool SHOW_OPPONENTS;

  private:
    // Wypisuje c n razy.
    void printChar(int n, char c);
    // Wypisuje liczbę 1- lub 2-cyfrową. Dodaje spację przed, jeśli 1-cyfrowa.
    void printNum(int x);
    // Linia złożona ze znaków -.
    void printHorizontalLine();
    void printCharacter(int x, int y);

    // Wczytuje dwuznakową definicję pola z wejścia. Zwraca true wtw odniesiono
    // sukces.
    bool loadField(istream& in, char* fieldDef);
    // Zwraca true wtw sukces.
    bool pointInDir(int x, int y, Direction k, int &nx, int &ny);
    // Przeprowadza walkę pomiędzy (x, y) a (nx, ny) przy założeniu, że ma to
    // sens.
    void doFight(int x, int y, int nx, int ny);
    // Sprawdza, czy można wykonać ruch. Uwzględnia walkę. (x, y) to
    // współrzędne postaci, k to kierunek ruchu, pulaPkt to dostępna pula
    // punktów.
    // Zwracane wartości: (nx, ny) - punkt docelowy, nowyTyp - typ pola, na
    // jakie trzeba zmienić pole docelowe (w przypadku D i M), ruchMozliwy == true
    // oznacza że należy przemieścić postać (bo nie zawsze trzeba -
    // przypadek Drwala).
    int movePossible(int x, int y, Direction k, int pointsPool,
                          int &nx, int &ny, Field::Type& newType,
                          bool& possibleMove);
    // Próba wykonania ruchu postacią (x, y) w kierunku k mając do dyspozycji
    // pulaPkt punktów. Uwzględnia również walkę. Zwraca wykorzystane punkty lub
    // -1 w przypadku niepowodzenia.
    int doMove(int x, int y, Direction k, int pointsPool);
    // Dodaje możliwe ruchy do wektora ruchy z punktu (x, y) mając do dyspozycji
    // pulaPkt punktów.
    void addPossibleMoves(int x, int y, vector<PossibleMove>& moves,
                                int pointsPool);
    // Widzialne pola to takie, które są bezpośrednio sąsiadujące z którąś z
    // postaci w danej chwili.
    void genVisibleFields(vector< vector<bool> >& visibleFields);

    Game* game;
    // Wielkość planszy.
    int height, width;
    // Teren.
    vector< vector<Field> > fields;
    // Postacie. NULL oznacza, że nikt nie stoi na danym polu.
    vector< vector<Character*> > characters;
};

#endif
