// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "MotionControllerComponent.h"
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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Body;
	// (2) 다이얼 메시 3개: Master, BGM, SFX 각각을 UStaticMeshComponent로 표현
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Dial_Master;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Dial_BGM;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Dial_SFX;

	// === 변수 ===
	// (3) 현재 Grab 상태인지 여부를 나타냅니다.
	bool bIsGrabbed;

	// (4) 어떤 모션 컨트롤러(왼손/오른손)가 다이얼을 잡았는지 저장
	UPROPERTY()
	UMotionControllerComponent* GrabController;

	// (5) Grab 시작 시점의 컨트롤러 월드 회전값
	FRotator GrabStartCtrlRot;

	// (6) Grab 시작 시점의 다이얼 로컬 회전값
	FRotator GrabStartDialRot;

	// (7) 현재 Grab 중인 다이얼 컴포넌트(StaticMeshComponent)의 포인터
	UPROPERTY()
	UStaticMeshComponent* GrabbedDialComponent;

	// === 다이얼 회전 범위 ===
	// (8) 다이얼이 돌아갈 수 있는 최소/최대 로컬 Yaw 각도
	UPROPERTY(EditAnywhere, Category = "Dial")
	float MinDialAngle;

	UPROPERTY(EditAnywhere, Category = "Dial")
	float MaxDialAngle;

	// === 오디오 조절용 에셋 ===
	// (9) SoundMix 에셋: 음량(Volume) 조절을 위해 사용
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundMix* MySoundMix;

	// (10) Master/BGM/SFX에 대응하는 SoundClass 에셋들
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundClass* MasterSoundClass;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundClass* BGMSoundClass;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundClass* SFXSoundClass;

	// === 함수 ===
	// (11) 다이얼을 잡는 순간(Pressed) VRPawn에서 호출하는 함수
	UFUNCTION(BlueprintCallable, Category = "Radio")
	void StartGrab(UMotionControllerComponent* Controller, UStaticMeshComponent* DialComponent);

	// (12) 다이얼을 놓는 순간(Released) VRPawn에서 호출하는 함수
	UFUNCTION(BlueprintCallable, Category = "Radio")
	void EndGrab();

private:
	// (13) Grab 중일 때 매 Tick마다 볼륨을 갱신하는 내부 함수
	void UpdateVolume();

};
