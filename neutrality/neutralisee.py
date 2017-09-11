# ----------------------------------------------------------------------------
# "THE NO-WARE LICENSE" (Revision -0):
# L\'eo Barr\'e wrote this file.  As long as you retain this notice you
# can do whatever you want with this stuff.
# ...That's it.
# ----------------------------------------------------------------------------
import sys, math
from getopt import getopt
from neutralihash import neutralihash
        

bound = -1
doubloncheck = False
X2bounds = dict();
X2bounds[ "10%"] =  2.706
X2bounds[  "5%"] =  3.841
X2bounds["2.5%"] =  5.024
X2bounds[  "1%"] =  6.635
X2bounds["0.1%"] = 10.828

def usage(state):
    if not state:
        print("PYTHON " + sys.argv[0] + " [OPTIONS] FILE")
        print("")
        print(u" -h       : displays this help")
        print(u" -s       : gives number of spare keys")
        print(u" -b BOUND : displays only \u03c7\u00b2 higher than BOUND")
        print(u"     - 10% : 2.706  \u2502  2.5% : 5.024  \u2502  0.1% : 10.828")
        print(u"     -  5% : 3.841  \u2502    1% : 6.635  \u2502")
    else:
        print("Option not recognized. Type '-h' to display help")
    print("")
    sys.exit(state)

optlist, args = getopt(sys.argv[1:], 'b:sh')
for opt in optlist:
    if opt[0] == '-b':
        if opt[1][-1] == '%':
            bound = X2bounds[opt[1]]
        else:
            bound = float(opt[1])
    elif opt[0] == '-s':
        doubloncheck = True
    elif opt[0] == '-h':
        usage(0)
    else:
        usage(1)

name = args[0]
if len(name) > 100:
    name = neutralihash(name)
f = open(name,'r')
# First line : r n t
line = f.readline().split(" ")
rsize = int(line[0])+1

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

hcount = [[0.0 for i in range(ntr_l)] for i in range(rsize)]
in_result_part = False
iterator = 0
ecount = 0
ecount0 = 0
usedks = dict()
usedkshere = []
alreadyusedks = 0

f.readline()

for line in f:
    if line[0] == '#':
        if in_result_part:
            iterator = 0
            ecount += ecount0
            ecount0 = 0
            if doubloncheck:
                for k in usedkshere:
                    if k in usedks:
                        alreadyusedks += 1
                    usedks[k] = 1
                usedkshere = []
        in_result_part = not in_result_part
    elif in_result_part:
        splitted = line.split(" ")
        for i in range(ntr_l):
            hcount[iterator][i] += float(splitted[i])
        iterator += 1
    else:
        if line[0] == '-':
            ecount0 = -1
            usedkshere = []
        elif doubloncheck:
            try:
                usedkshere.append(line.split(" ")[5])
            except IndexError:
                usedkshere.append(line.split(" ")[0])
        ecount0 +=1

printsize1 = max(int(math.log10(float(ecount)))+5,8);
lastchi2 = -1

# First line ###################################################################
tmp_string = u'       \u250c'
for j in range(ntr_l):
    tmp_string += u'\u252c\u2500' + printsize1*u'\u2500' + u'\u2500'
print tmp_string + u'\u2510'
# Second line : ivs ############################################################
tmp_string = u' \u250c\u2500\u2500\u2500\u2500\u2500\u2518'
for j in range(ntr_l):
    tmp_string += u'\u2502' + (printsize1+2-4)/2*' ' + 'iv{0:02}'.format(ntr[j]+1) + (printsize1+2-4)/2*' '
print tmp_string + u'\u2502'
# Third line : line ############################################################
tmp_string = u' \u251c\u2500\u2500\u2500\u2500\u2500\u2500'
for j in range(ntr_l):
    tmp_string += u'\u253c\u2500' + printsize1*u'\u2500' + u'\u2500'
print tmp_string + u'\u2524'
# Other lines ##################################################################
for i in range(rsize):
    chi2 = list(map((lambda x: 2*(x-ecount/2)*(x-ecount/2)/(ecount/2)), hcount[i]))
    if reduce((lambda x,y : x or y > bound),  [False] + chi2):
        lastchi2 = i
#### Moyenne ###################################################################
        tmp_string = ""
        for j in range(ntr_l):
            tmp_string += u'{0:{1}.3f} \u2502 '.format(100*hcount[i][j]/(ecount),printsize1)
        print u' \u2502 {0:4} \u2502 '.format(i) + tmp_string
#### Chi Square ################################################################
        tmp_string = ""
        for j in range(ntr_l):
            tmp_string += u'{0:{1}.3f} \u2502 '.format(chi2[j],printsize1)
        print u' \u2502 {0:4} \u2502 '.format(' ') + tmp_string
#### Line ######################################################################
        tmp_string = u' \u251c\u2500\u2500\u2500\u2500\u2500\u2500'
        for j in range(ntr_l):
            tmp_string += u'\u253c\u2500' + printsize1*u'\u2500' + u'\u2500'
        print tmp_string + u'\u2524'
    elif lastchi2 == i-1:
#### Dots ######################################################################
        tmp_string = u' \u250a      '
        for j in range(ntr_l):
            tmp_string += u'\u250a ' + printsize1*' ' + ' '
        print tmp_string + u'\u250a\n' + tmp_string + u'\u250a'
#### Line ######################################################################
        tmp_string = u' \u251c\u2500\u2500\u2500\u2500\u2500\u2500'
        for j in range(ntr_l):
            tmp_string += u'\u253c\u2500' + printsize1*u'\u2500' + u'\u2500'
        print tmp_string + u'\u2524'
# Last lines####################################################################
tmp_string = u' \u2514\u252c\u2500\u2500\u2500\u2500\u2510'
for j in range(ntr_l):
    tmp_string += u'\u2502' + (printsize1+2-4)/2*' ' + 'iv{0:02}'.format(ntr[j]+1) + (printsize1+2-4)/2*' '
print tmp_string + u'\u2502'

tmp_string = u'  \u2502    \u2514'
for j in range(ntr_l):
    tmp_string += u'\u2534\u2500' + printsize1*u'\u2500' + u'\u2500'
print tmp_string + u'\u2518'
print u'  \u251c' + (int(math.log10(ecount))+12)*u'\u2500' + u'\u2510'
print u'  \u2502 on {0} tests \u2502'.format(ecount)
if doubloncheck:
    print u'  \u2502   {0:{1}} spares  \u2502'.format(alreadyusedks,int(math.log10(ecount)))
    print u'  \u2502 {0:>{1}}%   \u2502'.format(u'\u27fe ' + str(round(float(alreadyusedks)/float(ecount)*100*100)/100),int(math.log10(ecount)+7))
print u'  \u2514' + (int(math.log10(ecount))+12)*u'\u2500' + u'\u2518'
