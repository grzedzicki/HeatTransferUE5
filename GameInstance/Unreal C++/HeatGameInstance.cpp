// Fill out your copyright notice in the Description page of Project Settings.


#include "HeatGameInstance.h"
#include "File_Manager.h"
#include "Kismet/GameplayStatics.h"

void UHeatGameInstance::CreateFileManager()
{
}

void UHeatGameInstance::LoadFile(FString FileName)
{
	USaveGame* OpenFile = UGameplayStatics::LoadGameFromSlot(FileName, 0);
	OpenFileObject = Cast<UFile_Manager>(OpenFile);
	if (!OpenFileObject)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("UHeatGameInstance::No saved games found. Trying to save a new one."));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("UHeatGameInstance::Loaded File"));
		SaveFileName = FileName;
		LevelName = OpenFileObject->LevelName;
		Grid_Size_1 = OpenFileObject->GridSize_1;
		Grid_Size_2 = OpenFileObject->GridSize_2;
		Grid_Size_3 = OpenFileObject->GridSize_3;
		SliderVal = OpenFileObject->SliderValue;
		DataFromFile = OpenFileObject->DataToFile;
		Alpha = OpenFileObject->alpha;
		Delta_x = OpenFileObject->delta_x;
		Delta_y = OpenFileObject->delta_y;
		Delta_z = OpenFileObject->delta_z;
		Delta_t = OpenFileObject->delta_t;
		Long_distance = OpenFileObject->long_distance;
		Big_size = OpenFileObject->big_size;
		Node_size = OpenFileObject->node_size;
		UE_LOG(LogTemp, Warning, TEXT("In instance"), );
		UE_LOG(LogTemp, Warning, TEXT("Instance Value: %d"), DataFromFile.Num());
	}
}

void UHeatGameInstance::SaveFile(TMap<int32, FPassiveData> DataToSave)
{
	SaveFileObject = Cast<UFile_Manager>(UGameplayStatics::CreateSaveGameObject(UFile_Manager::StaticClass()));
	SaveFileObject->LevelName = LevelName;
	SaveFileObject->GridSize_1 = Grid_Size_1;
	SaveFileObject->GridSize_2 = Grid_Size_2;
	SaveFileObject->GridSize_3 = Grid_Size_3;
	SaveFileObject->SliderValue = SliderVal;
	SaveFileObject->DataToFile = DataToSave;
	SaveFileObject->alpha = Alpha;
	SaveFileObject->delta_x = Delta_x;
	SaveFileObject->delta_y = Delta_y;
	SaveFileObject->delta_z = Delta_z;
	SaveFileObject->delta_t = Delta_t;
	SaveFileObject->long_distance = Long_distance;
	SaveFileObject->big_size = Big_size;
	SaveFileObject->node_size = Node_size;
	
	if(UGameplayStatics::SaveGameToSlot(SaveFileObject, SaveFileName, 0))
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Saved Data To File"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Failed to save to file"));
}
