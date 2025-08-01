// Created by Jorge Puerto. All Rights Reserved.

#pragma once

struct FCreateFactoryOptions;

struct DirectoryCopierAndParser : public IPlatformFile::FDirectoryVisitor
{
	FString SourcePath;
	FString TargetPath;
	bool bReplaceFileIfExists;
	TArray<FString> BlacklistKeywords;
	TMap<FString, FString> PathReplacementsMap;
	TMap<FString, FString> FileReplacementsMap;

	DirectoryCopierAndParser(const FString& InSourcePath, const FString& InTargetPath, const bool bInReplaceFileIfExists,
		const TArray<FString>& InBlacklistKeywords, const TMap<FString, FString>& InPathReplacementsMap, const TMap<FString, FString>& InFileReplacementsMap) :
	SourcePath(InSourcePath), TargetPath(InTargetPath), bReplaceFileIfExists(bInReplaceFileIfExists),
	BlacklistKeywords(InBlacklistKeywords), PathReplacementsMap(InPathReplacementsMap), FileReplacementsMap(InFileReplacementsMap)
	{}
	
	bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override;
	
	static void ParseFile(const FString& FilePath, const TMap<FString,FString>& ReplacementsMap);
};

class CreateFactoryCommand
{
public:

	static bool Run(const FCreateFactoryOptions& Options);
	
};
