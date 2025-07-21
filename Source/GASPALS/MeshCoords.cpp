#include "MeshCoords.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h" 
#include "Rendering/SkeletalMeshRenderData.h" 
#include "Rendering/SkinWeightVertexBuffer.h"
#include "DrawDebugHelpers.h" 
#include <fstream>   
#include <sstream>    
#include <string>     
#include <iostream>   
#include <iomanip> 
#include "Containers/Array.h"
#include <fstream>
#include <sstream>
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h" 
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Async/Async.h"
#include "HAL/PlatformFilemanager.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include <array>
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "MathHelpers.h"
#include "UImageCapture.h"
#include "CsvUtils.h"
#include "Async/Async.h"
#include "ImageUtils.h"       
#include "Misc/FileHelper.h"  
#include "HAL/PlatformFilemanager.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "Misc/Paths.h"
#include "HAL/PlatformMisc.h"
AMeshCoords::AMeshCoords()
{
    PrimaryActorTick.bCanEverTick = true;
    Dir = FPlatformMisc::GetEnvironmentVariable(TEXT("METADATA_PATH"));
    if (Dir.IsEmpty())
    {
        Dir = FPaths::Combine(FPaths::ProjectDir(), TEXT("MetaData"));
    }
    FilterKeywords = {
      TEXT("neck_01"),
      TEXT("spine_01"),
      TEXT("FACIAL_C_Hair5"),
      TEXT("FACIAL_R_12IPV_NeckB4"),
      TEXT("wrist_outer_l"),
      TEXT("wrist_outer_r"),
      TEXT("lowerarm_in_l"),
      TEXT("lowerarm_in_r"),
      TEXT("clavicle_out_r"),
      TEXT("clavicle_out_l"),
      TEXT("ankle_bck_l"),
      TEXT("ankle_bck_r"),
      TEXT("calf_knee_l"),
      TEXT("calf_knee_r"),
      TEXT("thigh_r"),
      TEXT("thigh_l"),

    };
}

void AMeshCoords::BeginPlay()
{
    Super::BeginPlay();
    Initialize();
}

void AMeshCoords::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TickCounter++;
    int32 x = 0;
	if (TickCounter % x == 0)   // Nehme nur alle x Ticks die Daten auf, um die Performance zu verbessern
    {
        FindAllCharactersAndTheirComponents();
    }

    TimeAccumulated += DeltaTime;
    FrameCount++;
    if (TimeAccumulated >= 1.0f) 
        AverageFPS = FrameCount / TimeAccumulated;
        FrameCount = 0;
        TimeAccumulated = 0.f;
}


void AMeshCoords::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    SaveMetaHumanData();
    Super::EndPlay(EndPlayReason);
    CsvUtils::WriteNumberToFile(AverageFPS,Dir/TEXT("AVG_FPS.txt"));
}



bool AMeshCoords::BoneNameMatchesFilter(const FString& BoneName, const TArray<FString>& Filter)
{
    for (const FString& Keyword : Filter)
    {
        if (BoneName.Contains(Keyword))
        {
            return true;
        }
    }
    return false;
}


