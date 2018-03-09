#ifndef CLTOKEN_H
#define CLTOKEN_H

#include "attributes.h"
#include <fstream>
#include <vector>

class ClToken
   {
public:
   ClToken();
   char *name() { return tokenName; }
   char *inhalt() { return tokenInhalt; }
   ClToken *getNaechstes(int i);
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
   std::vector<ClToken*> naechste;
   int zahlNaechste;
   char *tokenInhalt;
   } ;

enum zustand { istStartTag, istEndTag } ;

#endif /* CLTOKEN_H */
