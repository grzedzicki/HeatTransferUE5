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

    /*
       Funkcja dostępna w Blueprintach, która otrzymuje parametry, siatkę temperatur oraz siatkę boolowską stałych 
       wartości i wywołująca klasę ThreadCalculations zajmującą się dokonywaniem obliczeń temperatury.
    */
void AThreadActor::InitCalculations(int32 grid_size_1, int32 grid_size_2, int32 grid_size_3, int32 heat_array_iteration_value, float alpha, float delta_x, float delta_y, float delta_z, float delta_t, TArray<float> CalculationArray, TArray<bool> BoolArray)
{
	UE_LOG(LogTemp, Warning, TEXT("AThreadActor::InitCalculations"));
	if (grid_size_1 > 0) {
		_grid_size_1 = grid_size_1;
		CalcThread = new FThreadCalculations(grid_size_1, grid_size_2, grid_size_3, heat_array_iteration_value, alpha, delta_x, delta_y, delta_z, delta_t, CalculationArray, BoolArray, this);
		CurrentRunningThread = FRunnableThread::Create(CalcThread, TEXT("Calculation Thread"));
	}
}

    /*
	Funkcja wyświelająca poprzednią iterację obliczeń.
    */
bool AThreadActor::GetPreviousStep()
{
	if(DataMap.Num() < 1) return false;
	if (UserIterator == 0) return false;
	UserIterator--;
	ThreadCalcArray = DataMap[UserIterator].HeatArrayFromMap;
	ThreadCalcBoolArray = DataMap[UserIterator].BoolArrayFromMap;
	CurrentHeatArrayIterationValue = DataMap[UserIterator].HeatArrayIterationValue;

	return true;
}

    /*
	Funkcja wyświelająca następną iterację obliczeń, jeśli takowe miały miejsce.
    */
void AThreadActor::GetNextStep()
{
	UE_LOG(LogTemp, Warning, TEXT("GetNextStep: Current UserIterator: %d"), UserIterator);
	UserIterator++;
	ThreadCalcArray = DataMap[UserIterator].HeatArrayFromMap;
	ThreadCalcBoolArray = DataMap[UserIterator].BoolArrayFromMap;
	CurrentHeatArrayIterationValue = DataMap[UserIterator].HeatArrayIterationValue;

}

    /*
	Funkcja zapisująca zmiany w siatce do tablic tymczasowych.
    */
