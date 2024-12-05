import subprocess
import argparse

parser = argparse.ArgumentParser(description="Run figsearch with specified arguments.")
parser.add_argument('command', nargs='+', help="The command and its arguments (e.g., 'figsearch hline input')")

# Parse arguments
args = parser.parse_args()

# Run the command provided by the user (e.g., ["figsearch", "hline", "input"])
result = subprocess.run(args.command, capture_output=True, text=True)

# Process the output from the command (expecting "R1 C1 R2 C2")
coordinates = result.stdout.strip().split()  # Expecting output: "R1 C1 R2 C2"
R1, C1, R2, C2 = map(int, coordinates)  # Convert the coordinates to integers

# Open the grid file and read the data, skipping the first line
with open(args.command[2], "r") as file:
    # Skip the first line and read the rest
    next(file)  # This skips the first line
    data = [list(map(int, line.split())) for line in file]
    print(R1, C1, R2, C2)

# Iterate over rows in the data
for i, row in enumerate(data):
    for j, value in enumerate(row):
        # Check if we are in the range of the red line
        if R1 == R2:
            if i == R1 and C1 <= j <= C2 and value == 1:
                print("\033[31m██\033[0m", end="")  # Red pixel
            elif value == 1:
                print("\033[97m██\033[0m", end="")
            else:
                print("  ", end="")    # Black pixel (double space)
        elif C1 == C2:
            if R1 <= i <= R2 and j == C1 and value == 1:
                print("\033[31m██\033[0m", end="")  # Red pixel
            elif value == 1:
                print("\033[97m██\033[0m", end="")
            else:
                print("  ", end="")    # Black pixel (double space)
        else:
            if ((i == R1 or i == R2 or j == C1 or j == C2) and R1 <= i <= R2 and C1 <= j <= C2 and value == 1):
                print("\033[31m██\033[0m", end="")
            elif value == 1:
                print("\033[97m██\033[0m", end="")
            else:
                print("  ", end="")
    print()  # Newline at the end of each row
