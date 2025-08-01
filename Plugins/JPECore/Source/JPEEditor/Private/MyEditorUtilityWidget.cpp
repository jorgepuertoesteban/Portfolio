// Created by Jorge Puerto. All rights reserved.

#include "MyEditorUtilityWidget.h"
#include "EditorAssetLibrary.h"
#include "Components/Button.h"
#include "Components/DetailsView.h"

#if ENGINE_MAJOR_VERSION == 4
#include "EditorLevelLibrary.h"
#elif ENGINE_MAJOR_VERSION == 5
#include "Subsystems/EditorActorSubsystem.h"
#endif


bool UMyEditorUtilityWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (!ensure(Button_OrganizeWorldOutliner != nullptr)) return false;
	Button_OrganizeWorldOutliner->OnClicked.AddDynamic(this, &UMyEditorUtilityWidget::OrganizeWorldOutliner);

	if (!ensure(Button_DeleteNullSMActors != nullptr)) return false;
	Button_DeleteNullSMActors->OnClicked.AddDynamic(this, &UMyEditorUtilityWidget::DeleteNullSMActors);

	if (!ensure(Button_SetTextureParameter != nullptr)) return false;
	Button_SetTextureParameter->OnClicked.AddDynamic(this, &UMyEditorUtilityWidget::SetTextureParameter);

	if (!ensure(Button_SearchAndReplace != nullptr)) return false;
	Button_SearchAndReplace->OnClicked.AddDynamic(this, &UMyEditorUtilityWidget::SearchAndReplace);

	if (!ensure(Button_RemoveEmptyFolders != nullptr)) return false;
	Button_RemoveEmptyFolders->OnClicked.AddDynamic(this, &UMyEditorUtilityWidget::RemoveEmptyFolders);

	DetailsView_General->SetObject(this);
	DetailsView_OrganizeWorldOutliner->SetObject(this);
	DetailsView_SetTextureParams->SetObject(this);
	DetailsView_SearchAndReplace->SetObject(this);

	return true;
}


void UMyEditorUtilityWidget::OrganizeWorldOutliner()
{
	for (AActor* Actor : GetAllLevelActors())
	{
		if (ensure(Actor))
		{
			const FName* FolderName = FolderMap.Find(Actor->GetClass());
			if (ensure(FolderName))
			{
				Actor->SetFolderPath(*FolderName);
			}
		}
	}
}

void UMyEditorUtilityWidget::DeleteNullSMActors()
{
	for (AActor* Actor : GetAllLevelActors())
	{
		if (ensure(Actor))
		{
			AStaticMeshActor* StaticMeshActor = dynamic_cast<AStaticMeshActor*>(Actor);
			if (ensure(StaticMeshActor))
			{
				UStaticMeshComponent* StaticMeshComponent = StaticMeshActor->GetStaticMeshComponent();
				if (ensure(StaticMeshComponent) && StaticMeshComponent->GetStaticMesh() == nullptr)
				{
					Actor->Destroy();
				}
			}
		}
	}
}

void UMyEditorUtilityWidget::SetTextureParameter()
{
	TArray<FString> AssetsInPath = UEditorAssetLibrary::ListAssets(FolderPath, bRecursive, false);
	for (FString AssetPath : AssetsInPath)
	{
		for (FString Pattern : LinearColorPattern)
		{
			if (AssetPath.Contains(Pattern))
			{
				UObject* Object = UEditorAssetLibrary::LoadAsset(AssetPath);
				if (!ensure(Object))
				{
					return;
				}
				UTexture2D* Texture = dynamic_cast<UTexture2D*>(Object);
				if (ensure(Texture))
				{
					Texture->SRGB = bSetRGB;
					Texture->CompressionSettings = Compression;
				}
			}
		}
	}
}

#pragma endregion

#pragma region SearchAndReplace

void UMyEditorUtilityWidget::SearchAndReplace()
{
	TArray<FString> AssetsInPath = UEditorAssetLibrary::ListAssets(FolderPath, bRecursive, false);
	for (FString AssetPath : AssetsInPath)
	{
		FString FileName = FPaths::GetCleanFilename(AssetPath);
		if (FileName.Contains(SearchPattern, SearchCase))
		{
			FString NewAssetName = FileName.Replace(*SearchPattern, *ReplacePattern, SearchCase);
			FString NewAssetPath = FPaths::Combine(FPaths::GetPath(AssetPath), NewAssetName);
			UEditorAssetLibrary::RenameAsset(AssetPath, NewAssetPath);
		}
	}
}

void UMyEditorUtilityWidget::RemoveEmptyFolders()
{
	TArray<FString> AssetsInPath = UEditorAssetLibrary::ListAssets(FolderPath, bRecursive, true);
	for (FString Asset : AssetsInPath)
	{
		if (!UEditorAssetLibrary::DoesDirectoryHaveAssets(Asset, bRecursive))
		{
			UEditorAssetLibrary::DeleteDirectory(Asset);
		}
	}
}

TArray<AActor*> UMyEditorUtilityWidget::GetAllLevelActors() const
{
#if ENGINE_MAJOR_VERSION == 4
	return UEditorLevelLibrary::GetAllLevelActors();
#elif ENGINE_MAJOR_VERSION == 5
	if (UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>())
	{
		return EditorActorSubsystem->GetAllLevelActors();
	}
	return TArray<AActor*>();
#endif
}
