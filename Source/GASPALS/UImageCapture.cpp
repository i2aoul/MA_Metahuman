// ImageCapture.cpp

#include "UImageCapture.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UImageCapture::Capture(USceneCaptureComponent2D* CaptureComp, const FString& Directory, const FString& FileName)
{
    if (!CaptureComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("ImageCapture: No SceneCaptureComponent provided."));
        return;
    }

    UTextureRenderTarget2D* RenderTarget = CaptureComp->TextureTarget;
    if (!RenderTarget)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("RenderTarget ist  %d ."), !RenderTarget));
        return;
    }

    FTextureRenderTargetResource* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();
    if (!RenderTargetResource)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("RenderTargetResource ist %d bones."), !RenderTargetResource));
        return;
    }

    // Pixel-Daten lesen
    TArray<FColor> Bitmap;
    FIntPoint RenderTargetSize(RenderTarget->SizeX, RenderTarget->SizeY);
    if (!RenderTargetResource->ReadPixels(Bitmap))
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Processed %d bones."), !RenderTargetResource->ReadPixels(Bitmap)));
        return;
    }

    // Textur aus Pixel-Daten erstellen
    UTexture2D* CapturedTexture = UTexture2D::CreateTransient(RenderTargetSize.X, RenderTargetSize.Y, PF_B8G8R8A8);
    if (!CapturedTexture)
    {
        UE_LOG(LogTemp, Error, TEXT("Fehler beim Erstellen einer transienten Textur."));
        return;
    }

    // Pixel-Daten in die Textur kopieren
    FTexture2DMipMap& Mip = CapturedTexture->GetPlatformData()->Mips[0];
    void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memcpy(TextureData, Bitmap.GetData(), Bitmap.Num() * sizeof(FColor));
    Mip.BulkData.Unlock();
    CapturedTexture->UpdateResource();

    // Optional: Speichern als PNG-Datei
    FString FullPath = FPaths::Combine(Directory / TEXT("IMG"), FileName);
    if (FFileHelper::CreateBitmap(*FullPath, RenderTargetSize.X, RenderTargetSize.Y, Bitmap.GetData()))
    {


    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Bild erfolgreich gespeichert unter: %s"), *FullPath));
    }
}

    int32 UImageCapture::GetNextImageIndex(const FString& Directory, const FString& Prefix, const FString& Extension)
    {
    // Stelle sicher, dass Directory mit Slash endet
    FString Dir = Directory;
    if (!Dir.EndsWith(TEXT("/")) && !Dir.EndsWith(TEXT("\\")))
    {
        Dir += TEXT("/");
    }

    // Liste aller Dateien im Ordner
    TArray<FString> Files;
    IFileManager::Get().FindFiles(Files, *Dir, *Extension.Mid(1)); // z.B. "png" ohne Punkt

    int32 MaxIndex = 0;
    // Regex oder manuelles Parsen: wir erwarten Dateinamen wie "Shot_001.png"
    for (const FString& FileName : Files)
    {
        if (!FileName.StartsWith(Prefix) || !FileName.EndsWith(Extension))
            continue;

        // Schneide Prefix und Extension weg
        FString NumberPart = FileName.Mid(
            Prefix.Len(),
            FileName.Len() - Prefix.Len() - Extension.Len()
        );

        // Versuche, die Zahl zu parsen
        int32 FoundIndex = FCString::Atoi(*NumberPart);
        MaxIndex = FMath::Max(MaxIndex, FoundIndex);
    }

    return MaxIndex + 1;
}