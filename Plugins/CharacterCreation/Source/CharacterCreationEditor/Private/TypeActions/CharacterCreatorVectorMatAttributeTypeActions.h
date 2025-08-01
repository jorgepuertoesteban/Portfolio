// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"
#include "AssetTypeActions_Base.h"

class FCharacterCreatorVectorMatAttributeTypeActions : public FAssetTypeActions_Base
{
public:
	FCharacterCreatorVectorMatAttributeTypeActions(EAssetTypeCategories::Type InAssetCategory);

	// IAssetTypeActions interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual const TArray<FText>& GetSubMenus() const override;
	// End of IAssetTypeActions interface

private:
	EAssetTypeCategories::Type AssetCategory;
};
