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
    // 1) 현재 손 회전, Δ 계산
    FQuat CurrentHandRotation = HeldHandMesh->GetComponentTransform().GetRotation();
    FQuat DeltaQuat = InitialHandRotation.Inverse() * CurrentHandRotation;

    // 2) Twist 축 (손 메시의 로컬 Y축)
    FVector TwistAxis = HeldHandMesh->GetComponentTransform().GetUnitAxis(EAxis::Y);

    // 3) (기존처럼) 부호 있는 각도 구한 뒤, 음수 처리로 방향 반전
    float TwistAngle = -GetSignedTwistAngle(DeltaQuat, TwistAxis);

    // 4) 누적 회전값 계산 & 클램프
    float NewDialPitch = GrabStartDialRot.Pitch + TwistAngle;
    NewDialPitch = FMath::Clamp(NewDialPitch, MinDialAngle, MaxDialAngle);

    // 5) Dummy 회전에 적용
    FRotator NewRot = Dummy->GetRelativeRotation();
    NewRot.Pitch = NewDialPitch;
    Dummy->SetRelativeRotation(NewRot);

    // 6) **항상** 기준 리베이스 (매 틱마다)
    InitialHandRotation = CurrentHandRotation;
    GrabStartDialRot = NewRot;

    // 7) [Min,Max]→[2,0] 으로 매핑 (볼륨 반전)
    float NewVolume = FMath::GetMappedRangeValueClamped(
        FVector2D(MinDialAngle, MaxDialAngle),
        FVector2D(2.0f, 0.0f),
        NewDialPitch
    );
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("%f"), NewVolume));
    // 8) SoundMix 적용
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
    // RotationDelta를 축+각도로 분해
    FVector Axis;
    float   AngleRad;
    RotationDelta.ToAxisAndAngle(Axis, AngleRad);

    // 라디안을 도(°)로 변환
    float AngleDeg = FMath::RadiansToDegrees(AngleRad);

    // 회전 축과 Axis 내적으로 부호 결정
    float Sign = (FVector::DotProduct(Axis, TwistAxis) >= 0.f) ? 1.f : -1.f;
    return AngleDeg * Sign;
}
