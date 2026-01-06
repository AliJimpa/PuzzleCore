// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleActor.generated.h"

class UPuzzleComponent;

UCLASS()
class PUZZLECORE_API APuzzleActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APuzzleActor();

protected:
	// The default puzzle component
	UPROPERTY(VisibleAnywhere, Instanced, BlueprintReadOnly, Category = "Puzzle")
	UPuzzleComponent *PuzzleComponent;
};
