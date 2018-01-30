#ifndef AUFTRAG_H
#define AUFTRAG_H

#include "token.h"
#include <time.h> //für Repräsentation der bestell-/ankuftszeit

//Die Klasse Auftrag repräsentiert Aufträge aus der XML-Instanz Auftragsliste.
//Die Objekte sind in einer linked list organisiert.

struct bestelltesProdukt {
    int produktNr;
    int anzahl;
};
struct kundeS {
    int kundenNr;
    char *name;
    char *str;
    int hausNr;
    int plz;
    char *stadt;
};

class Auftrag
{
public:
    Auftrag();
    int getNo() {return auftragsNr;}
    kundeS *getKunde() {return kunde;}
    bestelltesProdukt *getPosten(int i) {return posten[i];}
    int getAnzPosten() {return anzPosten;}
    Auftrag *getNext() {return naechste;}
    char *getLieferZeit();
    int fill(ClToken *wurzel);
    int fill(ClToken *wurzel, int zaehler);
private:
    Auftrag *naechste;
    int auftragsNr;
    kundeS *kunde;
    bestelltesProdukt *posten[10];
    int anzPosten;
    time_t bestellZeit;
    time_t ankzuftsZeit;
    double lieferZeit;
};

#endif /* AUFTRAG_H */
