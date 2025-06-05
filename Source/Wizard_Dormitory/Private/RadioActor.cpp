// Fill out your copyright notice in the Description page of Project Settings.


#include "RadioActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARadioActor::ARadioActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1) 루트 컴포넌트 생성
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);
	Body->SetCollisionProfileName(TEXT("BlockAll"));

	// 2) Dial_Master 생성 및 루트에 부착
	Dial_Master = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dial_Master"));
	Dial_Master->SetupAttachment(Body);
	Dial_Master->SetRelativeRotation(FRotator::ZeroRotator);
	Dial_Master->SetCollisionProfileName(TEXT("BlockAll"));

	// 3) Dial_BGM 생성
	Dial_BGM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dial_BGM"));
	Dial_BGM->SetupAttachment(Body);
	Dial_BGM->SetRelativeRotation(FRotator::ZeroRotator);
	Dial_BGM->SetCollisionProfileName(TEXT("BlockAll"));

	// 4) Dial_SFX 생성
	Dial_SFX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dial_SFX"));
	Dial_SFX->SetupAttachment(Body);
	Dial_SFX->SetRelativeRotation(FRotator::ZeroRotator);
	Dial_SFX->SetCollisionProfileName(TEXT("BlockAll"));

	// 5) 기본값 초기화
	bIsGrabbed = false;
	GrabController = nullptr;
	GrabbedDialComponent = nullptr;
	MinDialAngle = -60.f;
	MaxDialAngle = 60.f;

	// 오디오 에셋은 에디터에서 연결
	MySoundMix = nullptr;
	MasterSoundClass = nullptr;
	BGMSoundClass = nullptr;
	SFXSoundClass = nullptr;

}

// Called when the game starts or when spawned
void ARadioActor::BeginPlay()
{
	Super::BeginPlay();

	
}
	

// Called every frame
void ARadioActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Grab 상태일 때만 매 Tick마다 볼륨 업데이트
	if (bIsGrabbed && GrabController && GrabbedDialComponent)
	{
		UpdateVolume();
	}

}

void ARadioActor::StartGrab(UMotionControllerComponent* Controller, UStaticMeshComponent* DialComponent)
{
	// (1) 이미 Grab 중이거나 입력이 잘못되었으면 무시
	if (bIsGrabbed || !Controller || !DialComponent)
	{
		return;
	}

	// (2) Grab 상태로 전환, 변수에 저장
	bIsGrabbed = true;
	GrabController = Controller;
	GrabbedDialComponent = DialComponent;

	// (3) Grab 시작 시점 회전값 저장
	GrabStartCtrlRot = Controller->GetComponentRotation();
	GrabStartDialRot = DialComponent->GetRelativeRotation();

	// (4) 손 메시 Attach/Detach는 블루프린트(VRPawn)에서 처리
}

void ARadioActor::EndGrab()
{
	// (1) Grab 중이 아니면 무시
	if (!bIsGrabbed)
	{
		return;
	}

	// (2) Grab 해제, 변수 초기화
	bIsGrabbed = false;
	GrabController = nullptr;
	GrabbedDialComponent = nullptr;

	// (3) 손 메시 Attach/Detach는 블루프린트(VRPawn)에서 처리
}

void ARadioActor::UpdateVolume()
{
	if (!GrabController || !GrabbedDialComponent)
	{
		return;
	}

	// (1) 현재 컨트롤러 회전값
	FRotator CurrentCtrlRot = GrabController->GetComponentRotation();

	// (2) ΔYaw 계산: 얼마나 손목을 돌렸는지를 구함
	float DeltaRoll = CurrentCtrlRot.Roll - GrabStartCtrlRot.Roll;

	// (3) 다이얼 로컬 Yaw 계산, 범위 제한 (Clamp)
	float StartRoll = GrabStartDialRot.Roll;
	float NewDialRoll = StartRoll + DeltaRoll;
	NewDialRoll = FMath::Clamp(NewDialRoll, MinDialAngle, MaxDialAngle);

	// (4) 실제 다이얼 메시를 회전
	FRotator NewLocalRot = GrabbedDialComponent->GetRelativeRotation();
	NewLocalRot.Roll = NewDialRoll;
	GrabbedDialComponent->SetRelativeRotation(NewLocalRot);

	// (5) 0~1로 정규화한 뒤, 0~2 범위로 재조정
	float Normalized01 = (NewDialRoll - MinDialAngle) / (MaxDialAngle - MinDialAngle);
	Normalized01 = FMath::Clamp(Normalized01, 0.f, 1.f);

	// 0~1 범위를 0~2로 확장: 0 → 0, 0.5 → 1, 1 → 2
	float NewVolume = Normalized01 * 2.f;

	// (6) 어떤 다이얼인지 판단 → SoundClass 선택
	USoundClass* TargetClass = nullptr;
	if (GrabbedDialComponent == Dial_Master)
	{
		TargetClass = MasterSoundClass;
	}
	else if (GrabbedDialComponent == Dial_BGM)
	{
		TargetClass = BGMSoundClass;
	}
	else if (GrabbedDialComponent == Dial_SFX)
	{
		TargetClass = SFXSoundClass;
	}

	// (7) SoundMix를 통해 볼륨 적용
	if (TargetClass && MySoundMix)
	{
		UGameplayStatics::SetSoundMixClassOverride(
			this,
			MySoundMix,
			TargetClass,
			NewVolume,   // 0~2 범위로 설정된 값
			1.f,
			0.f,
			true
		);
		UGameplayStatics::PushSoundMixModifier(this, MySoundMix);
	}
}
