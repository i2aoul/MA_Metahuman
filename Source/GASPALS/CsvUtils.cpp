#include "CsvUtils.h"
#include <fstream>
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
namespace CsvUtils
{
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

    static void TransposeCache3D(const TMap<int32, FString>& Cache1, const TMap<int32, FString>& Cache2, TArray<TArray<FString>>& OutData)
    {
        int Offset = OutData.Num(); 

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

    static void WriteCSV(const FString& FilePath, const TArray<FString>& Headers, const TArray<TArray<FString>>& Data)
    {
        FString Dir = FPaths::GetPath(FilePath);
        if (!IFileManager::Get().DirectoryExists(*Dir))
        {
            IFileManager::Get().MakeDirectory(*Dir, true);
        }
        std::ofstream File(TCHAR_TO_UTF8(*FilePath));
        if (!File.is_open())
        {
            UE_LOG(LogTemp, Error, TEXT("CsvUtils: Konnte Datei nicht öffnen: %s"), *FilePath);
            return;
        }
        UE_LOG(LogTemp, Log, TEXT("CsvUtils: Schreibe CSV mit %d Spalten und %d Zeilen nach %s"),
            Headers.Num(), Data.Num(), *FilePath);
        for (auto& H : Headers)   File << TCHAR_TO_UTF8(*H) << ";";
        File << "\n";
        for (int32 R = 0; R < Data.Num(); ++R)
        {
            File << (R + 1) << ";";
            for (auto& Val : Data[R])    File << TCHAR_TO_UTF8(*Val) << ";";
            File << "\n";
        }
    }

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
        FString Dir = FPaths::GetPath(FilePath);
        if (!IFileManager::Get().DirectoryExists(*Dir))
        {
            IFileManager::Get().MakeDirectory(*Dir, true);
        }
        FString Text = FString::SanitizeFloat(Number);
        FFileHelper::SaveStringToFile(Text + LINE_TERMINATOR, *FilePath,
            FFileHelper::EEncodingOptions::AutoDetect,
            &IFileManager::Get(),
            EFileWrite::FILEWRITE_Append);
    }   
 }

   