void AMeshCoords::FindAllCharactersAndTheirComponents()
{
    TArray<AActor*> Characters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Characters);
  
    TArray<FString> FilterKeywordsEmpty = {
    };
    int32 Loop=0;
    for (AActor* Actor : Characters)
    {
        ACharacter* Character = Cast<ACharacter>(Actor);
        if (Character)
        {      

            TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
            Character->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);

            TArray<UCameraComponent*> CameraComponents;
            Character->GetComponents<UCameraComponent>(CameraComponents);

            TArray<USceneCaptureComponent2D*> SceneCaptureComponents;
            Character->GetComponents<USceneCaptureComponent2D>(SceneCaptureComponents);
            int32 nr = 0;

            for (UCameraComponent* CameraComponent : CameraComponents)
            {
                for (USceneCaptureComponent2D* SceneCaptureComponent : SceneCaptureComponents)
                {
                    for (USkeletalMeshComponent* SkeletalMesh : SkeletalMeshComponents)
                    {
                        FString ComponentName = SkeletalMesh->GetName();
                        if (ComponentName == TEXT("Face"))
                        {
                            ProcessBones(SkeletalMesh, CameraComponent, SceneCaptureComponent, FilterKeywords, Metahuman2DMeshDataFace, Metahuman3DMeshDataFace, Loop, true);
                        }

                        if (ComponentName == TEXT("Body"))
                        {
                            ProcessBones(SkeletalMesh, CameraComponent, SceneCaptureComponent, FilterKeywords, Metahuman2DMeshDataBod, Metahuman3DMeshDataBod, Loop, false);
                        }
                    }
                }
             }  
            Loop++;
        }
    }
    Loop = 0;
}


void AMeshCoords::ProcessBones(
    USkeletalMeshComponent* InMeshComponent,
    UCameraComponent* CameraComponent,
    USceneCaptureComponent2D* CaptureComponent,
    const TArray<FString>& Filter,
    TArray<TMap<int32, FString>>& Bone2DCache,
    TArray<TMap<int32, FString>>& Bone3DCache,
    int32 LoopIndex,
    bool images
)
{
    if (!InMeshComponent || !InMeshComponent->GetSkeletalMeshAsset() || !CaptureComponent)
    {
        return;
    }
    UTextureRenderTarget2D* RT = CaptureComponent->TextureTarget;
    const float Width = RT ? RT->SizeX : 0.f;
    const float Height = RT ? RT->SizeY : 0.f;
    const float FOV = CaptureComponent->FOVAngle;
    const FMatrix Intrinsic = MathHelpers::ComputeIntrinsicMatrix(FOV, Width, Height);
    int32 BoneCount = InMeshComponent->GetNumBones();
    for (int32 BoneIndex = 0; BoneIndex < BoneCount; ++BoneIndex)
    {
        FName BoneName = InMeshComponent->GetBoneName(BoneIndex);
        FString BoneNameStr = BoneName.ToString();
        if (!Filter.IsEmpty() && !BoneNameMatchesFilter(BoneNameStr, Filter))
        {
            continue;
        }
        FVector WorldPosition = InMeshComponent->GetBoneLocation(BoneName);
        FVector CamSpacePos = CameraComponent->GetComponentTransform().InverseTransformPosition(WorldPosition);
        FString Data3D = FString::Printf(TEXT(";%0.3f;%0.3f;%0.3f"), CamSpacePos.X, CamSpacePos.Y, CamSpacePos.Z);
        Data3D.ReplaceInline(TEXT("."), TEXT(","));
        if (Bone3DCache[LoopIndex].Contains(BoneIndex))
        {
            Bone3DCache[LoopIndex][BoneIndex] += Data3D;
        }
        else
        {
            Bone3DCache[LoopIndex].Add(BoneIndex, FString::Printf(TEXT("%d;%s"), BoneIndex, *BoneNameStr) + Data3D);
        }

        FVector2D ScreenPosition;
        bool bProjected = MathHelpers::ProjectToImage(Intrinsic, CaptureComponent->GetComponentTransform().InverseTransformPosition(WorldPosition), ScreenPosition);
        if (bProjected)
        {
            FString Data2D = FString::Printf(TEXT(";%0.1f;%0.1f"), ScreenPosition.X, ScreenPosition.Y);
            Data2D.ReplaceInline(TEXT("."), TEXT(","));
            if (Bone2DCache[LoopIndex].Contains(BoneIndex))
            {
                Bone2DCache[LoopIndex][BoneIndex] += Data2D;
            }
            else
            {
                Bone2DCache[LoopIndex].Add(BoneIndex, FString::Printf(TEXT("%d;%s"), BoneIndex, *BoneNameStr) + Data2D);
            }
        }
    }
    if (images && RT)
    {
        FTextureRenderTargetResource* RTResource = RT->GameThread_GetRenderTargetResource();
        TArray<FColor> Bitmap;
        RTResource->ReadPixels(Bitmap);

        FString OutDir = Dirs[LoopIndex]/TEXT("OUT");
        const FString Prefix = TEXT("Shot_");
        const FString Extension = TEXT(".bmp");
        int32 NextIndex = UImageCapture::GetNextImageIndex(OutDir, Prefix, Extension);
        FString FileName = FString::Printf(TEXT("Shot_%d.bmp"), NextIndex);
        FString OutPath = OutDir/ FileName;
        if (!IFileManager::Get().DirectoryExists(*OutDir))
        {
            IFileManager::Get().MakeDirectory(*OutDir, true);
        }

        Async(
            EAsyncExecution::ThreadPool,
            [Bitmap = MoveTemp(Bitmap), Width, Height, OutPath = MoveTemp(OutPath)]() mutable
            {
                IImageWrapperModule& WrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));

                TSharedPtr<IImageWrapper> ImageWrapper = WrapperModule.CreateImageWrapper(EImageFormat::BMP);

                ImageWrapper->SetRaw(
                    Bitmap.GetData(),
                    Bitmap.Num() * sizeof(FColor),
                    Width,
                    Height,
                    ERGBFormat::BGRA,
                    8
                );
                const TArray64<uint8>& BmpData64 = ImageWrapper->GetCompressed(100);
                TArray<uint8> BmpData(BmpData64.GetData(), BmpData64.Num());
                FFileHelper::SaveArrayToFile(BmpData, *OutPath);
            }  
        );     

    }

}

