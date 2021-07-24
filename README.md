# Robot Sorting Algorithms

---

## Project overview
Project's aim was to create, develop, implemet and test sorting algorithms specially designed for solving color buckets sorting problem. <br>
During project's realization we have created 3 algorithms, which were further implemented and tested. <br>
Below one can see detailed project's description, as well as scrupulous algorithm's explanation.

---

## Problem statement (story time!)
Company, which hired us, produces paints in 4 colors:

- ![](https://via.placeholder.com/15/00FFFF/000000?text=+) `Cyan (C)`
- ![](https://via.placeholder.com/15/FF00FF/000000?text=+) `Magenta (M)`
- ![](https://via.placeholder.com/15/FFFF00/000000?text=+) `Yellow (Y)`
- ![](https://via.placeholder.com/15/618358/000000?text=+) `Khaki (K)`

We have to sort color buckets on production line. We can use a robot, which is capable of grabbing any 4 adjacent buckets and placing them at the end of the production line. <br>
**The result of robot's work, should be production line filled with buckets placed in repeating pattern CMYK.** <br>
Robot order consists of single integer number, representing the beginning of adjacent color buckects, which will be moved to the end of the string. <br><br>
*Example:* sequence `CCMYYKKC` with order `1` will result in `CKKCCMYY` (positions are numbered from 0).

Our goal is to develop algorithms, which will produce robot's orders list, which will result in sorted sequence. We know starting buckets sequence.

---

## Algorithms
We have come up with three ideas of possible algorithms and implemented them. Below we present detailed descriptions and analysis each of them. <br><br>
*Note: when perfroming algorithm execution time analysis, we always talk about pesimistic situation "also known as big O notation". Also, we assume that one robot move is an atomic operation with cost equal to 1 (which in reality is not even close to be true)*

--- 

### Brute force algorithm
#### Author: Bartosz Świrta
Brute force algorithm (as the name suggests) is comprehensive search through all possible solutions. Steps:

1. If sequence is sorted, go to point 5.
2. Generate all possible sequences accessible from current position. Positions should be sorted accordingly to instruction list length.
3. Fetch first  not visited state.
4. Go to point 1.
5. Return intstruction list associated with current state.

As one can easily see, brute force algorithm is technically speakig a [DFS algorithm](https://en.wikipedia.org/wiki/Depth-first_search). <br>
It's execution speed is directly connected with the amount of states possible to generate. <br>
Let's suppose that total length of sequence is equal to **N** and each color occurs **C,M,Y,K** times (for each color respectively). Amount of states, which are possible, is given by the following formula: <br><br>

<img src="https://render.githubusercontent.com/render/math?math=\binom{N}{C} \binom{N-C}{K} \binom{N-C-K}{Y}" width="300"> <br>

For now, let's suppose that each letter occurs exactly <img src="https://render.githubusercontent.com/render/math?math=\frac{N}{4}" width="15"> times. Then:<br><br>

<img src="https://render.githubusercontent.com/render/math?math=\binom{N}{0.25N} \binom{0.75N}{0.25N} \binom{0.5N}{0.25N}=\frac{N!}{(0.25N)! \cdot (0.75N)!} \cdot \frac{(0.75N)!}{(0.25N)! \cdot (0.5N)!} \cdot \frac{(0.5N)!}{(0.25N)! \cdot (0.25N)!}= \frac{N!}{(0.25N)! \cdot (0.25N)! \cdot (0.25N)! \cdot (0.25N)!}" width="1000"> <br>

It is rather difficult to put it in a more readable form. <br>
For ilustration, how slow this algorithm is, for *N=16* it will need (at worst case scenario) 63 063 000 moves. It makes him rather unusable for problems bigger than ~20. <br>
On the other hand, it is the only algorithm, which always finds optimal solution (least robot moves required) and always sorts whole production line. <br> 

---

### Universal algorithm
#### Author: Radosław Radziukiewicz
Universal sorting algorithm was designed to sort any input data, without any assumptions about is. Steps: <br>

Suppose that *l* means position of last element which is not sorted. At the beginning *l* equals to 0.

1. Find currently needed color (equal to *Pattern[l%4]* where *Pattern=[C, M, Y, K]*). If needed color is not present in unsorted part or unsorted part is smaller then 5, end execution.
2. Check, whether needed color is present at position *a*, such that <img src="https://render.githubusercontent.com/render/math?math=a_{}\in [l_{}, n_{}) \land a_{}\bmod4=0" width="175">, where *a* is mapped as follows: <br> [l, n) -> [0, n-l).
3. If it is present, go to point 7.
4. Among positions greater then *l*, find one at which needed color is present.
5. Move 4 colors, containing needed color to the end of the line.
6. Perform continous movements moving elements at positions n-5, n-4, n-3, n-2 to the end of the line, untill color is present at needed position *H* where *h%4=0*. It can be proven that this operation always succeedes.
7. Move redundant elements between positions *l* and *h* untill needed color is present at position *l*.
8. Increment *l* by 1 and go to step 1.

Above's algorithm has interesting execution time analysis. Step 1. requires O(n) operations (we need to look through N/4 positions). If we do not succeed in finding color, we need to find required color (time equal to O(n)) and place it at the end-fine position. It can be done (Steps 5 and 6) in O(1) time (it always needs no more then 10 moves). Finally, moving color to required positions needs O(n) time. All those actions are necessary for all elements, so will be done N times. Finally, we can state that overall complexity is equal to **O(n(n+1+n))=O(n^2)**.<br>
#### BUT!!!
This analysis is a bit wrong. In reality, this algorithm works a lot faster! <br>
Let's take a closer look at step 2 and suppose that each letter has occurrence probability equal to 25%. Chance, that at single scanned position color is not present, is equal to 75%. For 15 positions it is equal to around 1%. We can see, that in reality, we can sort a single color in a constant time! Color will rather always be present at required position at the very beginning. Finally, at many cases, algorithm will be closer to have **O(n)** time complexity. Experiments do confirm this observation.

---

### Substring algorithm
#### Author: Radosław Radziukiewicz
Key observation, which was the inspiration for Substring Sorting algorithm, was that: it is possible, that some buckets will be already sorted. Maybe we can take advantage of that fact? Indeed, this algorithm attempts to use it to his advantage. Steps: 

1. Find longest substring which begins with needed color (which is equal to *Pattern[l%4]*). If such substring is not present, end execution. If unsorted part is smaller than 8, run Universal Sort.
2. Move longest substrng to the end of the whole string in a way, that it's beginning is placed at position *a*, such that <img src="https://render.githubusercontent.com/render/math?math=a_{}\in [l_{}, n_{}) \land a_{}\bmod4=0" width="175"> where *a* is mapped as follows: <br> [l, n) -> [0, n-l).
3. Move longest sorted substring to position *l*.
4. Increase *l* by substring length and go to step 1.


Above's algorithm time complexity analysis is also interesting. Let's suppose, that the longest substring found has length equal to *K*. Step 1. requires O(n) time since we have to scan through the whole string. Moving longest substring to the end of the string requires time equal to O(K). Finally, cost of moving substring to the beginning of the sorted part is equal to O(N-K). Such actions will be performed for all substrings, so N/K times. Finally, we can state that overall time coplexity is equal to: <br>
**O((N/K) * (N+K+(N-K))=O(N^2/K)**
#### More analysis
Now, if we suppose that *K* is directly associated with *N* (let's say that it is always equal to *N/20*) time complexity can be reduced to O(N). In fact, such assumption is rather very, very difficult to fulfill. In reality, *K* will be rather not greater then some constant. Also, assumption about that sorting actions will be performed N/K times is also an oversimplification. Finally, described algorithm has time complexity equal to **O(n^2)** and performed experiments confirm that fact.
