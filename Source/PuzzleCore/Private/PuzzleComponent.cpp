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
bool UPuzzleComponent::CanSolve(UObject *Solver) const
{
	int OrderNumber = 0;
	for (UPuzzleCheck *Check : Requirements)
	{
		OrderNumber++;
		// if the checkvalid reach to minimom threshould pass true
		if (OrderNumber == MinimomRequirement)
		{
			return true;
		}
		// check is class valid
		if (!IsValid(Check))
		{
			LOG_WARNING("Find invalid PuzzleCheck in Requirements");
			continue;
		}
		// Check Rule is custom function for extend additional rule for checking in blueprin
		if (!CheckRule(Check))
		{
			return false;
		}
		// Check Function inside Check puzzle
		if (!Check->ExecuteCheck(Solver, OrderNumber))
		{
			return false;
		}
	}
	return true;
}
EPuzzleState UPuzzleComponent::TrySolve(UObject *Solver)
{
	if (PuzzleState == EPuzzleState::Unavailable)
	{
		return EPuzzleState::Unavailable;
	}
	if (PuzzleState == EPuzzleState::Locked)
	{
		TryCount++;
		if (CanSolve(Solver))
		{
			SetState(EPuzzleState::Solved);
			OnSolved.Broadcast(this);
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
		}
	}
	return PuzzleState;
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

bool UPuzzleComponent::CheckRule_Implementation(UPuzzleCheck *PuzlleCheck) const
{
	return true;
}