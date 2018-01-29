#include "auftrag.h"
#include <string.h>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

time_t parseTime(char *in);
char *printTime(time_t time);
char *sekToHrMin(double sek);

Auftrag::Auftrag()
{
    auftragsNr = NULL;
    kunde = NULL;
    anzBestellt = 0;
    for(int i = 0; i < anzBestellt; i++) bestellt[i] = NULL;
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
        return 0;
    }

    if (zaehler >= wurzel->getZahlNaechstes()) return 0;

    ClToken *jetzt = wurzel->getNaechstes(zaehler);

    auftragsNr = atoi(jetzt->att.getValueByName("auftragsnr"));

    kunde = new kundeS;
    kunde->name = jetzt->getNaechstes(0)->getNaechstes(0)->inhalt();
    kunde->str = jetzt->getNaechstes(0)->getNaechstes(1)->getNaechstes(0)->inhalt();
    kunde->hausNr = atoi(jetzt->getNaechstes(0)->getNaechstes(1)->getNaechstes(1)->inhalt());
    kunde->plz = atoi(jetzt->getNaechstes(0)->getNaechstes(1)->getNaechstes(2)->inhalt());
    kunde->stadt = jetzt->getNaechstes(0)->getNaechstes(1)->getNaechstes(3)->inhalt();

    for(int i = 0;jetzt->getNaechstes(1+i) != NULL
        && strcmp(jetzt->getNaechstes(1+i)->name(),"produkt") == 0;
        i++)
    {
        bestellt[i] = new bestelltesProdukt;
        bestellt[i]->produktNr = atoi(jetzt->getNaechstes(1+i)->getNaechstes(0)->inhalt());
        bestellt[i]->anzahl = atoi(jetzt->getNaechstes(1+i)->getNaechstes(1)->inhalt());
        anzBestellt++;
    }

    bestellZeit = parseTime(jetzt->getNaechstes(1+anzBestellt)->inhalt());
    ankzuftsZeit = parseTime(jetzt->getNaechstes(2+anzBestellt)->inhalt());

    //Berechne Lieferzeit (Diff. lieferzeit, ankunftszeit)
    lieferZeit = difftime(ankzuftsZeit, bestellZeit);

    zaehler++;
    naechste = new Auftrag();
    return naechste->fill(wurzel, zaehler);
}

char *Auftrag::getLieferZeit()
{
    return sekToHrMin(lieferZeit);
}

time_t parseTime(char *in)
{
    struct tm tm;
    istringstream ss(in);
    ss >> get_time(&tm, "%Y-%m-%dT%R");
    time_t a = mktime(&tm);
    return mktime(&tm);
    //return tm;
}

char *printTime(time_t time)
{
    char buffer[80];
    struct tm *loc = localtime(&time);
    strftime(buffer, 80, "%Y-%m-%d %H:%M", loc);
    cout << buffer << endl;
    return buffer;
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

