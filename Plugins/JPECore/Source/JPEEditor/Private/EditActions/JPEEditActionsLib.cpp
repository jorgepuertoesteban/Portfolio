// Created by Jorge Puerto. All Rights Reserved.


#include "EditActions/JPEEditActionsLib.h"


#include "Components/PointLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/RectLightComponent.h"

#include "Engine/DirectionalLight.h"
#include "Engine/Light.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/PointLight.h"
#include "Engine/RectLight.h"
#include "Engine/ReflectionCapture.h"
#include "Engine/SpotLight.h"
#include "Engine/StaticMeshActor.h"


#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/UObjectGlobals.h"

#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"

FName FJPEEditActionsLib::ExtractedTag = "JPEEditActions_Extracted";
FString FJPEEditActionsLib::LevelFolderPrefix = "EX_";


void FJPEEditActionsLib::CreateFolderPath(AActor* Actor, UClass* ChosenClass, bool const bUseLevelNameAsRoot /*= true*/, AActor* OriginalActor/* = nullptr*/)
{
	FName const NewFolderName = bUseLevelNameAsRoot ? *FString::Printf(TEXT("%s%s/%s"), *LevelFolderPrefix, *OriginalActor->GetLevel()->GetOuter()->GetName(), *ChosenClass->GetName()) : *ChosenClass->GetName();
	Actor->SetFolderPath(NewFolderName);
}

bool FJPEEditActionsLib::HasConstructorComponents(const AActor* Actor)
{
	for (const UActorComponent* Comp : Actor->GetComponents())
	{
		if (Comp->CreationMethod == EComponentCreationMethod::UserConstructionScript)
		{
			return true;
		}
	}
	return false;
}

bool FJPEEditActionsLib::IsAnExtractedActor(const AActor* Actor)
{
	return Actor && Actor->Tags.Contains(FJPEEditActionsLib::ExtractedTag);
}

