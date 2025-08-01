#pragma once

class UCharacterCreatorGroom;
class USkeletalMesh;

struct FAddSkeletalsToCCGroomsOptions
{
	TArray<UCharacterCreatorGroom*> Grooms;
	TArray<USkeletalMesh*> Meshes;
	USkeletalMesh* SourceMesh;
};

class AddSkeletalsToCCGroomsCommand
{
public:

	static bool Run(const FAddSkeletalsToCCGroomsOptions& Options);
	
};
