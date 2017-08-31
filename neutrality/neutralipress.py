# ----------------------------------------------------------------------------
# "THE NO-WARE LICENSE" (Revision -0):
# L\'eo Barr\'e wrote this file.  As long as you retain this notice you
# can do whatever you want with this stuff.
# ...That's it.
# ----------------------------------------------------------------------------
import sys

name = sys.argv[1]
f = open(name,'r')

# First line : r n t
line = f.readline().split(" ")
rsize = int(line[0])

# Second line : IV model
iv_model = f.readline()[0:80]
ntr = []
ntr_l = 0
for i in range(80):
    if iv_model[i] == 'n':
        ntr.append(i)
        ntr_l += 1

#Third line : Key model
key_model = f.readline()[0:80]

keys = []
hcount = [[0 for i in range(ntr_l)] for i in range(rsize+1)]
in_result_part = False
iterator = 0
ecount = 0
ecount0 = 0
usedkshere = []

f.readline()

for line in f:
    if line[0] == '#':
        if in_result_part:
            iterator = 0
            ecount += ecount0
            ecount0 = 0
            keys += usedkshere
            usedkshere = []
        in_result_part = not in_result_part
    elif in_result_part:
        splitted = line.split(" ")
        for i in range(ntr_l):
            hcount[iterator][i] += int(splitted[i])
        iterator += 1
    else:
        if line[0] == '-':
            ecount0 = -1
            usedkshere = []
        else:
            usedkshere.append(line)
        ecount0 +=1
        
f.close()
f = open(name,'w')

f.write(str(rsize) + " rounds\n")
f.write(str(iv_model) + "\n")
f.write(str(key_model) + "\n")
f.write("#\n")
for k in keys:
    f.write(k)
f.write("#\n")
for h in hcount:
    f.write(" ".join(map(str,h)) + "\n")
f.write("#\n")
