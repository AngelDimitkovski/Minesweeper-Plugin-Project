// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperPlugin.h"
#include "MinesweeperPluginStyle.h"
#include "MinesweeperPluginCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName MinesweeperPluginTabName("MinesweeperPlugin");

#define LOCTEXT_NAMESPACE "FMinesweeperPluginModule"

void FMinesweeperPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMinesweeperPluginStyle::Initialize();
	FMinesweeperPluginStyle::ReloadTextures();

	FMinesweeperPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMinesweeperPluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMinesweeperPluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperPluginModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperPluginTabName, FOnSpawnTab::CreateRaw(this, &FMinesweeperPluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMinesweeperPluginTabTitle", "MinesweeperPlugin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMinesweeperPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMinesweeperPluginStyle::Shutdown();

	FMinesweeperPluginCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperPluginTabName);
}

TSharedRef<SDockTab> FMinesweeperPluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	const FMargin ContentPadding = FMargin(30.f, 30.f);
	const FMargin TextPadding = FMargin(10.f, 10.f);

	FSlateFontInfo TextFont = FCoreStyle::Get().GetFontStyle("EmbossedText");
	TextFont.Size = 10.f;

	const FText GridWidth = LOCTEXT("GridWidth", "Width:");
	const FText GridHeight = LOCTEXT("GridHeight", "Height:");
	const FText NumberOfMines = LOCTEXT("NumberOfMines", "Number Of Mines:");
	const FText GenerateNewGrid = LOCTEXT("GenerateNewGrid", "Generate New Grid");

	const FVector2D HorizontalSpacerSize = { 30.f, 1.f };
	const FVector2D VerticalSpacerSize = { 1.f, 20.f };
	
	const float TextBoxSize = 25.f;

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		.ContentPadding(ContentPadding)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			[
				/* Main Vertical Box - Contains all sections of the UI */
				SNew(SVerticalBox)

				/* 1st Row */
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					/* Width and Height Input Sections */
					SNew(SHorizontalBox)
	
					/* Width Input Section */
					+ SHorizontalBox::Slot()
					[
						SNew(SHorizontalBox)

						// Width Text
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(TextPadding)
						[
							SNew(STextBlock)
							.Text(GridWidth)
							.Font(TextFont)
							.Justification(ETextJustify::Center)
						]

						// Width Input Text Box
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SEditableTextBox)
							.MinDesiredWidth(TextBoxSize)
							.OnTextChanged_Raw(this, &FMinesweeperPluginModule::OnWidthInput)
							
						]
					]
					
					/* Separate Height and Width Input Sections */
					+ SHorizontalBox::Slot()
					[
						SNew(SSpacer)
						.Size(HorizontalSpacerSize)
					]

					/* Height Input Section */
					+ SHorizontalBox::Slot()
					[
						SNew(SHorizontalBox)

						// Height Text
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(TextPadding)
						[
							SNew(STextBlock)
							.Text(GridHeight)
							.Font(TextFont)
							.Justification(ETextJustify::Center)
						]

						// Height Input Text Box
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SEditableTextBox)
							.MinDesiredWidth(TextBoxSize)
							.OnTextChanged_Raw(this, &FMinesweeperPluginModule::OnHeightInput)
						]
					]
				]
				
				/* Spacer - Separate 1st and 2nd Row */
				+ SVerticalBox::Slot()
				[
					SNew(SSpacer)
					.Size(VerticalSpacerSize)
				]


				/* 2nd Row */
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)

					/* Number of Mines Input Section */
					+ SHorizontalBox::Slot()
					[
						SNew(SHorizontalBox)

						// Number of Mines Text
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(TextPadding)
						[
							SNew(STextBlock)
							.Text(NumberOfMines)
							.Font(TextFont)
							.Justification(ETextJustify::Center)
						]

						// Number Of Mines Input Text Box
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SEditableTextBox)
							.MinDesiredWidth(TextBoxSize)
							.OnTextChanged_Raw(this, &FMinesweeperPluginModule::OnNumberOfMinesInput)
						]
					]
				]

				/* Spacer - Separate 2nd and 3rd Row */
				+ SVerticalBox::Slot()
				[
					SNew(SSpacer)
					.Size(VerticalSpacerSize)
				]

				/* 3rd Row - Generate New Grid Button */
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SButton)
					.OnClicked_Raw(this, &FMinesweeperPluginModule::OnGenerateNewGridClicked)
					[
						SNew(STextBlock)
						.Text(GenerateNewGrid)
						.Font(TextFont)
						.Justification(ETextJustify::Center)
					]
				]

				/* Separate 3rd Row and Grid */
				+ SVerticalBox::Slot()
				[
					SNew(SSpacer)
					.Size(VerticalSpacerSize)
				]

				/* 4th Row - Button Grid */
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Expose(m_GridSlot)
			]
		];
}

void FMinesweeperPluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperPluginTabName);
}

FReply FMinesweeperPluginModule::OnGenerateNewGridClicked()
{
	// Handle invalid input
	if (m_GridWidth < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Please enter a valid width!"));

		return FReply::Unhandled();
	}
	if (m_GridHeight < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Please enter a valid height!"));

		return FReply::Unhandled();
	}
	if (m_MineCount < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Please enter a valid mine number!"));
		
		return FReply::Unhandled();
	}
	else if (m_MineCount > (m_GridWidth * m_GridHeight) )
	{
		UE_LOG(LogTemp, Warning, TEXT("Number of mines can't be bigger than the number of cells!"));

		return FReply::Unhandled();
	}

	// Create the main vertical box and attach it to the saved grid slot
	TSharedRef<SVerticalBox> MainVerticalBox = SNew(SVerticalBox);
	m_GridSlot->AttachWidget(MainVerticalBox);

	// Create needed arrays for grid construction
	TArray<SVerticalBox::FSlot*> RowSlots;
	RowSlots.AddUninitialized(m_GridHeight);

	TArray<SHorizontalBox::FSlot*> ColumnSlots;
	ColumnSlots.AddUninitialized(m_GridWidth);

	TArray<TSharedRef<SHorizontalBox>> HorizontalBoxes;
	TArray<TArray<SHorizontalBox::FSlot*>> GridCells;

	m_GridCells.Empty(); // Clear grid cells array before each game

	for (auto i = 0; i < m_GridHeight; ++i)
	{		
		MainVerticalBox->AddSlot().Expose(RowSlots[i]); // Add a vertical slot and save it in the array

		HorizontalBoxes.Add(SNew(SHorizontalBox)); // Add a horizontal box to each vertical slot

		RowSlots[i]->AttachWidget(HorizontalBoxes[i]); // Save horizontal boxes

		GridCells.Add(ColumnSlots); // Add array of horizontal slots to each vertical slot
	}

	// Initialize grid cell data needed for the game
	InitGame(m_GridWidth, m_GridHeight, m_MineCount);

	// Default button text
	const FText EmptyButton = LOCTEXT("EmptyButton", " ");

	//  Store all grid cells in the GridCells array of arrays
	for (auto i = 0; i < m_GridHeight; ++i)
	{
		for (auto j = 0; j < m_GridWidth; ++j)
		{
			HorizontalBoxes[i]->AddSlot().Expose(GridCells[i][j]); // Add a horizontal slot and save it in the array

			// Add a button to each grid cell / slot
			GridCells[i][j]->AttachWidget
			(
				SAssignNew(m_GridCellData[i][j].m_Button, SButton)
				.OnClicked_Raw(this, &FMinesweeperPluginModule::OnGridCellClicked, j, i)
				[
					SAssignNew(m_GridCellData[i][j].m_Text, STextBlock)
					.Text(EmptyButton)
					.MinDesiredWidth(23.5f)
				]
			);

			GridCells[i][j]->MaxWidth(23.5f);
		}
	}

	return FReply::Handled();
}

FReply FMinesweeperPluginModule::OnGridCellClicked(int a_PosX, int a_PosY)
{
	RevealCell(a_PosX, a_PosY);

	return FReply::Handled();
}

void FMinesweeperPluginModule::RevealCell(int a_PosX, int a_PosY)
{
	// Checks if the button isn't already disabled (Needed to prevent infinite recursion)
	if (m_GridCellData[a_PosY][a_PosX].m_Button->IsEnabled())
	{
		m_GridCellData[a_PosY][a_PosX].m_Button->SetEnabled(false);

		if (GameIsWon())
		{
			UE_LOG(LogTemp, Warning, TEXT("YOU WON!"));
		}

		if (m_GridCellData[a_PosY][a_PosX].m_bIsBomb)
		{
			m_GridCellData[a_PosY][a_PosX].m_Text->SetText(FString("X"));

			GameLost();
		}
		else if (m_GridCellData[a_PosY][a_PosX].m_NumberOfSurroundingBombs != 0)
		{
			FString NumSurrBombs = FString::FromInt(m_GridCellData[a_PosY][a_PosX].m_NumberOfSurroundingBombs);

			m_GridCellData[a_PosY][a_PosX].m_Text->SetText(NumSurrBombs);
		}
		else // if the cell has no surrounding bombs/mines
		{
			// Recursively call this function for all surrounding cells
			for (int i = a_PosY - 1; i <= a_PosY + 1; ++i)
			{
				for (int j = a_PosX - 1; j <= a_PosX + 1; ++j)
				{
					if ((i >= 0 && i < m_GridHeight) && (j >= 0 && j < m_GridWidth))
					{
						RevealCell(j, i);
					}
				}
			}
		}
	}
}

