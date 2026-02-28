#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Item/Fragment/FragmentTags.h"
#include "SocketFragment.generated.h"

USTRUCT()
struct FSocketFragment
{
	GENERATED_BODY()

public:
	FIntPoint GetOffset() const { return Offset; }
	FName GetSocket() const { return Socket; }
	FGameplayTag GetLayer() const { return Layer; }

protected:
	FIntPoint Offset = FIntPoint(0, 0);
	FName Socket{ NAME_None };
	FGameplayTag Layer = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FEast_Middle : public FSocketFragment
{
	GENERATED_BODY()
	
	FEast_Middle()
	{
		Offset = FIntPoint(1, 0);
		Socket = FName{ "East" };
		Layer = ::Layer::Middle;
	}
};

USTRUCT(BlueprintType)
struct FWest_Middle : public FSocketFragment
{
	GENERATED_BODY()
	
	FWest_Middle()
	{
		Offset = FIntPoint(-1, 0);
		Socket = FName{ "West" };
		Layer = ::Layer::Middle;
	}
};

USTRUCT(BlueprintType)
struct FNorth_Middle : public FSocketFragment
{
	GENERATED_BODY()
	
	FNorth_Middle()
	{
		Offset = FIntPoint( 0, 1);
		Socket = FName{ "North" };
		Layer = ::Layer::Middle;
	}
};

USTRUCT(BlueprintType)
struct FSouth_Middle : public FSocketFragment
{
	GENERATED_BODY()
	
	FSouth_Middle()
	{
		Offset = FIntPoint(0, -1);
		Socket = FName{ "South" };
		Layer = ::Layer::Middle;
	}
};

/*
 * Lower Layer
 */
USTRUCT(BlueprintType)
struct FEast_Lower : public FSocketFragment
{
	GENERATED_BODY()
	
	FEast_Lower()
	{
		Offset = FIntPoint(1, 0);
		Socket = FName{ "East_Lower" };
		Layer = ::Layer::Lower;
	}
};

USTRUCT(BlueprintType)
struct FWest_Lower : public FSocketFragment
{
	GENERATED_BODY()
	
	FWest_Lower()
	{
		Offset = FIntPoint(-1, 0);
		Socket = FName{ "West_Lower" };
		Layer = ::Layer::Lower;
	}
};

USTRUCT(BlueprintType)
struct FNorth_Lower : public FSocketFragment
{
	GENERATED_BODY()
	
	FNorth_Lower()
	{
		Offset = FIntPoint( 0, 1);
		Socket = FName{ "North_Lower" };
		Layer = ::Layer::Lower;
	}
};

USTRUCT(BlueprintType)
struct FSouth_Lower : public FSocketFragment
{
	GENERATED_BODY()
	
	FSouth_Lower()
	{
		Offset = FIntPoint(0, -1);
		Socket = FName{ "South_Lower" };
		Layer = ::Layer::Lower;
	}
};

/*
 * Upper Layer
 */
USTRUCT(BlueprintType)
struct FEast_Upper : public FSocketFragment
{
	GENERATED_BODY()
	
	FEast_Upper()
	{
		Offset = FIntPoint(1, 0);
		Socket = FName{ "East_Upper" };
		Layer = ::Layer::Upper;
	}
};

USTRUCT(BlueprintType)
struct FWest_Upper : public FSocketFragment
{
	GENERATED_BODY()
	
	FWest_Upper()
	{
		Offset = FIntPoint(-1, 0);
		Socket = FName{ "West_Upper" };
		Layer = ::Layer::Upper;
	}
};

USTRUCT(BlueprintType)
struct FNorth_Upper : public FSocketFragment
{
	GENERATED_BODY()
	
	FNorth_Upper()
	{
		Offset = FIntPoint( 0, 1);
		Socket = FName{ "North_Upper" };
		Layer = ::Layer::Upper;
	}
};

USTRUCT(BlueprintType)
struct FSouth_Upper : public FSocketFragment
{
	GENERATED_BODY()
	
	FSouth_Upper()
	{
		Offset = FIntPoint(0, -1);
		Socket = FName{ "South_Upper" };
		Layer = ::Layer::Upper;
	}
};