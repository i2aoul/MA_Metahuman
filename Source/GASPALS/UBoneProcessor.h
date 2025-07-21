#pragma once

#include "CoreMinimal.h"
#include "UBoneProcessor.generated.h"

UCLASS()
class GASPALS_API UBoneProcessor : public UObject
{
    GENERATED_BODY()

public:
    /**
     * Initialisiert das System mit dem aktuellen World-Kontext und einer Liste von Schlüsselwörtern,
     * die zur Filterung relevanter Bones verwendet werden.
     * @param WorldContext - Zeiger auf die aktuelle Spielwelt.
     * @param FilterKeywords - Liste von Schlüsselwörtern zur Auswahl bestimmter Bone-Namen.
     */
    void Initialize(UWorld* WorldContext, const TArray<FString>& FilterKeywords);

    /**
     * Verarbeitet alle Charaktere in der Szene:
     *  - Extrahiert die 3D-Koordinaten der Bones und schreibt sie in die Datei Csv3D.
     *  - Projiziert die 3D-Koordinaten auf 2D-Bildebene und schreibt sie in die Datei Csv2D.
     *  - Optional: Bildaufnahme je nach Konfiguration möglich.
     * @param FrameIndex - Der aktuelle Frame-Index zur Synchronisation der Daten.
     * @param Csv2D - Dateihandle für die 2D-Koordinatendatei (CSV).
     * @param Csv3D - Dateihandle für die 3D-Koordinatendatei (CSV).
     */
    void ProcessAllCharacters(int32 FrameIndex, IFileHandle* Csv2D, IFileHandle* Csv3D);

    /** Wenn true, werden pro Frame Bilder aufgenommen und gespeichert */
    UPROPERTY()
    bool bCaptureImages = false;

    /** Verzeichnis, in dem die aufgenommenen Bilder gespeichert werden */
    UPROPERTY()
    FString OutputDirectory;

    /** Helferobjekt, das für das Aufnehmen von Bildern verwendet wird */
    UPROPERTY()
    class UImageCapture* ImageCapture;


private:
    UWorld* WorldContext = nullptr;
    TArray<FString> FilterKeywords;
};
