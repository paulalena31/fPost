#include "clui.h"

string logo[7] =
{
    "   ______           __ ",
    "  / _/ _ \\___  ___ / /_",
    " / _/ ___/ _ \\(_-</ __/",
    "/_//_/   \\___/___/\\__/ ",
    "",
    "Getr\204nkeversand-Management",
    ""
};
string optionenHaupt[6] = {
    "Daten einlesen",
    "Daten konvertieren",
    "Daten durchsuchen",
    "Daten exportieren",
    "Statistiken",
    "Beenden"
};
string optionenLesen[2] = {
    "Artikelliste einlesen",
    "Auftragsliste einlesen"
};
string optionenExport[2] = {
    "Gesamte InventarArtikel-Liste exportieren",
    "Letzte Suchergebnisse exportieren"
};

void druckeOptionen(string optionen[], int anz);
string sekToHrMin(double sek);

clUI::clUI()
{
    breite = 50;
    inventar = NULL;
    artikel = NULL;
    auftrag = NULL;
}
void clUI::mainMenu()
{
    bool exit = false;
    while(!exit)
    {
        //Logo
        for (int i = 0; i < breite; i++) cout << "-";
        for (int i = 0; i < 7; i++) cout << endl << logo[i];
        cout << endl;

        //Menu-Optionen
        druckeOptionen(optionenHaupt, 6);
        for (int i = 0; i < breite; i++) cout << "-";
        cout << endl;

        //Menuauswahl
        char asuwahl;
        cin >> asuwahl;
        switch(asuwahl)
        {

        //Einlesen
        case'1':
            druckeOptionen(optionenLesen,2);
            cin >> asuwahl;
            switch(asuwahl)
            {
                case'1':
                {
                    if(artikel != NULL)
                    {
                        cout << "Es wurde bereits eine Artikelliste eingelesen. M\224chten Sie die Daten \201berschreiben? (j/n)" << endl;
                        char antwort[1];
                        cin >> antwort;
                        if (strcmp(antwort,"j") == 0) {
                            artikel = NULL;
                        } else if (strcmp(antwort,"n") == 0) {
                            break;
                        }
                    }
                    cout << "Standard: artikel.dtd  artikel.xml"<< endl;
                    ClToken *token = leseDaten();
                    artikel = new Artikel();
                    artikel->fill(token);
                    artikel->drucke();
                    break;
                }
                case'2':
                {
                    if(auftrag != NULL)
                    {
                        cout << "Es wurde bereits eine Auftragsliste eingelesen. M\224chten Sie die Daten \201berschreiben? (j/n)" << endl;
                        char antwort[1];
                        cin >> antwort;
                        if (strcmp(antwort,"j") == 0) {
                            auftrag = NULL;
                        } else if (strcmp(antwort,"n") == 0) {
                            break;
                        }
                    }
                    cout << "Standard: auftraege.dtd  auftraege.xml"<< endl;
                    ClToken *token = leseDaten();
                    auftrag = new Auftrag();
                    auftrag->fill(token);
                    auftrag->drucke();
                    break;
                }
                default:
                    break;
            }
            break;

        //Konvertieren
        case'2':
            if(artikel == NULL) cout << "Es wurde noch keine Artikelliste eingelesen." << endl;
            if(auftrag == NULL) cout << "Es wurde noch keine Auftragsliste eingelesen." << endl;
            if(artikel !=NULL && auftrag != NULL)
            {
                konvertieren();
            }
            break;

        //Durchsuchen
        case'3':
        {
            if(artikel == NULL || auftrag == NULL){
                cout << "Bitte lesen Sie die Artikel- und Auftragsliste ein." << endl;
                break;
            } else if (inventar == NULL){
                cout << "Bitte konventieren Sie die eingelesen Daten, bevor Sie sie durchsuchen." << endl;
                break;
            }

            vector<ArtikelInventar*> ergebnis;
            ergebnis = durchsuchen();

            letzteSuche = ergebnis; //speichere Suchergebnisse zum späteren exportieren

            if (ergebnis.empty()) cout << "Nichts gefunden." << endl;
            for(int i = 0; i < ergebnis.size(); i++) ergebnis[i]->drucke();
            break;
        }

        //Exportieren
        case'4':
            druckeOptionen(optionenExport, 2);
            cin >> asuwahl;
            switch(asuwahl)
            {
                case'1':
                    exportieren();
                break;
                case'2':
                    if(letzteSuche.empty())
                    {
                        cout << "Keine letzte Suche gefunden." << endl;
                        break;
                    }
                    exportieren(letzteSuche);
            }
            break;
        //Statistiken
        case'5':
            if(inventar == NULL)
            {
                cout << "Bitte konvertieren Sie zuerst die Artikel- und Auftragsliste." << endl;
                break;
            }
            statistiken();
            break;
        case'6':
            exit = true;
            break;
        default:
            cout << "Bitte wählen Sie eine Option aus." << endl;
            break;
        }
    }

}

