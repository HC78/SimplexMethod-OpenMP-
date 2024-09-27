import coptpy as copt

mps_file_path = r"C:\Users\SEOW HUI CHEE\source\repos\SimplexMethod(OpenMP)\SimplexMethod(OpenMP)\problem.mps"

try:
    # Create an environment and model
    env = copt.Envr()
    model = env.createModel()

    # Read the MPS file into the model
    model.read(mps_file_path)

    # Solve the model
    model.solve()

    # Check if an optimal solution was found
    if model.status == copt.COPT.OPTIMAL:
        print("Optimal Solution (non-zero variables):")
        for var in model.getVars():
            value = var.getInfo('value')
            if value != 0:  # Only print variables with non-zero values
                print(f"{var.name} = {value}")
        
        print(f"Maximum Z = {model.objval}")
    else:
        print("No optimal solution found or model did not solve successfully.")

except copt.CoptError as ce:
    print(f"CoptError: {ce}")
    print("Ensure COPT is installed correctly and that the environment variables are set.")

except Exception as e:
    print(f"An error occurred: {e}")