bool FJPEEditActionsLib::SpawnActorsFromComponents(AActor* Actor, UWorld* World, UClass* ChosenClass, const FExtractOptions& Options)
{
	if (!Actor || !World || !ChosenClass)
	{
		UE_LOG(LogTemp, Error, TEXT("invalid parameters: !Actor || !World || !ChosenClass"));
		return false;
	}
	UE_LOG(LogTemp, Log, TEXT("Replacing Actor %s"), *Actor->GetName());
	if (!Actor->GetRootComponent())
	{
		UE_LOG(LogTemp, Error, TEXT("No Root Component for Actor: %s"), *Actor->GetName());
		return false;
	}
	if (!Options.bReplaceAlreadyExtracted && FJPEEditActionsLib::IsAnExtractedActor(Actor))
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is Extracted and Options bReplaceAlreadyExtracted is false, we skip this Actor"), *Actor->GetName());
		return false;
	}

	AActor* SpawnedActor = nullptr;

	if (!Options.bSpawnOneActorPerComponent)
	{
		//Spawn only one Actor
		SpawnedActor = World->SpawnActor<AActor>(AActor::StaticClass(), Actor->GetTransform());
		FJPEEditActionsLib::CreateFolderPath(SpawnedActor, ChosenClass, Options.bUseLevelNameAsRootFolder, Actor);
		FActorLabelUtilities::SetActorLabelUnique(SpawnedActor, ChosenClass->GetName());
		SpawnedActor->Tags.Add(FJPEEditActionsLib::ExtractedTag);

		USceneComponent* RootComponent = SpawnedActor->GetRootComponent();
		if (!RootComponent)
		{
			RootComponent = NewObject<USceneComponent>(SpawnedActor);			
			SpawnedActor->AddInstanceComponent(RootComponent);
			RootComponent->SetWorldTransform(Actor->GetTransform());
			RootComponent->OnComponentCreated();
			RootComponent->RegisterComponent();
			SpawnedActor->SetRootComponent(RootComponent);
		}
		RootComponent->SetMobility(Options.bForceStatic ? EComponentMobility::Static : Actor->GetRootComponent()->Mobility.GetValue());		
	}

	bool bHasExtracted = false;

	for (UActorComponent* Comp : Actor->GetComponents())
	{
		if (UStaticMeshComponent* OriginalStaticMeshComp = Cast<UStaticMeshComponent>(Comp))
		{
			if (UStaticMesh* StaticMesh = OriginalStaticMeshComp->GetStaticMesh())
			{
				bHasExtracted = true;

				UStaticMeshComponent* CreatedComp = nullptr;

				if (Options.bSpawnOneActorPerComponent)
				{

					AStaticMeshActor* MeshActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), OriginalStaticMeshComp->GetComponentTransform());
					FJPEEditActionsLib::CreateFolderPath(MeshActor, ChosenClass, Options.bUseLevelNameAsRootFolder, Actor);
					FActorLabelUtilities::SetActorLabelUnique(MeshActor, StaticMesh->GetName());
					MeshActor->Tags.Add("JPEEditActions_Extracted");
					CreatedComp = MeshActor->GetStaticMeshComponent();
					CreatedComp->SetMobility(Options.bForceStatic ? EComponentMobility::Static : OriginalStaticMeshComp->Mobility.GetValue());
				}
				else
				{
					CreatedComp = NewObject<UStaticMeshComponent>(SpawnedActor);
					//CreatedComp->CreationMethod = EComponentCreationMethod::Instance;
					SpawnedActor->AddInstanceComponent(CreatedComp);
					CreatedComp->SetWorldTransform(OriginalStaticMeshComp->GetComponentTransform());
					CreatedComp->SetMobility(Options.bForceStatic ? EComponentMobility::Static : OriginalStaticMeshComp->Mobility.GetValue());
					CreatedComp->AttachToComponent(SpawnedActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
					CreatedComp->OnComponentCreated();
					CreatedComp->RegisterComponent();
				}

				CreatedComp->SetStaticMesh(StaticMesh);

				for (int32 i = 0; i < OriginalStaticMeshComp->GetNumMaterials(); i++)
				{
					if (UMaterialInterface* Material = OriginalStaticMeshComp->GetMaterial(i))
					{
						if (!Material->IsA<UMaterialInstanceDynamic>())//We don't set the MID since we don't know their origin
						{
							CreatedComp->SetMaterial(i, Material);
						}
					}
				}
			}
		}
		else if (ULightComponent* OriginalLightComp = Cast<ULightComponent>(Comp))
		{
			bHasExtracted = true;

			ULightComponent* NewLightComponent = nullptr;

			if (Options.bSpawnOneActorPerComponent)
			{
				ALight* LightActor = nullptr;

				if (OriginalLightComp->IsA<UDirectionalLightComponent>())
				{
					LightActor = World->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), OriginalLightComp->GetComponentTransform());
				}
				else if (OriginalLightComp->IsA<USpotLightComponent>())//WARNING: USpotLightComponent is a subclass of UPointLightComponent, so it must be tested first
				{
					LightActor = World->SpawnActor<ASpotLight>(ASpotLight::StaticClass(), OriginalLightComp->GetComponentTransform());
				}
				else if (OriginalLightComp->IsA<UPointLightComponent>())
				{
					LightActor = World->SpawnActor<APointLight>(APointLight::StaticClass(), OriginalLightComp->GetComponentTransform());
				}
				else if (OriginalLightComp->IsA<URectLightComponent>())
				{
					LightActor = World->SpawnActor<ARectLight>(ARectLight::StaticClass(), OriginalLightComp->GetComponentTransform());
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("Class not found for light component"));
					continue;
				}

				FJPEEditActionsLib::CreateFolderPath(LightActor, ChosenClass, Options.bUseLevelNameAsRootFolder, Actor);
				FActorLabelUtilities::SetActorLabelUnique(LightActor, ChosenClass->GetName());
				LightActor->Tags.Add("JPEEditActions_Extracted");

				NewLightComponent = LightActor->GetLightComponent();
				NewLightComponent->SetMobility(Options.bForceStatic ? EComponentMobility::Static : OriginalLightComp->Mobility.GetValue());
			}
			else
			{
				NewLightComponent = NewObject<ULightComponent>(SpawnedActor, OriginalLightComp->GetClass());
				//NewLightComponent->CreationMethod = EComponentCreationMethod::Instance;
				SpawnedActor->AddInstanceComponent(NewLightComponent);
				NewLightComponent->SetWorldTransform(OriginalLightComp->GetComponentTransform());
				NewLightComponent->SetMobility(Options.bForceStatic ? EComponentMobility::Static : OriginalLightComp->Mobility.GetValue());
				NewLightComponent->AttachToComponent(SpawnedActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
				NewLightComponent->OnComponentCreated();
				NewLightComponent->RegisterComponent();
			}

			{//Set light values
				NewLightComponent->SetIntensity(OriginalLightComp->Intensity);
				NewLightComponent->SetLightColor(OriginalLightComp->GetLightColor());

				if (OriginalLightComp->IsA<USpotLightComponent>())//WARNING: USpotLightComponent is a subclass of UPointLightComponent, so it must be tested first
				{
					USpotLightComponent* OriginSpotLightComp = Cast<USpotLightComponent>(OriginalLightComp);
					USpotLightComponent* SpotLightComponent = Cast<USpotLightComponent>(NewLightComponent);

					SpotLightComponent->SetAttenuationRadius(OriginSpotLightComp->AttenuationRadius);
					SpotLightComponent->SetInnerConeAngle(OriginSpotLightComp->InnerConeAngle);
					SpotLightComponent->SetOuterConeAngle(OriginSpotLightComp->OuterConeAngle);
					SpotLightComponent->SetIntensityUnits(OriginSpotLightComp->IntensityUnits);
				}
				else if (OriginalLightComp->IsA<UPointLightComponent>())
				{
					UPointLightComponent* PointLightComponent = Cast<UPointLightComponent>(NewLightComponent);
					UPointLightComponent* OriginPointLightComp = Cast<UPointLightComponent>(OriginalLightComp);
					PointLightComponent->SetAttenuationRadius(OriginPointLightComp->AttenuationRadius);
					PointLightComponent->SetIntensityUnits(OriginPointLightComp->IntensityUnits);
				}
			}

			//TODO: IMPORTANT, if we get this working we don't need the above
			//StaticDuplicateObject() is for duplicating the objects.
			//UEngine::FCopyPropertiesForUnrelatedObjectsParams CopyParams;
			//CopyParams.bNotifyObjectReplacement = false;
			//CopyParams.bPreserveRootComponent = false;
			//UEngine::CopyPropertiesForUnrelatedObjects(LightComp, LightActor->GetLightComponent(), CopyParams);

			//Todo: this doesn't work, it should work, but it doesn't. So That's why we copy the most representative values.
			//for (TFieldIterator<FProperty> PropIt(LightComp->StaticClass()); PropIt; ++PropIt)
			//{
			//	FProperty* Property = *PropIt;

			//	if (!Property->IsA<FObjectProperty>())
			//	{
			//		EditorUtilities::CopySingleProperty(LightComp, LightActor->GetLightComponent(), Property);
			//	}
			//}
			//End Important TODO
		}
	}

	if (bHasExtracted)
	{
		UE_LOG(LogTemp, Log, TEXT("Actor %s Succesfully replaced"), *Actor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Actor %s didn't have any Components to extract"), *Actor->GetName());
		if (SpawnedActor)
		{
			SpawnedActor->Destroy();
		}
	}

	return bHasExtracted;
}
