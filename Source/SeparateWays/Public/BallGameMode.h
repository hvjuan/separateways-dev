// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BallGameMode.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class SEPARATEWAYS_API ABallGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ball|Inputs")
	TArray<UInputMappingContext*> PlayerInputContexts;

private:
	void CreateLocalPlayer();
};
