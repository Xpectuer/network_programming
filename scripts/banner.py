path='./1.txt'
with open(path) as f:
    for l in f:
        s=f.readline()
        print(s,end='#')
        
