enum status {frei, inBearbeitung};
enum ezustand {direktive, element, attribute, abhaengigElement,
               abhaengigAttribute, noise};
using namespace std;
#include <fstream>

class ClElement
   {
public:
   ClElement();
   ClElement(char *string);
   ClElement *verarbeite(ifstream& datei);
   char *getName(void) { return name; }
   void addTag(char * string);
   void addAttribute(char * string);
   int getAnzahlTags() { return zahlNaechste; }
   void druckeElement(int ebene,ClElement *wurzel);
   ClElement *sucheElement(char *suche);
   int elementIstErlaubt(char *name);
   int attributeIstErlaubt(char *name);
private:
   int zahlNaechste;
   char name[64];
   ClElement *naechste[10];
   enum status status;
   int zahlAttribute;
   char attributes[10][64];
   } ;
