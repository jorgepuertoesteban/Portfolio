// Created by Jorge Puerto. All Rights Reserved.

#pragma once

class FJPEAssetContextMenu
{
public:
	static void Register();

protected:
	static void ExecuteExportAssets();
	static bool CanExecuteExportAssets();
	
	static bool ExportJPEAssets(const TArray<FAssetData>& AssetsData);
};