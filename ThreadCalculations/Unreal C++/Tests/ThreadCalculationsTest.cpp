// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"
#include "Math/UnrealMathUtility.h" 
#include "ThreadCalculations.h"
#include "Logging/LogMacros.h"

/*
    Deklaracja testu jednostkowego
*/
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FThreadCalculationsTest, "ThreadCalculations.CalculateArray", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FThreadCalculationsTest::RunTest(const FString& Parameters)
{
    class FThreadCalculations* ThreadCalculations = nullptr;
    ThreadCalculations = new FThreadCalculations();
    /*
        Deklaracja oczekiwanej tablicy, zawiera wartości pochodzące z obliczeń dokonanych w języku Python
    */
    TArray<float> ExpectedHeatArray; // 
    ExpectedHeatArray.Init(0, 64);
    ExpectedHeatArray[5] = 100.0f;
    ExpectedHeatArray[21] = 100.0f;

    /*
        Deklaracja tablic, na podstawie których dokonywane są obliczenia
    */
    TArray<float> TestHeatArray;
    TArray<bool> TestBoolArray;
    TestHeatArray.Init(0, 64);
    TestHeatArray[5] = 100.0f;
    TestBoolArray.Init(false, 64);
    TestBoolArray[5] = true;

    /*
        Deklaracja zmiennych wymaganych do wykonania obliczeń
    */
    ThreadCalculations->delta_t = 1.0f;
    ThreadCalculations->delta_x = 1.0f;
    ThreadCalculations->delta_y = 1.0f;
    ThreadCalculations->delta_z = 1.0f;
    ThreadCalculations->alpha = 1.0f;
    ThreadCalculations->grid_size_1 = 4;
    ThreadCalculations->grid_size_2 = 4;
    ThreadCalculations->grid_size_3 = 4;
    ThreadCalculations->OriginalHeatArray = TestHeatArray;
    ThreadCalculations->NewHeatArray = TestHeatArray;
    ThreadCalculations->BoolArray = TestBoolArray;

    /*
        Deklaracja ilości iteracji obliczeń oraz wywołanie samych obliczeń
    */
    ThreadCalculations->heat_array_iteration_value = 1;
    ThreadCalculations->CalculateArray();

    /*
        Porównanie oczekiwanych wyników z otrzymanymi
        Jeśli obie tablice są takie same, test zwraca prawdę, w przeciwnym wypadku fałsz.
    */
    TestEqual("Calculations Test", ExpectedHeatArray, ThreadCalculations->OriginalHeatArray);

    return true;
}
