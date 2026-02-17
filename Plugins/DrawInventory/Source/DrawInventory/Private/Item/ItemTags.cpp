
#include "Item/ItemTags.h"


namespace Item
{
	namespace Currency
	{
		UE_DEFINE_GAMEPLAY_TAG(Key, "Item.Currency.Key");
		UE_DEFINE_GAMEPLAY_TAG(Battery, "Item.Currency.Battery");
		UE_DEFINE_GAMEPLAY_TAG(Oxygen, "Item.Currency.Oxygen");
	}
	namespace Equipment
	{
		namespace Weapons
		{
			UE_DEFINE_GAMEPLAY_TAG(Axe, "Item.Equipment.Weapons.Axe");
			UE_DEFINE_GAMEPLAY_TAG(Sword, "Item.Equipment.Weapons.Sword");
			UE_DEFINE_GAMEPLAY_TAG(Knife, "Item.Equipment.Weapons.Knife")
			UE_DEFINE_GAMEPLAY_TAG(Bloodthorn, "Item.Equipment.Weapons.Bloodthorn")
			UE_DEFINE_GAMEPLAY_TAG(NightsEdge, "Item.Equipment.Weapons.NightsEdge")
			UE_DEFINE_GAMEPLAY_TAG(Shadowbane, "Item.Equipment.Weapons.Shadowbane")
			UE_DEFINE_GAMEPLAY_TAG(Staff, "Item.Equipment.Weapons.Staff")
		}
		
		namespace Cloaks
		{
			UE_DEFINE_GAMEPLAY_TAG(RedCloak, "Item.Equipment.Cloaks.RedCloak");
			UE_DEFINE_GAMEPLAY_TAG(BlueCloak, "Item.Equipment.Cloaks.BlueCloak");
			UE_DEFINE_GAMEPLAY_TAG(GreenCloak, "Item.Equipment.Cloaks.GreenCloak")
			UE_DEFINE_GAMEPLAY_TAG(YellowCloak, "Item.Equipment.Cloaks.YellowCloak")
			UE_DEFINE_GAMEPLAY_TAG(DarkGrayCloak, "Item.Equipment.Cloaks.DarkGrayCloak")
		}
		
		namespace Masks
		{
			UE_DEFINE_GAMEPLAY_TAG(SteelMask, "Item.Equipment.Masks.SteelMask");
			UE_DEFINE_GAMEPLAY_TAG(GreenMask, "Item.Equipment.Masks.GreenMask");
			UE_DEFINE_GAMEPLAY_TAG(BlueMask, "Item.Equipment.Masks.BlueMask")
			UE_DEFINE_GAMEPLAY_TAG(YellowMask, "Item.Equipment.Masks.YellowMask")
			UE_DEFINE_GAMEPLAY_TAG(GrayMask, "Item.Equipment.Masks.GrayMask")
		}
	}

	namespace Consumable
	{
		namespace Potions
		{
			namespace Red
			{
				UE_DEFINE_GAMEPLAY_TAG(Small, "Item.Consumable.Potions.Red.Small");
				UE_DEFINE_GAMEPLAY_TAG(Large, "Item.Consumable.Potions.Red.Large");
			}
			
			namespace Blue
			{
				UE_DEFINE_GAMEPLAY_TAG(Small, "Item.Consumable.Potions.Blue.Small");
				UE_DEFINE_GAMEPLAY_TAG(Large, "Item.Consumable.Potions.Blue.Large");
			}
		}
	}
	
	namespace Craftable
	{
		UE_DEFINE_GAMEPLAY_TAG(FireFernFruit, "Item.Craftable.FireFernFruit");
		UE_DEFINE_GAMEPLAY_TAG(LuminDaisy, "Item.Craftable.LuminDaisy");
		UE_DEFINE_GAMEPLAY_TAG(ScorchPetalBlossom, "Item.Craftable.ScorchPetalBlossom");
	}
}

