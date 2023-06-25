// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreadCalculations.h"
#include "..\Public\ThreadCalculations.h"
#include "ThreadActor.h"

FThreadCalculations::FThreadCalculations(int32 _grid_size_1, int32 _grid_size_2, int32 _grid_size_3, int32 _heat_array_iteration_value, float _alpha, float _delta_x, float _delta_y, float _delta_z, float _delta_t, TArray<float> _CalculationArray, TArray<bool> _BoolArray, AThreadActor* FunActor)
{
	if (FunActor) {
		grid_size_1 = _grid_size_1;
		grid_size_2 = _grid_size_2;
		grid_size_3 = _grid_size_3;
		heat_array_iteration_value = _heat_array_iteration_value;
		alpha = _alpha;
		delta_x = _delta_x;
		delta_y = _delta_y;
		delta_z = _delta_z;
		delta_t = _delta_t;
		NewHeatArray = _CalculationArray;
		OriginalHeatArray = NewHeatArray;
		BoolArray = _BoolArray;
		CurrentThreadActor = FunActor;
	}
}

FThreadCalculations::FThreadCalculations()
{
}

bool FThreadCalculations::Init()
{
	bStopThread = false;
	return true;
}

uint32 FThreadCalculations::Run()
{
	CalculateArray();
	CurrentThreadActor->ThreadCalcArray = NewHeatArray;
	CurrentThreadActor->ThreadCalcBoolArray = BoolArray;
	CurrentThreadActor->CurrentHeatArrayIterationValue = heat_array_iteration_value;
	CurrentThreadActor->InsertToMap();
	CurrentThreadActor->SumIterationsValues();

	return uint32();
}

void FThreadCalculations::Stop()
{
}

void FThreadCalculations::CalculateArray()
{
	gamma = alpha * delta_t;
	for (int iteration = 0; iteration < heat_array_iteration_value; iteration++) {
		for (int k = 1; k < grid_size_1 - 1; k++)
			for (int i = 1; i < grid_size_3 - 1; i++)
				for (int j = 1; j < grid_size_2 - 1; j++)
				{
					float value = ReadArray(k, i, j) + gamma * (((ReadArray(k + 1, i, j) - 2 * (ReadArray(k, i, j)) + ReadArray(k - 1, i, j)) / (FMath::Pow(delta_x, 2))) + ((ReadArray(k, i + 1, j) - 2 * (ReadArray(k, i, j)) + ReadArray(k, i - 1, j)) / (FMath::Pow(delta_y, 2))) + ((ReadArray(k, i, j - 1) - 2 * (ReadArray(k, i, j)) + ReadArray(k, i, j + 1)) / (FMath::Pow(delta_z, 2))));
					SetArray(k, i, j, value);
				}
		OriginalHeatArray = NewHeatArray;
	}
	UE_LOG(LogTemp, Display, TEXT("FThreadCalculations::CalculateArray Finished"));

}
										           
float FThreadCalculations::ReadArray(int num_tablicy, int i, int j)
{
	int index = num_tablicy * grid_size_2 * grid_size_3 + i * grid_size_2 + j; // jesli x jest wieksze

	return OriginalHeatArray[index];
}

void FThreadCalculations::SetArray(int num_tablicy, int i, int j, float value)
{
	int index = num_tablicy * grid_size_2 * grid_size_3 + i * grid_size_2 + j;
	
	if (BoolArray[index] == false)
		NewHeatArray[index] = value;
	else return;
}

void FThreadCalculations::PrintArray()
{
	UE_LOG(LogTemp, Warning, TEXT("FThreadCalculations::PrintArray"));

	int arraysize = NewHeatArray.Num();

	for (int i = 0; i < arraysize; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f"), NewHeatArray[i]);
	}
}

