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
    TArray<FColor> Bitmap;
    FIntPoint RenderTargetSize(RenderTarget->SizeX, RenderTarget->SizeY);
    if (!RenderTargetResource->ReadPixels(Bitmap))
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Processed %d bones."), !RenderTargetResource->ReadPixels(Bitmap)));
        return;
    }
    UTexture2D* CapturedTexture = UTexture2D::CreateTransient(RenderTargetSize.X, RenderTargetSize.Y, PF_B8G8R8A8);
    if (!CapturedTexture)
    {
        UE_LOG(LogTemp, Error, TEXT("Fehler beim Erstellen einer transienten Textur."));
        return;
    }
    FTexture2DMipMap& Mip = CapturedTexture->GetPlatformData()->Mips[0];
    void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memcpy(TextureData, Bitmap.GetData(), Bitmap.Num() * sizeof(FColor));
    Mip.BulkData.Unlock();
    CapturedTexture->UpdateResource();
    FString FullPath = FPaths::Combine(Directory / TEXT("IMG"), FileName);
    if (FFileHelper::CreateBitmap(*FullPath, RenderTargetSize.X, RenderTargetSize.Y, Bitmap.GetData()))
    {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Bild erfolgreich gespeichert unter: %s"), *FullPath));
	}
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Bild erfolgreich gespeichert unter: %s"), *FullPath));
    }
}

int32 UImageCapture::GetNextImageIndex(const FString& Directory, const FString& Prefix, const FString& Extension)
    {
    FString Dir = Directory;
    if (!Dir.EndsWith(TEXT("/")) && !Dir.EndsWith(TEXT("\\")))
    {
        Dir += TEXT("/");
    }
    TArray<FString> Files;
    IFileManager::Get().FindFiles(Files, *Dir, *Extension.Mid(1)); 
    int32 MaxIndex = 0;
    for (const FString& FileName : Files)
    {
        if (!FileName.StartsWith(Prefix) || !FileName.EndsWith(Extension))
            continue;
        FString NumberPart = FileName.Mid(
            Prefix.Len(),
            FileName.Len() - Prefix.Len() - Extension.Len()
        );
        int32 FoundIndex = FCString::Atoi(*NumberPart);
        MaxIndex = FMath::Max(MaxIndex, FoundIndex);
    }

    return MaxIndex + 1;
}