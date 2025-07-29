// Created by Jorge Puerto. All Rights Reserved.


#include "UI/ActionListInfo.h"

UActionListInfo* UActionListInfo::NEW(FFocusInfo NewInfo, int NewIndex)
{
	UActionListInfo* obj = NewObject<UActionListInfo>();
	obj->Info = NewInfo;
	obj->Index = NewIndex;
	return obj;
}
