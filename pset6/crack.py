import crypt, time
import sys
from string import ascii_lowercase, ascii_uppercase

start_time = time.time()
argc = len(sys.argv)

if argc>1 and len(sys.argv[1])>1:
    salt = sys.argv[1][0] + sys.argv[1][1]
    
    for c1 in ["", *ascii_lowercase, *ascii_uppercase]:
        for c2 in ["", *ascii_lowercase, *ascii_uppercase]:
            for c3 in ["", *ascii_lowercase, *ascii_uppercase]:
                for c4 in [*ascii_lowercase, *ascii_uppercase]:
                    if crypt.crypt(c1+c2+c3+c4, salt) == sys.argv[1]:
                        print(c1+c2+c3+c4)
                        print("{}ms".format((time.time()-start_time)*1000))
                        quit()
                        