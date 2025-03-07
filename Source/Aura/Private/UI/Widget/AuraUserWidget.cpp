// Copyright Dorian Souc


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	if (WidgetController == InWidgetController)
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetController already set, skipping"));
		return;
	}
	WidgetController = InWidgetController;
	WidgetControllerSet();
	UE_LOG(LogTemp, Warning, TEXT("SetWidgetController called for %s"), *GetName());
}