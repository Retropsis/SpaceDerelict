// Retropsis 2026

#include "Widget/Knowledge/KnowledgePage.h"
#include "Components/VerticalBox.h"
#include "Widget/Knowledge/KnowledgeWidget.h"

void UKnowledgePage::AddWidget(UKnowledgeWidget* NewWidget)
{
	TObjectPtr<UKnowledgeWidget> FoundWidget = nullptr;
	for (UKnowledgeWidget* Widget : KnowledgeWidgets)
	{
		if (IsValid(Widget) && Widget->GetKnowledgeTag().MatchesTagExact(NewWidget->GetKnowledgeTag()))
		{
			FoundWidget = Widget;
		}
	}
	if (!IsValid(FoundWidget))
	{
		VerticalBox_Log->AddChildToVerticalBox(NewWidget);
		KnowledgeWidgets.Add(NewWidget);
	}
}
