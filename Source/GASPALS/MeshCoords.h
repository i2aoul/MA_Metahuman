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
	AMeshCoords();
	/**
	 * Verarbeitet die Bones eines Skelett-Meshes.
	 * Extrahiert 2D- und 3D-Koordinaten, speichert sie in den übergebenen Caches und speichert optional Bilder.
	 *
	 * @param InMeshComponent Das zu analysierende Skeletal Mesh.
	 * @param CameraComponent Die Kamera, von der projiziert wird.
	 * @param CaptureComponent Optionaler SceneCaptureComponent2D zum Speichern von Bildern.
	 * @param FilterKeywords Liste an Schlüsselwörtern zur Filterung der Bones.
	 * @param Bone2DCache Cache zur Speicherung der projizierten 2D-Koordinaten.
	 * @param Bone3DCache Cache zur Speicherung der 3D-Koordinaten.
	 * @param LoopIndex Index zur Identifikation des Frames oder der Iteration.
	 * @param images Wenn true, werden Bilder gespeichert.
	 */
	void ProcessBones(USkeletalMeshComponent* InMeshComponent, UCameraComponent* CameraComponent, USceneCaptureComponent2D* CaptureComponent, const TArray<FString>& FilterKeywords, TArray<TMap<int32, FString>>& Bone2DCache, TArray<TMap<int32, FString>>& Bone3DCache, int32 LoopIndex, bool images);

	/**
	 * Projiziert die Bones eines Skelett-Meshes auf den Bildschirm und speichert die 2D-Koordinaten.
	 *
	 * @param InMeshComponent Das zu analysierende Mesh.
	 * @param InCameraComponent Die verwendete Kamera.
	 * @param BoneCache Cache für die berechneten 2D-Koordinaten.
	 */
	void ProcessBonesWithScreenProjection(USkeletalMeshComponent* InMeshComponent, UCameraComponent* InCameraComponent, TMap<int32, FString>& BoneCache);

	/**
	 * Prüft, ob ein Punkt im Weltkoordinatensystem von der Kamera sichtbar ist.
	 *
	 * @param WorldPosition Die Weltkoordinaten des Punkts.
	 * @return true, wenn der Punkt sichtbar ist; sonst false.
	 */
	bool IsBoneVisibleFromCamera(const FVector& WorldPosition);

	/**
	 * Zeichnet Debug-Markierungen für einen Bone im 3D-Raum.
	 *
	 * @param WorldPosition Die Weltposition des Bones.
	 * @param BoneIndex Der Index des Bones.
	 * @param BoneName Der Name des Bones.
	 */
	void DrawBoneDebug(const FVector& WorldPosition, int32 BoneIndex, const FName& BoneName);

	/**
	 * Prüft, ob ein Bone-Name mit einem der angegebenen Filter-Schlüsselwörter übereinstimmt.
	 *
	 * @param BoneName Der zu überprüfende Bone-Name.
	 * @param FilterKeywords Die Liste der Filter-Schlüsselwörter.
	 * @return true, wenn eine Übereinstimmung vorliegt; sonst false.
	 */
	bool BoneNameMatchesFilter(const FString& BoneName, const TArray<FString>& FilterKeywords);

	/**
	 * Sucht alle Charaktere im aktuellen Level.
	 */
	void FindAllCharactersInLevel();

	/**
	 * Sucht alle Charaktere im Level sowie deren zugehörige Komponenten.
	 */
	void FindAllCharactersAndTheirComponents();

	/**
	 * Speichert den übergebenen 3D-Bone-Cache in einer CSV-Datei.
	 *
	 * @param FilePath Pfad zur CSV-Datei.
	 * @param BoneCache Die zu speichernden 3D-Daten.
	 */
	void SaveCached3DDataToCSV(const FString& FilePath, TMap<int32, FString>& BoneCache);

	/**
	 * Speichert den übergebenen 2D-Bone-Cache in einer CSV-Datei.
	 *
	 * @param FilePath Pfad zur CSV-Datei.
	 * @param BoneCache Die zu speichernden 2D-Daten.
	 */
	void SaveCached2DDataToCSV(const FString& FilePath, TMap<int32, FString>& BoneCache);

	/**
	 * Lädt eine CSV-Datei mit Bone-Koordinaten und zeichnet die Bones im Debug-Modus.
	 *
	 * @param FilePath Pfad zur CSV-Datei.
	 * @param IterationIndex Der zu visualisierende Iterationsindex.
	 */
	void DrawDebugIterationsFromCSV(const FString& FilePath, int32 IterationIndex);

	/**
	 * Speichert ein Bild aus einem RenderTarget in hoher Qualität.
	 *
	 * @param RenderTarget Das zu speichernde RenderTarget.
	 * @param Directory Zielverzeichnis.
	 * @param FileName Dateiname.
	 */
	void CaptureHighQualityImage(UTextureRenderTarget2D* RenderTarget, const FString& Directory, const FString& FileName);

	/**
	 * Führt alle Schritte zum Speichern von MetaHuman-Daten aus.
	 */
	void SaveMetaHumanData();

	/**
	 * Gibt die projizierten 2D-Koordinaten der Körper-Bones im Log aus.
	 */
	void PrintMetahuman2DMeshDataBody();

	/**
	 * Initialisiert interne Daten und Referenzen.
	 */
	void Initialize();

	/**
	 * Zeichnet eine Debug-Box mit markierter Vorder- und Rückseite basierend auf Kameraposition.
	 *
	 * @param World Die aktuelle Weltreferenz.
	 * @param CameraPosition Position der Kamera.
	 * @param CameraRotation Rotation der Kamera.
	 * @param BoxExtent Ausdehnung der Box.
	 */
	void DrawDebugBoxWithFrontAndBack(UWorld* World, const FVector& CameraPosition, const FRotator& CameraRotation, const FVector& BoxExtent);

	/**
	 * Wandelt eine Weltposition in Bildschirmkoordinaten um.
	 *
	 * @param CameraComponent Die Kamerakomponente.
	 * @param WorldPosition Die Weltkoordinaten des Punkts.
	 * @param OutScreenPosition Rückgabe der Bildschirmposition.
	 * @return true bei Erfolg, false sonst.
	 */
	bool ConvertWorldToScreenWithCameraComponent(UCameraComponent* CameraComponent, const FVector& WorldPosition, FVector2D& OutScreenPosition);

	/**
	 * Berechnet präzise 2D-Bildkoordinaten auf Basis interner Kameraparameter.
	 *
	 * @param CameraComponent Die verwendete Kamera.
	 * @param WorldPosition Die Weltkoordinaten.
	 * @param FocalLength Brennweite der Kamera.
	 * @param ImageWidth Bildbreite in Pixeln.
	 * @param ImageHeight Bildhöhe in Pixeln.
	 * @param OutImageCoordinates Rückgabe der Bildkoordinaten.
	 * @return true bei Erfolg, false sonst.
	 */
	bool Compute2DImageCoordinates(UCameraComponent* CameraComponent, const FVector& WorldPosition, float FocalLength, float ImageWidth, float ImageHeight, FVector2D& OutImageCoordinates);

	/**
	 * Berechnet die intrinsische Kameramatrix.
	 *
	 * @param FOV Sichtfeld in Grad.
	 * @param ImageWidth Breite des Bildes in Pixeln.
	 * @param ImageHeight Höhe des Bildes in Pixeln.
	 * @return Die berechnete 3x3-Kameramatrix.
	 */
	FMatrix ComputeIntrinsicMatrix(float FOV, int ImageWidth, int ImageHeight);

	/**
	 * Speichert zwei 2D-Caches in einer CSV-Datei (z. B. für linken und rechten Charakter).
	 *
	 * @param FilePath Pfad zur CSV-Datei.
	 * @param BoneCache1 Erster Cache.
	 * @param BoneCache2 Zweiter Cache.
	 */
	void SaveCached2DDataToCSVs(const FString& FilePath, TMap<int32, FString>& BoneCache1, TMap<int32, FString>& BoneCache2);

	/**
	 * Speichert zwei 3D-Caches in einer CSV-Datei (z. B. für linken und rechten Charakter).
	 *
	 * @param FilePath Pfad zur CSV-Datei.
	 * @param BoneCache1 Erster Cache.
	 * @param BoneCache2 Zweiter Cache.
	 */
	void SaveCached3DDataToCSVs(const FString& FilePath, TMap<int32, FString>& BoneCache1, TMap<int32, FString>& BoneCache2);

	/**
	 * Schreibt Kameraparameter im SPICA-kompatiblen Format in eine Datei.
	 *
	 * @param FilePath Pfad zur Zieldatei.
	 * @param Intr Die intrinsische Matrix.
	 * @param Dist Die Distortion-Vektoren (Verzerrung).
	 * @param R Rotationsmatrix.
	 * @param T Translationsvektor.
	 */
	void WriteSpicaCalibCompatible(const FString& FilePath, const FMatrix& Intr, const FVector& Dist, const FMatrix& R, const FVector& T);

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* MovementComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
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
