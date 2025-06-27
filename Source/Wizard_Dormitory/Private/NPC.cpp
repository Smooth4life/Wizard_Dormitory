// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Kismet/GameplayStatics.h"
#include "PlayGameModeBase.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include <NPCDialogueRow.h>

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
	//0번 슬롯(피부) 머터리얼 인스턴스 (향후 피부색/질감 추가되면 사용)
	UMaterialInterface* BaseMaterial = GetMesh()->GetMaterial(0);
	FaceMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	GetMesh()->SetMaterial(FaceMaterialIndex, FaceMaterialInstance);

	//눈 머터리얼
	UMaterialInterface* EyeBaseMat = GetMesh()->GetMaterial(1);
	EyeMaterialInstance = UMaterialInstanceDynamic::Create(EyeBaseMat, this);
	GetMesh()->SetMaterial(1, EyeMaterialInstance);

	//입 머터리얼 나중에 메쉬에 추가되면 주석 없애기만 하면됨
	/*UMaterialInterface* MouthBaseMat = GetMesh()->GetMaterial(2);
	MouthMaterialInstance = UMaterialInstanceDynamic::Create(MouthBaseMat, this);
	GetMesh()->SetMaterial(2, MouthMaterialInstance);*/


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
	//if (FaceMaterialInstance)
	//{
	//	FaceMaterialInstance->SetVectorParameterValue("EyeUV", FLinearColor(VisualData.FaceData.EyeUV.X, VisualData.FaceData.EyeUV.Y, 0, 0));
	//	FaceMaterialInstance->SetVectorParameterValue("MouthUV", FLinearColor(VisualData.FaceData.MouthUV.X, VisualData.FaceData.MouthUV.Y, 0, 0));
	//	//UE_LOG(LogTemp, Warning, TEXT("Setting EyeUV: X=%f, Y=%f"), VisualData.FaceData.EyeUV.X, VisualData.FaceData.EyeUV.Y);

	//}
	if (EyeMaterialInstance)
	{
		EyeMaterialInstance->SetVectorParameterValue("EyeUV",
			FLinearColor(VisualData.FaceData.EyeUV.X, VisualData.FaceData.EyeUV.Y, 0, 0));
		//UE_LOG(LogTemp, Warning, TEXT("Setting EyeUV: X=%f, Y=%f"), VisualData.FaceData.EyeUV.X, VisualData.FaceData.EyeUV.Y);
		//UE_LOG(LogTemp, Warning, TEXT("Slot 1 Material: %s"), *GetMesh()->GetMaterial(1)->GetName());

	}

	if (MouthMaterialInstance)
	{
		MouthMaterialInstance->SetVectorParameterValue("MouthUV",
			FLinearColor(VisualData.FaceData.MouthUV.X, VisualData.FaceData.MouthUV.Y, 0, 0));
	}

	CurrentVisualData = VisualData;
	NPCDisplayName = VisualData.DisplayName;
	NPCDisplayID = LexToString(VisualData.StudentID);
	NPCDisplayAffiliation = VisualData.AffiliationName;
	NPCNameIndex = VisualData.NameIndex;

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
	if (!CurrentVisualData.AffiliationEffect)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("AAAAAAAAAAAAAAA"));
		return;
	}
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

void ANPC::NameIdentify()
{
	if (CurrentVisualData.NameIdentify)
	{
		UGameplayStatics::PlaySound2D(
			this,
			CurrentVisualData.NameIdentify
		);
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

void ANPC::PlayRandomDialogue(UDataTable* DialogueTable)
{
	if (!DialogueTable) return;

	const TArray<FName> RowNames = DialogueTable->GetRowNames();
	if (RowNames.Num() == 0) return;

	const FName Chosen = RowNames[FMath::RandRange(0, RowNames.Num() - 1)];
	const FNPCDialogueRow* Row = DialogueTable->FindRow<FNPCDialogueRow>(Chosen, TEXT("RandomDialogue"));

	if (Row)
	{
		UGameplayStatics::PlaySound2D(this, Row->Sound);
		ShowSubtitle(Row->Subtitle);

		GetWorldTimerManager().SetTimerForNextTick([this]()
			{
				FTimerHandle TimerHandle;
				GetWorldTimerManager().SetTimer(TimerHandle, [this]() { HideSubtitle(); }, 2.0f, false);
			});
	}
}

void ANPC::PlayNameDialogue(UDataTable* NameLineTable)
{
	if (!NameLineTable || NPCNameIndex < 0)
		return;

	const TArray<FName> RowNames = NameLineTable->GetRowNames();

	if (!RowNames.IsValidIndex(NPCNameIndex))
		return;

	const FName RowKey = RowNames[NPCNameIndex];
	const FNPCDialogueRow* Row = NameLineTable->FindRow<FNPCDialogueRow>(RowKey, TEXT("PlayNameDialogue"));

	if (!Row)
		return;

	UGameplayStatics::PlaySound2D(this, CurrentVisualData.NameIdentify);
	ShowSubtitle(Row->Subtitle);
}


