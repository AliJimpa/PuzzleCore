// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleComponent.h"
#include "PuzzleDebug.h"

// Sets default values for this component's properties
UPuzzleComponent::UPuzzleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
// Called when the game starts
void UPuzzleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (bAutoAvailable)
	{
		SetAvaliblePuzzle();
	}
	for (UPuzzleCheck *Check : Requirements)
	{
		if (IsValid(Check))
		{
			Check->BeginPuzzle(this);
		}
		else
		{
			LOG_WARNING("Find invalid PuzzleCheck in Requirements");
		}
	}
}
// Called every frame
void UPuzzleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
// Called when the game ends
void UPuzzleComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (UPuzzleCheck *Check : Requirements)
	{
		if (IsValid(Check))
		{
			Check->EndPuzzle(this);
		}
		else
		{
			LOG_WARNING("Find invalid PuzzleCheck in Requirements");
		}
	}

	Super::EndPlay(EndPlayReason);
}

EPuzzleState UPuzzleComponent::GetPuzzleState() const
{
	return PuzzleState;
}
bool UPuzzleComponent::IsPuzzleSolved() const
{
	return PuzzleState == EPuzzleState::Solved;
}
bool UPuzzleComponent::IsPuzzleLocked() const
{
	return PuzzleState == EPuzzleState::Locked;
}
bool UPuzzleComponent::IsPuzzleActive() const
{
	return PuzzleState != EPuzzleState::Unavailable;
}
FName UPuzzleComponent::GetPuzzleName() const
{
	return PuzzleName;
}
int32 UPuzzleComponent::GetTryCount() const
{
	return TryCount;
}
bool UPuzzleComponent::CanSolvePuzzle(UObject *Solver) const
{
	int32 OrderNumber = 0;
	int32 PassedCount = 0;
	const int32 RequiredPassCount = bUseMinimumRequirement && MinimumRequirement > 0 ? MinimumRequirement : Requirements.Num();

	for (UPuzzleCheck *Check : Requirements)
	{
		++OrderNumber;

		if (!IsValid(Check))
		{
			LOG_WARNING("Invalid PuzzleCheck found in Requirements");
			continue;
		}

		// Pre-check gate (custom extension point)
		if (!PrePuzzleCheck(Check))
		{
			if (!bUseMinimumRequirement)
			{
				return false;
			}
			continue;
		}

		// Execute actual puzzlecheck
		if (!Check->ExecuteCheck(Solver, OrderNumber))
		{
			if (!bUseMinimumRequirement)
			{
				return false;
			}
			continue;
		}

		// Passed this check
		++PassedCount;

		if (PassedCount >= RequiredPassCount)
		{
			return true;
		}
	}

	return PassedCount >= RequiredPassCount;
}
bool UPuzzleComponent::TrySolvePuzzle(UObject *Solver)
{
	if (PuzzleState == EPuzzleState::Unavailable)
	{
		return false;
	}

	if (PuzzleState == EPuzzleState::Failed && !bAllowAttemptAfterFailure)
	{
		PRINT("Cannot trySolve: Puzzle is Failed");
		return false;
	}

	if (PuzzleState != EPuzzleState::Solved)
	{
		TryCount++;
		if (CanSolvePuzzle(Solver))
		{
			SetState(EPuzzleState::Solved);
			OnSolved.Broadcast(this);
			return true;
		}
		else
		{
			if (bAutoReset)
			{
				ResetPuzzle();
			}
			else
			{
				SetState(EPuzzleState::Failed);
				OnFailed.Broadcast(this);
			}
			return false;
		}
	}
	else
	{
		LOG("The puzzle already solved 'Requirements' dident checked!");
		return true;
	}
}
void UPuzzleComponent::ResetPuzzle(bool bActive)
{
	for (UPuzzleCheck *Check : Requirements)
	{
		if (!IsValid(Check))
		{
			LOG_WARNING("Find invalid PuzzleCheck in Requirements");
			continue;
		}
		Check->ResetPuzzle(bActive);
	}
	TryCount = 0;
	SetState(bActive ? EPuzzleState::Locked : EPuzzleState::Unavailable);
	OnReset.Broadcast(bActive, this);
}
void UPuzzleComponent::SetAvaliblePuzzle(bool bEnable)
{
	SetState(EPuzzleState::Locked);
}
void UPuzzleComponent::SetState(const EPuzzleState NewState)
{
	PuzzleState = NewState;
	OnStateChanged.Broadcast(NewState, this);
}
bool UPuzzleComponent::PrePuzzleCheck_Implementation(UPuzzleCheck *PuzlleCheck) const
{
	return true;
}