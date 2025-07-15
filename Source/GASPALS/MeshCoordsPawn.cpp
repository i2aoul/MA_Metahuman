// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshCoordsPawn.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h" 
#include "Rendering/SkeletalMeshRenderData.h" 
#include "Rendering/SkinWeightVertexBuffer.h"
#include "DrawDebugHelpers.h" 
#include <fstream>    // F�r std::ifstream und std::ofstream
#include <sstream>    // F�r std::stringstream
#include <string>     // F�r std::string
#include <iostream>   // Optional f�r Debug-Ausgaben
#include <iomanip> // F�r std::fixed und std::setprecision

#include <fstream>
#include <sstream>
#include "DrawDebugHelpers.h"

#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h" 
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"


#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
// Sets default values
AMeshCoordsPawn::AMeshCoordsPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    MovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("MovementComponent"));
    MovementComponent->UpdatedComponent = RootComponent;
    // Allgemeine Einstellungen
    MovementComponent->GravityScale = 1.0f;
    MovementComponent->MaxAcceleration = 800.0f;
    MovementComponent->BrakingFrictionFactor = 1.0f;
    MovementComponent->BrakingFriction = 0.0f;
    MovementComponent->CrouchedHalfHeight = 60.0f;
    MovementComponent->bUseSeparateBrakingFriction = true;
    MovementComponent->Mass = 100.0f;

    // Bewegungseinstellungen
    MovementComponent->MaxStepHeight = 45.0f;
    MovementComponent->GroundFriction = 5.0f;
    MovementComponent->MaxWalkSpeed = 500.0f;
    MovementComponent->MaxWalkSpeedCrouched = 300.0f;
    MovementComponent->MinAnalogWalkSpeed = 150.0f;
    MovementComponent->BrakingDecelerationWalking = 1500.0f;

    // Springen / Fallen
    MovementComponent->JumpZVelocity = 500.0f;
    MovementComponent->BrakingDecelerationFalling = 0.0f;
    MovementComponent->AirControl = 0.25f;
    MovementComponent->AirControlBoostMultiplier = 2.0f;
    MovementComponent->AirControlBoostVelocityThreshold = 25.0f;
    MovementComponent->FallingLateralFriction = 0.0f;

    // Schwimmen
    MovementComponent->MaxSwimSpeed = 300.0f;
    MovementComponent->BrakingDecelerationSwimming = 0.0f;
    MovementComponent->Buoyancy = 1.0f;

    // Fliegen
    MovementComponent->MaxFlySpeed = 600.0f;
    MovementComponent->BrakingDecelerationFlying = 0.0f;
    MovementComponent->MaxCustomMovementSpeed = 600.0f;

    // Drehung
    MovementComponent->bUseControllerDesiredRotation = true;
    MovementComponent->bOrientRotationToMovement = false; // Laut Screenshot deaktiviert
    MovementComponent->RotationRate = FRotator(0.0f, 0.0f, -1.0f);

    // Physik-Interaktion
    MovementComponent->bEnablePhysicsInteraction = true;
    MovementComponent->TouchForceFactor = 500.0f;
    MovementComponent->PushForceFactor = 750000.0f;
    MovementComponent->StandingDownwardForceScale = 1.0f;
    MovementComponent->InitialPushForceFactor = 500.0f;
    MovementComponent->MaxTouchForce = 250.0f;
    MovementComponent->RepulsionForce = 2.5f;

    // NavMesh-Bewegung
    MovementComponent->bUpdateOnlyIfRendered = false;
    MovementComponent->bAutoUpdateTickRegistration = true;
    MovementComponent->bTickBeforeOwner = true;
    MovementComponent->bAutoRegisterUpdatedComponent = true;

    // Erstelle die CapsuleComponent
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));

    // Setze die CapsuleComponent als Root-Komponente
    RootComponent = CapsuleComponent;

    // Konfiguriere die Größe der Kapsel
    CapsuleComponent->InitCapsuleSize(30.0f, 86.0f); // Radius: 30.0, Höhe: 86.0

    // Setze die Kollisionseinstellungen
    CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
    CapsuleComponent->CanCharacterStepUpOn = ECB_No; // Verhindert das Hochsteigen
    CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
    CapsuleComponent->SetCanEverAffectNavigation(true); // Aktiviert Navigation

    // Aktiviert, dass die Kapsel ein dynamisches Hindernis ist
    CapsuleComponent->bDynamicObstacle = true;

    // Ticken aktivieren
    CapsuleComponent->SetTickableWhenPaused(true); // (Falls benötigt, ist optional)
    CapsuleComponent->PrimaryComponentTick.bStartWithTickEnabled = true;
    CapsuleComponent->PrimaryComponentTick.TickInterval = 0.0f; // Standardwert (kein Intervall)


}

