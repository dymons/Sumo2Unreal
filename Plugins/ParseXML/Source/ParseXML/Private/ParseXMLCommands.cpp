// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ParseXMLCommands.h"

#define LOCTEXT_NAMESPACE "FParseXMLModule"

void FParseXMLCommands::RegisterCommands()
{
    UI_COMMAND(PluginAction, "ParseXML", "Execute ParseXML action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE