#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "ItemFragment.generated.h"

class ARoomActor;
class AEquipActor;
class UCompositeBase;
class APlayerController;

USTRUCT(BlueprintType)
struct FItemFragment
{
	GENERATED_BODY()

	FItemFragment() {}
	FItemFragment(const FItemFragment&) = default;
	FItemFragment& operator=(const FItemFragment&) = default;
	FItemFragment(FItemFragment&&) = default;
	FItemFragment& operator=(FItemFragment&&) = default;
	virtual ~FItemFragment() {}

	virtual void Manifest() {}
	
	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(FGameplayTag Tag) { FragmentTag = Tag; }

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta=(Categories="Fragment"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

/*
 * ItemFragment specifically for assimilation into a widget
 */
USTRUCT(BlueprintType)
struct FInventoryItemFragment : public FItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UCompositeBase* Composite) const;

protected:
	bool MatchesWidgetTag(const UCompositeBase* Composite) const;
};

USTRUCT(BlueprintType)
struct FGridFragment : public FItemFragment
{
	GENERATED_BODY()

	FIntPoint GetGridSize() const { return GridSize; }
	void SetGridSize(FIntPoint Size) { GridSize = Size; }
	float GetGridPadding() const { return GridPadding; }
	void SetGridPadding(float Padding) { GridPadding = Padding; }
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FIntPoint GridSize{ 1, 1 };

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	float GridPadding{ 0.f };
};

USTRUCT(BlueprintType)
struct FImageFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UCompositeBase* Composite) const override;
	UTexture2D* GetIcon() const { return Icon; }

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<UTexture2D> Icon{nullptr};

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FVector2D Dimensions{ 44.f, 44.f };
};

USTRUCT(BlueprintType)
struct FTextFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UCompositeBase* Composite) const override;
	void SetText(const FText& Text) { FragmentText = Text; }
	FText GetText() const { return FragmentText; }

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FText FragmentText;
};

USTRUCT(BlueprintType)
struct FLabeledNumberFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UCompositeBase* Composite) const override;
	virtual void Manifest() override;
	
	void SetLabelText(const FText& Text) { Text_Label = Text; }
	FText GetLabelText() const { return Text_Label; }
	void SetValue(float NewValue) { Value = NewValue; }
	float GetValue() const { return Value; }

	bool bRandomizeOnManifest{true};

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FText Text_Label;
	
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	float Value{0.f};
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	float Min{0.f};
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	float Max{0.f};

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	bool bCollapseLabel{false};
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	bool bCollapseValue{false};

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	int32 MinFractionalDigits{1};
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	int32 MaxFractionalDigits{1};
};

USTRUCT(BlueprintType)
struct FStackableFragment : public FItemFragment
{
	GENERATED_BODY()

	int32 GetMaxStackSize() const { return MaxStackSize; }
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	int32 MaxStackSize{1};
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	int32 StackCount{1};
};

/*
 * Consumables
 */
USTRUCT(BlueprintType)
struct FConsumeModifier : public FLabeledNumberFragment
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC) {}	
};

USTRUCT(BlueprintType)
struct FConsumableFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UCompositeBase* Composite) const override;
	virtual void OnConsume(APlayerController* PC);
	virtual void Manifest() override;

private:
	UPROPERTY(EditAnywhere, Category="Inventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FConsumeModifier>> ConsumeModifiers;
};

USTRUCT(BlueprintType)
struct FHealthPotionFragment : public FConsumeModifier
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FManaPotionFragment : public FConsumeModifier
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC) override;
};

/*
 * Equippables
 */
USTRUCT()
struct FEquipmentModifier : public FLabeledNumberFragment
{
	GENERATED_BODY()

	virtual void OnEquip(APlayerController* PC) {}
	virtual void OnUnequip(APlayerController* PC) {}
};

USTRUCT(BlueprintType)
struct FStrengthModifier : public FEquipmentModifier
{
	GENERATED_BODY()

	virtual void OnEquip(APlayerController* PC) override;
	virtual void OnUnequip(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FArmorModifier : public FEquipmentModifier
{
	GENERATED_BODY()

	virtual void OnEquip(APlayerController* PC) override;
	virtual void OnUnequip(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FDamageModifier : public FEquipmentModifier
{
	GENERATED_BODY()

	virtual void OnEquip(APlayerController* PC) override;
	virtual void OnUnequip(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FEquipmentFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UCompositeBase* Composite) const override;
	virtual void Manifest() override;
	void OnEquip(APlayerController* PC);
	void OnUnequip(APlayerController* PC);
	AEquipActor* SpawnAttachedActor(USkeletalMeshComponent* AttachMesh) const;
	void DestroyAttachedActor() const;
	FGameplayTag GetEquipmentType() const { return EquipmentType; }
	void SetEquippedActor(AEquipActor* EquipActor);
	
	bool bEquipped{false};

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory"/*, meta=(ExcludeBaseStruct)*/)
	TArray<TInstancedStruct<FEquipmentModifier>> EquipmentModifiers;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<AEquipActor> EquipActorClass = nullptr;

	TWeakObjectPtr<AEquipActor> EquippedActor = nullptr;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FName SocketAttachPoint{ NAME_None };

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FGameplayTag EquipmentType = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FRoomFragment : public FInventoryItemFragment
{
	GENERATED_BODY()
	
public:
	virtual void Assimilate(UCompositeBase* Composite) const override;
	virtual void Manifest() override;
	void OnSpawn(APlayerController* PC);
	ARoomActor* SpawnRoomActor(UObject* Outer) const;
	ARoomActor* GetRoomActor() const { return RoomActor.Get(); }
	FGameplayTag GetRoomType() const { return RoomType; }
	void SetSpawnedRoomActor(ARoomActor* Room);
	TMap<FIntPoint, FName> GetSockets() const { return Sockets; }
	int32 GetYaw() const { return Yaw; }
	void SetYaw(int32 NewYaw) { Yaw = NewYaw; }

private:	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<ARoomActor> RoomActorClass = nullptr;

	UPROPERTY()
	TObjectPtr<ARoomActor> RoomActor = nullptr;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TMap<FIntPoint, FName> Sockets;

	int32 Yaw{ 0 };

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FGameplayTag RoomType = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FRequirementFragment : public FItemFragment
{
	GENERATED_BODY()
	
public:
	FGameplayTag GetItemType() const { return RequirementType; }
	void SetRequirementType(FGameplayTag Type) { RequirementType = Type; }
	int32 GetAmount() const { return RequirementAmount; }
	void SetRequirementAmount(int32 NewAmount) { RequirementAmount = NewAmount; }
	UTexture2D* GetIcon() const { return RequirementIcon; }
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	UTexture2D* RequirementIcon = nullptr;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta=(Categories="Item.Currency"))
	FGameplayTag RequirementType = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	int32 RequirementAmount{1};
};

USTRUCT(BlueprintType)
struct FValuableFragment : public FItemFragment
{
	GENERATED_BODY()
	
public:
	TMap<UTexture2D*, int32> GetValuables() const { return Valuables; };
	TArray<TSubclassOf<AActor>> GetValuableItems() const { return ValuableItems; };
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TMap<UTexture2D*, int32> Valuables;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TArray<TSubclassOf<AActor>> ValuableItems;
};