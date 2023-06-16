// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Public/HAL/Runnable.h"

//Forward Declarations
class FRunnableThread;
class AThreadActor;

class FThreadCalculations : public FRunnable
{
	
public:	
	// Sets default values for this actor's properties
	FThreadCalculations(int32 grid_size_1, int32 grid_size_2, int32 grid_size_3, float _alpha, float _delta_x, float _delta_y, float _delta_z, float _delta_t, TArray<float> CalculationArray, TArray<bool> BoolArray, AThreadActor* FunActor);
	FThreadCalculations();
	bool bStopThread;

	virtual bool Init();

	virtual uint32 Run();

	virtual void Stop();


private:

	AThreadActor* CurrentThreadActor;

public: 

	int32 grid_size_1;

	int32 grid_size_2;

	int32 grid_size_3;

	float alpha;

	float delta_x;

	float delta_y;

	float delta_z;

	float delta_t;

	float gamma;

	TArray<float> NewHeatArray;

	TArray<float> OriginalHeatArray;

	TArray<bool> BoolArray;

	void CalculateArray();

	float ReadArray(int num_tablicy, int i, int j);

	void SetArray(int num_tablicy, int i, int j, float value);

	void PrintArray();
};