// Called when the game starts or when spawned
void AMeshCoordsPawn::BeginPlay()
{
    Super::BeginPlay();

    // DrawDebugIterationsFromCSV("C:/Users/Raoul/Documents/Unreal Projects/Masterarbeit/BoneDataBody.csv", 50);
}

// Called every frame
void AMeshCoordsPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // FindAllCharactersAndTheirComponents();
}

void AMeshCoordsPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    //SaveCachedDataToCSV(TEXT("C:/Users/Raoul/Documents/Unreal Projects/Masterarbeit/BoneDataFace.csv"), BoneDataCacheFace);
    //SaveCachedDataToCSV(TEXT("C:/Users/Raoul/Documents/Unreal Projects/Masterarbeit/BoneDataBody.csv"), BoneDataCacheBody);
    Super::EndPlay(EndPlayReason);
}



void AMeshCoordsPawn::ProcessBones(USkeletalMeshComponent * InMeshComponent, UCameraComponent * InCameraComponent, const TArray<FString>&FilterKeywords, TMap<int32, FString>&BoneCache)
{
    if (!InMeshComponent || !InMeshComponent->GetSkeletalMeshAsset() || !InCameraComponent)
    {
        return;
    }
    int32 BoneCount = InMeshComponent->GetNumBones();
    for (int32 BoneIndex = 0; BoneIndex < BoneCount; ++BoneIndex)
    {
        // Abrufen des Bone-Namens aus dem BoneIndex
        FName BoneName = InMeshComponent->GetBoneName(BoneIndex);
        FString BoneNameStr = BoneName.ToString(); // Konvertieren von FName zu FString

        FVector WorldPosition = InMeshComponent->GetBoneLocation(BoneName); // Hier wird BoneName verwendet
        FVector CameraRelativePosition = InCameraComponent->GetComponentTransform().InverseTransformPosition(WorldPosition);

        FString NewData = FString::Printf(TEXT(";%.3f;%.3f;%.3f"), CameraRelativePosition.X, CameraRelativePosition.Y, CameraRelativePosition.Z);
        NewData = NewData.Replace(TEXT("."), TEXT(","));
        // Verwenden des BoneIndex als eindeutigen Schl�ssel f�r den Cache
        if (BoneCache.Contains(BoneIndex))
        {
            BoneCache[BoneIndex] += NewData;
        }
        else
        {
            FString BoneData = FString::Printf(TEXT("%d;%s"), BoneIndex, *BoneNameStr);
            BoneData += NewData;
            BoneCache.Add(BoneIndex, BoneData);
        }
    }
    int MaxIterations = 0;
    for (const auto& Pair : BoneCache)
    {
        TArray<FString> ParsedValues;
        Pair.Value.ParseIntoArray(ParsedValues, TEXT(";"), true);

        // Berechnen der Anzahl der `x, y, z`-Eintr�ge
        int Iterations = (ParsedValues.Num() - 2) / 3;
        if (Iterations > MaxIterations)
        {
            MaxIterations = Iterations;
        }
    }
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("MaxIterations count: %d"), MaxIterations));
}




// Zeichnet eine Debug-Kugel und zeigt eine Meldung auf dem Bildschirm an
void AMeshCoordsPawn::DrawBoneDebug(const FVector & WorldPosition, int32 BoneIndex, const FName & BoneName)
{
    DrawDebugSphere(
        GetWorld(),
        WorldPosition,
        0.5f,           // Radius der Kugel
        4,              // Segmente
        FColor::Red,    // Farbe
        false,           // Persistent
        0.5f,          // Dauer (dauerhaft, wenn -1.0f)
        0,              // Tiefenpriorit�t
        0.1f            // Linienst�rke
    );

    /*if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            5.0f,
            FColor::Blue,
            FString::Printf(TEXT("Bone %d (%s): World Position: X=%.2f, Y=%.2f, Z=%.2f"),
                BoneIndex,
                *BoneName.ToString(),
                WorldPosition.X,
                WorldPosition.Y,
                WorldPosition.Z)
        );
    }*/
}

// Funktion zur Filterung der Bone-Namen
bool AMeshCoordsPawn::BoneNameMatchesFilter(const FName & BoneName, const TArray<FString>&FilterKeywords)
{
    FString BoneNameStr = BoneName.ToString();
    for (const FString& Keyword : FilterKeywords)
    {
        if (BoneNameStr.Contains(Keyword))
        {
            return true;
        }
    }
    return false;
}


