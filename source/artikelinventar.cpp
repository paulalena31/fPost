#include "artikelinventar.h"
#include <iostream>
using namespace std;

string stringToken(string name, string inhalt, bool isChild);
string stringToken(string name, string inhalt, bool isChild, string attName, string attVal);

ArtikelInventar::ArtikelInventar()
{
    naechste = NULL;
    anzBestellt = 0;
}

void ArtikelInventar::kopiere(Artikel *artikel)
{
    //Kopiert alle Daten aus Der Artikel-Struktur, die für die weitere Verwendung genutzt werden
    name = artikel->getName();
    artikelnr = artikel->getNo();
    anzFlaschen = artikel->getAnzFlaschen();
    volFlaschen = artikel->getVolFlaschen();
    inventar = artikel->getInventar();
    normalpreis = artikel->getPreis(0);
    angebotspreis = artikel->getPreis(1);

    if (artikel->getNext() != NULL)
    {
        naechste = new ArtikelInventar;
        naechste->kopiere(artikel->getNext());
    }
}
void ArtikelInventar::verarbeite(Auftrag *auftrag)
{
    //Funktion geht alle Posten in allen Aufträgen durch und ändert entsprechend der Bestellungen Werte
    //des korrespondierenden ArtikelInventar-Objektes
    //Zu verändernde Werte: -Aktualisiertes Inventar (je nach bestellungen)
    //                      -Welcher Kunde (KundenNr) hat wie viel Bestellt

    Auftrag *jetzt = auftrag;

    for(;;)
    {
        for(int i = 0; i < jetzt->getAnzPosten(); i++) //iteriere Posten
        {
            bestelltesProdukt *aktuellerPosten = jetzt->getPosten(i);
            getArtikelByNo( aktuellerPosten->produktNr )->offsetInventar(-aktuellerPosten->anzahl); //Aktualisiere Inventar
            getArtikelByNo( aktuellerPosten->produktNr )->addBestellt(jetzt->getKunde()->kundenNr, aktuellerPosten->anzahl); //Aktualisiere "bestelltVon" und "bestelltAnz"
        }
        if (jetzt->getNext() == NULL) break;
        else jetzt = jetzt->getNext();
    }
}

ArtikelInventar *ArtikelInventar::getArtikelByNo(int no)
{
    //Sucht ArtikelInventar-Objekt nach der ArtikelNr und gibt dies zurück. Nur bei Anwendung im Wurzel-Objekt sinvoll.
    if(artikelnr == no) return this;
    if(getNext() == NULL)
    {
        cout << "Kein Artikel mit der Artikelnr " << no << " gefunden." << endl;
        return NULL;
    }
    return getNext()->getArtikelByNo(no);
}

void ArtikelInventar::addBestellt(int kundenNr, int anzahl)
{
    //Fügt Bestellungen dem ArtikelInventar-Objekt in den
    //synchronisierten Arrays bestelltVon (kundenNr) und bestelltAnz (Anzahl) hinzu
    for (int i = 0; i < anzBestellt; i++)
    {
        //Ist dieser Kunde (diese KundenNr) bereits im Array vertreten, wird der hinterlegte Wert nur aktualisiert
        if(bestelltVon[i] == kundenNr)
        {
            bestelltVon[i] += anzahl;
            break;
        }
    }
    bestelltVon[anzBestellt] = kundenNr;
    bestelltVonAnz[anzBestellt] = anzahl;
    anzBestellt ++;
}


void ArtikelInventar::speichern(ofstream &datei)
{
    string buffer = stringToken("inventarArtikel", "\n" +
                         stringToken("name", name, true) +
                         stringToken("inventar", to_string(inventar), true)
                         ,false, "artikelnr", to_string(artikelnr));
    cout << buffer << endl;
    datei << buffer;

    if(naechste != NULL)
    {
        getNext()->speichern(datei);
    }
}

void ArtikelInventar::drucke()
{
    cout << "Artikel: " << name << "   ArtikelNr: " << artikelnr << endl;
    cout << "   " << anzFlaschen << " Flaschen a " << volFlaschen << "l pro Kasten" << endl;
    cout << "   aktuelles Inventar: " << inventar << endl;
    cout << "   zuletzt bestellt von: " << endl;
    for(int i = 0; i < anzBestellt; i++)
    {
        cout << "     KundenNr: " << bestelltVon[i] << " (" << bestelltVonAnz[i] << " mal)" << endl;
    }
}

string stringToken(string name, string inhalt, bool isChild)
{
    string buffer;
    if(isChild) buffer += "    ";
    buffer += "<" + name + ">";
    buffer += inhalt;
    buffer += "</" + name + ">\n";
    return buffer;
}
string stringToken(string name, string inhalt, bool isChild, string attName, string attVal)
{
    string buffer;
    if(isChild) buffer += "    ";
    buffer += "<" + name + " ";
    buffer += attName + "=\"" + attVal + "\">";
    buffer += inhalt;
    buffer += "</" + name + ">\n";
    return buffer;
}

