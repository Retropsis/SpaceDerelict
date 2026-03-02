// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FirstPersonCharacter.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UFirstPersonCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DRAWINVENTORY_API IFirstPersonCharacter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USkeletalMeshComponent* GetFirstPersonSkeletalMesh() const;
};
