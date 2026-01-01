// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleComponent.h"

// Sets default values for this component's properties
UPuzzleComponent::UPuzzleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UPuzzleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}
// Called every frame
void UPuzzleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Implement PuzzleInterface Interface
EPuzzleState UPuzzleComponent::GetPuzzleState_Implementation() const
{
	return PuzzleState;
}
bool UPuzzleComponent::IsPuzzleSolved_Implementation() const
{
	return PuzzleState == EPuzzleState::Solved;
}
bool UPuzzleComponent::IsPuzzleUnlocked_Implementation() const
{
	return PuzzleState == EPuzzleState::Solved || PuzzleState == EPuzzleState::Failed;
}
bool UPuzzleComponent::IsPuzzleActive_Implementation() const
{
	return PuzzleState != EPuzzleState::Unavailable;
}
FName UPuzzleComponent::GetPuzzleName_Implementation() const
{
	return PuzzleName;
}
bool UPuzzleComponent::CanSolve_Implementation(UObject *Solver) const
{
	return PuzzleState == EPuzzleState::Unavailable;
	// if (Solver == nullptr)
	// {
	// }
}
bool UPuzzleComponent::TrySolve_Implementation(UObject *Solver)
{
	// if (!CanSolve(Solver))
	// {
	// 	return false;
	// }

	// TryCount++;

	// // Default behavior: auto-solve
	// PuzzleState = EPuzzleState::Solved;
	return true;
}
int32 UPuzzleComponent::GetTryCount_Implementation() const
{
	return TryCount;
}
void UPuzzleComponent::ResetPuzzle_Implementation()
{
	//PuzzleState = EPuzzleState::Inactive;
	TryCount = 0;
}
