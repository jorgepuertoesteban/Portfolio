#pragma once

class UCharacterCreator;
class UCustomObject;
class FTabManager;

class CHARACTERCREATIONEDITOR_API FCharacterCreatorToolkit : public FAssetEditorToolkit
{
public:
	void Initialize(const TSharedPtr<IToolkitHost>& InitToolkitHost, UCharacterCreator* Style);
	
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;

	virtual FName GetToolkitFName() const override { return FName("CustomObjectEditor"); }
	virtual FText GetBaseToolkitName() const override { return INVTEXT("Custom Object Editor"); }
	virtual FString GetWorldCentricTabPrefix() const override { return "CustomObjectEditor"; }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::White; }
	
	static TSharedRef<FCharacterCreatorToolkit> CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UObject*>& InObjects);


private:
	TObjectPtr<UCharacterCreator> CharacterCreator = nullptr;
};
