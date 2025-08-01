#pragma once
#include "SCommonEditorViewportToolbarBase.h"
#include "SEditorViewport.h"

class ABuildingBase;
class UCharacterCreator;
class FAdvancedPreviewScene;

class SCharacterCreatorViewport: public SEditorViewport, public ICommonEditorViewportToolbarInfoProvider
{
public:
	SLATE_BEGIN_ARGS(SCharacterCreatorViewport) : _EditingObject(nullptr) {}
		SLATE_ARGUMENT(UCharacterCreator*, EditingObject )
	SLATE_END_ARGS()

	~SCharacterCreatorViewport();

	
	void Construct(const FArguments& InArgs);

	//Toolbar interface
	virtual TSharedRef<SEditorViewport> GetViewportWidget() override {return SharedThis(this);}
	virtual TSharedPtr<FExtender> GetExtenders() const override {return MakeShareable(new FExtender);}
	virtual void OnFloatingButtonClicked() override {}

	TSharedPtr<FAdvancedPreviewScene> AdvancedPreviewScene = nullptr;
	TSoftObjectPtr<UCharacterCreator> CharacterCreator = nullptr;

protected:
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	TSharedPtr<FEditorViewportClient> LevelViewportClient;

	/** The delegate that handles when an object property got changed (any object). */
	FDelegateHandle InputObjectPropertyChangedHandle;
};
