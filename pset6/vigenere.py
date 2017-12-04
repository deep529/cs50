import sys

argc = len(sys.argv)

if(argc!=2):
    print("Enter key in Command line argument")
else:
    key = str(sys.argv[1])
    
    s = input("plaintext:  ")
    
    j = 0
    keyLen = len(key)
    
    print("ciphertext: ", end="")
    
    for i in range(len(s)):
        c = s[i]
        j = j%keyLen
        
        if(c>="A" and c<="Z"):
            c = chr(ord(c) + (ord(key[j])-65)%26) 
            
            if(c>"Z"):
                c = chr(ord(c)-26)
            
            j = j + 1
        
        elif(c>="a" and c<="z"):
            c = chr(ord(c) + (ord(key[j])-65)%26) 
            
            if(c>"z"):
                c = chr(ord(c)-26)
            
            j = j + 1
        
        print(c, end="")
    print("")    