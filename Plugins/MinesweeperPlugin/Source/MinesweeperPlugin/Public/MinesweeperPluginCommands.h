// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MinesweeperPluginStyle.h"

class FMinesweeperPluginCommands : public TCommands<FMinesweeperPluginCommands>
{
public:

	FMinesweeperPluginCommands()
		: TCommands<FMinesweeperPluginCommands>(TEXT("MinesweeperPlugin"), NSLOCTEXT("Contexts", "MinesweeperPlugin", "MinesweeperPlugin Plugin"), NAME_None, FMinesweeperPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};