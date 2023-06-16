// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThreadCalculations.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "File_Manager.h"
#include "StructData.h"
#include "HeatGameInstance.h"
#include "ThreadActor.generated.h"

UCLASS()
class CLEANHEATTRANSFER_API AThreadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThreadActor();

	UFUNCTION(BlueprintCallable, Category = "instance")
		void TalkToInstance();

	UFUNCTION(BlueprintCallable, Category = "Calculations")
		void InitCalculations(int32 grid_size_1, int32 grid_size_2, int32 grid_size_3, float _alpha, float _delta_x, float _delta_y, float _delta_z, float _delta_t, TArray<float> CalculationArray, TArray<bool> BoolArray);

	UFUNCTION(BlueprintCallable, Category = "Calculations")
		bool GetPreviousStep();

	UFUNCTION(BlueprintCallable, Category = "Calculations")
		bool CheckIfArrayExist(TArray<float> CalculationArray, TArray<bool> BoolArray);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FPassiveData> DataMap;

	TMap<int32, FPassiveData> TempDataMap;

	void DeleteFurtherData();

	void GetNextStep();

	UFUNCTION(BlueprintCallable, Category = "Calculations")
		void SaveToTemp(TArray<float> CalculationArray, TArray<bool> BoolArray);

	UFUNCTION(BlueprintCallable, Category = "Calculations")
		void ResetDataInC();

	UFUNCTION(BlueprintCallable, Category = "Calculations")
		bool ReadFromTempBackwards();

	UFUNCTION(BlueprintCallable, Category = "Calculations")
		bool ReadFromTempFurther(TArray<float> CalculationArray, TArray<bool> BoolArray);

	UFUNCTION(BlueprintCallable, Category = "Calculations")
		void StartActor();

	UFUNCTION(BlueprintCallable, Category = "Files")
		void CheckSave();

	UFUNCTION(BlueprintCallable, Category = "Files")
		void SaveDataToFile();

	UFUNCTION(BlueprintCallable, Category = "Files")
		void LoadDataFromFile();

	int32 FindValueInMap(TArray<float> CalculationArray, TArray<bool> BoolArray);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class FThreadCalculations* CalcThread = nullptr;
	FRunnableThread* CurrentRunningThread = nullptr;


public:	

	class UHeatGameInstance* MyHeatGameInstance;

	UPROPERTY(BlueprintReadWrite, Category = "Settings")
		UFile_Manager* SaveFileObject;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapExample)
		TArray<float> ThreadCalcArray; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapExample)
		TArray<bool> ThreadCalcBoolArray; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapExample)
		int32 size;

	void InsertToMap();

	int32 MapIterator = -1;

	int32 UserIterator = -1;

	int32 TempIterator = -1;

	int32 _grid_size_1;

	int32 _grid_size_2;

	int32 _grid_size_3;

	float _alpha;

	float _delta_x;

	float _delta_y;

	float _delta_z;

	float _delta_t;

	bool ReadBackWasUsed = false;

	void DeleteFurtherTemp();
};
