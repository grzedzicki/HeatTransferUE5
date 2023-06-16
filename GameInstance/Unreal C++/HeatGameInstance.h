// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "File_Manager.h"
#include "StructData.h"
#include "HeatGameInstance.generated.h"

UCLASS()
class CLEANHEATTRANSFER_API UHeatGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "Settings")
		UFile_Manager* OpenFileObject;

	UPROPERTY(BlueprintReadWrite, Category = "Settings")
		UFile_Manager* SaveFileObject;

	UPROPERTY(BlueprintReadWrite)
	FString SaveFileName;

	UFUNCTION(BlueprintCallable)
		void CreateFileManager();

	UPROPERTY(BlueprintReadWrite)
		int32 Grid_Size_1;

	UPROPERTY(BlueprintReadWrite)
		int32 Grid_Size_2;

	UPROPERTY(BlueprintReadWrite)
		int32 Grid_Size_3;

	UPROPERTY(BlueprintReadWrite)
		int32 SliderVal;

	UPROPERTY(BlueprintReadWrite)
		FString LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Alpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Delta_x;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Delta_y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Delta_z;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Delta_t;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Long_distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Big_size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Node_size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FPassiveData> DataFromFile;

	UFUNCTION(BlueprintCallable, Category = "File Manager")
		void LoadFile(FString FileName);

	UFUNCTION(BlueprintCallable, Category = "File Manager")
		void SaveFile(TMap<int32, FPassiveData> DataToSave);

};
