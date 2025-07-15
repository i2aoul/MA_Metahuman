// BoneProcessor.cpp

#include "UBoneProcessor.h"
#include "CsvUtils.h"
#include "MathHelpers.h"
#include "UImageCapture.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

void UBoneProcessor::Initialize(UWorld* InWorld, const TArray<FString>& InFilterKeywords)
{
    WorldContext = InWorld;
    FilterKeywords = InFilterKeywords;
}

void UBoneProcessor::ProcessAllCharacters(int32 FrameIndex, IFileHandle* Csv2D, IFileHandle* Csv3D)
{
    if (!WorldContext) return;

    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(WorldContext, ACharacter::StaticClass(), Actors);

    for (AActor* A : Actors)
    {
        ACharacter* Char = Cast<ACharacter>(A);
        if (!Char) continue;

        // Alle Komponenten sammeln
        TArray<USkeletalMeshComponent*> MeshComps;
        Char->GetComponents<USkeletalMeshComponent>(MeshComps);

        TArray<UCameraComponent*> CamComps;
        Char->GetComponents<UCameraComponent>(CamComps);

        TArray<USceneCaptureComponent2D*> SCComps;
        Char->GetComponents<USceneCaptureComponent2D>(SCComps);

        for (auto* Mesh : MeshComps)
        {
            for (auto* Cam : CamComps)
            {
                for (auto* SC : SCComps)
                {
                    if (!Mesh->GetSkeletalMeshAsset() || !SC->TextureTarget) continue;

                    // Pro Bone
                    const int32 NumBones = Mesh->GetNumBones();
                    FMatrix Intr = MathHelpers::ComputeIntrinsicMatrix(
                        Cam->FieldOfView,
                        SC->TextureTarget->SizeX,
                        SC->TextureTarget->SizeY
                    );

                    for (int32 i = 0; i < NumBones; ++i)
                    {
                        FName BoneName = Mesh->GetBoneName(i);
                        FString BoneStr = BoneName.ToString();

                        // Filter
                        bool bMatch = FilterKeywords.Num() == 0;
                        for (auto& K : FilterKeywords)
                            if (BoneStr.Contains(K)) { bMatch = true; break; }
                        if (!bMatch) continue;

                        // Welt ? Kamera-Raum
                        FVector WorldPos = Mesh->GetBoneLocation(BoneName);
                        FVector CamSpace = Cam->GetComponentTransform().InverseTransformPosition(WorldPos);

                        // 3D schreiben
                        {
                            FString Line3D = FString::Printf(
                                TEXT("%d;%s;%s;%.3f;%.3f;%.3f\n"),
                                FrameIndex,
                                *Char->GetName(),
                                *BoneStr,
                                CamSpace.X, CamSpace.Y, CamSpace.Z
                            );
                      
                        }

                        // 2D-Projektion
                        FVector2D Img;
                        if (MathHelpers::ProjectToImage(Intr, CamSpace, Img))
                        {
                            FString Line2D = FString::Printf(
                                TEXT("%d;%s;%s;%.1f;%.1f\n"),
                                FrameIndex,
                                *Char->GetName(),
                                *BoneStr,
                                Img.X, Img.Y
                            );
                      
                        }
                    }

                    // Optional: Bild speichern
                    if (bCaptureImages)
                    {
                        ImageCapture->Capture(SC, OutputDirectory, FString::Printf(TEXT("%s_%d.png"), *Char->GetName(), FrameIndex));
                    }
                }
            }
        }
    }
}
