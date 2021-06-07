Autorzy: Bartosz Świrta, Radosław Radziukiewicz

------------------------------------------------------------------------------------------
Specyfikacja problemu: 

Na półce są ułożone w przypadkowej kolejności pojemniki z atramentami w czterech kolorach: C, M, Y i K. 
Należy je uporządkować za pomocą robota w taki sposób, aby od lewej strony półki znalazły się kolejno uporządkowane czwórki CMYK. 
Jeśli liczby pojemników są nierówne, pojemniki nadmiarowe mogą pozostać nieuporządkowane, na prawo od wszystkich uporządkowanych czwórek.
Robot w jednym ruchu może pobrać jednocześnie dokładnie cztery sąsiednie pojemniki z dowolnego miejsca na półce i przenieść je na prawą stronę, 
za wszystkie pojemniki, a następnie dosunąć część pojemników w lewo, wypełniając lukę. Ułożyć plan pracy robota, dyktujący w kolejnych ruchach, 
którą czwórkę pojemników przenieść na prawą stronę. Wykonać jak najmniejszą liczbę ruchów. Porównać czas obliczeń i wyniki różnych metod.

------------------------------------------------------------------------------------------
Metody uruchomienia programu:

Opis flag:
    -gp     : generator parematryczny, pradopodobieństwo wylosowania każdej z liter jest takie samo i wynosi 0.25
    -gs     : generator podciągowy, generuje podciągi o długości zgodnej z rozkładem N(8,5) i pradopodobieństwiem, że wygenerowany podciag jest uporządkowany równym 0.6
    -gns    : generator podciągowy, generuje dane wejściowe w których nie występują posortowane podciągi
    -us     : universal_sort, algortym uniwersalny
    -ss     : substring_sort, algorytm podciągowy
    -bs     : brute_force_sort, algortym brutalny

1) ./aal_cmyk -i
Tryb interaktywny, udostepnia on możliwość podania właśnej instancji problemu i rozwiązanie jej przy użyciu wszystkich dostępnych algorytmów.
Następnie na standardowe wyjście wypisywane są kolejne kroki robota, wraz ze znacznikami, która czwórka zostanie przestawiona. Domyślnie dane są pobierane
ze standardowego wejścia i kierowane na standarddowe wyjście, ale możliwe jest ich przekierowanie w konsoli terminala. 
W trybie tym sprawdzana jest poprawność wprowadzonych danych.

2) ./aal_cmyk -s -gp/-gs/-gns <problem size> -us -ss -bs -f <result file>
Tryb generujący konkretną instancję problemu według zadanych parametrów i rozwiązujący ją określonymi algorytmami. 

Flagi -gp/-gs/-gns  : określają rodzaj generatora, można wywłać program tylko z jedną flagą na raz
<problem size>      : rozmiar problemu (liczba całkowita)
Flagi -us -ss -bs   : określają algorytmy, które będą użyte do rozwiązania problemu. Należy podac conajmniej jedną. 
-f <result file>    : gdzie ma zostać zapisany wynik działania. Może nie być wyspecyfikowany, program wtedy nie zapisze wyniku.

W trybie tym wyniki działania algorytmów są domyślnie wypisywane na standardowe wyjście niezależnie czy plik został wyspecyfikowany.
Wypisywane dane diagnostyczne zawierają:     
    - Nazwa algorytmu
    - Instancja problemu
    - Wynik dzialania algorytmu(stan danych po posortowaniu)
    - Rozmiar problemu
    - Liczba krokow
    - Czas dzialania algorytmu

3) ./aal_cmyk -t -gp/-gs/-gns <problem size> <step> <number of iterations> -us -ss -bs -f <output file>
Tryb pozwalający na testowanie algorytmu. Rozwiązuje on problem o danym rozmiarze wygenerowanym przez generator o zadynch parametrach
przy użyciu wyspecyfikowanych algorytmów. Następnie wykonuje zwiększenie rozmiaru problemy tyle razy ile iteracji podano w argumentach wywołania.
Dane zapisywane są w formie pliku cvs o nazwie podanej przez użytkownika (niepodanie pliku skutkuje niezapisaniem wyników).

Flagi -gp/-gs/-gns      : określają rodzaj generatora, można wywłać program tylko z jedną flagą na raz
<problem size>          : rozmiar problemu (liczba całkowita)
<step>                  : krok
<number of iterations>  : ilość iteracji (ile kroków zostanie wykonanych)
Flagi -us -ss -bs       : określają algorytmy, które będą użyte do rozwiązania problemu. Należy podac conajmniej jedną. 
-f <result file>        : gdzie ma zostać zapisany wynik działania. Program powiadamia użytkownika jeżeli plik nie został podany.

Wypisywane dane diagnostyczne zawierają: 
    - Rozmiar problemu
    - Liczba krokow
    - Czas dzialania algorytmu

------------------------------------------------------------------------------------------
Konwencja dotycząca danych wejściowych i prezentacji wyników:

