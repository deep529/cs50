import nltk
from termcolor import colored

# define a list of +ve & -ve words
positiveWordList = []
negativeWordList = []

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        # open file containing +ve words
        file = open("positive-words.txt", "r")
        
        # add all words by iterating over file & skipping comments & striping newline endings
        for line in file:
            if line[0]==';' or line[0]==' ' or line[0]=='\n':
                continue
            else:
                positiveWordList.append(line.strip('\n'))
        file.close()
    
        # same thing with -ve words
        file = open("negative-words.txt", "r")
        
        for line in file:
            if line[0]==';' or line[0]==' ' or line[0]=='\n':
                continue
            else:
                negativeWordList.append(line.strip('\n'))
        file.close()
        

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        tokens = nltk.word_tokenize(text)

        score = 0
        lenp = len(positiveWordList)
        lenn = len(negativeWordList)

        for target in tokens:
            # first check in positive
            
            start = 0
            end = lenp-1
            while end>=start:
                i = int((start + end)/2)
                
                flag = compare(target, positiveWordList[i])
                
                if flag == 1:
                    start = i + 1
                elif flag == -1:
                    end = i - 1
                else:
                    break
                
            # if found increment score and choose next word
            if positiveWordList[i] == target:
                score = score + 1
                continue
            
            # check in negative
            start = 0
            end = lenn-1
            while end>=start:
                i = int((start + end)/2)
                
                flag = compare(target, negativeWordList[i])
            
                if flag == 1:
                    start = i + 1
                elif flag == -1:
                    end = i - 1
                else:
                    break
                
            # if found decrement score and choose next word
            if negativeWordList[i] == target:
                score = score - 1
                continue

            # not found in any, don't do anything 
        
        return score
        
# tells the relative position of string 'target' wrt string 's'
# ie. returns 1 if target has larger index, -1 if lesser, 0 if s==target
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
    
    # if target is substring of s then search on left
    if l1 < l2:
        return -1
    # else s is substring of target then search on right    
    else:
        return 1    