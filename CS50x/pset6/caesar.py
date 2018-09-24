import sys
import cs50

# Allow two arguments only, program will exit otherwise
if len(sys.argv) != 2:
    # change error message
    print("Please provide one argument only")
    exit(1)

# Use the the second argument as key
k = int(sys.argv[1])

# Print instructions on screen, ask for input and calculate the length of it
print("plaintext: ", end="")
ptxt = cs50.get_string()
n = len(ptxt)
print("ciphertext: ", end="")

# Iterate over each char
for c in ptxt:

# If the current char is in lowercase, rotate by k positions. Alpha is used to set a alphabetical order starting from zero
    if str.islower(c):
        alpha = ord(c) - 97
        print(chr((alpha + k) % 26 + 97), end="")

# If the current char is in uppercase, find its numeric position on alphabet, rotate by k positions and print it. Modulo is used so Z becomes A.
    if str.isupper(c):
        print(chr((ord(c) - 65 + k ) % 26 + 65), end="")

# If the i'th char is not a letter print it whithout unchanged.
    if not str.isalpha(c):
        print(c, end="")

# Print new line (layout)
print()
