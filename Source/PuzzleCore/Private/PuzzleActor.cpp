// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleActor.h"
#include "PuzzleComponent.h"

// Sets default values
APuzzleActor::APuzzleActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Enable replication for this actor
	bReplicates = true;

	// Create default PuzzleComponent
	PuzzleComponent = CreateDefaultSubobject<UPuzzleComponent>(TEXT("PuzzleComponent"));
	// Ensure the component is replicated by default
	PuzzleComponent->SetIsReplicated(true);
}
