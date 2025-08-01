// Created by Jorge Puerto. All Rights Reserved.


#include "DetailCustomization/BASkeletalMeshDetailCustomization.h"
#include "UObject/Class.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"

#define LOCTEXT_NAMESPACE "FBASkeletalMeshDetailCustomization"

TSharedRef<IDetailCustomization> FBASkeletalMeshDetailCustomization::MakeInstance()
{
	return MakeShareable(new FBASkeletalMeshDetailCustomization);
}

void FBASkeletalMeshDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);

	//Create a category
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("JPE Character Creation Tools");
	Category.AddCustomRow(LOCTEXT("CharacterCreation", "Character Creation"))
		.ValueContent()
		[
			SNew(SButton)
			.Text(LOCTEXT("FBASkeletalMeshDetailCustomization_Button", "This is a button"))
			.OnClicked(FOnClicked::CreateStatic(&FBASkeletalMeshDetailCustomization::ExecuteOnClick, &DetailBuilder))
		];
}

FReply FBASkeletalMeshDetailCustomization::ExecuteOnClick(IDetailLayoutBuilder* DetailBuilder)
{
	UE_LOG(LogTemp, Log, TEXT("FBASkeletalMeshDetailCustomization::ExecuteToolCommand"));
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE