def compare(target, s):
    if target==s:
        return 0
    
    l1 = len(target)
    l2 = len(s)
    
    if l1<l2:
        lmin = l1
    else:
        lmin = l2
    
    for i in range(lmin):
        if target[i] > s[i]:
            return 1
        elif target[i] < s[i]:
            return -1
     
    # executes if 1 string is substring of other    
    
    # if target is sub string of s then search on left
    if l1 < l2:
        return -1
    # else s is substring of target then search on right    
    else:
        return 1


if __name__ == "__main__":
    
    file = open("positive-words.txt", "r")
    positiveWordList = []
    for line in file:
        if line[0]==';' or line[0]==' ' or line[0]=='\n':
            continue
        else:
            positiveWordList.append(line.strip('\n'))
    
    file.close()    
    
    size = len(positiveWordList)
    
        
        start = 0
        end = size-1
        
        while end>=start:
            i = int((start + end)/2)
            
            flag = compare(target, positiveWordList[i])
            
            if flag == 1:
                start = i + 1
            elif flag == -1:
                end = i - 1
            else:
                break
        
                    
