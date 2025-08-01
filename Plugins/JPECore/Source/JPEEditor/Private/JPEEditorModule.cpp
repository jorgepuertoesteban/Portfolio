// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "JPEEditorModule.h"
#include "JPEEditorCommands.h"
#include "JPEEditorEdMode.h"

#include "JPEMainMenuSystem/JPEMainMenuSystem.h"

#include "LevelEditor.h"

#include "CoreMinimal.h"

#include "ToolMenu.h"
#include "ToolMenus.h"

#include "Widgets/Docking/SDockTab.h"

#include "BaseEditorTool.h"
#include "BaseEditorToolCustomization.h"
#include "TemplateSettings.h"
#include "ContentBrowser/JPEAssetContextMenu.h"
#include "EditActions/JPEEditActions.h"
#include "Features/EditorFeatures.h"
#include "Features/IPluginsEditorFeature.h"
#include "Interfaces/IPluginManager.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Widgets/SWindow.h"
#if ENGINE_MAJOR_VERSION == 4
#include "Styling/SlateStyle.h"
#include "EditorStyleSet.h"
#elif ENGINE_MAJOR_VERSION == 5
#include "Styling/AppStyle.h"
#endif


static const FName JPEEditorTabName("JPEEditorTab");

#define LOCTEXT_NAMESPACE "JPEEditorModule"

class FJPEEditorModule : public IJPEEditorModule
{
private:
	TSharedPtr<class FUICommandList> PluginCommands;

	FJPEEditActions JPEEditActions;
	FJPEAssetContextMenu JPEAssetContextMenu;

	TArray<TSharedPtr<FPluginTemplateDescription>> PluginTemplateDescriptions;

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:
	//From Unreal Video
	static void AddMenuCommands(FMenuBuilder& MenuBuilder);
	static void AddToolbarCommands(FToolBarBuilder& ToolbarBuilder);
	static void CreateToolListMenu(class FMenuBuilder& MenuBuilder);
	static void TriggerTool(UClass* ToolClass);
	static void OnToolWindowClosed(const TSharedRef<SWindow>& WindowBeingClosed, TWeakObjectPtr<UBaseEditorTool> ToolInstance);
	//End From Unreal Video
};

void FJPEEditorModule::StartupModule()
{
	//Custom Class Layout
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomClassLayout("BaseEditorTool", FOnGetDetailCustomizationInstance::CreateStatic(&FBaseEditorToolCustomization::MakeInstance));
		PropertyModule.NotifyCustomizationModuleChanged();
	}

	FJPEEditorStyle::Initialize();
	FJPEEditorStyle::ReloadTextures();

	//EditorMode
	FEditorModeRegistry::Get().RegisterMode<FJPEEditorEdMode>(
		FJPEEditorEdMode::EM_JPEEditorEdModeId,
		NSLOCTEXT("EditorModes", "JPEEditorEdMode", "JPE"),
		FSlateIcon(FJPEEditorStyle::GetAppStyleSetName(), "JPEEditor.TabIcon", "JPEEditor.TabIcon.Small"),
		true /*,
		300*/
	);
	//End EditorMode
	FJPEEditorCommands::Register();

	FJPEMainMenuSystem::Register();
	JPEEditActions.Register();
	JPEAssetContextMenu.Register();


	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction
	(
		FJPEEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateStatic(&FJPEMainMenuSystem::Launch)
	);

	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));

	//From Unreal Video
	TSharedRef<FExtender> MenuExtender(new FExtender());
	MenuExtender->AddMenuExtension(
		"EditMain",
		EExtensionHook::After,
		PluginCommands.ToSharedRef(),
		FMenuExtensionDelegate::CreateStatic(&FJPEEditorModule::AddMenuCommands));

	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	//End From Unreal Video

	if (!IModularFeatures::Get().IsModularFeatureAvailable(EditorFeatures::PluginsEditor))
	{
		return;
	}

	IPluginsEditorFeature& PluginEditor = IModularFeatures::Get().GetModularFeature<IPluginsEditorFeature>(EditorFeatures::PluginsEditor);
	for (const auto& TemplateDescription : PluginTemplateDescriptions)
	{
		PluginEditor.RegisterPluginTemplate(TemplateDescription.ToSharedRef());
	}

	IPluginManager& PluginManager = IPluginManager::Get();
	if (const TSharedPtr<IPlugin> Plugin = PluginManager.GetModuleOwnerPlugin(TEXT("JPECore")); Plugin.IsValid())
	{
		// ExtenstionPluginsTemplates plugin base dir
		const FString BaseDir = Plugin->GetBaseDir();

		for (const FExtenstionPluginsTemplateData& Template : GetDefault<UTemplateSettings>()->PluginTemplates)
		{
			TSharedPtr<FPluginTemplateDescription> Description =
				MakeShareable(new FPluginTemplateDescription(Template.Label, Template.Description, BaseDir + "/" + Template.Path, false, EHostType::Runtime, ELoadingPhase::Default));

			PluginTemplateDescriptions.Add(Description);
			PluginEditor.RegisterPluginTemplate(Description.ToSharedRef());
		}
	}
}


