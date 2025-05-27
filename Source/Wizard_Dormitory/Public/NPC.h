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

	//외형적용 함수
	UFUNCTION(BlueprintCallable)
	void ApplyVisual(const FNPCVisualData& VisualData);
	/*
	// 입장 시작 (BP에서 이동/애니메이션 구현)
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC")
	void StartEntrance();
	*/
	// 출구 이동
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC")
	void StartExit();

	// 입구로 복귀
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC")
	void StartReturn();
	

	// 출구 도착 후 호출
	UFUNCTION(BlueprintCallable, Category = "NPC")
	void NotifyExitComplete();

	// 입구 도착 후 호출
	UFUNCTION(BlueprintCallable, Category = "NPC")
	void NotifyReturnComplete();

	// 플레이어 판단 전달
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
