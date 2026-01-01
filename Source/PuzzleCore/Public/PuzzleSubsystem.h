// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PuzzleSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPuzzleEvent, EPuzzleState);
/**
 *
 */
UCLASS()
class PUZZLECORE_API UPuzzleSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// FOnPuzzleEvent OnPuzzleEvent;

	// void NotifyPuzzleEvent(EPuzzleState Event)
	// {
	// 	OnPuzzleEvent.Broadcast(Event);
	// }
};
