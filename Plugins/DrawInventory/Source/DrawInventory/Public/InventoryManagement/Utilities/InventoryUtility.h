// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Component/ItemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Widget/Utiliies/WidgetUtiliies.h"
#include "InventoryUtility.generated.h"

class UInventoryBase;
class UHoverItem;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API UInventoryUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UInventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static EGridCategory GetItemCategoryFromItemComponent(const UItemComponent* ItemComponent);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void ItemHovered(APlayerController* PC, UInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void ItemUnhovered(APlayerController* PC);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UHoverItem* GetHoverItem(APlayerController* PC);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UInventoryBase* GetInventoryWidget(APlayerController* PC);
	
	template<typename T, typename FuncT>
	static void ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Function);
};

template<typename T, typename FuncT>
void UInventoryUtility::ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Function)
{
	for (int32 j = 0; j < Range2D.Y; ++j)
	{
		for (int32 i = 0; i < Range2D.X; ++i)
		{
			const FIntPoint Coordinates = UWidgetUtiliies::GetPositionFromIndex(Index, GridColumns) + FIntPoint(i, j);
			const int32 TileIndex = UWidgetUtiliies::GetIndexFromPosition(Coordinates, GridColumns);
			if (Array.IsValidIndex(TileIndex))
			{
				Function(Array[TileIndex]);
			}
		}
	}
}