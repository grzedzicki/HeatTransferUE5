[Powrót](../README.md)<br />
 
[Unreal/C++](https://github.com/grzedzicki/HeatTransferUE5/tree/main/ThreadActor/Unreal%20C%2B%2B) Kod źródłowy
  
  
Blueprinty:

CTRL+SCROLL oddalenie/przybliżenie  
Poruszanie się prawym przyciskiem myszy  

## Create Thread Reference
Funkcja wywołująca FRunnable ThreadCalculations zajmujące się obliczeniami kolejnych kroków czasowych.
<iframe width=1280 height=650 src="https://blueprintue.com/render/frwruwjx/" scrolling="no" allowfullscreen></iframe>


## BP_ThreadActor
Po otrzymaniu wyników z FRunnable przekazuje nowe wartości do NodeOwnera i informuje o potrzebie aktualizacji wyświetlanej siatki.
<iframe width=1280 height=650 src="https://blueprintue.com/render/eo00ghzt/" scrolling="no" allowfullscreen></iframe>


## Calculate Next Step
Przekazanie siatki temperatur klasie FRunnable w celu wykonania obliczeń.
<iframe width=1280 height=650 src="https://blueprintue.com/render/2eg6cpfu" scrolling="no" allowfullscreen></iframe>


## Return New Array
Po wykonaniu obliczeń prze FRunnable pobranie wyników z owej klasy.
<iframe width=1280 height=650 src="https://blueprintue.com/render/365gr9ux/" scrolling="no" allowfullscreen></iframe>

