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

From figure 3.1 to figure 3.7, it shows that 16 threads is the appropriate number of threads for handling smaller data, the arrays of size up to 10000 x 10000. It is due to the overhead by creating and managing a larger number of threads. When the dataset is small, this overhead outweighs the benefits gained from parallelism because each thread ends up with very little work to handle or process. Hence, using too many threads can degrade performance rather than improve it. However for large problems it shows it gains more performance gain using 32 threads. The workload is big enough without being slowed down by the overhead of managing too many threads. Therefore, parallel computing handles the work more efficiently and leads to better performance as the number of threads increases.

![image](https://github.com/user-attachments/assets/016674cc-2f20-43d3-bb4c-af704b867590)

From figure 3.8, it shows that parallel computing becomes dominant at more than 20000*20000 size of data.  

<b>Verification</b> <br/>
Steps: 
1. Install HiSpy Library Using Command pip install highspy 
2. Use Python code to verify  (It load the linear programming problem from the MPS file then solve the problem using the HiGHS solver)
3. Comparing the solution provided by the HiGHS solver with the results from the C++ code <br/>
***Except for HiGHS, can use COPT, CYLP, CPLEX and so on** <br/>

![image](https://github.com/user-attachments/assets/5c9f6b49-e002-4d6a-a095-df5cc6799d1e)
![image](https://github.com/user-attachments/assets/ef2fc301-1462-4a17-ad68-97a3ac8fc361)
![image](https://github.com/user-attachments/assets/8817d8bc-e288-47f1-a370-097cb3e22282)

This is to make sure the random generated constraints will not lead to 2 cases : <br/>
- There are two or more constraints are parallel which does not have a clear intersection point that lead to no maximum result. <br/>

![image](https://github.com/user-attachments/assets/e91c0259-5219-42de-bd5f-8666f8bdc3be) 

- The constraints has no overlapping region that satisfies all the inequalities which does not have feasible solution.<br/>

![image](https://github.com/user-attachments/assets/0ba13ad4-595d-42ca-99b0-f27cdd3f55ce)

<br/>For the contribution in this project, when applied OMP parallel, my coding can produce result faster than the well known solver, run in parallel computing can have performance with appropriate number of threads, variables and constraints.

Note: The code can run in any notebook such as colab without needed to install anything on your laptop or computer. <br/>
20000*20000 or more verification couldn't verify using the MPS file because it needs more powerful distributed computing. <br/>
There is performance gain when run multiple thread.





