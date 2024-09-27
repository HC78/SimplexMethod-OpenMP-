from cylp import PyCplex

mps_file_path = r"C:\Users\SEOW HUI CHEE\source\repos\SimplexMethod(OpenMP)\SimplexMethod(OpenMP)\problem.mps"

try:
    # Create a CPLEX solver instance
    solver = PyCplex.Cplex()
    
    # Read the MPS file into the solver
    solver.read(mps_file_path)

    # Solve the model
    solver.solve()

    # Check if an optimal solution was found
    if solver.solution.get_status() == solver.solution.status.optimal:
        print("Optimal Solution (non-zero variables):")
        for var in solver.variables.get_names():
            value = solver.solution.get_values(var)
            if value != 0:  # Only print variables with non-zero values
                print(f"{var} = {value}")
        
        print(f"Maximum Z = {solver.solution.get_objective_value()}")
    else:
        print("No optimal solution found or model did not solve successfully.")

except Exception as e:
    print(f"An error occurred: {e}")
