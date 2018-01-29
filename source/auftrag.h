#include "token.h"
#include <time.h>

struct bestelltesProdukt {
    int produktNr;
    int anzahl;
};
struct kundeS {
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
    bestelltesProdukt *getProdukt(int i) {return bestellt[i];}
    Auftrag *getNext() {return naechste;}
    char *getLieferZeit();
    int fill(ClToken *wurzel);
    int fill(ClToken *wurzel, int zaehler);
private:
    Auftrag *naechste;
    int auftragsNr;
    kundeS *kunde;
    bestelltesProdukt *bestellt[10];
    int anzBestellt;
    time_t bestellZeit;
    time_t ankzuftsZeit;
    double lieferZeit;
};
