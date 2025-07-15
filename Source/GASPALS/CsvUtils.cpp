// CsvUtils.cpp
#include "CsvUtils.h"
#include <fstream>
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
namespace CsvUtils
{
    // --- Helper: Header-Generierung für 3D-Daten ---
    static void Add3DHeaders(const TMap<int32, FString>& Cache1, const TMap<int32, FString>& Cache2, TArray<FString>& OutHeaders)
    {
        for (const auto& Pair : Cache1)
        {
            TArray<FString> Parts;
            Pair.Value.ParseIntoArray(Parts, TEXT(";"), true);
            if (Parts.Num() > 1)
            {
                const FString& Name = Parts[1];
                OutHeaders.Add(Name + TEXT("-x"));
                OutHeaders.Add(Name + TEXT("-y"));
                OutHeaders.Add(Name + TEXT("-z"));
            }
        }
        for (const auto& Pair : Cache2)
        {
            TArray<FString> Parts;
            Pair.Value.ParseIntoArray(Parts, TEXT(";"), true);
            if (Parts.Num() > 1)
            {
                const FString& Name = Parts[1];
                OutHeaders.Add(Name + TEXT("-x"));
                OutHeaders.Add(Name + TEXT("-y"));
                OutHeaders.Add(Name + TEXT("-z"));
            }
        }
    }

    // --- Helper: Header-Generierung für 2D-Daten ---
    static void Add2DHeaders(const TMap<int32, FString>& Cache1, const TMap<int32, FString>& Cache2, TArray<FString>& OutHeaders)
    {
        for (const auto& Pair : Cache1)
        {
            TArray<FString> Parts;
            Pair.Value.ParseIntoArray(Parts, TEXT(";"), true);
            if (Parts.Num() > 1)
            {
                const FString& Name = Parts[1];
                OutHeaders.Add(Name + TEXT("-x"));
                OutHeaders.Add(Name + TEXT("-y"));
            }
        }
        for (const auto& Pair : Cache2)
        {
            TArray<FString> Parts;
            Pair.Value.ParseIntoArray(Parts, TEXT(";"), true);
            if (Parts.Num() > 1)
            {
                const FString& Name = Parts[1];
                OutHeaders.Add(Name + TEXT("-x"));
                OutHeaders.Add(Name + TEXT("-y"));
            }
        }
    }

    // --- Helper: Transponieren der 3D-Daten ---
    static void TransposeCache3D(const TMap<int32, FString>& Cache1, const TMap<int32, FString>& Cache2, TArray<TArray<FString>>& OutData)
    {
        int Offset = OutData.Num(); // bleibt 0, um das ursprüngliche Verhalten nicht zu ändern

        for (const auto& Pair : Cache1)
        {
            TArray<FString> Parts;
            Pair.Value.ParseIntoArray(Parts, TEXT(";"), true);
            if (Parts.Num() < 5) continue;

            for (int i = 2; i < Parts.Num(); i += 3)
            {
                int Row = (i - 2) / 3;
                if (OutData.Num() <= Row) OutData.AddDefaulted();
                OutData[Row].Add(Parts[i]);
                OutData[Row].Add(Parts[i + 1]);
                OutData[Row].Add(Parts[i + 2]);
            }
        }

        for (const auto& Pair : Cache2)
        {
            TArray<FString> Parts;
            Pair.Value.ParseIntoArray(Parts, TEXT(";"), true);
            if (Parts.Num() < 5) continue;

            for (int i = 2; i < Parts.Num(); i += 3)
            {
                int Row = Offset + (i - 2) / 3;
                if (OutData.Num() <= Row) OutData.AddDefaulted();
                OutData[Row].Add(Parts[i]);
                OutData[Row].Add(Parts[i + 1]);
                OutData[Row].Add(Parts[i + 2]);
            }
        }
    }

