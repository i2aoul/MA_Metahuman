#pragma once

#include "CoreMinimal.h"
#include "UImageCapture.generated.h"

class USceneCaptureComponent2D;

UCLASS()
class GASPALS_API UImageCapture : public UObject
{
    GENERATED_BODY()

public:

    /**
     * Erstellt einen Screenshot über eine SceneCaptureComponent2D und speichert das Bild als Datei.
     * @param CaptureComp - Die Capture-Komponente, die das Bild rendert.
     * @param Directory - Das Zielverzeichnis für die Bilddatei.
     * @param FileName - Optionaler Dateiname (wenn leer, wird ein Index verwendet).
     */
    UFUNCTION()
    static void Capture(USceneCaptureComponent2D* CaptureComp, const FString& Directory, const FString& FileName = TEXT(""));

    /**
     * Komprimiert ein RGBA-Bildarray in das PNG-Format.
     * @param Width - Bildbreite in Pixeln.
     * @param Height - Bildhöhe in Pixeln.
     * @param RawRGBA - Rohdaten des Bildes im RGBA-Format.
     * @param OutPng - Ausgabe-Array mit den PNG-kodierten Bytes.
     */
    void CompressImageArrayToPNG(int32 Width, int32 Height, const TArray<FColor>& RawRGBA, TArray<uint8>& OutPng);

    /**
     * Speichert ein Byte-Array in eine Datei.
     * @param Data - Die zu speichernden Binärdaten.
     * @param FilePath - Der vollständige Pfad zur Zieldatei.
     * @return true bei Erfolg, false bei Fehler.
     */
    bool SaveArrayToFile(const TArray<uint8>& Data, const FString& FilePath);

    /**
     * Ermittelt den nächsten freien Index für einen Dateinamen im angegebenen Verzeichnis.
     * @param Directory - Das Verzeichnis, in dem nach bestehenden Dateien gesucht wird.
     * @param Prefix - Der Präfix des Dateinamens.
     * @param Extension - Die Dateiendung (z. B. "png").
     * @return Der nächste verfügbare Index.
     */
    static int32 GetNextImageIndex(const FString& Directory, const FString& Prefix, const FString& Extension);

};
