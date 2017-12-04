import sys

argc = len(sys.argv)

if argc!=2:
    print("Enter key in command line argument")
else:
    s = input("plaintext:  ")
    
    print("ciphertext: ", end="")
    for i in range(len(s)):
        c = ord(s[i])
        
        if(c>=65 and c<=90):
            c = c + int(sys.argv[1])%26
            if(c>90):
                c = c - 26
        elif(c>=97 and c<=122):
            c = c + int(sys.argv[1])%26
            if(c>122):
                c = c - 26
        
        print(chr(c), end="")
    
    print("")    