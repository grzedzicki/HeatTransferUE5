// Fill out your copyright notice in the Description page of Project Settings.

#include "ThreadActor.h"
#include "..\Public\ThreadActor.h"
#include "File_Manager.h"
#include "HeatGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include <CleanHeatTransfer/Public/HeatGameInstance.h>
#include "..\Public\HeatGameInstance.h"

// Sets default values
AThreadActor::AThreadActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AThreadActor::InitCalculations(int32 grid_size_1, int32 grid_size_2, int32 grid_size_3, float alpha, float delta_x, float delta_y, float delta_z, float delta_t, TArray<float> CalculationArray, TArray<bool> BoolArray)
{
	if (grid_size_1 > 0) {
		_grid_size_1 = grid_size_1;
		CalcThread = new FThreadCalculations(grid_size_1, grid_size_2, grid_size_3, alpha, delta_x, delta_y, delta_z, delta_t, CalculationArray, BoolArray, this);
		CurrentRunningThread = FRunnableThread::Create(CalcThread, TEXT("Calculation Thread"));
	}
}

bool AThreadActor::GetPreviousStep()
{
	if(DataMap.Num() < 1) return false;
	if (UserIterator == 0) return false;
	UserIterator--;
	ThreadCalcArray = DataMap[UserIterator].HeatArrayFromMap;
	ThreadCalcBoolArray = DataMap[UserIterator].BoolArrayFromMap;
	return true;
}

void AThreadActor::GetNextStep()
{
	UserIterator++;
	ThreadCalcArray = DataMap[UserIterator].HeatArrayFromMap;
	ThreadCalcBoolArray = DataMap[UserIterator].BoolArrayFromMap;
}

void AThreadActor::SaveToTemp(TArray<float> CalculationArray, TArray<bool> BoolArray)
{
	TempIterator++;
	FPassiveData PairOfArrays;
	PairOfArrays.HeatArrayFromMap = CalculationArray;
	PairOfArrays.BoolArrayFromMap = BoolArray;
	TempDataMap.Add(TempIterator, PairOfArrays);
}

void AThreadActor::ResetDataInC()
{
	for (int i = DataMap.Num() - 1; i >= 0; i--)
		DataMap.Remove(i);
	MapIterator = -1;
	UserIterator = -1;

	for (int i = TempDataMap.Num() - 1; i >= 0; i--)
		TempDataMap.Remove(i);
	TempIterator = -1;
}

bool AThreadActor::ReadFromTempBackwards()
{
	UE_LOG(LogTemp, Warning, TEXT("Current TempIterator: %d"), TempIterator);
	if (TempIterator == 0) return false;
	TempIterator--;
	ThreadCalcArray = TempDataMap[TempIterator].HeatArrayFromMap;
	ThreadCalcBoolArray = TempDataMap[TempIterator].BoolArrayFromMap;
	ReadBackWasUsed = true;
	UE_LOG(LogTemp, Warning, TEXT("Current TempIterator: %d"), TempIterator);
	return true;
}

bool AThreadActor::ReadFromTempFurther(TArray<float> CalculationArray, TArray<bool> BoolArray)
{
	if (TempIterator == TempDataMap.Num() - 1) { 
		ReadBackWasUsed = false;
		return false; 
	}
	for (int i = 0; i < ThreadCalcArray.Num(); i++)
		if(ThreadCalcArray[i] != 0)
	for (int i = 0; i < ThreadCalcArray.Num(); i++)
		if (TempDataMap[TempIterator].HeatArrayFromMap[i] != 0)
	if (CalculationArray == TempDataMap[TempIterator].HeatArrayFromMap && BoolArray == TempDataMap[TempIterator].BoolArrayFromMap) {
		TempIterator++;
		ThreadCalcArray = TempDataMap[TempIterator].HeatArrayFromMap;
		ThreadCalcBoolArray = TempDataMap[TempIterator].BoolArrayFromMap;
	}
	return true;
}

