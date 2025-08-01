#pragma once

class UGroomAsset;
class USkeletalMesh;

struct FGenerateCharacterCreatorGroomOptions
{
	UGroomAsset* GroomAsset;
	USkeletalMesh* SourceSkeletalMesh;
	TArray<USkeletalMesh*> TargetSkeletalMeshes;
};

class GenerateCharacterCreatorGroomCommand
{
public:

	static bool Run(const FGenerateCharacterCreatorGroomOptions& Options);
	
};