bool FMinesweeperPluginModule::GameIsWon()
{
	for (int i = 0; i < m_GridHeight; ++i)
	{
		for (int j = 0; j < m_GridWidth; ++j)
		{
			if (!m_GridCellData[i][j].m_bIsBomb && m_GridCellData[i][j].m_Button->IsEnabled())
			{
				return false;
			}
		}
	}

	return true;
}

void FMinesweeperPluginModule::GameLost()
{
	UE_LOG(LogTemp, Error, TEXT("YOU LOST!"));

	for (int i = 0; i < m_GridHeight; ++i)
	{
		for (int j = 0; j < m_GridWidth; ++j)
		{
			if (m_GridCellData[i][j].m_bIsBomb)
			{
				m_GridCellData[i][j].m_Text->SetText(FString("X"));
			}
		}
	}
}

void FMinesweeperPluginModule::OnWidthInput(const FText& Value)
{
	if (Value.IsNumeric())
	{
		m_GridWidth = FCString::Atoi(*Value.ToString());

		UE_LOG(LogTemp, Display, TEXT("Width: %d"), m_GridWidth);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Please enter a valid width!"));

		m_GridWidth = -1;
	}
}

void FMinesweeperPluginModule::OnHeightInput(const FText& Value)
{
	if (Value.IsNumeric())
	{
		m_GridHeight = FCString::Atoi(*Value.ToString());

		UE_LOG(LogTemp, Display, TEXT("Height: %d"), m_GridHeight);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Please enter a valid height!"));

		m_GridHeight = -1;
	}
}

void FMinesweeperPluginModule::OnNumberOfMinesInput(const FText& Value)
{
	if (Value.IsNumeric())
	{
		m_MineCount = FCString::Atoi(*Value.ToString());

		UE_LOG(LogTemp, Display, TEXT("Number of mines: %d"), m_MineCount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Please enter a valid number of mines!"));

		m_MineCount = -1;
	}
}

void FMinesweeperPluginModule::IncrementBombCountOfSurroundingCells(int a_PosX, int a_PosY)
{
	for (int i = a_PosY - 1; i <= a_PosY + 1; ++i)
	{
		for (int j = a_PosX - 1; j <= a_PosX + 1; ++j)
		{
			if ((i >= 0 && i < m_GridHeight) && (j >= 0 && j < m_GridWidth))
			{
				++m_GridCellData[i][j].m_NumberOfSurroundingBombs;
			}
		}
	}
}

void FMinesweeperPluginModule::InitGame(int a_Width, int a_Height, int a_NumberOfMines)
{
	// Clear the cell data before each run
	m_GridCellData.Empty();
	
	// Construct the 2-dimensional array of cell data
	TArray<CellData> RowOfCells;
	RowOfCells.AddDefaulted(a_Width);
	for (auto i = 0; i < a_Height; ++i)
	{
		m_GridCellData.Add(RowOfCells);
	}

	// Place desired number of bombs in the grid
	for (auto i = 0; i < a_NumberOfMines; ++i)
	{
		int spawnX;
		int spawnY;

		do
		{
			spawnX = FMath::RandRange(0, a_Width - 1);
			spawnY = FMath::RandRange(0, a_Height - 1);
		}
		while (m_GridCellData[spawnY][spawnX].m_bIsBomb); // Loop while you find a cell that is not a bomb

		m_GridCellData[spawnY][spawnX].m_bIsBomb = true; // Make that cell a bomb

		IncrementBombCountOfSurroundingCells(spawnX, spawnY); // Increase the number of bombs of surrounding cells
	}
}

void FMinesweeperPluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMinesweeperPluginCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweeperPluginCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeperPluginModule, MinesweeperPlugin)