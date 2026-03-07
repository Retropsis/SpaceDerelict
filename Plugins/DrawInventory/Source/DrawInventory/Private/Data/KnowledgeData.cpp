// Retropsis 2026


#include "Data/KnowledgeData.h"

FSymbolData UKnowledgeData::GetSymbolDataByTag(const FGameplayTag& Tag)
{
	for (const FSymbolData& Symbol : Symbols)
	{
		if (Symbol.KnowledgeTag.MatchesTagExact(Tag))
		{
			return Symbol;
		}
	}
	return FSymbolData();
}

FSymbolData UKnowledgeData::GetNumericSymbolDataByInt(int32 Integer)
{
	for (const FSymbolData& Symbol : Symbols)
	{
		if (FCString::Atoi(*Symbol.KnowledgeTag.GetTagLeafName().ToString()) == Integer)
		{
			return Symbol;
		}
	}
	return FSymbolData();
}
