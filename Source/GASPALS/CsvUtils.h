// CsvUtils.h
#pragma once

#include "CoreMinimal.h"

/**
 * Utility-Funktionen zum Speichern von Bone-Koordinaten in CSV-Dateien.
 */
namespace CsvUtils
{
    /** Speichert 3D-Koordinaten aus zwei Bone-Caches in eine CSV-Datei. */
    void SaveCached3DDataToCSVs(const FString& FilePath, const TMap<int32, FString>& BoneCache1, const TMap<int32, FString>& BoneCache2);

    /** Speichert 2D-Koordinaten aus zwei Bone-Caches in eine CSV-Datei. */
    void SaveCached2DDataToCSVs(const FString& FilePath, const TMap<int32, FString>& BoneCache1, const TMap<int32, FString>& BoneCache2);

    /** Speichert AVG-FPS der Anwendung. */
    void WriteNumberToFile(float Number, const FString& FilePath);
}
