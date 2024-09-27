<b>Introduction</b> <br/>
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
![image](https://github.com/user-attachments/assets/eb311eb6-694d-4c3d-843f-4a3038eb5125)
![image](https://github.com/user-attachments/assets/e964cf1f-6c61-463f-8d7a-12afd895800e)
![image](https://github.com/user-attachments/assets/dc80aae2-91d2-4e06-b744-27d298d6ea2e)
![image](https://github.com/user-attachments/assets/0bd520f8-dd57-416c-9a15-07d2ee9ba08b)

<br/>This is a small real life problem with 3 variabes and 3 constraints, the solution would be quickly figuring out. Now it come to a problem, the execution time would be slower and slower when there is a increasing number of variables and constraints. <br/>
<br/>
<b>Parallel Computing (OpenMP)</b> <br/>
In this project, I want to parallise the simplex method process using OpenMP. The part that I parallelise is pivot column selection, pivot row selection and pivoting. <br/>
Then I compare the performance gain against serial with parallel and plot the graphs below. 



<b>Verification</b>
<br/>when run in serial, well known  solver cant but my result can produce faster bcs run in parallel->contribution in this project, run in parallel perforamcne can upperformance, 
how to product MPS file so that can 
can show code, put link to notebook(ex: colab-> remote pc where can test), provide guidance, can run in colab, no need install anything, any notebook+show how run program for cplex, highspy, can go in find anymore, (try ....),
performance of single thread, if run multiple thread can see performance gain



