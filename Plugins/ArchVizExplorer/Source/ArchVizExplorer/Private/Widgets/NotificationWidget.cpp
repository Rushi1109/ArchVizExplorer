// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NotificationWidget.h"

void UNotificationWidget::NativeConstruct() {
	Super::NativeConstruct();
}

void UNotificationWidget::SetError(FText ErrorText) {
	ShowTextBox();

	NotificationText->SetText(ErrorText);
	NotificationText->SetColorAndOpacity(FSlateColor{FLinearColor{1.f, 0.f, 0.f, 1.f}});

	FTimerHandle UnusedHandle;

	GetWorld()->GetTimerManager().SetTimer(
		UnusedHandle, this, &UNotificationWidget::HideTextBox, 2.5, false);
}

void UNotificationWidget::SetSuccess(FText SuccessText) {
	ShowTextBox();

	NotificationText->SetText(SuccessText);
	NotificationText->SetColorAndOpacity(FSlateColor{ FLinearColor{0.f, 1.f, 0.f, 1.f} });

	FTimerHandle UnusedHandle;

	GetWorld()->GetTimerManager().SetTimer(
		UnusedHandle, this, &UNotificationWidget::HideTextBox, 2.5, false);
}


void UNotificationWidget::ShowTextBox() {
	NotificationSizeBox->SetVisibility(ESlateVisibility::Visible);
}

void UNotificationWidget::HideTextBox() {
	NotificationSizeBox->SetVisibility(ESlateVisibility::Hidden);
}