# 📌 Projektübersicht

Dieses Projekt zielt darauf ab, die Genauigkeit von neuronalen Netzwerken zur **2D-Pose-Schätzung** zu verbessern, indem hochwertige synthetische Trainingsdaten mit **MetaHumans** erzeugt werden.

## 🔗 Verwendete Basis

Dieses Projekt basiert auf dem [**GASP-Projekt**](https://github.com/PolygonHive/GASP-ALS.git), welches als Grundlage für die Szenenlogik und Datenaufzeichnung dient.
Insbesondere wurde die Blueprint-Klasse `CBP_Sandbox_Character` erweitert. Diese ist im Projekt unter **Blueprints** zu finden.

---

## 🔄 Automatische Datengenerierung mit MetaHumans

Um mit der Datengenerierung zu beginnen, muss ein MetaHuman-Charakter heruntergeladen und entsprechend vorbereitet werden:

1. **MetaHuman vorbereiten:**

   * Öffne die **Klassen-Einstellungen** deines MetaHuman-Blueprints.
   * Wähle im **Details-Fenster** unter **Parent Class** die Klasse `SBP_SandboxCharacter` aus. Dadurch erbt der MetaHuman alle Funktionen des Sandbox-Charakters.

2. **Komponenten anpassen:**

   * Verschiebe den **Body** im **Komponenten-Fenster** unter das **Mesh**.
   * Passe die **Transformation** des Bodys so an, dass er in dieselbe Richtung wie das Mesh zeigt.

3. **Animation einrichten:**

   * Setze unter **Anim Class** die Klasse `ABP_Generic_Retarget`.

4. **Tag hinzufügen:**

   * Unter **Tags** einen neuen Tag im `Index[0]` mit dem Namen
     `"RTG_UEFN_to_Metahuman_nrw"` hinzufügen.

5. **LODSync konfigurieren:**

   * Wähle die Komponente `LODSync` im Komponentenbaum aus.
   * Setze im Details-Fenster den Wert **Num LODs** auf `4`, um alle benötigten LOD-Stufen korrekt zu synchronisieren.

6. **Blueprint-Events einfügen:**

   * Öffne den **Event Graph**.
   * Füge die Nodes **Parent: Tick** und **Parent: BeginPlay** ein.
   * Weitere Logik ist nicht notwendig.

7. **MetaHuman in Szene platzieren:**

   * Jetzt kann der vorbereitete MetaHuman in der Szene platziert und zur automatisierten Datengenerierung verwendet werden.

8. **Render Target Qualität anpassen:**

   * Wähle das **Render\_Target**-Objekt in deinem Level aus.
   * Im **Details-Fenster** findest du Einstellungen zur Auflösung und zum **Capture Quality**.
   * Passe die **Auflösung** (z. B. 1920x1080) und den **Capture Quality**-Wert (z. B. von Low, Medium bis High) an, um die Bildqualität der Aufnahmen zu optimieren.

---

## 🎬 Hintergrund anpassen

Um den Hintergrund der Szene zu verändern:

* Öffne das Material `MP_EX` (Medienplayer-Material).
* Setze im Property-Feld `backgroundTexture` den Pfad zu einem Bild oder Video.
* Falls ein Video nicht automatisch startet, **klicke es manuell im Medienplayer an**, um die Wiedergabe zu starten.

---

## 🧩 Datenannotation

Die Annotation der Pose-Daten erfolgt über die integrierte C++-Klasse `MeshCoords`.
Diese Klasse extrahiert automatisch die relevanten 2D-Koordinaten der Skelettpunkte während der Laufzeit und speichert sie für den Trainingsprozess.

**Speicherort:**

* Optional kann eine Systemvariable `METADATA_PATH` gesetzt werden, um die Ausgabe an einen benutzerdefinierten Pfad umzuleiten.
* Ist `METADATA_PATH` nicht definiert, werden die Annotierungsdaten standardmäßig im Projektordner unter `Saved/Metadata/` abgelegt.
