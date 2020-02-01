# Whizzle

## Kurze Zusammenfassung

Umsetzung des Kartenspiels Wizard von Ken Fisher erschienen im Jahr 1984 in Anlehnung an der Deutschen Version vom Verlag Amigo.


## Detaillierte Beschreibung/Auflistung der Funktionalität

* Drei bis sechs Spieler sollen ueber ein Netzwerk miteinander spielen können (Peer-to-peer) [Netwerk Multiplayer]
* Spieler sind in der Lage im Spiel miteinander über ein Chat kommunizieren [Ingame Chat (Netzwerk)]
* Navigation wird mittels eines Menues realsiiert [Menü]
* Spiel verfuegt ueber Animationen [Einfache Animationen]
* Das Spiel ist Betriebssystemunabhängig (Linux, Windows, macOS) [Betriebssystemunabhängig]
* Das Spiel verwendet ueber Sound [Sound]
* (Optional) Controllerunterstützung


## Externe Bibliotheken/Tools

* Git - Versionscontrolle
* CMake - Build Tool
* SFML - Multimedia-Framework
* Boost - General-purpose library
* Perf - Profiler


## Besondere Anforderungen

Zur Wiedergabe und Ablauf des Spieles muessen die einzelnen Spieler miteinander synchronisiert werden. 
Bei einer unstabilien Netzwerk Verbindung muessen Wiederverbindungen eingeleitet werden, um das Spiel weiterfuehren zu koennen.


## Author
* **Till Vogel** - *Student* - [Technische Universitaet Braunschweig](https://www.tu-braunschweig.de/)


## License

This project is licensed under the GPL License - see the [LICENSE.md](LICENSE.md) file for details

