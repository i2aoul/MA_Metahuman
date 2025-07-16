#pragma once

#include "CoreMinimal.h"

namespace CsvUtils
{
    /**
     * Speichert 3D-Koordinaten aus zwei Bone-Caches in eine CSV-Datei.
     *
     * @param FilePath Pfad zur Zieldatei (inkl. Dateiname und .csv-Endung).
     * @param BoneCache1 Erster Bone-Cache mit 3D-Koordinaten (z. B. Charakter A).
     * @param BoneCache2 Zweiter Bone-Cache mit 3D-Koordinaten (z. B. Charakter B).
     */
    void SaveCached3DDataToCSVs(const FString& FilePath, const TMap<int32, FString>& BoneCache1, const TMap<int32, FString>& BoneCache2);

    /**
     * Speichert 2D-Koordinaten aus zwei Bone-Caches in eine CSV-Datei.
     *
     * @param FilePath Pfad zur Zieldatei (inkl. Dateiname und .csv-Endung).
     * @param BoneCache1 Erster Bone-Cache mit 2D-Koordinaten (z. B. Charakter A).
     * @param BoneCache2 Zweiter Bone-Cache mit 2D-Koordinaten (z. B. Charakter B).
     */
    void SaveCached2DDataToCSVs(const FString& FilePath, const TMap<int32, FString>& BoneCache1, const TMap<int32, FString>& BoneCache2);

    /**
     * Schreibt eine einzelne Gleitkommazahl (z. B. durchschnittliche FPS) in eine Textdatei.
     *
     * @param Number Die zu speichernde Zahl (z. B. AVG-FPS).
     * @param FilePath Pfad zur Zieldatei (inkl. Dateiname und .txt-Endung).
     */
    void WriteNumberToFile(float Number, const FString& FilePath);
}