Dane wejściowe, jeśli użytkownik wybierze opcję wymagającą ich podania, to ciąg  znaków C, M, Y, K w dowolnej kolejności.
Wyniki są zapisywane do plików .txt i/lub wyświetlane na ekranie.

------------------------------------------------------------------------------------------
Krótki opis metody rozwiązania, zastosowanych algorytmów i struktur danych:

1) Algorytm brutalny:
    Przeszukuje w sposób wyczerpujący przestrzeń stanów uprządkowania ciągu oraz wybiera tą ścieżkę, która doprowadzi do najszybszego uporządkowania zadanego ciągu.
    Wykorzystuje do tego algorytm BFS, kolejne stany generowane są na bierząco, wewnętrznie korzysta ze struyktury std::map do przechowywania przestrzeni przeszukiwań.

2) Algorytm uniwersalny:
    Ciąg będziemy sortowali poprzez odpowiednie ustawianie kolejnych pojemników na takich pozycjach, że dzięki ciągłemu wybieraniu elementów pośrednich, będziemy w stanie przenieść poszukiwany przez nas kolor na zadaną pozycję. 
    Element będzie “wsuwał” się na swoją pozycję od prawej strony, co będzie efektem uzupełniania luk pomiędzy pojemnikami. Wewnętrznie algorytm korzysta z std::vector do przechowywania i obróbki dostarczonych danych.

3) Algorytm podciagowy:
    Algorytm przenosi jak najdłuższe podciągi na koniec badanego ciągu. Finalnym krokiem jest przesunięcie podciągu na początek rozpatrywanego ciągu (tzn. Na początek posortowanej części).
    Wewnętrznie algorytm korzysta z std::vector do przechowywania i obróbki dostarczonych danych.

------------------------------------------------------------------------------------------
Przewodnik po plikach źródłowych:

1) Pliki .hpp
    alg_sort.hpp        : zawiera deklaracje zaimplementowanych algorytmów sortujacych
    constants.hpp       : zawiera definicję stałych używanych w programie
    data_generator.hpp  : zawiera deklarację hierarchi klas generatorów. Hierarchia składa się z jednego abstrakcyjnego generatora DataGenerator i dziedziczących po nim wyspecjalizowanych generatorów
    robot.hpp           : zawiera deklarację kalsy InstructionList, która jest wrapperem na std::vector. Dodatkowo deklaruję klasę robot, która jest symulatorem działania robota
    utils.hpp           : zawiera szablon użyty do obliczania czasu w ms, jaki upłynął między dwoma stemplami czasu std::chrono::time_point. Zawiera też deklarację różnych użytkowych funkcji takich jak: otwieranie pliku, wypisywanie danych do strumienia itp.

2) Pliki .cpp           
    aal.cpp             : plik wejściowy do programu, zawiera funkcję main(). Odpowiada za parsowanie argumentów wywołania na stosowne wywołania dalszej części programu. Implementuje interfejs użytkownika.
    alg_sort.cpp        : zawiera definicję funkcji zadeklarowanych w alg_sort.hpp
    data_generator.cpp  : zawiera definicję hierarchii klas zadeklarowanych w data_generator.hpp
    robot.cpp           : zawiera definicję klasy robot z robot.hpp i metod implementujących symulator robota
    utils.cpp           : zawiera definicję funkcji użytkowych zadeklarowanych w utils.hpp

-------------------------------------------------------------------------------------------
Inne informacje dodatkowe o szczególnych decyzjach projektowych (np. ograniczenia dotyczące 
rozmiaru problemu, charakterystyki generatorów danych testowych, specjalne konwencje w 
alokacji pamięci dynamicznej, wymagania dotyczące typów parametryzujących szablony, 
konwencje związane z obsługą sytuacji wyjątkowych, itp.):

Nie występuje jawna dynamiczna alokacja pamięci dla zbioru wejściowego ze wzgledu na wykorzystanie kontenerów STL.
Jedyna alokacja dynamiczna występujaca w programie to utworzenie generatora, nie wymagana ona żadnych specjalnych konwecnji.

Program nie obsługuje sytuacji wyjątkowych poprzez mechanizm wyjątków. W programie korzystamy z takich mechanizmów jak sprawdzanie rozmiarów, 
zwracanych kontenerów i std::optional.

Charakterystki generatorów:
    - generator parematryczny   : Pradopodobieństwo wylosowania każdej z liter jest zdawane oddzielnie i jest stałe przez cały proces generacji.
    - generator podciągowy      : Generuje podciągi o długości zgodnej z rozkładem normalnym i pewnym pradopodobieństwiem, że podciąg będzie posortowany. 
                                  Odpowiednie ustawienie parametrów umożliwia uzyskanie generatora, który będzie generował dane, w których posortowane podciągi będą występowały bardzo rzadko,
                                  w praktyce długość posortowanego podciągu nie przekroczy 3.
    - generator permutacyjny    : Generuje dane które są dozwoloną permutacją (tzn. wynikającą z poprawnych ruchów robota) posortowanego ciagu. Każdy wygenerowany przez niego ciąg daje się w pełni posortować.


