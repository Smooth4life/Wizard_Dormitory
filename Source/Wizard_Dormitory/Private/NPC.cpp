// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Kismet/GameplayStatics.h"
#include "PlayGameModeBase.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//�޽÷ε�
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
	//���� ���͸������
	UMaterialInterface* BaseMaterial = GetMesh()->GetMaterial(0);
	FaceMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	GetMesh()->SetMaterial(FaceMaterialIndex, FaceMaterialInstance);

}

void ANPC::ApplyVisual(const FNPCVisualData& VisualData)
{
	//headSocket �Ӹ����� �̸�
	//�Ӹ��� ��� ���̱�
	if (VisualData.HairMesh)
	{
		//�̹� �� �ִٸ� ���ֱ�
		if (HairComponent)
		{
			HairComponent->DestroyComponent();
		}

		HairComponent = NewObject<UStaticMeshComponent>(this, TEXT("HairComponent"));
		HairComponent->SetStaticMesh(VisualData.HairMesh);
		HairComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("headSocket"));
		HairComponent->RegisterComponent();
	}

	//���͸��� UV�Ķ���� ����
	//�����Ķ������ �̸��� ���߸� �ٲ�
	if (FaceMaterialInstance)
	{
		FaceMaterialInstance->SetVectorParameterValue("EyeUV", FLinearColor(VisualData.FaceData.EyeUV.X, VisualData.FaceData.EyeUV.Y, 0, 0));
		FaceMaterialInstance->SetVectorParameterValue("MouthUV", FLinearColor(VisualData.FaceData.MouthUV.X, VisualData.FaceData.MouthUV.Y, 0, 0));
	}

}

void ANPC::NotifyExitComplete()
{
	if (APlayGameModeBase* GM = Cast<APlayGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->ApplyNextSeed(); // ���� NPC ó��
	}
}

void ANPC::NotifyReturnComplete()
{
	if (APlayGameModeBase* GM = Cast<APlayGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->ApplyNextSeed(); // ���� NPC ó��
	}
}

void ANPC::HandleNPCDecision(bool bAccepted)
{
	if (bAccepted)
	{
		StartExit(); // �ⱸ�� �̵�
	}
	else
	{
		StartReturn(); // �Ա��� ����
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

