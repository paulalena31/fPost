#include "clui.h"

string optionen[4] = {
    "Artikelliste einlesen",
    "B",
    "C",
    "Beenden"
};

clUI::clUI()
{
    breite = 50;
    hoehe = 20;
    wurzel = NULL;
    token = NULL;
}
void clUI::mainMenu()
{
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

    for (int i = 0; i < breite; i++) cout << "-";
    for (int i = 0; i < 7; i++) cout << endl << logo[i];
    cout << endl;
    for (int i = 0; i < 4; i++){
        cout << "    (" << i+1 << ") " << optionen[i] << endl;
    }
    for (int i = 0; i < breite; i++) cout << "-";
    cout << endl;

    char auswahl;
    cin >> auswahl;
    switch(auswahl)
    {
    case'1':
        leseDaten();
        mainMenu();
        break;
    case'4':
        break;
    default:
        cout << "Bitte wählen Sie eine Option aus." << endl;
        mainMenu();
        break;
    }

}

ClToken *clUI::leseDaten()
{
    if (token != NULL && wurzel != NULL)
    {
        cout << "Es wurden bereits Daten eingelesen. Möchten Sie die Daten überschreiben? (j/n)" << endl;
        char *antwort;
        cin >> antwort;
        if (strcmp(antwort,"j") == 0) {
            token = NULL;
            wurzel = NULL;
        } else if (strcmp(antwort,"n") == 0) {
            return NULL;
        }
    }

    char dateiname[100];
    ifstream eingabe;
    wurzel = new ClElement();

    cout << "DTD-Dateiname: " << endl;
    cin >> dateiname;
    eingabe.open(dateiname);

    wurzel=wurzel->verarbeite(eingabe);
    //wurzel->druckeElement(0,wurzel);
    cout << "DTD geladen." << endl;
    eingabe.close();

    cout << "XML-Dateiname:" << endl;
    cin >> dateiname;
    eingabe.open(dateiname);

    token=new ClToken;

    if (token->getToken(eingabe,NULL,wurzel)!=0) cout << "XML erfolgreich geladen." << endl;
    eingabe.close();

}
