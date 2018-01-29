#include <iostream>
#include <string.h>
#include <windows.h>
#include "token.h"
#include "artikel.h"
#include "auftrag.h"

using namespace std;

class clUI
{
public:
    clUI();
    void mainMenu();
    ClToken *leseDaten();
    void zeigeBaum(ClToken *token);

private:
    int breite;
    int hoehe;
    Artikel *artikel;
    Auftrag *auftrag;
};
