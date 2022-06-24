// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

struct CellData
{
	TSharedPtr<SButton> m_Button = nullptr;
	TSharedPtr<STextBlock> m_Text = nullptr;
	int m_NumberOfSurroundingBombs = 0;
	bool m_bIsBomb = false;
};

class FMinesweeperPluginModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

private:
	int m_GridWidth;
	int m_GridHeight;
	int m_MineCount;

	SVerticalBox::FSlot* m_GridSlot;					// Reference to the slot of the whole button grid
	TArray<TArray<SHorizontalBox::FSlot*>> m_GridCells; // Reference to the slot of each cell
	TArray<TArray<CellData>> m_GridCellData;			// Data of all grid cells (see CellData struct)

	TSharedPtr<class FUICommandList> PluginCommands;

	void OnWidthInput(const FText& Value);
	void OnHeightInput(const FText& Value);
	void OnNumberOfMinesInput(const FText& Value);

	FReply OnGenerateNewGridClicked();
	FReply OnGridCellClicked(int a_PosX, int a_PosY);

	void InitGame(int a_Width, int a_Height, int a_NumberOfMines);		// Initializes m_GridCellData array
	bool GameIsWon();													// Checks if the game is won
	void GameLost();													// Reveals all bombs and prints GAME LOST
	void RevealCell(int a_PosX, int a_PosY);							// Reveals the cell and calls itself recursively if it has no surrounding bombs
	void IncrementBombCountOfSurroundingCells(int a_PosX, int a_PosY);  // Called when a mine/bomb is placed in the grid on construction

	void RegisterMenus();
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
};