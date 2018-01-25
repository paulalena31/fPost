#include "attributes.h"
#include <fstream>

class ClToken
   {
public:
   ClToken();
   char *name() { return tokenName; }
   char *inhalt() { return tokenInhalt; }
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
