[Powrót](../README.md)<br />
 
[Unreal/C++](https://github.com/grzedzicki/HeatTransferUE5/tree/main/ThreadActor/Unreal%20C%2B%2B) Kod źródłowy
  
  
Blueprinty:

CTRL+SCROLL oddalenie/przybliżenie  
Poruszanie się prawym przyciskiem myszy  

## Create Thread Reference
Funkcja wywołująca FRunnable ThreadCalculations zajmujące się obliczeniami kolejnych kroków czasowych.
<iframe width=1280 height=650 src="https://blueprintue.com/render/ef7oi9qg/" scrolling="no" allowfullscreen></iframe>


## BP_ThreadActor
Po otrzymaniu wyników z FRunnable przekazuje nowe wartości do NodeOwnera i informuje o potrzebie aktualizacji wyświetlanej siatki.
<iframe width=1280 height=650 src="https://blueprintue.com/render/zejea3zc/" scrolling="no" allowfullscreen></iframe>


## Calculate Next Step
Przekazanie siatki temperatur klasie FRunnable w celu wykonania obliczeń.
<iframe width=1280 height=650 src="https://blueprintue.com/render/t9wtuf49" scrolling="no" allowfullscreen></iframe>


## Return New Array
Po wykonaniu obliczeń prze FRunnable pobranie wyników z owej klasy.
<iframe width=1280 height=650 src="https://blueprintue.com/render/-fijfrg4/" scrolling="no" allowfullscreen></iframe>

