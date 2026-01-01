// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "Command.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced, CollapseCategories, EditInlineNew)
class PUZZLECORE_API UCommand : public UObject
{
	GENERATED_BODY()
public:

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interact")
	// EInteractMode InteractMode = EInteractMode::WalkMode;

	// UPROPERTY(EditDefaultsOnly, Category = "Interact")
	// bool bUseApplyMode;;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact",meta = (UseComponentPicker, EditCondition = "bUseApplyMode", EditConditionHides))
	// EApplyMode ApplyMode = EApplyMode::Selected;


	// UPROPERTY(EditDefaultsOnly, Category = "Interact")
	// bool bUseComponent;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowedClasses="/Script/Engine.StaticMeshComponent",UseComponentPicker, EditCondition = "bUseComponent && ApplyMode == EApplyMode::Selected", EditConditionHides))
	// TArray<FComponentReference> ComponentRef;

	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void Execute(class UActorComponent* Parent);

	// UFUNCTION(BlueprintCallable,BlueprintImplementableEvent,BlueprintPure, Category = "Interact")
	// void GetInteractionComponent(class UActorComponent*& outInteractComponent);

	void NativeExecute();
};
