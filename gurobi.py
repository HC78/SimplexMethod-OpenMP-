import gurobipy as gp
from gurobipy import GRB

# Define the path to your MPS file
mps_file_path = r"C:\Users\SEOW HUI CHEE\source\repos\SimplexMethod(OpenMP)\SimplexMethod(OpenMP)\problem.mps"

try:
    # Create a new model by reading the MPS file
    model = gp.read(mps_file_path)

    # Solve the model
    model.optimize()

    # Check if an optimal solution was found
    if model.status == GRB.OPTIMAL:
        print("Optimal Solution (non-zero variables):")
        for var in model.getVars():
            value = var.x
            if value != 0:  # Only print variables with non-zero values
                print(f"{var.varName} = {value}")
        
        print(f"Maximum Z = {model.objVal}")
    else:
        print("No optimal solution found or model did not solve successfully.")

except gp.GurobiError as e:
    print(f"GurobiError: {e}")
    print("Ensure Gurobi is installed correctly and that the license is active.")

except Exception as e:
    print(f"An error occurred: {e}")
