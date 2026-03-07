// Retropsis 2026

#include "Widget/Knowledge/KnowledgeLog.h"
#include "Interaction/KnowledgeComponent.h"
#include "KnowledgeLog/KnowledgeTags.h"
#include "Widget/Knowledge/KnowledgePage.h"
#include "Widget/Knowledge/SymbolWidget.h"

void UKnowledgeLog::OnKnowledgeReceived(const UKnowledgeComponent* KnowledgeComponent) const
{
	if (!IsValid(KnowledgeComponent)) return;
	
	if (KnowledgeComponent->GetKnowledgeCategory().MatchesTagExact(Knowledge::Category::Symbol))
	{
		USymbolWidget* SymbolWidget = CreateWidget<USymbolWidget>(GetOwningPlayer(), SymbolWidgetClass);
		const FKnowledgeImage* KnowledgeImage = KnowledgeComponent->GetFragmentOfType<FKnowledgeImage>();
		const FKnowledgeText* KnowledgeText = KnowledgeComponent->GetFragmentOfType<FKnowledgeText>();
		if (!KnowledgeImage || !KnowledgeText) return;
		
		SymbolWidget->SetImage(KnowledgeImage->GetImage());
		SymbolWidget->SetText(KnowledgeText->GetText());
		SymbolWidget->SetKnowledgeTag(KnowledgeComponent->GetKnowledgeTag());
		Page_Symbols->AddWidget(SymbolWidget);
	}
}