void AThreadActor::SaveToTemp(TArray<float> CalculationArray, TArray<bool> BoolArray)
{
	TempIterator++;
	FPassiveData ArraysToTemp;
	ArraysToTemp.HeatArrayFromMap = CalculationArray;
	ArraysToTemp.BoolArrayFromMap = BoolArray;
	ArraysToTemp.HeatArrayIterationValue = CurrentHeatArrayIterationValue;
	TempDataMap.Add(TempIterator, ArraysToTemp);

    /*
	Funkcja usuwająca wszystkie tablice.
    */
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

    /*
	Funkcja wczytująca poprzednie dane. (CTRL+Z)
    */
bool AThreadActor::ReadFromTempBackwards()
{
	if (TempIterator == 0) return false;
	TempIterator--;
	ThreadCalcArray = TempDataMap[TempIterator].HeatArrayFromMap;
	ThreadCalcBoolArray = TempDataMap[TempIterator].BoolArrayFromMap;
	CurrentHeatArrayIterationValue = TempDataMap[TempIterator].HeatArrayIterationValue;

	ReadBackWasUsed = true;
	return true;
}

    /*
	Funkcja wyświelająca kolejne dane, jeśli takie istnieją oraz
 	pod warunkiem, że nie dokonano żadnych zmian w siatce. (CTRL+Y)
    */
bool AThreadActor::ReadFromTempFurther(TArray<float> CalculationArray, TArray<bool> BoolArray)
{
	if (TempIterator == TempDataMap.Num() - 1) { 
		ReadBackWasUsed = false;
		return false; 
	}

	if (CalculationArray == TempDataMap[TempIterator].HeatArrayFromMap && BoolArray == TempDataMap[TempIterator].BoolArrayFromMap) {
		TempIterator++;
		ThreadCalcArray = TempDataMap[TempIterator].HeatArrayFromMap;
		ThreadCalcBoolArray = TempDataMap[TempIterator].BoolArrayFromMap;
		CurrentHeatArrayIterationValue = TempDataMap[TempIterator].HeatArrayIterationValue;

		UE_LOG(LogTemp, Warning, TEXT("Loaded CTRL+Y"), );
	}
	return true;
}

    /*
	Funkcja wywoływana w celu zapisania pustej siatki na indeksie 0.
    */
void AThreadActor::StartActor()
{
	_grid_size_1 = MyHeatGameInstance->Grid_Size_1;
	_grid_size_2 = MyHeatGameInstance->Grid_Size_2;
	_grid_size_3 = MyHeatGameInstance->Grid_Size_3;
	TArray<float> IntArray;
	TArray<bool> BoolArray;
	int32 iteration = 0;
	IntArray.Init(0, _grid_size_1 * _grid_size_2 * _grid_size_3);
	BoolArray.Init(false, _grid_size_1 * _grid_size_2 * _grid_size_3);
	FPassiveData Arrays;
	Arrays.HeatArrayFromMap = IntArray;
	Arrays.BoolArrayFromMap = BoolArray;
	Arrays.HeatArrayIterationValue = iteration;
	DataMap.Add(0, Arrays);
	TempDataMap.Add(0, Arrays);
	MapIterator = 0;
	UserIterator = MapIterator;
	TempIterator = UserIterator;
}

    /*
	Funkcja wykorzystywana podczas debugowania w celu sprawdzenia działania zapisu/odczytu plików.
    */
void AThreadActor::CheckSave()
{
	USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot("MySave", 0);
	SaveFileObject = Cast<UFile_Manager>(LoadedGame);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Trying to load a saved game."));
	if (!SaveFileObject)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No saved games found. Trying to save a new one."));
		SaveFileObject = Cast<UFile_Manager>(UGameplayStatics::CreateSaveGameObject(UFile_Manager::StaticClass()));
		const bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveFileObject, "saveeeee", 0);
		if(IsSaved)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Saved."));
		else
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Can't save."));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Got Save"));
	}
}


    /*
	Metoda sumująca ilość wykonanych iteracji w zależności od aktualnie wyświetlanej siatki
    */
void AThreadActor::SumIterationsValues()
{
	int32 value = 0;
	if (UserIterator == 0) {
		SumIterations = 0;
		return;
	}
	
	for (int32 Index = 1; Index <= UserIterator; Index++)
	{
		value += DataMap[Index].HeatArrayIterationValue;
	}
	SumIterations = value;
}

    /*
	Funkcja przekazująca GameInstance TMapę zawierającą wszystkie dokonane obliczenia
    */
void AThreadActor::SaveDataToFile()
{
	UE_LOG(LogTemp, Warning, TEXT("DataMap.Num: %d"), DataMap.Num());

	MyHeatGameInstance->SaveFile(DataMap);
}

    /*
	Funkcja wczytująca dane z pliku korzystająca z GameInstance
    */
void AThreadActor::LoadDataFromFile()
{
	DataMap = MyHeatGameInstance->DataFromFile;
	MapIterator = DataMap.Num() - 1;
	UserIterator = MapIterator;
	_alpha = MyHeatGameInstance->Alpha;
	_delta_x = MyHeatGameInstance->Delta_x;
	_delta_y = MyHeatGameInstance->Delta_y;
	_delta_z = MyHeatGameInstance->Delta_z;
	UE_LOG(LogTemp, Warning, TEXT("Loaded Value: %d"), UserIterator);
	ThreadCalcArray = DataMap[MapIterator].HeatArrayFromMap;
	ThreadCalcBoolArray = DataMap[MapIterator].BoolArrayFromMap;
	CurrentHeatArrayIterationValue = DataMap[MapIterator].HeatArrayIterationValue;
}

    /*
	Funkcja wyszukująca indeksu podanej siatki
    */
