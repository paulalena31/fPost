#include <iostream>
using namespace std;
#include <fstream>
#include <string.h>
#include "token.h"

ClToken::ClToken()
{
    *tokenName='\0';
    zahlNaechste = 0;
    for(int i = 0; i < 10; i++) naechste[i] = NULL;
    tokenInhalt=new char[1];
    *tokenInhalt='\0';
}

void ClToken::addChildToken(){
    naechste[zahlNaechste] = new ClToken();
    zahlNaechste++;
}

//element: aktuelles Element in DTD-Baum; wurzel: Wurzel des DTD-Baumes
int  ClToken::getToken(
        ifstream              &datei,
        ClElement                   *element,
        ClElement                           *wurzel)
{
    int zaehler;
    enum zustand zustand;
    char zeichen;
    char *ziel, *quelle;
    char puffer[100];

    cleanToken();

    for (zaehler=0;;)
    {
        datei.get(zeichen);
        if (datei.eof())
        {
            if (*tokenName == '\0' && naechste[0] == NULL && tokenInhalt == NULL)
                return fillToken(0);
            return fillToken(1);
        }
        switch(zeichen)
        {
        case '<':
            datei.get(zeichen);
            if (zeichen=='/')
            {
                zustand = istEndTag;
                if (zaehler!=0)
                {
                    puffer[zaehler]='\0';
                    tokenInhalt = new char[zaehler+1];
                    strcpy(tokenInhalt,puffer);
                }
            }
            else
            {
                datei.putback(zeichen);
                if (*tokenName!='\0')
                {
                    //gehe eine Ebene tiefer
                    datei.putback('<');

                    addChildToken();
                    if (naechste[zahlNaechste-1]->getToken(datei,element,wurzel)==0)
                    {
                        return fillToken(0);
                    }
                }
                else zustand=istStartTag;
            }
            zaehler=0;
            break;
        case '>':
            puffer[zaehler]='\0';
            if (zustand==istEndTag)
            {
                if (strcmp(tokenName,puffer))
                {
                    cout << "Fehlerhaftes End Tag" << endl;
                    cout << "Erhalten: '" << puffer << "'; erwartet: '"
                         << tokenName << "'" << endl;
                    return fillToken(0);
                }
                return fillToken(1);
            }
            if (zustand==istStartTag)
            {
                for (ziel=tokenName,quelle=puffer;*quelle!=' ' && *quelle!= '\0';
                     ziel++,quelle++) *ziel=*quelle;
                *ziel='\0';
                if (!strcmp(tokenName,wurzel->getName())) element=wurzel;
                else
                {
                    if (element->elementIstErlaubt(tokenName) < 0)
                    {
                        cout << "Fehlerhaftes Start Tag: '" << tokenName << "'" << endl;
                        return fillToken(0);
                    } else {
                        element=wurzel->sucheElement(tokenName);
                    }
                }
                att.getAttList(puffer,element);
            }
            zaehler=0;
            break;
        case '\n':
            break;
        default:
            puffer[zaehler]=zeichen;
            zaehler++;
            break;
        }
    }
}

int ClToken::fillToken(
        int                    mode)
{
    if (*tokenName=='\0')
        strcpy(tokenName,"Unbekanntes Element");
    if (tokenInhalt==NULL)
    {
        tokenInhalt=new char[1];
        *tokenInhalt='\0';
    }

    return mode;
}

void ClToken::cleanToken(void)
{
    *tokenName='\0';
    if (tokenInhalt!=NULL)
    {
        delete tokenInhalt;
        tokenInhalt=NULL;
    }
}

void ClToken::druckeToken(
        int                       ebene)
{
    druckeTokenEbene(ebene);
    cout << "Token: " << name() << " - " << inhalt() << endl;
    if (att.zahlAtt() > 0)
    {
        for (int i=0;i<att.zahlAtt();i++)
        {
            druckeTokenEbene(ebene);
            cout << "Attribut " << att.zeigeAttName(i) << " hat den Wert "
                 << att.zeigeAttWert(i) << endl;
        }
    }
    //if (tokenChild!=NULL) tokenChild->druckeToken(ebene+1);
    //if (tokenSibling!=NULL) tokenSibling->druckeToken(ebene);
}

void ClToken::druckeTokenEbene(
        int                            ebene)
{
    while (ebene > 0)
    {
        cout << "| ";
        ebene = ebene - 1;
    }
}
