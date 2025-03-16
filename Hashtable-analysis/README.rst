##################
Hashtable Analysis
##################

******
Autor:
******
Michał Kowieski 293130

**************
Treść zadania:
**************
Przedmiotem analizy jest tablica mieszająca: tablica przechowuje rekordy zawierające napisy. Długość
tablicy jest ograniczona arbitralnie przez pewną stałą K. Dla danego napisu s obliczamy k=M(s) i
umieszczamy strukturę reprezentującą napis w tablicy mieszającej: H[k]. W przypadku kolizji funkcji
mieszającej (H[k] zajęte) reprezentujące napis s struktury danych zapisywane są w sposób
alternatywny zobacz warianty). Przedmiotem implementacji powinno być: dodanie i usunięcie
elementów w H[]. Wybór funkcji mieszającej M(s) do decyzji projektanta - ale patrz wariant 3


**W13:**
tablica H[k,n] (gdzie n=0...N) jest dwuwymiarowe, elementy kolidujące zapisywane są w lokalizacjach H[k,0], H[k,1], itd. (Oczywiście, przekroczenie przez drugi indeks rozmiaru tablicy będzie powodować odrzucenie elementu)

**W21:**
Testy przeprowadzić dla: listy słów języka polskiego wygenerowanych z zadanych tekstów. Generator należy wykonać samodzielnie.

**W31:**
Zastosować jedną funkcję mieszającą; dodatkowo przeprowadzić analizę dla enumeracji tablicy (wydobycia wszystkich elementów).

*******************
Działanie programu:
*******************

Na wejście program dostaje ciąg napisów. Najpierw dodaje je wszystkie do tablicy. Następnie generuje wewnętrzny stan tablicy mieszającej (jakie napisy się tam znajdują oraz w których są "kubełkach"). Ten stan zwraca użytkownikowi.

************************
Korzystanie z  programu:
************************

Program główny:
===============

.. code-block:: bash

  hashtable [OPTIONS] ROWS COLUMNS COMMAND [ARGS]

Tworzy tablicę mieszającą posiadającą ROWS wierszy oraz
COLUMNS column.

**Args:**

- rows (int): Ilość wierszy tablicy mieszającej.
- columns (int): Ilość kolumn tablicy mieszającej.

Komendy:
--------

IO:
"""
.. code-block:: bash

  hashtable [OPTIONS] ROWS COLUMNS io [OPTIONS] INPUT OUTPUT

Pobiera słowa z INPUT. Potem dodaje je wszystkie do tablicy mieszającej.
Następnie na OUTPUT zostaje wygenerowany wewnętrzny stan tablicy
mieszającej.

**Args:**

- input (file): Plik na podstawie którego będą generowane dane.
- output (file): Plik do którego zostanie przekazany wynik.


GENERATE:
"""""""""
.. code-block:: bash

  hashtable [OPTIONS] ROWS COLUMNS generate [OPTIONS] INPUT OUTPUT NUMBER_OF_WORDS

Generuje NUMBER_OF_WORDS słow na podstawie INPUT. Potem dodaje je
wszystkie do tablicy mieszającej. Następnie na OUTPUT zostaje
wygenerowany wewnętrzny stan tablicy mieszającej.

**Args:**

- input (file): Plik na podstawie którego będą generowane dane.
- output (file): Plik do którego zostanie przekazany wynik.
- number_of_words (int): Liczba generowanych słów.


BENCHMARK:
"""""""""
.. code-block:: bash

  hashtable [OPTIONS] ROWS COLUMNS benchmark [OPTIONS] INPUT OUTPUT INITIAL_NUMBER_OF_WORDS STEP NUMBER_OF_PROBLEMS NUMBER_OF_INSTANCES

Wykonuje funckję generate z pomiarem czasu dla rosnącej wartości
NUMBER_OF_WORDS. Przeprowadza porównanie ze słożonością teoretyczną.

**Args:**

- input (file): Plik na podstawie którego będą generowane dane.
- output (file): Plik do którego zostanie przekazany wynik.
- initial_number_of_words (int): Początkowa wartość NUMBER_OF_WORDS.
- step (int): Krok o ile będzie zwiększana wartość NUMBER_OF_WORDS.
- number_of_problems (int): Ilość rozwiązywanych problemów.
- number_of_insances (int): Ilość instancji problemu.


**************************
Krótki opis implementacji:
**************************

Oczyszczacz tekstu:
===================
Algorytm:
---------

1. Podziel tekst na pojedyńcze słowa
2. Dla każdego słowa
    1. Usuń znaki które nie są literami z początku słowa
    2. Usuń znaki które nie są literami z końca słowa
3. Zwróć słowa składające się tylko ze znaków


Generator słów:
===============
Na podstawie danej listy słów generator zwraca z jednakowyn rozkładem
każde ze słów.


Funkcja mieszająca:
===================
Zastosowana została tak zwana "wielomianowa mieszająca funkcja krocząca" (ang. polynomial rolling hash function).
::

  hash(s) = s[0] + s[1] * p + s[2] * p^2 + ... + s[n-1] * p^(n-1) mod m

- s - napis dla którego chcemy wyliczyć wartość skrótu
- n - długość napisu s
- m - duża stała
- p - duża stała


Tablica mieszająca:
===================
Tablica mieszająca przechowuje napisy i ich hash'e w tablicy dwuwymiarowej.
Elementy kolidujące zapisywane są w lokalizacjach H[k,0], H[k,1], itd.
Przekroczenie przez drugi indeks rozmiaru tablicy odrzuca element.


****************
Moduły źródłowe:
****************
**hashtableanalysis/**

Pakiet główny. Zawiera 3 inne pakiety:

- **cleartext/**
Pakiet odpowiedzialny za oczyszczanie tekstów.

- **wordgenerator/**
Pakiet odpowiedzalny za generowanie słów.

- **hashtable/**
Pakiet odpowiedzalny za tablicę mieszającej, funckję mieszającą oraz główny program CLI.


***************************
Wykorzystywane technologie:
***************************
* Język - python3
* Biblioteki - pandas, click, sphinx, pytest


*********************
Informacje dodatkowe:
*********************

Zachowanie w warunkach brzegowych:
==================================
* Jeżeli dodanie napisu nie jest możliwe ponieważ tablica jest pełna rzucany jest odpowiedni wyjątek.
* Jeżeli usunięcie napisu nie jest możliwe ponieważ danego napisu nie ma rzucany jesty odpowiedni wyjątek.