int32 AThreadActor::FindValueInMap(TArray<float> CalculationArray, TArray<bool> BoolArray)
{
	for (auto& Elem : DataMap) {
		if (Elem.Value.HeatArrayFromMap == CalculationArray && Elem.Value.BoolArrayFromMap == BoolArray)
			return Elem.Key;
	}
	return -1;
}

    /*
	Funkcja wywoływana w momencie, gdy znajdujemy się przykładowo w "Iteracji 2/3" i chcemy wyświetlić siatkę 3/3
 	Sprawdza ona czy obecnie wyświetlana siatka jest zgodna z zapisaną siatką.
  	Jeśli tak, to wyświtla kolejną siatkę, a jeśli nie to usuwa wszystkie iteracje po obecnej i dokonuje nowych obliczeń od tej iteracji.
    */
bool AThreadActor::CheckIfArrayExist(TArray<float> CalculationArray, TArray<bool> BoolArray, int32 heat_array_iteration_value)
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
	UE_LOG(LogTemp, Warning, TEXT("AThreadActor::CheckIfArrayExist"));
	TArray<float> HeatArray = DataMap[UserIterator].HeatArrayFromMap;
	TArray<bool> _BoolArray = DataMap[UserIterator].BoolArrayFromMap;
	int32 HeatArrayIterationValue = DataMap[UserIterator].HeatArrayIterationValue;

	if (HeatArray == CalculationArray && _BoolArray == BoolArray && HeatArrayIterationValue == heat_array_iteration_value) {
		UE_LOG(LogTemp, Warning, TEXT("Arrays are the same -> Try Load From Map"));
		GetNextStep();
		return true;
	}
	if(HeatArrayIterationValue != heat_array_iteration_value)
		UE_LOG(LogTemp, Warning, TEXT("Different VALUES got: %d, expected %d"), HeatArrayIterationValue, heat_array_iteration_value);

	UE_LOG(LogTemp, Warning, TEXT("Different Arrays -> Deleting old new step calculations"));
	DeleteFurtherData();
	return false;
}

    /*
	Funkcja usuwająca dane, w przypadku jeśli zmianiła się siatka
    */
void AThreadActor::DeleteFurtherData()
{
	for (int i = UserIterator+1; i < DataMap.Num(); i++)
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

    /*
	Funkcja wywoływana w przypadku dokonania nowych obliczeń siatki
 	Przekazuje ona siatkę temperatur, siatkę boolowską oraz wartość iteracji
    */
void AThreadActor::InsertToMap()
{
	MapIterator++;
	UserIterator++;
	TempIterator++;
	UE_LOG(LogTemp, Warning, TEXT("AThreadActor::InsertToMap"));
	UE_LOG(LogTemp, Warning, TEXT("User: %d"), UserIterator);
	UE_LOG(LogTemp, Warning, TEXT("User: %d"), MapIterator);
	UE_LOG(LogTemp, Warning, TEXT("%d"), CurrentHeatArrayIterationValue);
	FPassiveData Arrays;
	Arrays.HeatArrayFromMap = ThreadCalcArray;
	Arrays.BoolArrayFromMap = ThreadCalcBoolArray;
	Arrays.HeatArrayIterationValue = CurrentHeatArrayIterationValue;
	DataMap.Add(MapIterator, Arrays);
	TempDataMap.Add(TempIterator, Arrays);
}

    /*
	Funkcja usuwająca wartości tymczasowe
    */
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

    /*
	Funkcja usuwająca wątek obliczeniowy
    */
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

