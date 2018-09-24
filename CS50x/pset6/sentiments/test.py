# file = open("char.txt", "r")
import nltk
setlist =  set()

# for line in file:
#     if not line.startswith(";"):
#         print(line)
#     #print(line.strip(), end = "")
    
arquivo = 'char.txt'


filepos = open(arquivo,"r")
for line in filepos:
    if not line.startswith(";") and not line.startswith("."):
        setlist.add(line.strip())
    
print()        
for item in setlist:
    print(item)
print()

# tokenizer = nltk.tokenize.TweetTokenizer()
# tokens = tokenizer.tokenize(text)
# for item in tokens:
#     print(item)