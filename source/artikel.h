#ifndef ARTIKEL_H
#define ARTIKEL_H

#include "token.h"

//Die Klasse Artikel repräsentiert Artikel aus der Artikelliste-XML-Instanz.
//Die Objekte sind in einer linked list organisiert.

class Artikel
{
public:
    Artikel();
    char *getName() {return name;}
    double getPreis();
    double getPreis(int i);
    int getNo() {return artikelnr;}
    int getAnzFlaschen() {return anzFlaschen;}
    int getVolFlaschen() {return volFlaschen;}
    int getInventar() {return inventar;}
    void drucke();
    Artikel *getNext() {return naechste;}
    void fill(ClToken *wurzel);
private:
    char *name;
    int artikelnr;
    int anzFlaschen;
    double volFlaschen;
    int inventar;
    double normalpreis;
    double angebotspreis;
    Artikel *naechste;
    int fill(ClToken *wurzel, int zaehler);
};

#endif /* ARTIKEL_H */
