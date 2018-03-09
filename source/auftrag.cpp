#include "auftrag.h"
#include <string.h>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

time_t parseTime(char *in);
string printTime(time_t time);
char *sekToHrMin(double sek);

Auftrag::Auftrag()
{
    naechste = NULL;
    auftragsNr = NULL;
    kunde = NULL;
}

int Auftrag::fill(ClToken *wurzel)
{
    return fill(wurzel, 0);
}

int Auftrag::fill(ClToken *wurzel, int zaehler)
{
    if (strcmp(wurzel->name(), "auftraege") != 0)
    {
        cout << "Token ist keine Auftragsliste." << endl;
        return NULL;
    }

    ClToken *jetzt = wurzel->getNaechstes(zaehler);

    auftragsNr = atoi(jetzt->att.getValueByName("auftragsnr"));

    kunde = new kundeS;
    kunde->kundenNr = atoi(jetzt->getNaechstes(0)->att.getValueByName("kundennr"));
    kunde->name = jetzt->getNaechstes(0)->getNaechstes(0)->inhalt();
    kunde->str = jetzt->getNaechstes(0)->getNaechstes(1)->getNaechstes(0)->inhalt();
    kunde->hausNr = atoi(jetzt->getNaechstes(0)->getNaechstes(1)->getNaechstes(1)->inhalt());
    kunde->plz = atoi(jetzt->getNaechstes(0)->getNaechstes(1)->getNaechstes(2)->inhalt());
    kunde->stadt = jetzt->getNaechstes(0)->getNaechstes(1)->getNaechstes(3)->inhalt();

    for(int i = 0; jetzt->getNaechstes(1+i) != NULL
        && strcmp(jetzt->getNaechstes(1+i)->name(),"produkt") == 0;
        i++)
    {
        posten.push_back(new bestelltesProdukt);
        posten.back()->produktNr = atoi(jetzt->getNaechstes(1+i)->getNaechstes(0)->inhalt());
        posten.back()->anzahl = atoi(jetzt->getNaechstes(1+i)->getNaechstes(1)->inhalt());
    }

    bestellZeit = parseTime(jetzt->getNaechstes(1+posten.size())->inhalt());
    ankzuftsZeit = parseTime(jetzt->getNaechstes(2+posten.size())->inhalt());

    //Berechne Lieferzeit (Diff. lieferzeit, ankunftszeit)
    lieferZeit = difftime(ankzuftsZeit, bestellZeit);

    zaehler++;

    if (zaehler >= wurzel->getZahlNaechstes()) return 0;

    naechste = new Auftrag();
    naechste->fill(wurzel, zaehler);
}

char *Auftrag::getLieferZeit()
{
    return sekToHrMin(lieferZeit);
}

void Auftrag::drucke()
{
    cout << "Auftrag Nr: " << auftragsNr << "     bestellt: " << printTime(bestellZeit)
         << "  geliefert: " << printTime(ankzuftsZeit) << endl;
    cout << "Kunde Nr: " << kunde->kundenNr << " " << kunde->name << ", " << kunde->str << " "
         << kunde->hausNr << ", " << kunde->plz << " " << kunde->stadt << endl;
    cout << "Posten: " << endl;
    for (int i=0; i < posten.size(); i++) cout << "    " << posten[i]->anzahl << "mal   ArtikelNr: " << posten[i]->produktNr << endl;



    cout << "\n" << endl;

    if(getNext() != NULL) getNext()->drucke();
}

time_t parseTime(char *in)
{
    struct tm tm;
    istringstream ss(in);
    ss >> get_time(&tm, "%Y-%m-%dT%R");
    time_t a = mktime(&tm);
    return mktime(&tm);
}

string printTime(time_t time)
{
    char buffer[17];
    struct tm *loc = localtime(&time);
    strftime(buffer, 17, "%Y-%m-%d %H:%M", loc);
    string out(buffer);
    return out;
}
char *sekToHrMin(double sek)
{
    int minutes = sek / 60;
    int hours = minutes / 60;
    minutes = minutes % 60;
    char buffer[6];
    buffer[0] = hours;
    buffer[2] = ':';
    buffer[3] = minutes;
    return buffer;
}