void AMeshCoordsPawn::FindAllCharactersInLevel()
{
    TArray<AActor*> Characters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Characters);

    // Anzahl der gefundenen Charaktere anzeigen
    if (GEngine)
    {
        // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Total Characters Found: %d"), Characters.Num()));
    }

    for (AActor* Actor : Characters)
    {
        ACharacter* Character = Cast<ACharacter>(Actor);
        if (Character)
        {
            FString CharacterName = Character->GetName();
            FVector CharacterLocation = Character->GetActorLocation();

            // Debug-Meldung im Log und auf dem Bildschirm


            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Found Character: %s"), *CharacterName));
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Location: X=%.2f, Y=%.2f, Z=%.2f"),
                    CharacterLocation.X, CharacterLocation.Y, CharacterLocation.Z));
            }
        }
    }
}


void AMeshCoordsPawn::FindAllCharactersAndTheirComponents()
{
    TArray<AActor*> Characters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Characters);
    TArray<FString> FilterKeywords = {
        TEXT("Lip"), TEXT("Eye"), TEXT("Nose"), TEXT("Pupil"), TEXT("Foot"), TEXT("Thigh"), TEXT("calf"),
        TEXT("Pinky"), TEXT("Wrist"), TEXT("Ankle"), TEXT("Rib"), TEXT("Neck"), TEXT("Thorax"),
        TEXT("Toe"), TEXT("Spine"), TEXT("Elbow"), TEXT("Skull"), TEXT("Chest"), TEXT("Clavicle"),
        TEXT("Arm"), TEXT("Knee"), TEXT("Back"), TEXT("Hip"), TEXT("Hand"), TEXT("Finger"),
        TEXT("Jaw"), TEXT("Shoulder"), TEXT("Leg"), TEXT("Head"), TEXT("Pelvis"),
    };

    for (AActor* Actor : Characters)
    {
        ACharacter* Character = Cast<ACharacter>(Actor);
        if (Character)
        {
            FString CharacterName = Character->GetName();
            UE_LOG(LogTemp, Warning, TEXT("Found Character: %s"), *CharacterName);

            TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
            Character->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);

            TArray<UCameraComponent*> CameraComponents;
            Character->GetComponents<UCameraComponent>(CameraComponents);

            // Komponenten debuggen und verarbeiten
            for (UCameraComponent* CameraComponent : CameraComponents)
            {
                for (USkeletalMeshComponent* SkeletalMesh : SkeletalMeshComponents)
                {
                    FString ComponentName = SkeletalMesh->GetName();

                    // �berpr�fen, ob der Name 'Face_0' oder 'CharacterMesh0' ist
                    if (ComponentName == TEXT("Face_0"))
                    {
                        ProcessBones(SkeletalMesh, CameraComponent, FilterKeywords, BoneDataCacheFace);
                    }
                    if (ComponentName == TEXT("CharacterMesh0"))
                    {
                        ProcessBones(SkeletalMesh, CameraComponent, FilterKeywords, BoneDataCacheBody);
                    }
                }
            }
        }
    }
}


void AMeshCoordsPawn::SaveCachedDataToCSV(const FString & FilePath, TMap<int32, FString>&BoneCache)
{
    std::ofstream csvFile(TCHAR_TO_UTF8(*FilePath));

    // Berechnen der Anzahl der Iterationen basierend auf dem ersten Eintrag
    int NumIterations = 0;
    if (BoneCache.Num() > 0)
    {
        TArray<FString> ParsedValues;
        BoneCache.CreateConstIterator()->Value.ParseIntoArray(ParsedValues, TEXT(";"), true);
        NumIterations = (ParsedValues.Num() - 2) / 3; // Subtrahieren von BoneIndex und BoneName
    }

    // Header schreiben
    csvFile << "BoneIndex;BoneName";
    for (int i = 1; i <= NumIterations; ++i)
    {
        csvFile << ";x" << i << ";y" << i << ";z" << i;
    }
    csvFile << "\n";

    // Setzen der Pr�zision f�r Dezimalzahlen
    csvFile << std::fixed << std::setprecision(10);

    // Datenzeilen schreiben
    for (const auto& Pair : BoneCache)
    {
        TArray<FString> ParsedValues;
        Pair.Value.ParseIntoArray(ParsedValues, TEXT(";"), true);

        // Sicherstellen, dass mindestens `BoneIndex` und `BoneName` vorhanden sind
        if (ParsedValues.Num() < 2) continue;

        // Schreibe `BoneIndex` und `BoneName` in die CSV
        csvFile << TCHAR_TO_UTF8(*ParsedValues[0]) << ";" << TCHAR_TO_UTF8(*ParsedValues[1]) << ";";

        // Schreibe alle `x, y, z`-Werte getrennt durch Semikolons
        for (int i = 2; i < ParsedValues.Num(); ++i)
        {
            csvFile << TCHAR_TO_UTF8(*ParsedValues[i]);
            if (i < ParsedValues.Num() - 1) // Kein Semikolon nach dem letzten Eintrag
            {
                csvFile << ";";
            }
        }
        csvFile << "\n";
    }

    csvFile.close();
    UE_LOG(LogTemp, Log, TEXT("Bone coordinates saved to %s with %d iterations"), *FilePath, NumIterations);
}



