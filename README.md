![image](https://github.com/user-attachments/assets/2923423d-609c-4157-9f08-722137d13330)<b>Introduction</b> <br/>
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
-- Select Pivot Column: Choose the column with the most negative coefficient in the objective row.<br/>
-- Select Pivot Row: Choose the row with the minimum ratio of the right-hand side value to the pivot column value.<br/>
-- Pivoting: Perform row operations to make the pivot element 1 and other pivot column row be 0.<br/>
-- Iteration: Repeat until no negative coefficients remain in the objective row (optimal solution). <br/>

![image](https://github.com/user-attachments/assets/d4bd9650-0aef-419c-8fb2-f3f4f31b6092) <br/><br/>
![image](https://github.com/user-attachments/assets/747a4b9d-9bc2-4457-a7ff-4ee8c504a783) <br/><br/>
![image](https://github.com/user-attachments/assets/08b79b3b-5096-496a-a51e-5f3db296142f)

This is a small real life problem with 3 variabes and 3 constraints, the solution would be quickly figuring out. Now it come to a problem, the execution time would be slower and slower when there is a increasing number of variables and constraints. <br/>
<br/>
<b>Parallel Computing (OpenMP)</b> <br/>
In this project, I want to parallise the simplex method process using OpenMP. The part that I parallelise is pivot column selection, pivot row selection and pivoting. <br/>
Then I compare the performance gain against serial with parallel and plot the graphs below. 
![image](https://github.com/user-attachments/assets/54b10d5c-af30-4eb6-8bff-a89b2bbcb88b)
![image](https://github.com/user-attachments/assets/878b7c72-e995-432b-8c2a-771957bf2988)
![image](https://github.com/user-attachments/assets/8104e6d6-7642-4646-9da6-7540f39b9178)
![image](https://github.com/user-attachments/assets/6350f0d8-eb20-4159-b029-49bdca0cab57)
![image](https://github.com/user-attachments/assets/73a38874-e00d-4564-8ee5-05685898800f)
![image](https://github.com/user-attachments/assets/d33e51aa-c29b-482c-bd41-53a3761c58e4)
![image](https://github.com/user-attachments/assets/ce80ff2c-eb73-4294-be45-e5a707b0dc81)
![image](https://github.com/user-attachments/assets/016674cc-2f20-43d3-bb4c-af704b867590)

<b>Verification</b>
Steps: 
1. Install HiSpy Library Using Command pip install highspy 
2. Use Python code to verify  (It load the linear programming problem from the MPS file then solve the problem using the HiGHS solver)
3. Comparing the solution provided by the HiGHS solver with the results from the C++ code
**Except for HiGHS, can use COPT, CYLP, CPLEX and so on**

![image](https://github.com/user-attachments/assets/5c9f6b49-e002-4d6a-a095-df5cc6799d1e)
![image](https://github.com/user-attachments/assets/ef2fc301-1462-4a17-ad68-97a3ac8fc361)
![image](https://github.com/user-attachments/assets/8817d8bc-e288-47f1-a370-097cb3e22282)
![image](https://github.com/user-attachments/assets/6129a210-f60b-435b-b697-4cae4dc39169)

<br/>when run in serial, well known  solver cant but my result can produce faster bcs run in parallel->contribution in this project, run in parallel perforamcne can upperformance, 
how to product MPS file so that can 
can show code, put link to notebook(ex: colab-> remote pc where can test), provide guidance, can run in colab, no need install anything, any notebook+show how run program for cplex, highspy, can go in find anymore, (try ....),
performance of single thread, if run multiple thread can see performance gain



