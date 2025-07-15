// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

#include "MeshCoordsPawn.generated.h"

UCLASS()
class GASPALS_API AMeshCoordsPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeshCoordsPawn();

	UFUNCTION(BlueprintCallable, Category = "VertexCoordinates")
	//void GetWorldVertexPositions();
	//void GetWorldBonePositions(USkeletalMeshComponent* InMeshComponent);
	//void GetWorldBonePositions2(USkeletalMeshComponent* InMeshComponent);
	//bool BoneNameMatchesFilter(const FName& BoneName, const TArray<FString>& FilterKeywords);
	void ProcessBones(USkeletalMeshComponent* InMeshComponent, UCameraComponent* InCameraComponent, const TArray<FString>& FilterKeywords, TMap<int32, FString>& BoneCache);
	bool IsBoneVisibleFromCamera(const FVector& WorldPosition);
	void DrawBoneDebug(const FVector& WorldPosition, int32 BoneIndex, const FName& BoneName);
	bool BoneNameMatchesFilter(const FName& BoneName, const TArray<FString>& FilterKeywords);
	void FindAllCharactersInLevel();
	void FindAllCharactersAndTheirComponents();
	void SaveCachedDataToCSV(const FString& FilePath, TMap<int32, FString>& BoneCache);
	void DrawDebugIterationsFromCSV(const FString& FilePath, int32 IterationIndex);
	// Individual functions for key press/release
	UFUNCTION(BlueprintCallable, Category = "Input Simulation")
	void PressCtrlKey();
	UFUNCTION(BlueprintCallable, Category = "Input Simulation")
	void ReleaseCtrlKey();
	UFUNCTION(BlueprintCallable, Category = "Input Simulation")
	void PressShiftKey();
	UFUNCTION(BlueprintCallable, Category = "Input Simulation")
	void ReleaseShiftKey();
	UFUNCTION(BlueprintCallable, Category = "Input Simulation")
	void MouseLeftButtonDown();
	UFUNCTION(BlueprintCallable, Category = "Input Simulation")
	void MouseLeftButtonUp();

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


	TMap<int32, FString> BoneDataCacheFace;
	TMap<int32, FString> BoneDataCacheBody;

};
