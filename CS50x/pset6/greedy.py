import cs50

q = 25
d = 10
n = 5
p = 1

sq = 0
sd = 0
sn = 0
sp = 0

while True:
    print("O hai! How much change is owed?")
    o = cs50.get_float()
    if o > 0:
        break

x = o * 100;
c = round(x)

if c >= q:
    sq = c // q;
    c = c % q
    
if c >= d:
    sd = c // d
    c = c % d
    
if c >= n:
    sn = c / n
    c = c % n 
    
if c >= p:
    sp = c / p
    
t = sq + sd + sn + sp
print(int(t))