void AMeshCoordsPawn::DrawDebugIterationsFromCSV(const FString & FilePath, int32 IterationIndex)
{
    std::ifstream csvFile(TCHAR_TO_UTF8(*FilePath));
    if (!csvFile.is_open())
    {
        UE_LOG(LogTemp, Error, TEXT("CSV file could not be opened: %s"), *FilePath);
        return;
    }

    std::string Line; // Tempor�re std::string f�r das Einlesen
    bool IsHeader = true;

    // Berechne die Spaltenindizes f�r x, y, z der gew�hlten Iteration
    int32 xIndex = 2 + (IterationIndex - 1) * 3;
    int32 yIndex = xIndex + 1;
    int32 zIndex = xIndex + 2;

    // Iteriere �ber jede Zeile der CSV
    while (std::getline(csvFile, Line))
    {
        if (IsHeader) // �berspringe die Header-Zeile
        {
            IsHeader = false;
            continue;
        }

        // Konvertiere die eingelesene Zeile von std::string zu FString
        FString LineFString(Line.c_str());

        // Parsen der Zeile und Extrahieren der Koordinaten
        TArray<FString> ParsedValues;
        LineFString.ParseIntoArray(ParsedValues, TEXT(";"), true);

        // �berpr�fen, ob gen�gend Werte vorhanden sind
        if (ParsedValues.Num() <= zIndex)
        {
            continue; // Springe diese Zeile, wenn die Werte f�r x, y, z nicht vorhanden sind
        }

        // Extrahiere den BoneName und die `xN, yN, zN` Koordinaten
        FString BoneName = ParsedValues[1];
        float x = FCString::Atof(*ParsedValues[xIndex]);
        float y = FCString::Atof(*ParsedValues[yIndex]);
        float z = FCString::Atof(*ParsedValues[zIndex]);
        FVector Position(x, y, z);

        // Zeichne den Debug-Kreis f�r die gew�hlte Iteration
        DrawDebugCircle(
            GetWorld(),
            Position,         // Mittelpunkt des Kreises
            1.0f,            // Radius
            32,               // Anzahl der Segmente
            FColor::Blue,     // Farbe
            true,            // Dauerhaft?
            0,            // Dauer (0 = dauerhaft)
            0,                // Tiefe
            5.0f              // Dicke
        );

        // Anzeigen der Koordinaten und BoneName auf dem Bildschirm
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                5.0f,
                FColor::Green,
                FString::Printf(TEXT("Bone: %s - X%d=%.2f, Y%d=%.2f, Z%d=%.2f"), *BoneName, IterationIndex, x, IterationIndex, y, IterationIndex, z)
            );
        }
    }

    csvFile.close();
    UE_LOG(LogTemp, Log, TEXT("Finished drawing iteration %d debug circles from CSV"), IterationIndex);
}

// Press Ctrl key
void AMeshCoordsPawn::PressCtrlKey()
{
#if PLATFORM_WINDOWS
    keybd_event(VK_CONTROL, 0, 0, 0);
    UE_LOG(LogTemp, Warning, TEXT("Ctrl key pressed"));
#endif
}

// Release Ctrl key
void AMeshCoordsPawn::ReleaseCtrlKey()
{
#if PLATFORM_WINDOWS
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
    UE_LOG(LogTemp, Warning, TEXT("Ctrl key released"));
#endif
}

// Press Shift key
void AMeshCoordsPawn::PressShiftKey()
{
#if PLATFORM_WINDOWS
    keybd_event(VK_SHIFT, 0, 0, 0);
    UE_LOG(LogTemp, Warning, TEXT("Shift key pressed"));
#endif
}

// Release Shift key
void AMeshCoordsPawn::ReleaseShiftKey()
{
#if PLATFORM_WINDOWS
    keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
    UE_LOG(LogTemp, Warning, TEXT("Shift key released"));
#endif
}

// Mouse left button down
void AMeshCoordsPawn::MouseLeftButtonDown()
{
#if PLATFORM_WINDOWS
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    UE_LOG(LogTemp, Warning, TEXT("Mouse left button down"));
#endif
}

// Mouse left button up
void AMeshCoordsPawn::MouseLeftButtonUp()
{
#if PLATFORM_WINDOWS
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    UE_LOG(LogTemp, Warning, TEXT("Mouse left button up"));
#endif
}



