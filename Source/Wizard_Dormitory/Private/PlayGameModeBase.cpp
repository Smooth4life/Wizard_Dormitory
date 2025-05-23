// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameModeBase.h"
#include "NPC.h"
#include "NPCUtils.h"
#include "Kismet/GameplayStatics.h"

void APlayGameModeBase::BeginPlay()
{
	FNPCSeedData TestSeed;
	TestSeed.HairIndex = 1;
	TestSeed.EyeIndex = 0;
	TestSeed.MouthIndex = 2;

	FVector SpawnLocation(0, 0, 100.0f);
	SpawnNPCWithSeed(TestSeed, SpawnLocation);



}

void APlayGameModeBase::SpawnNPCWithSeed(const FNPCSeedData& Seed, const FVector& SpawnLocation)
{
	if (!NPCClass)return;
	FActorSpawnParameters Params;
	ANPC* NPC = GetWorld()->SpawnActor<ANPC>(NPCClass, SpawnLocation, FRotator::ZeroRotator, Params);
	if (NPC)
	{
		FNPCVisualData Visual = ConvertSeedToVisual(Seed, NPCLibrary);
		NPC->ApplyVisual(Visual);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("1"));
	}


}