    // --- Helper: Transponieren der 2D-Daten ---
    static void TransposeCache2D(const TMap<int32, FString>& Cache1, const TMap<int32, FString>& Cache2, TArray<TArray<FString>>& OutData)
    {
        int Offset = OutData.Num();

        for (const auto& Pair : Cache1)
        {
            TArray<FString> Parts;
            Pair.Value.ParseIntoArray(Parts, TEXT(";"), true);
            if (Parts.Num() < 4) continue;

            for (int i = 2; i < Parts.Num(); i += 2)
            {
                int Row = (i - 2) / 2;
                if (OutData.Num() <= Row) OutData.AddDefaulted();
                OutData[Row].Add(Parts[i]);
                OutData[Row].Add(Parts[i + 1]);
            }
        }

        for (const auto& Pair : Cache2)
        {
            TArray<FString> Parts;
            Pair.Value.ParseIntoArray(Parts, TEXT(";"), true);
            if (Parts.Num() < 4) continue;

            for (int i = 2; i < Parts.Num(); i += 2)
            {
                int Row = Offset + (i - 2) / 2;
                if (OutData.Num() <= Row) OutData.AddDefaulted();
                OutData[Row].Add(Parts[i]);
                OutData[Row].Add(Parts[i + 1]);
            }
        }
    }

    // --- Helper: CSV schreiben ---
    static void WriteCSV(const FString& FilePath, const TArray<FString>& Headers, const TArray<TArray<FString>>& Data)
    {
        // 1) Verzeichnis anlegen (falls nötig)
        FString Dir = FPaths::GetPath(FilePath);
        if (!IFileManager::Get().DirectoryExists(*Dir))
        {
            IFileManager::Get().MakeDirectory(*Dir, true);
        }

        // 2) Datei öffnen und prüfen
        std::ofstream File(TCHAR_TO_UTF8(*FilePath));
        if (!File.is_open())
        {
            UE_LOG(LogTemp, Error, TEXT("CsvUtils: Konnte Datei nicht öffnen: %s"), *FilePath);
            return;
        }

        // Optional: Logge wie viele Spalten und Zeilen geschrieben werden
        UE_LOG(LogTemp, Log, TEXT("CsvUtils: Schreibe CSV mit %d Spalten und %d Zeilen nach %s"),
            Headers.Num(), Data.Num(), *FilePath);

        // Header
        for (auto& H : Headers)   File << TCHAR_TO_UTF8(*H) << ";";
        File << "\n";

        // Zeilen
        for (int32 R = 0; R < Data.Num(); ++R)
        {
            File << (R + 1) << ";";
            for (auto& Val : Data[R])    File << TCHAR_TO_UTF8(*Val) << ";";
            File << "\n";
        }
    }

    // --- Öffentliche Funktionen ---
    void SaveCached3DDataToCSVs(const FString& FilePath, const TMap<int32, FString>& BoneCache1, const TMap<int32, FString>& BoneCache2)
    {
        if (BoneCache1.Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("BoneCache ist leer. Keine Daten zum Speichern."));
            return;
        }

        TArray<FString> Headers;
        Headers.Add(TEXT("Frame"));
        Add3DHeaders(BoneCache1, BoneCache2, Headers);

        TArray<TArray<FString>> Transposed;
        TransposeCache3D(BoneCache1, BoneCache2, Transposed);

        WriteCSV(FilePath, Headers, Transposed);

        UE_LOG(LogTemp, Log, TEXT("Bone coordinates saved to %s"), *FilePath);
    }

    void SaveCached2DDataToCSVs(const FString& FilePath, const TMap<int32, FString>& BoneCache1, const TMap<int32, FString>& BoneCache2)
    {
        if (BoneCache1.Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("BoneCache ist leer. Keine Daten zum Speichern."));
            return;
        }

        TArray<FString> Headers;
        Headers.Add(TEXT("Frame"));
        Add2DHeaders(BoneCache1, BoneCache2, Headers);

        TArray<TArray<FString>> Transposed;
        TransposeCache2D(BoneCache1, BoneCache2, Transposed);

        WriteCSV(FilePath, Headers, Transposed);

        UE_LOG(LogTemp, Log, TEXT("2D Bone coordinates saved to %s"), *FilePath);
         }


    void WriteNumberToFile(float Number, const FString& FilePath)
    {
        // Stelle sicher, dass das Verzeichnis existiert
        FString Dir = FPaths::GetPath(FilePath);
        if (!IFileManager::Get().DirectoryExists(*Dir))
        {
            IFileManager::Get().MakeDirectory(*Dir, true);
        }

        // Konvertiere Zahl in FString (z.B. mit gewünschter Formatierung)
        FString Text = FString::SanitizeFloat(Number);

        // Schreib die Zahl (plus Newline) in die Datei (anhängen)
        FFileHelper::SaveStringToFile(Text + LINE_TERMINATOR, *FilePath,
            FFileHelper::EEncodingOptions::AutoDetect,
            &IFileManager::Get(),
            EFileWrite::FILEWRITE_Append);
    }   
 }

   