void AMeshCoords::SaveMetaHumanData()
{
   
 
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    int32 f = 0;
    for (int32 MetaHumanIndex = 0; MetaHumanIndex < Dirs.Num(); ++MetaHumanIndex)
    {
        FString FileDir = Dirs[MetaHumanIndex];
        PlatformFile.CreateDirectoryTree(*FileDir);
        FString MetahumanDir2D = FileDir / TEXT("Metahuman2D.csv");
        FString MetahumanDir3D = FileDir / TEXT("Metahuman3D.csv");
        TMap<int32, FString>& Face = Metahuman3DMeshDataFace[MetaHumanIndex];
        TMap<int32, FString>& Face2D = Metahuman2DMeshDataFace[MetaHumanIndex];
        TMap<int32, FString>& Bod = Metahuman3DMeshDataBod[MetaHumanIndex];
        TMap<int32, FString>& Bod2D = Metahuman2DMeshDataBod[MetaHumanIndex];
        CsvUtils::SaveCached3DDataToCSVs(MetahumanDir3D,Face, Bod);
        CsvUtils::SaveCached2DDataToCSVs(MetahumanDir2D,Face2D, Bod2D);
		CsvUtils::WriteNumberToFile(AverageFPS,FileDir/TEXT("AVG_FPS.txt"));
    }
}


void AMeshCoords::Initialize()
{
    TArray<AActor*> Characters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Characters);
    TArray<FName> StaticMarkerNames;
    TArray<FVector> StaticMarkerPositions;
    for (AActor* Actor : Characters)
    {
        ACharacter* Character = Cast<ACharacter>(Actor);
        if (Character)
        {
            Metahuman2DMeshDataFace.Add(TMap<int32, FString>());
            Metahuman3DMeshDataFace.Add(TMap<int32, FString>());
            Metahuman2DMeshDataBod.Add(TMap<int32, FString>());
            Metahuman3DMeshDataBod.Add(TMap<int32, FString>());
            FString CharacterName = Character->GetName();
            FString FileDir = Dir / CharacterName;
            Dirs.Add(FileDir);
            
         
        }
    }

}


