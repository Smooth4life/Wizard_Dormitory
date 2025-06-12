// RadioActor.cpp
#include "RadioActor.h"
#include "Kismet/GameplayStatics.h"

ARadioActor::ARadioActor()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
    Body->SetupAttachment(RootComponent);
    Body->SetCollisionProfileName(TEXT("BlockAll"));

    Dummy = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy"));
    Dummy->SetupAttachment(Body);
    Dummy->SetRelativeRotation(FRotator::ZeroRotator);

    Dial_BGM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dial_BGM"));
    Dial_BGM->SetupAttachment(Dummy);
    Dial_BGM->SetCollisionProfileName(TEXT("BlockAll"));

    bIsGrabbed = false;
    HeldHandMesh = nullptr;
    MinDialAngle = -60.f;
    MaxDialAngle = 60.f;

    MySoundMix = nullptr;
    BGMSoundClass = nullptr;
}

void ARadioActor::BeginPlay()
{
    Super::BeginPlay();
}

void ARadioActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsGrabbed && HeldHandMesh)
    {
        UpdateVolume();
    }
}

void ARadioActor::StartGrab(USkeletalMeshComponent* HandMesh)
{
    if (bIsGrabbed || HeldHandMesh) return;

    bIsGrabbed = true;
    HeldHandMesh = HandMesh;

    InitialHandRotation = HeldHandMesh->GetComponentTransform().GetRotation();
    GrabStartDialRot = Dummy->GetRelativeRotation();
}

void ARadioActor::EndGrab()
{
    if (!bIsGrabbed) return;

    bIsGrabbed = false;
    HeldHandMesh = nullptr;
}

void ARadioActor::UpdateVolume()
{
    // 1) ���� �� ȸ��, �� ���
    FQuat CurrentHandRotation = HeldHandMesh->GetComponentTransform().GetRotation();
    FQuat DeltaQuat = InitialHandRotation.Inverse() * CurrentHandRotation;

    // 2) Twist �� (�� �޽��� ���� Y��)
    FVector TwistAxis = HeldHandMesh->GetComponentTransform().GetUnitAxis(EAxis::Y);

    // 3) (����ó��) ��ȣ �ִ� ���� ���� ��, ���� ó���� ���� ����
    float TwistAngle = -GetSignedTwistAngle(DeltaQuat, TwistAxis);

    // 4) ���� ȸ���� ��� & Ŭ����
    float NewDialPitch = GrabStartDialRot.Pitch + TwistAngle;
    NewDialPitch = FMath::Clamp(NewDialPitch, MinDialAngle, MaxDialAngle);

    // 5) Dummy ȸ���� ����
    FRotator NewRot = Dummy->GetRelativeRotation();
    NewRot.Pitch = NewDialPitch;
    Dummy->SetRelativeRotation(NewRot);

    // 6) **�׻�** ���� �����̽� (�� ƽ����)
    InitialHandRotation = CurrentHandRotation;
    GrabStartDialRot = NewRot;

    // 7) [Min,Max]��[2,0] ���� ���� (���� ����)
    float NewVolume = FMath::GetMappedRangeValueClamped(
        FVector2D(MinDialAngle, MaxDialAngle),
        FVector2D(2.0f, 0.0f),
        NewDialPitch
    );
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("%f"), NewVolume));
    // 8) SoundMix ����
    UGameplayStatics::SetSoundMixClassOverride(
        this,
        MySoundMix,
        BGMSoundClass,
        NewVolume,
        1.f,
        0.f,
        true
    );
    UGameplayStatics::PushSoundMixModifier(this, MySoundMix);
}


FQuat ARadioActor::GetTwistQuat(const FQuat& RotationDelta, const FVector& TwistAxis)
{
    FVector Axis = TwistAxis.GetSafeNormal();
    FVector DeltaV = FVector(RotationDelta.X, RotationDelta.Y, RotationDelta.Z);
    FVector Project = FVector::DotProduct(DeltaV, Axis) * Axis;
    FQuat   TwistQ = FQuat(Project.X, Project.Y, Project.Z, RotationDelta.W);
    return TwistQ.GetNormalized();
}

float ARadioActor::GetTwistAngleDegrees(const FQuat& RotationDelta, const FVector& TwistAxis)
{
    FQuat Twist = GetTwistQuat(RotationDelta, TwistAxis);
    float AngleRad = 2.f * FMath::Acos(FMath::Clamp(Twist.W, -1.f, 1.f));
    return FMath::RadiansToDegrees(AngleRad);
}

float ARadioActor::GetSignedTwistAngle(const FQuat& RotationDelta, const FVector& TwistAxis)
{
    // RotationDelta�� ��+������ ����
    FVector Axis;
    float   AngleRad;
    RotationDelta.ToAxisAndAngle(Axis, AngleRad);

    // ������ ��(��)�� ��ȯ
    float AngleDeg = FMath::RadiansToDegrees(AngleRad);

    // ȸ�� ��� Axis �������� ��ȣ ����
    float Sign = (FVector::DotProduct(Axis, TwistAxis) >= 0.f) ? 1.f : -1.f;
    return AngleDeg * Sign;
}