ClToken *clUI::leseDaten()
{
    //Schriftfarben:
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ClElement *wurzel = NULL;
    ClToken *token = NULL;

    char dateiname[100];
    ifstream eingabe;
    wurzel = new ClElement();

    cout << "DTD-Dateiname:" << endl;
    cin >> dateiname;
    eingabe.open(dateiname);
    if(!eingabe.is_open()) {
        cout << "Datei nicht gefunden." << endl;
        mainMenu();
        return NULL;
    }
    wurzel=wurzel->verarbeite(eingabe);
    cout << "DTD geladen." << endl;
    eingabe.close();

    cout << "XML-Dateiname:" << endl;
    cin >> dateiname;
    eingabe.open(dateiname);

    token=new ClToken;

    if (token->getToken(eingabe,NULL,wurzel)!=0)
    {
        SetConsoleTextAttribute(hConsole, 2);
        cout << "XML erfolgreich geladen und validiert." << endl;
        SetConsoleTextAttribute(hConsole, 15);
    } else {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "Beim Validieren traten Fehler auf." << endl;
        SetConsoleTextAttribute(hConsole, 15);
    }
    eingabe.close();
    return token;
}

void clUI::konvertieren()
{
    inventar = new ArtikelInventar();
    inventar->kopiere(artikel);
    inventar->verarbeite(auftrag);
    cout << "Daten konvertiert." << endl;

    exportieren();
}

vector<ArtikelInventar*> clUI::durchsuchen()
{
    //Durchsucht den Datensatz und gibt eine Liste mit den Ergebnissen aus.

    vector<ArtikelInventar*> ergebnis;
    cout << "In welcher Kategorie soll gesucht werden? (name, artikelnr, normalpreis, angebotspreis, anzahlBestellungen)" << endl;
    for(;;){
        char eingabe[15];
        cin >> eingabe;

        //Prüfung, ob Kategorie valide ist
        char *kat[5] = {"name", "artikelnr", "normalpreis", "angebotspreis", "anzahlBestellt"};
        bool valid = false;
        for (int i = 0; i < 5; i++) if(strcmp(eingabe, kat[i])==0) valid = true;
        if(!valid){
            cout << "Kategorie nicht vorhanden." << endl;
            break;
        }
        char term[50];
        cout << "Bitte Suchbegriff eingeben: " << endl;
        cin >> term;
        if(ergebnis.empty()) ergebnis = inventar->suche(eingabe, term);
        else
        { //mit weiterem suchterm einschränken:
            ergebnis = inventar->suche(eingabe, term, ergebnis);
        }

        cout << "M\224chten Sie einen weiteren Suchterm hinzuf\201gen? (j/n)" << endl;
        cin >> eingabe;
        if(strcmp(eingabe, "n") == 0) break;
        else if(strcmp(eingabe, "j") == 0){
            cout << "In welcher weiteren Kategorie soll gesucht werden? (name, artikelnr, normalpreis, angebotspreis)" << endl;
        } else {break;}
    }

    return ergebnis;
}

