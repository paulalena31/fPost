#ifndef ARTIKELINVENTAR_H
#define ARTIKELINVENTAR_H

#include <fstream>
#include <vector>
#include "auftrag.h"
#include "artikel.h"

using namespace std;

//ArtikelInventar fügt sich aus Objekten der Auftrag und Artikelklasse zusammen.
//Die Objekte der Klasse repräsentieren die Aritkel im Inventar mit zusätzlichen Informationen dazu,
//von wem und wie oft ein bestimmtes Produkt bestellt wurde.

class ArtikelInventar
{
public:
    ArtikelInventar();
    int getAnzBestellt() {return anzBestellt;}
    int getKundenNr(int i) {return bestelltVon[i];}
    void addBestellt(int kundenNr, int anzahl);
    char *getName() {return name;}
    int getNo() {return artikelnr;}
    double getPreis();
    double getNormalpreis() {return normalpreis;}
    double getAngebotspreis() {return angebotspreis;}
    int getInventar() {return inventar;}
    int getAnzFlaschen() {return anzFlaschen;}
    int getVolFlaschen() {return volFlaschen;}

    void offsetInventar(int o) {inventar += o;}
    std::vector<ArtikelInventar*> suche(char *kategorie, char *term);
    std::vector<ArtikelInventar*> suche(char *kategorie, char *term, std::vector<ArtikelInventar*> in);
    ArtikelInventar *getArtikelByNo(int no);

    void kopiere(Artikel *artikel);
    void verarbeite(Auftrag *auftrag);
    ArtikelInventar *getNext() {return naechste;}
    void drucke();
    void speichern(ofstream &datei);
private:
    char *name;
    int artikelnr;
    int anzFlaschen;
    double volFlaschen;
    int inventar;
    double normalpreis;
    double angebotspreis;

    int anzBestellt;
    std::vector<int> bestelltVon;
    std::vector<int> bestelltVonAnz;
    double avrAnzBestellt;
    ArtikelInventar *naechste;
};

#endif // ARTIKELINVENTAR_H
