import highspy.highs as highs

mps_file_path = r"C:\Users\SEOW HUI CHEE\source\repos\SimplexMethod(OpenMP)\SimplexMethod(OpenMP)\problem.mps"

try:
    # Create a HiGHS model
    model = highs.Highs()

    # Read the MPS file
    status = model.readModel(mps_file_path)
    
    if status != highs.HighsStatus.kOk:
        raise Exception("Failed to read the MPS file.")

    # Solve the model
    solve_status = model.run()

    if solve_status == highs.HighsStatus.kOk:
        print("Optimal Solution (non-zero variables):")
        solution = model.getSolution()

        # Loop through and print non-zero variables
        for idx, value in enumerate(solution.col_value):
            if value != 0:
                var_name = f"x{idx + 1}"  
                print(f"{var_name} = {value}")

        # Print objective value
        print(f"Maximum Z = {model.getObjectiveValue()}")

    else:
        print("No optimal solution found or model did not solve successfully.")

except Exception as e:
    print(f"An error occurred: {e}")
