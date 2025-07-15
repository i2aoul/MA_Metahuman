// UBoneProcessor.h

#pragma once

#include "CoreMinimal.h"
#include "UBoneProcessor.generated.h"

UCLASS()
class GASPALS_API UBoneProcessor : public UObject
{
    GENERATED_BODY()

public:
    /** Initialize with world context and the list of bone-name filters */
    void Initialize(UWorld* WorldContext, const TArray<FString>& FilterKeywords);

    /**
     * Processes all characters in the scene:
     *  - extracts 3D bone positions and writes to Csv3D
     *  - projects to 2D and writes to Csv2D
     *  - optionally captures images
     */
    void ProcessAllCharacters(int32 FrameIndex, IFileHandle* Csv2D, IFileHandle* Csv3D);

    /** If true, will capture and save images each frame */
    UPROPERTY()
    bool bCaptureImages = false;

    /** Directory where captured images will be saved */
    UPROPERTY()
    FString OutputDirectory;

    /** Helper object used to capture images */
    UPROPERTY()
    class UImageCapture* ImageCapture;

private:
    UWorld* WorldContext = nullptr;
    TArray<FString> FilterKeywords;
};