void clUI::statistiken()
{
    string buffer;
    //Anzahl Kästen/ Flaschen und Gesamtvolumen im Inventar
    int summeK = 0;
    int summeF = 0;
    double summeV = 0;

    ArtikelInventar *jetzt = inventar;
    while(jetzt->getNext() != NULL)
    {
        summeK += jetzt->getInventar();
        summeF += jetzt->getInventar() * jetzt->getAnzFlaschen();
        summeV += jetzt->getInventar() * jetzt->getAnzFlaschen() * jetzt->getVolFlaschen();
        jetzt = jetzt->getNext();
    }
    cout << "Inventar:" << endl;
    cout << "  K\204sten: " << summeK << "   Flaschen: " << summeF << "   Gesamtvolumen: " << summeV << "l" << endl;
    buffer += "Inventar:\n   K\204sten: " + to_string(summeK) + "   Flaschen: " + to_string(summeF) + "   Gesamtvolumen: " + to_string(summeV) + "l\n";

    //Anzahl verkaufter K\204sten nach letztem Import der Auftragsliste
    int summeInit = 0;
    Artikel *jetztArtikel = artikel;
    while(jetztArtikel->getNext() != NULL)
    {
        summeInit += jetztArtikel->getInventar();
        jetztArtikel = jetztArtikel->getNext();
    }
    int verkauft = summeInit - summeK;
    cout << "K\204sten verkauft nach letztem Auftragsimport: " << verkauft << endl;
    buffer += "K\204sten verkauft nach letztem Auftragsimport: " + to_string(verkauft) + "\n";

    //Durchschnittliche Lieferzeit
    double summeZeit = 0;
    int anzAuftraege = 0;
    Auftrag *jetztAuftrag = auftrag;
    while(jetztAuftrag->getNext() != NULL)
    {
        summeZeit += jetztAuftrag->getLieferZeitDouble();
        anzAuftraege++;
        jetztAuftrag = jetztAuftrag->getNext();
    }
    double durchschnitt = summeZeit / anzAuftraege;
    string test = sekToHrMin(durchschnitt);
    cout << "Durchschnittliche Lieferzeit: " << sekToHrMin(durchschnitt) << endl;
    buffer += "Durchschnittliche Lieferzeit: " + sekToHrMin(durchschnitt) + "\n";

    char dateiname[50];
    cout << "Bitte geben Sie einen Dateinamen f\201r die Ausgabgsdatei ein: ";
    cin >> dateiname;
    ofstream datei(dateiname);
    if (datei.is_open())
    {
        datei << buffer;
    }
}

void clUI::exportieren()
{
    char dateiname[50];
    cout << "Bitte geben Sie einen Dateinamen f\201r die Ausgabgsdatei ein: ";
    cin >> dateiname;
    ofstream datei(dateiname);
    if (datei.is_open())
    {
        ArtikelInventar *jetzt = inventar;
        inventar->speichern(datei);
        while(jetzt->getNext() != NULL)
        {
            jetzt = jetzt->getNext();
            jetzt->speichern(datei);
        }
    }
}

void clUI::exportieren(vector<ArtikelInventar*> in)
{
    char dateiname[50];
    cout << "Bitte geben Sie einen Dateinamen f\201r die Ausgabgsdatei ein: ";
    cin >> dateiname;
    ofstream datei(dateiname);
    if (datei.is_open())
    {
        for(int i = 0; i < in.size(); i++) in[i]->speichern(datei);
        datei.close();
    }
}

void druckeOptionen(string optionen[], int anz){
    for (int i = 0; i < anz; i++){
        cout << "    (" << i+1 << ") " << optionen[i] << endl;
    }
}

string sekToHrMin(double sek)
{
    int minutes = sek / 60;
    int hours = minutes / 60;
    minutes = minutes % 60;

    string buffer = "";
    if(hours < 10) buffer += "0";
    char b[3];
    sprintf(b, "%d", hours);
    buffer += b;
    buffer += ":";
    if(minutes < 10) buffer += "0";
    sprintf(b, "%d", minutes);
    buffer += b;
    return buffer;
}
