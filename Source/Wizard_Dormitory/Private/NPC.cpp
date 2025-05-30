// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Kismet/GameplayStatics.h"
#include "PlayGameModeBase.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//메시로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -89.0f));
		GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));
	}

}



// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	//동적 머터리얼생성
	UMaterialInterface* BaseMaterial = GetMesh()->GetMaterial(0);
	FaceMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	GetMesh()->SetMaterial(FaceMaterialIndex, FaceMaterialInstance);

}

void ANPC::ApplyVisual(const FNPCVisualData& VisualData)
{
	//headSocket 머리소켓 이름
	//머리에 헤어 붙이기
	if (VisualData.HairMesh)
	{
		//이미 헤어가 있다면 없애기
		if (HairComponent)
		{
			HairComponent->DestroyComponent();
		}

		HairComponent = NewObject<UStaticMeshComponent>(this, TEXT("HairComponent"));
		HairComponent->SetStaticMesh(VisualData.HairMesh);
		HairComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("headSocket"));
		HairComponent->RegisterComponent();
	}

	//머터리얼 UV파라미터 적용
	//벡터파라미터의 이름을 맞추면 바뀜
	if (FaceMaterialInstance)
	{
		FaceMaterialInstance->SetVectorParameterValue("EyeUV", FLinearColor(VisualData.FaceData.EyeUV.X, VisualData.FaceData.EyeUV.Y, 0, 0));
		FaceMaterialInstance->SetVectorParameterValue("MouthUV", FLinearColor(VisualData.FaceData.MouthUV.X, VisualData.FaceData.MouthUV.Y, 0, 0));
	}

	CurrentVisualData = VisualData;
	NPCDisplayName = VisualData.DisplayName;



}

void ANPC::NotifyExitComplete()
{
	if (APlayGameModeBase* GM = Cast<APlayGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->ApplyNextSeed(); // 다음 NPC 처리
	}
}

void ANPC::NotifyReturnComplete()
{
	if (APlayGameModeBase* GM = Cast<APlayGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->ApplyNextSeed(); // 다음 NPC 처리
	}
}

void ANPC::HandleNPCDecision(bool bAccepted)
{
	if (bAccepted)
	{
		StartExit(); // 출구로 이동
	}
	else
	{
		StartReturn(); // 입구로 복귀
	}
}

void ANPC::ShowAffiliationEffect(UNiagaraSystem* Effect)
{
	if (!CurrentVisualData.AffiliationEffect) return;

	// 이미 붙어 있으면 제거
	if (AffiliationEffectComponent)
	{
		AffiliationEffectComponent->DestroyComponent();
		AffiliationEffectComponent = nullptr;
	}

	// 손 소켓에 부착
	AffiliationEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		CurrentVisualData.AffiliationEffect,
		GetMesh(),
		TEXT("hand_rSocket"),// 손 소켓 이름 (스켈레탈 메시에 실제 있는 이름이어야 함)
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);
}

void ANPC::HideAffiliationEffect()
{
	if (AffiliationEffectComponent)
	{
		AffiliationEffectComponent->DestroyComponent();
		AffiliationEffectComponent = nullptr;
	}
}


// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

