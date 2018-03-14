# fPost
fPost – CLI für den Getränkeversand
https://github.com/stereolith/fPost
> Projekt im Rahmen des Seminars "C++ - Programmieren - 1" im Wintersemester 2017/ 2018.

## Dokumentation

fPost ist ein Managementsystem für einen Getränkeversand.
Daten in Form einer Artikelliste (xml) lassen sich mit einer Auftragsliste (xml) zu einer ArtikelInventar-Liste zusammenführen, 
die eine erweiterte Artikelliste mit aktualisiertem Inventar und weiteren Informationen ist. Die zusammengeführte Datei lässt sich
wiederum als xml-Dokument ausgeben.

### Menüführung:
 
#### (1) Daten einlesen

 - ermöglicht, die Artikel- und Auftragsliste einzulesen. Die Daten müssen mit den dazugehörigen DTD-Dateien im Build-Verzeichnis  (oder anderes angegebenes Hauptverzeichnis) liegen. Beispieldaten sind im Ordner "data" zu finden.
   Die Standard-Dateinamen der Beispieldaten sind:
	 - artikel.dtd
	 - artikel.xml
	 - auftraege.dtd
	 - auftraege.xml


#### (2) Daten konvertieren
 - 	führt beide eingelesenen Dateien zusammen und speichert diese in Form einer XML-Instanz unter einem beliebigen Dateinamen.


#### (3) Daten durchsuchen
 - erlaubt, die Daten nach Termen und Kategorien zu durchsuchen
	 1. Kategorie eingeben (z.B. name) 
	 2. Suchterm eingebn (z.B. Fanta)
	 3. ggf. neuen Suchterm hinzufügen mit Eingabe von "j", um Suche einzuschränken (z.B. "normalpreis": "10,99")
		---sonst mit "n" Suche bestätigen
	4. Die Ergebnisse werden in der Konsole ausgegeben. Die Ergebnisse können mit der Option (4) im Hauptmenü exportiert
		werden.

#### (4) Daten exportieren
	

 - (1) Gesamte InventarArtikel-Liste exportieren
		 - Gibt die Möglichkeit, die gesamten zusammengeführten Daten in einer Datei als XML-Instanz zu speichern
 - (2) Letzte Suchergebnisse exporteiren
		 - Gibt die Möglichkeit, die Liste der letzten Suchergebnisse in einer Datei als XML-Instanz zu speichern

#### (5) Statistiken

 - Zeigt einige Statistiken an, die aus den zusammengeführten Daten berechnet werden:
		 - Anzahl Kästen, Flaschen und das Gesamtvolumen im Inventar
		 - Verkaufte Kästen nach dem letzten Auftragsimport (Anzahl verkaufter Kästen in der importierten Auftragsdatei)
		 - Durchschnittliche Lieferzeit

Die Statistiken können auch in einer Ausgabedatei gespeichert werden.

#### (6) Beenden

 - Beendet das Programm

## Funktionsweise

#### Import der Artikel- und Auftragsliste 

 - Beide XML-Instanzen werden per DTD validiert `Artikel::fill(ClToken  *wurzel,  int  zaehler)`  und `Auftrag::fill(ClToken  *wurzel,  int  zaehler)`
 - Die Daten in mehrere artikel und auftrag-Objekte überführt, die untereinander als linked list organisiert sind 
 - Zahlenwerte wie angebots- und normalpreis werden in double-Variablen konvertiert
 - Zeitangaben aus der Auftragsliste werden zu [time_t-Objekten](http://www.cplusplus.com/reference/ctime/time/) konvertiert (zur einfacheren Berechnung der Lieferzeit)
 - Lieferzeit wird mit Hilfe der difftime-Funktion in Sekunden in der Variable lieferzeit gespeichert


#### Konvertierung /Zusammenführung

 - artikel- und auftrag-Objekte werden in ArtikelInventar-Objekten zusammengeführt (jedes ArtikelInventar-Objekt repräsentiert einen Artikel):	
 	- Daten aus artikel-Objekten werden in ArtikelInventar-Objekte kopiert (z.B. name, normalpreis, angebotspreis) `ArtikelInventar::kopiere(Artikel  *artikel)`
	 - Informationen zu Häufigkeit der Bestellung und daraus berechnetes aktualisiertes Inventar werden aus Information der auftrag-Objekte den ArtikelInventar-Objekten hinzugefügt `ArtikelInventar::verarbeite(Auftrag  *auftrag)`
	 - Synchronisierte Vectoren mit Daten dazu, welche Kunden dieses Produkt bestellt haben, wird mit Kundennummer und Bestellanzahl hinzugefügt `ArtikelInventar::verarbeite(Auftrag  *auftrag)`
	 - Ausgabe/ Export der Daten als XML-Instanz `ArtikelInventar::speichern(ofstream  &datei)`

#### Suche

 - Suche erfragt Kategorie und Suchterm
 - Suchterm wird in der der Kategorie entsprechenden Variable in jedem ArtikelInventar-Objekt verglichen: Funktion `ArtikelInventar::suche()`
 - Bei jedem positiven Vergleich wird ein Pointer des gefundenen ArtikelInventar-Obejtes in einem Vector hinzugefügt
 - Bei dem Hinzufügen eines weiteren Suchterms wird anhand der überladenen suchen()-Funktion jedes ArtikelInventar-Obejet aus dem Ergebnis-Vector des vorherigen Schrittes anhand des neuen Suchterms durchsucht, dann wird wieder ein Vector mit den Ergebnissen zusammengestellt
 - Wird kein weiterer Suchterm mehr angegeben, werden die ArtikelInventar-Obejekte aus dem Ergebnis-Vektor ausgegeben
 - Ein exportieren der Ergebnisse ist über Option (4) möglich, da der letzte Ergebnis-Vektor immer gespeichert wird.

#### Exportieren

 - Variablwerte der ArtikelInventar-Objekte werden in Form einer XML-Instanz in eine Datei geschrieben.
 - `ArtikelInventar::speichern(ofstream  &datei)` und Helferfunktion `stringToken()` in ArtikelInventar-Klasse sorgt für entsprechende Formatierung und Einrückung

#### Statistiken
 - Daten wie Anzahl Verkaufter Kästen und Anzahl von Kästen/Flaschen im Inventar werden aus allen ArtikelInventar-Objekten berechnet `clUI::statistiken()`
 - Die Durchschnittliche Lieferzeit wird aus allen auftrag-Objekten berechnet
 - Statistiken können exportiert werden
