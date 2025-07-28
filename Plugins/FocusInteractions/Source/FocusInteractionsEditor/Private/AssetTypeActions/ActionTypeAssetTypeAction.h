// Created by Bionic Ape. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"
#include "AssetTypeActions_Base.h"

class FActionTypeAssetTypeAction : public FAssetTypeActions_Base
{
public:
	FActionTypeAssetTypeAction(EAssetTypeCategories::Type InAssetCategory);

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
