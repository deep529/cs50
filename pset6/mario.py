n = int(input("Enter height:"))

while n>23 or n<4:
    n = int(input("Enter number between 4 and 23:"))
    
spaces = n-1

for i in range(1,n+1):
    for j in range(0,spaces):
        print(" ", end="")
    
    for j in range(0,i):
        print("#", end="")
        
    print("  ", end="")    
    
    for j in range(0,i):
        print("#", end="")
    
    print("")
    spaces = spaces - 1