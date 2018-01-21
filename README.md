# fPost
fPost – CLI für den Getränkeversand

Das CLI für den Getränkeversand „fPost“ leistet Inventar- sowie Auftragsmanagement. Das Programm bietet die Möglichkeit, das Inventar nach Variablen wie Artikelnummer, Name oder Preis zu durchsuchen. Eine Verbindung mit dem Auftragsverlauf lässt sich das noch vorhandene Inventar, die Lieferzeit und Nachfrage berechnen. Die folgenden XML-Dokumente werden Importiert:

Produkt-Liste (mit DTD validiert):

	•	Produktname
	•	Artikelnummer
	•	Anzahl Flaschen in Kasten
	•	Preis
	•	Angebotspreis (optional)
	•	Initial-Inventar (Anzahl)
	
Auftragsverlauf:

	•	Auftragsnummer
	•	Kundenname
	•	Lieferdresse
	•	Bestellte Produkte, Anzahl
	•	Gesamtpreis (berechnet aus bestellte Produkte)
	•	Bestellzeitpunkt
	•	Lieferzeitpunkt
	
Diese Dokumente werden zu der folgenden XML-Instanz zusammengefügt:

	•	Aktualisiertes Inventar (berechnet aus bestellten Produkten)
	•	Lieferzeit (berechnet aus Bestellzeitpunkt und Lieferzeitpunkt)
	•	Nachfrage (Anzahl der Bestellungen, berechnet aus bestellten Produkten)
