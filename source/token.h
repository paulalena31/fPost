#ifndef CLTOKEN_H
#define CLTOKEN_H

#include "attributes.h"
#include <fstream>

class ClToken
   {
public:
   ClToken();
   char *name() { return tokenName; }
   char *inhalt() { return tokenInhalt; }
   ClToken *getNaechstes(int i) {return naechste[i];}
   int getZahlNaechstes() {return zahlNaechste;}
   void addChildToken();
   void druckeToken(int ebene);
   int getToken(ifstream &datei,ClElement *element,ClElement *wurzel);
   ClattToken att;
private:
   void cleanToken();
   void druckeTokenEbene(int ebene);
   int fillToken(int mode);
   char tokenName[64];
   ClToken *naechste[10];
   int zahlNaechste;
   char *tokenInhalt;
   } ;

enum zustand { istStartTag, istEndTag } ;

#endif /* CLTOKEN_H */
