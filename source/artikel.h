#include "token.h"

class Artikel
{
public:
    Artikel();
    char *getName() {return name;}
    float getPreis();
    int getNo() {return artikelnr;}
    int getAnzFlaschen() {return anzFlaschen;}
    int getVolFlaschen() {return volFlaschen;}
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
