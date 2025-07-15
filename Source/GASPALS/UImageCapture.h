// UImageCapture.h

#pragma once

#include "CoreMinimal.h"
#include "UImageCapture.generated.h"

class USceneCaptureComponent2D;

UCLASS()
class GASPALS_API UImageCapture : public UObject
{
    GENERATED_BODY()

public:

    UFUNCTION()
    static void Capture(USceneCaptureComponent2D* CaptureComp, const FString& Directory, const FString& FileName = TEXT(""));
    void CompressImageArrayToPNG(
        int32 Width,
        int32 Height,
        const TArray<FColor>& RawRGBA,
        TArray<uint8>& OutPng
    );

    bool SaveArrayToFile(
        const TArray<uint8>& Data,
        const FString& FilePath
    );
    static int32 GetNextImageIndex(const FString& Directory, const FString& Prefix, const FString& Extension);
};
