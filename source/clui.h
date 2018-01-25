#include <iostream>
#include <string.h>
#include "token.h"
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
    ClToken *token;
    ClElement *wurzel;
};
