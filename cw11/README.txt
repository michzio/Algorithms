Plik zawiera zmodyfikowany dotychczasowy program dla algorytmu Dijkstry, z poprawkami z programu Przystanki Komunikacji miejskiej (krawędzie umożliwiające konstrukcję grafu nieskierownego). 

Główne zmiany wprowadzono począwszy od komentarza /**** MIN SPANNING TREE ****/
Powyżej znajdują się jedynie deklaracje nowych klas: Kruskal, DisjointSet 

Kompilacja programu: 
g++ -std=c++11 -o kruskal min_spanning_tree.cpp

Uruchomienie programu: 
./kruskal graph1.txt

Przykładowe rozwiązanie z wikipedii testowanego grafu:
grapht1txt.gif 

Plik z danymi wejściowymi? 
Graf skierowany? -> algorytm Kruskala chyba nadaje się tylko do grafów nieskierowanych 
Znalazłem coś takiego: 
For directed graphs, the minimum spanning tree problem is called the Arborescence problem and can be solved in quadratic time using the Chu–Liu/Edmonds algorithm. 


Plik: min_spanning_tree.cpp 
Zawiera modyfikacje struktury zbiorów rozłącznych DisjointSet 
w taki sposób aby przyśpieszyć jej działanie. Wprowadzone zmiany: 
- Union by height - podczas sumy zbiorów zawsze podpinamy niższe drzewo do wyższego 
- Path Compression - spłaszczamy strukture drzewa podczas wyszukiwania podpinając
          	     napotkane wierzchołki bezpośrednio do korzenia (reprezentanta zbioru)
