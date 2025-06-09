// RadioActor.h
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "RadioActor.generated.h"

UCLASS()
class WIZARD_DORMITORY_API ARadioActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ARadioActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /** Components **/
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* Body;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* Dummy;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* Dial_BGM;

    /** Grab 상태 **/
    bool bIsGrabbed;

    UPROPERTY()
    USkeletalMeshComponent* HeldHandMesh;

    /** Dial 회전 범위 **/
    UPROPERTY(EditAnywhere, Category = "Dial")
    float MinDialAngle;

    UPROPERTY(EditAnywhere, Category = "Dial")
    float MaxDialAngle;

    /** Audio assets **/
    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundMix* MySoundMix;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundClass* BGMSoundClass;

    /** Blueprint 호출 함수 **/
    UFUNCTION(BlueprintCallable, Category = "Radio")
    void StartGrab(USkeletalMeshComponent* HandMesh);

    UFUNCTION(BlueprintCallable, Category = "Radio")
    void EndGrab();

    /** Grab 시작 시 저장할 회전 상태 **/
    FQuat InitialHandRotation;
    FRotator GrabStartDialRot;

private:
    // 매 Tick마다 볼륨 및 Dial 회전 업데이트
    void UpdateVolume();

    // RotationDelta 쿼터니온에서 Twist 축 성분만 추출
    static FQuat GetTwistQuat(const FQuat& RotationDelta, const FVector& TwistAxis);

    // Twist 쿼터니온으로부터 절대값 각도(°) 계산
    static float GetTwistAngleDegrees(const FQuat& RotationDelta, const FVector& TwistAxis);

    // Twist 쿼터니온으로부터 축 방향에 따른 부호 있는 각도(°) 계산
    static float GetSignedTwistAngle(const FQuat& RotationDelta, const FVector& TwistAxis);
};
