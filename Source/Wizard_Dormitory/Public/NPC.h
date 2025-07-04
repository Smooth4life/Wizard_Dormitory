// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCStructs.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"
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

	// 소속 이펙트 보여주기 (손 위에 스폰)
	UFUNCTION(BlueprintCallable, Category = "Affiliation")
	void ShowAffiliationEffect(UNiagaraSystem* Effect);

	// 소속 이펙트 제거
	UFUNCTION(BlueprintCallable, Category = "Affiliation")
	void HideAffiliationEffect();

	UFUNCTION(BlueprintCallable, Category = "NameIdentify")
	void NameIdentify();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Affiliation")
	UNiagaraComponent* AffiliationEffectComponent = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//피부 머터리얼
	UPROPERTY(EditDefaultsOnly, Category = "NPC")
	int32 FaceMaterialIndex = 0;

	UPROPERTY()
	FNPCVisualData CurrentVisualData;

	UPROPERTY(BlueprintReadOnly)
	FString NPCDisplayName;

	UPROPERTY(BlueprintReadOnly)
	FString NPCDisplayID;

	UPROPERTY(BlueprintReadOnly)
	FString NPCDisplayAffiliation;

	//눈, 입 머터리얼
	UPROPERTY()
	UMaterialInstanceDynamic* EyeMaterialInstance;
	UPROPERTY()
	UMaterialInstanceDynamic* MouthMaterialInstance;

	UPROPERTY(BlueprintReadOnly)
	int32 NPCNameIndex;
	
	// 자막 관련 DataTable 참조
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Subtitle")
	UDataTable* GreetingDialogueTable;

	UFUNCTION(BlueprintCallable, Category = "Subtitle")
	void PlayRandomDialogue(UDataTable* DialogueTable);

	// 자막 출력 - 블루프린트에서 구현
	UFUNCTION(BlueprintImplementableEvent, Category = "Subtitle")
	void ShowSubtitle(const FText& Text);

	UFUNCTION(BlueprintImplementableEvent, Category = "Subtitle")
	void HideSubtitle();

	//이름
	UFUNCTION(BlueprintCallable, Category = "Subtitle")
	void PlayNameDialogue(UDataTable* NameLineTable);


	// 학번 말하기
	UFUNCTION(BlueprintCallable, Category = "Subtitle")
	void PlayStudentIDDialogue();

	// 숫자 0~9에 대응하는 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StudentID|Sound")
	TArray<USoundBase*> DigitSounds;

	// “입니다” 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StudentID|Sound")
	USoundBase* IDEndingSound;


private:
	UPROPERTY()
	UStaticMeshComponent* HairComponent;

	UPROPERTY()
	UMaterialInstanceDynamic* FaceMaterialInstance;



	
};
