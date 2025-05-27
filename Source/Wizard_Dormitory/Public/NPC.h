// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCStructs.h"
#include "NPC.generated.h"

UCLASS()
class WIZARD_DORMITORY_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	//�������� �Լ�
	UFUNCTION(BlueprintCallable)
	void ApplyVisual(const FNPCVisualData& VisualData);
	/*
	// ���� ���� (BP���� �̵�/�ִϸ��̼� ����)
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC")
	void StartEntrance();
	*/
	// �ⱸ �̵�
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC")
	void StartExit();

	// �Ա��� ����
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC")
	void StartReturn();
	

	// �ⱸ ���� �� ȣ��
	UFUNCTION(BlueprintCallable, Category = "NPC")
	void NotifyExitComplete();

	// �Ա� ���� �� ȣ��
	UFUNCTION(BlueprintCallable, Category = "NPC")
	void NotifyReturnComplete();

	// �÷��̾� �Ǵ� ����
	UFUNCTION(BlueprintCallable, Category = "NPC")
	void HandleNPCDecision(bool bAccepted);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditDefaultsOnly, Category = "NPC")
	int32 FaceMaterialIndex = 0;

private:
	UPROPERTY()
	UStaticMeshComponent* HairComponent;
	UPROPERTY()
	UMaterialInstanceDynamic* FaceMaterialInstance;

	
};
