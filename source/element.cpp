using namespace std;
#include <string.h>
#include <iostream>
#include "element.h"

ClElement::ClElement()
{
    zahlNaechste=0;
    zahlAttribute=0;
    status=frei;
}

ClElement::ClElement(char *string)
{
    zahlNaechste=0;
    zahlAttribute=0;
    strcpy(name,string);
    status=frei;
}

ClElement *ClElement::verarbeite(ifstream& datei)
{
    char zeichen, letztes;
    char puffer[100];
    int zaehler;
    enum ezustand zustand = noise;
    ClElement *jetzt=NULL, *wurzel=NULL, *neu;

    for (datei.get(zeichen);!datei.eof();datei.get(zeichen))
    {
        switch(zeichen)
        {
        case '<':
            zustand=direktive;
            zaehler=0;
            break;
        case '>':
            if (letztes!=' ')
            {
                puffer[zaehler]='\0';
                switch(zustand)
                {
                case abhaengigElement:
                    jetzt->addTag(puffer);
                    break;
                case abhaengigAttribute:
                    jetzt->addAttribute(puffer);
                    break;
                case element:
                    //wofür ist dieser teil?
                    //neu=new ClElement(puffer);
                    //if (jetzt!=NULL) jetzt->setNext(neu);
                    //else wurzel=neu;
                    //jetzt=neu;
                    //zustand=abhaengigElement;
                    break;
                }
            }
            zustand=noise;
            break;
        case ' ':
            if (letztes==' ') continue;
            puffer[zaehler]='\0';
            zaehler=0;
            switch(zustand)
            {
            case direktive:
                if (!strcmp(puffer,"!ELEMENT")) zustand=element;
                else if (!strcmp(puffer,"!ATTLIST")) zustand=attribute;
                else
                {
                    cout << endl << "Diese Direktive verstehe ich nicht: " << puffer;
                    zustand=noise;
                }
                break;
            case element:
                if(wurzel==NULL){
                    neu = new ClElement(puffer);
                    wurzel = neu;
                } else {
                    neu = wurzel->sucheElement(puffer);
                    if(neu==NULL){
                        cout << "Element " << puffer << " nicht vorher deklariert." << endl;
                    }
                }
                jetzt=neu;
                zustand=abhaengigElement;
                break;
            case attribute:
                if (wurzel==NULL) neu=NULL;
                else neu=wurzel->sucheElement(puffer);
                if (neu==NULL)
                {
                    cout << endl << "Es gibt kein Element mit dem Namen: " << puffer << endl;
                    zustand=noise;
                }
                else
                {
                    zustand=abhaengigAttribute;
                    jetzt=neu;
                }
                break;
            case abhaengigElement:
                jetzt->addTag(puffer);
                break;
            case abhaengigAttribute:
                jetzt->addAttribute(puffer);
                break;
            }
            break;
        default:
            if (zustand!=noise) puffer[zaehler] = zeichen;
            zaehler++;
            break;
        }
        letztes=zeichen;
    }

    return wurzel;
}

void ClElement::addTag(
        char                  *string)
{
    naechste[zahlNaechste] = new ClElement(string);
    zahlNaechste++;
}

void ClElement::addAttribute(char *string)
{
    strcpy(attributes[zahlAttribute],string);
    zahlAttribute++;
}

void ClElement::druckeElement(int ebene, ClElement *wurzel)
{
    int j,i;
    ClElement *abhaengig;

    status=inBearbeitung;
    for (j=0;j<ebene;j++) cout << "|   ";
    cout << "Element " << name << " erlaubt die Attribute: " << endl;
    for (i=0;i<zahlAttribute;i++)
    {
        for (j=0;j<ebene;j++) cout << "|   ";
        cout << "  -" << attributes[i] << endl;
    }
    for (j=0;j<ebene;j++) cout << "|   ";
    cout << "und enthält die Tags: " << endl;
    for (i=0;i<zahlNaechste;i++)
    {
        for (j=0;j<ebene;j++) cout << "|   ";
        cout << "   " << naechste[i]->getName() << endl;
        abhaengig=wurzel->sucheElement(naechste[i]->getName());
        if (abhaengig != NULL) abhaengig->druckeElement(ebene+1,wurzel);
    }
    status=frei;
}

ClElement *ClElement::sucheElement(char *suche)
{
    ClElement *resultat;
    if (!strcmp(suche,name))
        return this;
    for(int i = 0; i<zahlNaechste; i++){
        resultat = naechste[i]->sucheElement(suche);
        if (resultat!=NULL) return resultat;
    }
    return NULL;
}

//--------------------------------------------------------

int ClElement::elementIstErlaubt(
        char                            *name)
{
    for (int i=0;i<zahlNaechste;i++)
        if (!strcmp(name,naechste[i]->getName())) return i;

    return -1;
}

int ClElement::attributeIstErlaubt(
        char                            *name)
{
    for (int i=0;i<zahlAttribute;i++){
        if (!strcmp(name,attributes[i])) return i;
    }

    return -1;
}

