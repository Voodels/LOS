# Function to print a centered pyramid of stars
def print_pyramid(rows):
    # Calculate the maximum width of the pyramid base
    # For 'rows' = 5, width = 9 (because 2*5 - 1 = 9 stars at the base)
    max_width = 2 * rows - 1

    # Loop through each row from 1 to 'rows' (inclusive)
    for i in range(1, rows + 1):
        # For each row, calculate:
        
        # 1. Number of spaces needed to center the stars
        #    Formula: (max_width - number of stars in this row) // 2
        #    Example:
        #      - For i=1: (9-1)//2 = 4 spaces
        #      - For i=2: (9-3)//2 = 3 spaces, etc.
        spaces = " " * ((max_width - (2*i - 1)) // 2)

        # 2. Number of stars needed
        #    Formula: (2*i - 1)
        #    - For row 1: 1 star
        #    - For row 2: 3 stars
        #    - For row 3: 5 stars, etc.
        stars = "*" * (2*i - 1)

        # 3. Print the line by joining spaces + stars
        print(spaces + stars)

# ---------------------------------------------------

# Main part of the program

# Ask user for the number of rows
rows = int(input("Enter number of rows: "))

# Call the function to print the pyramid
print_pyramid(rows)
