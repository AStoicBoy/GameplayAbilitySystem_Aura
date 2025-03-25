// Copyright Dorian Souc


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();

		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();

		return AttributeMenuWidgetController;
	}
	return AttributeMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class Uninitialized, Please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class Uninitialized, Please fill out BP_AuraHUD"));
	checkf(AttributeMenuWidgetClass, TEXT("Please Fill Attribute Menu Class"))
	checkf(AttributeMenuWidgetControllerClass, TEXT("Please Fill Attribute Controller Menu Class"))

	if (!OverlayWidget)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
		OverlayWidget = Cast<UAuraUserWidget>(Widget);

		const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
		TObjectPtr<UOverlayWidgetController> WidgetController = GetOverlayWidgetController(WidgetControllerParams);

		OverlayWidget->SetWidgetController(WidgetController);

		WidgetController->BroadcastInitialValues();

		Widget->AddToViewport();
	}

}
