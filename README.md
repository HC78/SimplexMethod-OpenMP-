![image](https://github.com/user-attachments/assets/95fa3645-f354-4f0c-96b7-4aa7d2b6255f)<b>Introduction</b> <br/>
Linear Programming optimizes an objective such as maximise profit or mininise cost under the constraints taken from linear equations or inequalities.<br/>
Simplex Method is an iterative algorithm that moves along the edges of the feasible region to find the optimal solution.<br/>
It efficiently starts from a corner point, improving the objective function at each step until reaching the maximum or minimum value.<br/>
![image](https://github.com/user-attachments/assets/d96565e9-c7e9-448a-b495-f8f73700c45a)<br/> Diagram 1: Simplex Method 

<b>Real Life Problem</b> <br/>
A factory produces three products: x1, x2 and x3​. Each product requires a certain amount of raw materials and labour to produce and the factory has limited resources. 
The objective is to maximise profit by determining how many units of each product to produce based on given the constraints of available materials and labour.
* Profit from Product 1 = $6 per unit
* Profit from Product 2 = $5 per unit
* Profit from Product 3 = $4 per unit <br/>
&#8594; objective function (profit) is Z=6x<sub>1</sub>+5x<sub>2</sub>+4x<sub>3​</sub><br/>

<ins>Constraints:</ins><br/>
Raw Material A: Each unit of Product 1, Product 2 and Product 3 requires different amounts of Material A. The factory has only 180 units of Material A available. <br/>
* Each unit of Product 1 requires 2 units of Material A.
* Each unit of Product 2 requires 1 unit of Material A.
* Each unit of Product 3 requires 1 unit of Material A. <br/>
&#8594; constraint is 2x<sub>1</sub>+ x<sub>2</sub> + x<sub>3​</sub>≤180<br/>

Labour: The factory has 300 labour hours available. <br/>
* Each unit of Product 1 requires 1 hour of labour.
* Each unit of Product 2 requires 3 hours of labour.
* Each unit of Product 3 requires 2 hours of labour. <br/>
&#8594; constraint is x<sub>1</sub>+ 3x<sub>2</sub> + 2x<sub>3​</sub>≤300<br/>

Raw Material B: Each unit of Product 1, Product 2, and Product 3 requires different amounts of Material B. The factory has 240 units of Material B available.<br/>
* Each unit of Product 1 requires 2 units of Material B.
* Each unit of Product 2 requires 1 unit of Material B.
* Each unit of Product 3 requires 2 units of Material B.<br/>
&#8594; constraint is 2x<sub>1</sub>+ 1x<sub>2</sub> + 2x<sub>3​</sub>≤240
<br/>
<i>How is the Simplex Method work?</i> <br/>
- Select Pivot Column: Choose the column with the most negative coefficient in the objective row.
- Select Pivot Row: Choose the row with the minimum ratio of the right-hand side value to the pivot column value.
- Pivoting: Perform row operations to make the pivot element 1 and other pivot column row be 0.
- Iteration: Repeat until no negative coefficients remain in the objective row (optimal solution). <br/>
![image](https://github.com/user-attachments/assets/0ff2cf19-af87-4638-b0c6-6b8054c8ee8a)
<br/>Diagram 2: Add slack variables to convert inequalities into equalities.<br/>
![image](https://github.com/user-attachments/assets/4888d5b5-4519-471a-9c9e-fb0c890e7525)
<br/>Diagram 3: Make pivot element to become 1 and others be 0<br/>
![image](https://github.com/user-attachments/assets/63ab4b3b-0a77-48bd-b3a1-1508217a2c61)
<br/>Diagram 4: x and y are basic variables (variable has a pivot element), z is non-basic variable(assign it to 0 in optimal solution)<br/>

<br/>This is a small real life problem with 3 variabes and 3 constraints, the solution would be quickly figuring out. Now it come to a problem, the execution time would be slower and slower when there is a increasing number of variables and constraints. <br/>
<br/>
<b>Parallel Computing (OpenMP)</b> <br/>
In this project, I want to parallise the simplex method process using OpenMP. The part that I parallelise is pivot column selection, pivot row selection and pivoting. <br/>
Then I compare the performance gain against serial with parallel and plot the graphs below. 
![image](https://github.com/user-attachments/assets/e8d71ceb-563d-44a9-a5de-212c3ebc7fe5)
![image](https://github.com/user-attachments/assets/d125618f-bdc7-481b-806a-333f5c4e0f16)
![image](https://github.com/user-attachments/assets/387c0f34-3a0c-44c3-a513-11b37e066446)
![image](https://github.com/user-attachments/assets/4d471e01-8443-4950-9539-89872d2df7cc)
![image](https://github.com/user-attachments/assets/da1c7967-b3b7-4d71-b8fe-1ae51aca090b)
![image](https://github.com/user-attachments/assets/3de840a6-dab8-4e8c-9805-18ba0dd389ea)
![image](https://github.com/user-attachments/assets/df05be0b-17c8-4c0a-ac6b-2c6bc08454b8)



<br/>when run in serial, well known  solver cant but my result can produce faster bcs run in parallel->contribution in this project, run in parallel perforamcne can upperformance, 
how to product MPS file so that can 
can show code, put link to notebook(ex: colab-> remote pc where can test), provide guidance, can run in colab, no need install anything, any notebook+show how run program for cplex, highspy, can go in find anymore, (try ....),
performance of single thread, if run multiple thread can see performance gain



