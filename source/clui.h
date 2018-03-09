#include <iostream>
#include <string.h>
#include <vector>
#include <windows.h> //für Terminalfarben
#include "token.h"
#include "artikel.h"
#include "auftrag.h"
#include "artikelinventar.h"

//Diese Klasse stellt das User-Interface und speichert Pointer zu eingelesenen Daten (Klasse "Artikel" und "Auftrag")
//und verarbeiteten Daten (Klasse "ArtikelInventar").

using namespace std;

class clUI
{
public:
    clUI();
    void mainMenu();
    ClToken *leseDaten();
    void konvertieren();
    vector<ArtikelInventar*> durchsuchen();
    void exportieren();
    void exportieren(vector<ArtikelInventar*> in);
    void statistiken();
    void zeigeBaum(ClToken *token);

private:
    int breite;
    ArtikelInventar *inventar;
    vector<ArtikelInventar*> letzteSuche;
    Artikel *artikel;
    Auftrag *auftrag;
};