void AThreadActor::StartActor()
{
	_grid_size_1 = MyHeatGameInstance->Grid_Size_1;
	_grid_size_2 = MyHeatGameInstance->Grid_Size_2;
	_grid_size_3 = MyHeatGameInstance->Grid_Size_3;
	TArray<float> IntArray;
	TArray<bool> BoolArray;
	IntArray.Init(0, _grid_size_1 * _grid_size_2 * _grid_size_3);
	BoolArray.Init(false, _grid_size_1 * _grid_size_2 * _grid_size_3);
	FPassiveData PairOfArrays;
	PairOfArrays.HeatArrayFromMap = IntArray;
	PairOfArrays.BoolArrayFromMap = BoolArray;
	DataMap.Add(0, PairOfArrays);
	TempDataMap.Add(0, PairOfArrays);
	MapIterator = 0;
	UserIterator = MapIterator;
	TempIterator = UserIterator;
}

void AThreadActor::SaveDataToFile()
{
	UE_LOG(LogTemp, Warning, TEXT("DataMap.Num: %d"), DataMap.Num());

	MyHeatGameInstance->SaveFile(DataMap);
}

void AThreadActor::LoadDataFromFile()
{
	DataMap = MyHeatGameInstance->DataFromFile;
	MapIterator = DataMap.Num() - 1;
	UserIterator = MapIterator;
	_alpha = MyHeatGameInstance->Alpha;
	_delta_x = MyHeatGameInstance->Delta_x; 
	_delta_y = MyHeatGameInstance->Delta_y;
	_delta_z = MyHeatGameInstance->Delta_z;
	ThreadCalcArray = DataMap[MapIterator].HeatArrayFromMap;
	ThreadCalcBoolArray = DataMap[MapIterator].BoolArrayFromMap;
}

int32 AThreadActor::FindValueInMap(TArray<float> CalculationArray, TArray<bool> BoolArray)
{
	for (auto& Elem : DataMap) {
		if (Elem.Value.HeatArrayFromMap == CalculationArray && Elem.Value.BoolArrayFromMap == BoolArray)
			return Elem.Key;
	}
	return -1;
}

bool AThreadActor::CheckIfArrayExist(TArray<float> CalculationArray, TArray<bool> BoolArray)
{
	if (ReadBackWasUsed) {
		ReadBackWasUsed = false;
		DeleteFurtherTemp();
		int32 FoundKey = FindValueInMap(CalculationArray, BoolArray);
		if (FoundKey != -1) {
			UserIterator = FoundKey;
			if(UserIterator < MapIterator)
				GetNextStep();
		}
	}

	if (UserIterator == MapIterator)
		return false;
	TArray<float> HeatArray = DataMap[UserIterator].HeatArrayFromMap;
	TArray<bool> _BoolArray = DataMap[UserIterator].BoolArrayFromMap;
	if (HeatArray == CalculationArray && _BoolArray == BoolArray) {
		UE_LOG(LogTemp, Warning, TEXT("Arrays are the same -> Try Load From Map"));
		GetNextStep();
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Different Arrays -> Deleting old new step calculations"));
	DeleteFurtherData();
	return false;
}

void AThreadActor::DeleteFurtherData()
{
	for (int i = UserIterator; i < DataMap.Num(); i++)
		DataMap.Remove(i);
	MapIterator = UserIterator;
}

// Called when the game starts or when spawned
void AThreadActor::BeginPlay()
{
	Super::BeginPlay();

	MyHeatGameInstance = Cast<UHeatGameInstance>
		(UGameplayStatics::GetGameInstance(GetWorld()));
}

void AThreadActor::TalkToInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("Answer from instance: %s"), *FString(MyHeatGameInstance->SaveFileName));
}

void AThreadActor::InsertToMap()
{
	MapIterator++;
	UserIterator++;
	TempIterator++;
	FPassiveData PairOfArrays;
	PairOfArrays.HeatArrayFromMap = ThreadCalcArray;
	PairOfArrays.BoolArrayFromMap = ThreadCalcBoolArray;
	DataMap.Add(MapIterator, PairOfArrays);
	TempDataMap.Add(TempIterator, PairOfArrays);
}

void AThreadActor::DeleteFurtherTemp()
{
	for (int i = TempIterator; i < TempDataMap.Num(); i++)
		TempDataMap.Remove(i);
}

// Called every frame
void AThreadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThreadActor::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UE_LOG(LogTemp, Warning, TEXT("Threads Owner: Killed Calculation Thread"));
	if (CurrentRunningThread && CalcThread)
	{
		CurrentRunningThread->Suspend(true);
		CalcThread->bStopThread = true;
		CurrentRunningThread->Suspend(false);
		CurrentRunningThread->Kill(false);
		CurrentRunningThread->WaitForCompletion();
		delete CalcThread;
	}
}