void FJPEEditorModule::AddToolbarCommands(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.AddToolBarButton(FJPEEditorCommands::Get().OpenPluginWindow);
}

void FJPEEditorModule::AddMenuCommands(FMenuBuilder& MenuBuilder)
{
#if ENGINE_MAJOR_VERSION == 4
	FSlateIcon DocMenuIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.BrowseDocumentation");
#elif ENGINE_MAJOR_VERSION == 5
	FSlateIcon DocMenuIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.BrowseDocumentation");
#endif

	MenuBuilder.AddSubMenu(
		LOCTEXT("JPETools", "JPE Tools"),
		LOCTEXT("JPETools", "JPE Tools"),
		FNewMenuDelegate::CreateStatic(&FJPEEditorModule::CreateToolListMenu),
		false,
		DocMenuIcon
	);
}


void FJPEEditorModule::ShutdownModule()
{
	FJPEMainMenuSystem::Unregister();
	FEditorModeRegistry::Get().UnregisterMode(FJPEEditorEdMode::EM_JPEEditorEdModeId);
	FJPEEditorStyle::Shutdown();
}

void FJPEEditorModule::CreateToolListMenu(class FMenuBuilder& MenuBuilder)
{
#if ENGINE_MAJOR_VERSION == 4
	FSlateIcon DocItemIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.BrowseDocumentation");
#elif ENGINE_MAJOR_VERSION == 5
	FSlateIcon DocItemIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.BrowseDocumentation");
#endif

	//Add to ToolListMenu
	for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
	{
		UClass* Class = *ClassIt;

		if (!Class->HasAnyClassFlags(CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_Abstract))
		{
			if (Class->IsChildOf(UBaseEditorTool::StaticClass()))
			{
				FString FriendlyName = Class->GetName();
				FText MenuDescription = FText::Format(LOCTEXT("ToolMenuDescription", "{0}"), FText::FromString(FriendlyName));
				FText MenuTooltip = FText::Format(LOCTEXT("ToolMenuTooltip", "Execute the {0} Tool"), FText::FromString(FriendlyName));

				FUIAction Action(FExecuteAction::CreateStatic(&FJPEEditorModule::TriggerTool, Class));

				MenuBuilder.AddMenuEntry(
					MenuDescription,
					MenuTooltip,
					FSlateIcon(),
					Action
				);
			}
		}
	}
}

void FJPEEditorModule::TriggerTool(UClass* ToolClass)
{
	UBaseEditorTool* ToolInstance = NewObject<UBaseEditorTool>(GetTransientPackage(), ToolClass);
	ToolInstance->AddToRoot();

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	TArray<UObject*> ObjectsToView;
	ObjectsToView.Add(ToolInstance);
	TSharedRef<SWindow> Window = PropertyModule.CreateFloatingDetailsView(ObjectsToView, /*bIsLocakble =*/false);

	Window->SetOnWindowClosed(FOnWindowClosed::CreateStatic(&FJPEEditorModule::OnToolWindowClosed, TWeakObjectPtr<UBaseEditorTool>(ToolInstance)));
}


void FJPEEditorModule::OnToolWindowClosed(const TSharedRef<SWindow>& WindowBeingClosed, TWeakObjectPtr<UBaseEditorTool> ToolInstance)
{
	ToolInstance->RemoveFromRoot();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FJPEEditorModule, JPEEditor)
