import cs50

# Asks for user's input and validate results. Height should be positive and no greater than 23.
while True:
    print("Height:", end="")
    a = cs50.get_int()
    if a >= 0 and a <= 23:
        break
# Iterate over a (height) to create rows and prints blocks and spaces as needed.    
for i in range(a):
    print(" " * (a - i - 1), end="")
    print("#" * (i + 2), end="")
    print()
    

    
    