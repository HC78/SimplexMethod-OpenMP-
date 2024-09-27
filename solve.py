from docplex.mp.model_reader import ModelReader
import cplex

mps_file_path = r"C:\Users\SEOW HUI CHEE\source\repos\SimplexMethod(OpenMP)\SimplexMethod(OpenMP)\problem.mps"

try:
    # Create a ModelReader instance
    reader = ModelReader()

    # Read the MPS file into a model
    model = reader.read_model(mps_file_path)

    # Solve the model
    model.solve()

    # Output the results
    print("Optimal Solution:")
    for var in model.iter_variables():
        print(f"{var.name} = {var.solution_value}")

    print(f"Maximum Z = {model.objective_value}")

except ImportError as ie:
    print(f"ImportError: {ie}")
    print("Ensure CPLEX is installed and environment variables are set correctly.")

except Exception as e:
    print(f"An error occurred: {e}")