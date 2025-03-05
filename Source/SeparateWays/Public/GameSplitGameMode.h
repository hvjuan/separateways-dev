// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameSplitGameMode.generated.h"

class ABall;

/**
 *
 */
UCLASS()
class SEPARATEWAYS_API AGameSplitGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameSplitGameMode();

protected:
	UFUNCTION(BlueprintCallable, Category = "GameSplit")
	void AddLocalPlayers();

	virtual void BeginPlay() override;
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};