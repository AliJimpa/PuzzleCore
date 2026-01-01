// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleInterface.h"
#include "PuzzleComponent.generated.h"

//UCLASS(Blueprintable, Abstract, ClassGroup = (PuzzleCore), meta = (BlueprintSpawnableComponent))
//UCLASS(Abstract, ClassGroup = (PuzzleCore), meta = (BlueprintSpawnableComponent))
UCLASS(Blueprintable, ClassGroup=(PuzzleCore), meta=(BlueprintSpawnableComponent=false, BlueprintType=true))
class PUZZLECORE_API UPuzzleComponent : public UActorComponent, public IPuzzleInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPuzzleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Puzzle")
	FName PuzzleName;
	EPuzzleState PuzzleState;
	int32 TryCount;

public:
	// Implement PuzzleInterface Interface
	virtual EPuzzleState GetPuzzleState_Implementation() const override;
	virtual bool IsPuzzleSolved_Implementation() const override;
	virtual bool IsPuzzleUnlocked_Implementation() const override;
	virtual bool IsPuzzleActive_Implementation() const override;
	virtual FName GetPuzzleName_Implementation() const override;
	virtual bool TrySolve_Implementation(UObject *Solver) override;
	virtual bool CanSolve_Implementation(UObject *Solver) const override;
	virtual int32 GetTryCount_Implementation() const override;
	virtual void ResetPuzzle_Implementation() override;
};
