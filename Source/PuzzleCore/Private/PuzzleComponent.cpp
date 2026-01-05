// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleComponent.h"
#include "PuzzleDebug.h"
#include "Net/UnrealNetwork.h"
#if !UE_BUILD_SHIPPING
#include "Kismet/KismetSystemLibrary.h"
#define PRINTSTRING(InString)          \
	UKismetSystemLibrary::PrintString( \
		GetWorld(), InString, true, false, FLinearColor::Red, 5.0f)
#else
#define PRINTSTRING(InString)
#endif

// Sets default values for this component's properties
UPuzzleComponent::UPuzzleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	SetIsReplicatedByDefault(true);
}
// Called when the game starts
void UPuzzleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (GetOwner()->GetNetMode() != NM_Standalone)
	{
		if (!GetOwner()->GetIsReplicated())
		{
			PRINTSTRING(FString::Printf(TEXT("%s owner is NOT replicated"), *GetOwner()->GetName()));
		}
	}

	if (GetOwner()->HasAuthority())
	{
		if (bAutoAvailable)
		{
			PuzzleState = EPuzzleState::Locked;
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
}
// Called when the game ends
void UPuzzleComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetOwner()->HasAuthority())
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
	}
	Super::EndPlay(EndPlayReason);
}
// Called for replicate properties function
void UPuzzleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	// Call the Super
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Add properties to replicated for the derived class
	DOREPLIFETIME(UPuzzleComponent, PuzzleState);
	DOREPLIFETIME(UPuzzleComponent, TryCount);
}

// Network Functions
void UPuzzleComponent::OnRep_PuzzleStateChanged()
{
	for (UPuzzleCheck *Check : Requirements)
	{
		if (!IsValid(Check))
		{
			LOG_WARNING("Find invalid 'PuzzleCheck' in requirements");
			continue;
		}
		Check->ChangePuzzleState(PuzzleState);
	}
	OnStateChanged.Broadcast(PuzzleState, this);
	switch (PuzzleState)
	{
	case EPuzzleState::Unavailable:
		OnReset.Broadcast(false, this);
		break;
	case EPuzzleState::Locked:
		OnReset.Broadcast(true, this);
		break;
	case EPuzzleState::Solved:
		OnSolved.Broadcast(this);
		break;
	case EPuzzleState::Failed:
		OnFailed.Broadcast(this);
		break;
	}
}

// Private Function
void UPuzzleComponent::SetState(const EPuzzleState NewState)
{
	if (PuzzleState == NewState)
		return;

	PuzzleState = NewState;

	if (GetOwner()->HasAuthority())
	{
		OnRep_PuzzleStateChanged();
	}
}

// Base PuzzleComponent Functions
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
	if (GetOwner()->HasAuthority())
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
	else
	{
		PRINTSTRING("SetAvailablePuzzle Should Call in Server");
		return false;
	}
}
bool UPuzzleComponent::TrySolvePuzzle(UObject *Solver)
{
	if (GetOwner()->HasAuthority())
	{
		if (PuzzleState == EPuzzleState::Unavailable)
		{
			return false;
		}

		if (PuzzleState == EPuzzleState::Failed && !bAllowAttemptAfterFailure)
		{
			PRINTSTRING(FString::Printf(TEXT("Cannot Solve: Puzzle's actor(%s) is Failed"), *GetOwner()->GetName()));
			return false;
		}

		if (PuzzleState != EPuzzleState::Solved)
		{
			TryCount++;
			if (CanSolvePuzzle(Solver))
			{
				SetState(EPuzzleState::Solved);
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
	else
	{
		PRINTSTRING("TrySolvePuzzle Should Call in Server");
		return false;
	}
}
void UPuzzleComponent::ResetPuzzle()
{
	if (GetOwner()->HasAuthority())
	{
		TryCount = 0;
		if (PuzzleState != EPuzzleState::Locked)
			SetState(EPuzzleState::Locked);
	}
	else
	{
		PRINTSTRING("ResetPuzzle Should Call in Server");
	}
}
void UPuzzleComponent::SetAvailablePuzzle(bool bEnable)
{
	if (GetOwner()->HasAuthority())
	{
		if (bEnable)
		{
			if (PuzzleState != EPuzzleState::Unavailable)
			{
				SetState(EPuzzleState::Unavailable);
			}
		}
		else
		{
			if (PuzzleState != EPuzzleState::Locked)
			{
				SetState(EPuzzleState::Locked);
			}
		}
	}
	else
	{
		PRINTSTRING("SetAvailablePuzzle Should Call in Server");
	}
}
bool UPuzzleComponent::PrePuzzleCheck_Implementation(UPuzzleCheck *PuzlleCheck) const
{
	return true;
}