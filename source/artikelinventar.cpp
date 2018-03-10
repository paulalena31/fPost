#include "artikelinventar.h"
#include <iostream>
#include <algorithm>
#include "string.h"

string stringToken(string name, string inhalt, int ebene);
string stringToken(string name, string inhalt, int ebene, string attName, string attVal);
double charToDouble(char *c);

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

vector<ArtikelInventar*> ArtikelInventar::suche(char *kategorie, char *term)
{
    //Durchsucht alle Artikel von diesem Objekt aus nach "term" in "kategorie", füllt einen vector mit
    //Ergebnissen (Pointer zu ArtikelInventar-Objekten)
    //

    vector<ArtikelInventar*> gefunden;
    if(strcmp(kategorie, "name") == 0){
        if(strcmp(name, term) == 0) gefunden.push_back(this);
    }
    else if (strcmp(kategorie, "artikelnr") == 0){
            if(artikelnr == atoi(term)) gefunden.push_back(this);
    }
    else if (strcmp(kategorie, "normalpreis") == 0){
            if(charToDouble(term) == normalpreis) gefunden.push_back(this);
    }
    else if (strcmp(kategorie, "angebotspreis") == 0){
            if(charToDouble(term) == angebotspreis) gefunden.push_back(this);
    }

    if(getNext() != NULL)
    {
        //Rekursive Ausführung, Suche in naechste-Objekt
        vector<ArtikelInventar*> next = getNext()->suche(kategorie, term);
        //zusammenfügen der zurückgegebenen Ergebnisse
        gefunden.reserve(gefunden.size() + next.size());
        gefunden.insert(gefunden.end(), next.begin(), next.end());
    }
    return gefunden;
}

vector<ArtikelInventar*> ArtikelInventar::suche(char *kategorie, char *term, vector<ArtikelInventar*> in)
{
    //Suche eingeschränkt nur innerhalb vector "in", bei Suche mit mehreren Suchtermen
    vector<ArtikelInventar*> gefunden;

    for(int i = 0; i < in.size(); i++)
    {
        if(strcmp(kategorie, "name") == 0){
            if(strcmp(in[i]->getName(), term) == 0) gefunden.push_back(in[i]);
        }
        else if (strcmp(kategorie, "artikelnr") == 0){
                if(in[i]->getNo() == atoi(term)) gefunden.push_back(in[i]);
        }
        else if (strcmp(kategorie, "normalpreis") == 0){
                if(charToDouble(term) == in[i]->getNormalpreis()) gefunden.push_back(in[i]);
        }
        else if (strcmp(kategorie, "angebotspreis") == 0){
                if(charToDouble(term) == in[i]->getAngebotspreis()) gefunden.push_back(in[i]);
        }
        else if (strcmp(kategorie, "anzahlBestellt") == 0){
                if(charToDouble(term) == in[i]->getAnzBestellt()) gefunden.push_back(in[i]);
        }
    }

    return gefunden;
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
    //synchronisierten vectors bestelltVon (kundenNr) und bestelltVonAnz (Anzahl) hinzu
    for (int i = 0; i < bestelltVon.size(); i++)
    {
        //Ist dieser Kunde (diese KundenNr) bereits im vector vertreten, wird der hinterlegte Wert nur aktualisiert
        if(bestelltVon[i] == kundenNr)
        {
            bestelltVon[i] += anzahl;
            break;
        }
    }
    bestelltVon.push_back(kundenNr);
    bestelltVonAnz.push_back(anzahl);
    anzBestellt += anzahl;
}


void ArtikelInventar::speichern(ofstream &datei)
{
    string bufferPreis = "";
    if(angebotspreis != NULL) bufferPreis = stringToken("angebotspreis", to_string(angebotspreis), 2);

    string bufferBestelltVon = "";
    for(int i = 0; i < bestelltVon.size(); i++)
    {
        bufferBestelltVon += stringToken("kunde", "\n" +
                                         stringToken("kundenNr", to_string(bestelltVon[i]), 2) +
                                         stringToken("anzBestellt", to_string(bestelltVonAnz[i]), 2) +
                                         "    "
                                         ,1);
    }

    string buffer = stringToken("inventarArtikel", "\n" +
                         stringToken("name", name, 1) +
                         stringToken("inventar", to_string(inventar), 1) +
                         stringToken("preis", "\n" +
                                     stringToken("normalpreis", to_string(normalpreis), 2) +
                                     bufferPreis + "    "
                                     ,1) +
                         stringToken("flaschen", "\n" +
                                     stringToken("anzahl", to_string(anzFlaschen), 2) +
                                     stringToken("volumen", to_string(volFlaschen), 2) + "    "
                                     , 1) +
                         bufferBestelltVon
                         ,0 , "artikelnr", to_string(artikelnr));

    cout << buffer << endl;
    datei << buffer;
}

void ArtikelInventar::drucke()
{
    cout << "\nArtikel: " << name << "   ArtikelNr: " << artikelnr << endl;
    cout << "   " << anzFlaschen << " Flaschen a " << volFlaschen << "l pro Kasten" << endl;
    cout << "   Normalpreis: " << normalpreis << " Euro"<<  endl;
    if(angebotspreis != NULL) cout << "      Angebotspreis "<< angebotspreis << " Euro" << endl;
    cout << "   aktuelles Inventar: " << inventar << endl;
    cout << "   " << anzBestellt << " K\204sten bestellt" << endl;
    cout << "   zuletzt bestellt von: " << endl;
    for(int i = 0; i < bestelltVon.size(); i++)
    {
        cout << "     KundenNr: " << bestelltVon[i] << " (" << bestelltVonAnz[i] << " mal)" << endl;
    }
    cout << endl;
}

double ArtikelInventar::getPreis()
{
    if(angebotspreis == NULL) return normalpreis;
        else return angebotspreis;
}

string stringToken(string name, string inhalt, int ebene)
{
    string buffer;
    for(int i = 0; i < ebene; i++) buffer += "    ";
    buffer += "<" + name + ">";
    buffer += inhalt;
    buffer += "</" + name + ">\n";
    return buffer;
}
string stringToken(string name, string inhalt, int ebene, string attName, string attVal)
{
    string buffer;
    for(int i = 0; i < ebene; i++) buffer += "    ";
    buffer += "<" + name + " ";
    buffer += attName + "=\"" + attVal + "\">";
    buffer += inhalt;
    buffer += "</" + name + ">\n";
    return buffer;
}

double charToDouble(char *c){
    replace(c, c+strlen(c), ',', '.');
    return atof(c);
}


