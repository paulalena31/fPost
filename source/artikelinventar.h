#ifndef ARTIKELINVENTAR_H
#define ARTIKELINVENTAR_H

#include <fstream>
#include "auftrag.h"
#include "artikel.h"

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
    void offsetInventar(int o) {inventar += o;}
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
    int bestelltVon[10];
    int bestelltVonAnz[10];
    double avrAnzBestellt;
    ArtikelInventar *naechste;
};

#endif // ARTIKELINVENTAR_H
