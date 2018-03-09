#include "artikel.h"
#include <string.h>
#include <algorithm>
#include <iostream>

using namespace std;

double toDouble(char *c);

Artikel::Artikel()
{
    artikelnr = NULL;
    name = NULL;
    anzFlaschen = NULL;
    volFlaschen = NULL;
    inventar = NULL;
    normalpreis = NULL;
    angebotspreis = NULL;
    naechste = NULL;
}

float Artikel::getPreis()
{
    if (angebotspreis != NULL) return angebotspreis;
    return normalpreis;
}
float Artikel::getPreis(int i)
{
    if (i == 0) return normalpreis;
    if (i == 1) return angebotspreis;
}

void Artikel::fill(ClToken *wurzel)
{
    fill(wurzel, 0);
}

int Artikel::fill(ClToken *wurzel, int zaehler)
{
    if (strcmp(wurzel->name(), "artikelliste") != 0)
    {
        cout << "Token keine Artikelliste.";
        return 0;
    }

    ClToken *jetzt = wurzel->getNaechstes(zaehler);
    name = jetzt->getNaechstes(0)->inhalt();
    anzFlaschen = atoi( jetzt->getNaechstes(1)->getNaechstes(0)->inhalt() );
    volFlaschen = toDouble( jetzt->getNaechstes(1)->getNaechstes(1)->inhalt() );

    normalpreis = toDouble( jetzt->getNaechstes(2)->getNaechstes(0)->inhalt());
    if (jetzt->getNaechstes(2)->getNaechstes(1) != NULL){
        angebotspreis = toDouble( jetzt->getNaechstes(2)->getNaechstes(1)->inhalt());
    }
    inventar = atoi( jetzt->getNaechstes(3)->inhalt());
    artikelnr = atoi(jetzt->att.getValueByName("artikelnr"));

    zaehler++;

    if (zaehler >= wurzel->getZahlNaechstes()) return 0;

    naechste = new Artikel();
    return naechste->fill(wurzel, zaehler);
}

void Artikel::drucke()
{
    cout << "Artikel: " << name << "   ArtikelNr: " << artikelnr << endl;
    cout << anzFlaschen << " Flaschen a " << volFlaschen << "l" << endl;
    cout << "initial-Inventar: " << inventar << endl;
    cout << "Preis: " << getPreis() << " Euro";
    if (angebotspreis != NULL) cout << " (Normalpreis: " << normalpreis << " Euro)";
    cout << "\n" << endl;
    if(getNext() != NULL) getNext()->drucke();
}

double toDouble(char *c){
    replace(c, c+strlen(c), ',', '.');
    return atof(c);
}

