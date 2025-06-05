// Fill out your copyright notice in the Description page of Project Settings.


#include "RadioActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARadioActor::ARadioActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1) ��Ʈ ������Ʈ ����
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);
	Body->SetCollisionProfileName(TEXT("BlockAll"));

	// 2) Dial_Master ���� �� ��Ʈ�� ����
	Dial_Master = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dial_Master"));
	Dial_Master->SetupAttachment(Body);
	Dial_Master->SetRelativeRotation(FRotator::ZeroRotator);
	Dial_Master->SetCollisionProfileName(TEXT("BlockAll"));

	// 3) Dial_BGM ����
	Dial_BGM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dial_BGM"));
	Dial_BGM->SetupAttachment(Body);
	Dial_BGM->SetRelativeRotation(FRotator::ZeroRotator);
	Dial_BGM->SetCollisionProfileName(TEXT("BlockAll"));

	// 4) Dial_SFX ����
	Dial_SFX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dial_SFX"));
	Dial_SFX->SetupAttachment(Body);
	Dial_SFX->SetRelativeRotation(FRotator::ZeroRotator);
	Dial_SFX->SetCollisionProfileName(TEXT("BlockAll"));

	// 5) �⺻�� �ʱ�ȭ
	bIsGrabbed = false;
	GrabController = nullptr;
	GrabbedDialComponent = nullptr;
	MinDialAngle = -60.f;
	MaxDialAngle = 60.f;

	// ����� ������ �����Ϳ��� ����
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

	// Grab ������ ���� �� Tick���� ���� ������Ʈ
	if (bIsGrabbed && GrabController && GrabbedDialComponent)
	{
		UpdateVolume();
	}

}

void ARadioActor::StartGrab(UMotionControllerComponent* Controller, UStaticMeshComponent* DialComponent)
{
	// (1) �̹� Grab ���̰ų� �Է��� �߸��Ǿ����� ����
	if (bIsGrabbed || !Controller || !DialComponent)
	{
		return;
	}

	// (2) Grab ���·� ��ȯ, ������ ����
	bIsGrabbed = true;
	GrabController = Controller;
	GrabbedDialComponent = DialComponent;

	// (3) Grab ���� ���� ȸ���� ����
	GrabStartCtrlRot = Controller->GetComponentRotation();
	GrabStartDialRot = DialComponent->GetRelativeRotation();

	// (4) �� �޽� Attach/Detach�� �������Ʈ(VRPawn)���� ó��
}

void ARadioActor::EndGrab()
{
	// (1) Grab ���� �ƴϸ� ����
	if (!bIsGrabbed)
	{
		return;
	}

	// (2) Grab ����, ���� �ʱ�ȭ
	bIsGrabbed = false;
	GrabController = nullptr;
	GrabbedDialComponent = nullptr;

	// (3) �� �޽� Attach/Detach�� �������Ʈ(VRPawn)���� ó��
}

void ARadioActor::UpdateVolume()
{
	if (!GrabController || !GrabbedDialComponent)
	{
		return;
	}

	// (1) ���� ��Ʈ�ѷ� ȸ����
	FRotator CurrentCtrlRot = GrabController->GetComponentRotation();

	// (2) ��Yaw ���: �󸶳� �ո��� ���ȴ����� ����
	float DeltaRoll = CurrentCtrlRot.Roll - GrabStartCtrlRot.Roll;

	// (3) ���̾� ���� Yaw ���, ���� ���� (Clamp)
	float StartRoll = GrabStartDialRot.Roll;
	float NewDialRoll = StartRoll + DeltaRoll;
	NewDialRoll = FMath::Clamp(NewDialRoll, MinDialAngle, MaxDialAngle);

	// (4) ���� ���̾� �޽ø� ȸ��
	FRotator NewLocalRot = GrabbedDialComponent->GetRelativeRotation();
	NewLocalRot.Roll = NewDialRoll;
	GrabbedDialComponent->SetRelativeRotation(NewLocalRot);

	// (5) 0~1�� ����ȭ�� ��, 0~2 ������ ������
	float Normalized01 = (NewDialRoll - MinDialAngle) / (MaxDialAngle - MinDialAngle);
	Normalized01 = FMath::Clamp(Normalized01, 0.f, 1.f);

	// 0~1 ������ 0~2�� Ȯ��: 0 �� 0, 0.5 �� 1, 1 �� 2
	float NewVolume = Normalized01 * 2.f;

	// (6) � ���̾����� �Ǵ� �� SoundClass ����
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

	// (7) SoundMix�� ���� ���� ����
	if (TargetClass && MySoundMix)
	{
		UGameplayStatics::SetSoundMixClassOverride(
			this,
			MySoundMix,
			TargetClass,
			NewVolume,   // 0~2 ������ ������ ��
			1.f,
			0.f,
			true
		);
		UGameplayStatics::PushSoundMixModifier(this, MySoundMix);
	}
}
