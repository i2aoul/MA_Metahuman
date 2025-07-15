/// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "MeshCoords.generated.h"


UCLASS()
class GASPALS_API AMeshCoords : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeshCoords();
	//void GetWorldVertexPositions();
	//void GetWorldBonePositions(USkeletalMeshComponent* InMeshComponent);
	//void GetWorldBonePositions2(USkeletalMeshComponent* InMeshComponent);
	//bool BoneNameMatchesFilter(const FName& BoneName, const TArray<FString>& FilterKeywords);
	void ProcessBones(USkeletalMeshComponent* InMeshComponent, UCameraComponent* CameraComponent ,  USceneCaptureComponent2D* CaptureComponent, const TArray<FString>& FilterKeywords, TArray<TMap<int32, FString>>& Bone2DCache, TArray<TMap<int32, FString>>& Bone3DCache, int32 LoopIndex,bool images);
	void ProcessBonesWithScreenProjection(USkeletalMeshComponent* InMeshComponent, UCameraComponent* InCameraComponent, TMap<int32, FString>& BoneCache);
	bool IsBoneVisibleFromCamera(const FVector& WorldPosition);
	void DrawBoneDebug(const FVector& WorldPosition, int32 BoneIndex, const FName& BoneName);
	bool BoneNameMatchesFilter(const FString& BoneName, const TArray<FString>& FilterKeywords);
	void FindAllCharactersInLevel();
	void FindAllCharactersAndTheirComponents();
	void SaveCached3DDataToCSV(const FString& FilePath, TMap<int32, FString>& BoneCache);
	void SaveCached2DDataToCSV(const FString& FilePath, TMap<int32, FString>& BoneCache);
	void DrawDebugIterationsFromCSV(const FString& FilePath, int32 IterationIndex);
	void CaptureHighQualityImage(UTextureRenderTarget2D* RenderTarget, const FString& Directory, const FString& FileName);
	void SaveMetaHumanData();
	void PrintMetahuman2DMeshDataBody();
	void Initialize();

	void DrawDebugBoxWithFrontAndBack(UWorld* World, const FVector& CameraPosition, const FRotator& CameraRotation, const FVector& BoxExtent);
	bool ConvertWorldToScreenWithCameraComponent(UCameraComponent* CameraComponent, const FVector& WorldPosition, FVector2D& OutScreenPosition);
	bool Compute2DImageCoordinates(UCameraComponent* CameraComponent, const FVector& WorldPosition, float FocalLength, float ImageWidth, float ImageHeight, FVector2D& OutImageCoordinates);
	FMatrix ComputeIntrinsicMatrix(float FOV, int ImageWidth, int ImageHeight);
	void SaveCached2DDataToCSVs(const FString& FilePath, TMap<int32, FString>& BoneCache1, TMap<int32, FString>& BoneCache2);
	void SaveCached3DDataToCSVs(const FString& FilePath, TMap<int32, FString>& BoneCache1, TMap<int32, FString>& BoneCache2);
	//void RemoveSandboxCharacters(AClass* SandboxCharacterClass);
	//void SpawnRandomBlueprint(FString FolderPath, FVector SpawnLocation, FRotator SpawnRotation);
	// Individual functions for key press/release
	/*void WriteSpicaContainer(
		const FString& FilePath,
		const FMatrix& Intr,
		const FVector& Kc,
		int32 FrameIndex,
		const FTransform& Extrinsic
	);*/
	void WriteSpicaCalibCompatible(
		const FString& FilePath,
		const FMatrix& Intr,      // Intrinsik
		const FVector& Dist,      // (k1,k2,p1,p2,k3) oder Deine Vektor-Anordnung
		const FMatrix& R,         // Rotation 3×3
		const FVector& T          // Translation
	);
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* MovementComponent;

	// Capsule Component as Root
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	TArray<TMap<int32, FString>> Metahuman2DMeshDataFace;
	TArray<TMap<int32, FString>> Metahuman3DMeshDataFace;
	TArray<TMap<int32, FString>> Metahuman2DMeshDataBod;
	TArray<TMap<int32, FString>> Metahuman3DMeshDataBod;
	TArray <FString> Dirs;
	FString Dir;
	int32 looping = 1;
	int32 TickCounter = 0;
	int32 _SpicaFrameCounter = 0;
	TArray<FString> FilterKeywords;
	int32 FrameCount = 0;
	float TimeAccumulated = 0.f;
	float AverageFPS = 0.f;
};
