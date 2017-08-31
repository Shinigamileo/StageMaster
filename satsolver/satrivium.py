# ----------------------------------------------------------------------------
# "THE NO-WARE LICENSE" (Revision -0):
# Léo Barré wrote this file.  As long as you retain this notice you
# can do whatever you want with this stuff.
# ...That's it.
# ----------------------------------------------------------------------------
import sys,getopt
from CNFmaker import *

################################################################

if sys.argv[1] == "-r":
    
    report = open(sys.argv[2],'r')
    
    values = []
    
    for line in report:
        if (line[0] == "s" and line[2] == "U"):
            sys.exit(1)
        if (line[0] == "v"):
            variables = list(map(int,filter(lambda x : x is not '', line[1:len(line)-1].split())))
            for x in variables:
                if x != 0:
                    values.append((x+abs(x))/(2*x))

    print("".join(list(map(str,values[:80]))))
    sys.exit(0)

################################################################

else:

    opts = 'xw:'
    optlist,args = getopt.getopt(sys.argv[1:],opts)
    
    do_xlauses = False
    warmup = 0

    for o,a in optlist:
        if o == '-x':
            do_xlauses = True
        elif o == '-w':
            warmup = int(a)

    ivstream = []

    for i in range(len(args)/2):
        assert len(args[2*i])==80, "Wrong sized IV"
        ivstream.append((list(map(int,args[2*i])),list(map(int,args[2*i+1])))) 
    
    f = DIMACS("cnf",do_xlauses=do_xlauses)
    
    K = []
    for i in range(80):
        K.append(f.Var("k{}".format(i)))

    index = -1
    for iv,stream in ivstream:    

        l = len(stream)
        index += 1
        
        f.Comment("STREAM NUMBER {}".format(index))

        f.Comment("    Building registers")
        
        
        A = []
        for i in range(-93,-80):
            A.append( f.Var("A{}[{}]".format(index,i)) )
        for i in range(-80,0):
            A.append(K[80+i])
        for i in range(0,l-66):
            A.append( f.Var("A{}[{}]".format(index,i)) )
        for i in range(93-80):
            f.False(A[i])
                
        B = []
        for i in range(-84,l-69):
            B.append( f.Var("B{}[{}]".format(index,i)) )
        for i in range(84-80):
            f.False(B[i])
        for i in range(80):
            if iv[i]:
                f.True(B[84-80+i])
            else:
                f.False(B[84-80+i])

        C = []
        for i in range(-111,l-66):
            C.append( f.Var("C{}[{}]".format(index,i)) )
        for i in range(111-108):
            f.True(C[i])
        for i in range(111-108,111):
            f.False(C[i])

        f.Comment("    Propagation")

        for r in range(l):
            t1 = f.Var("t1.{}_{}".format(index,r))
            f.Xor([A[ 93+r-66], A[r],-t1])
            t2 = f.Var("t2.{}_{}".format(index,r))
            f.Xor([B[ 84+r-69], B[r],-t2])
            t3 = f.Var("t3.{}_{}".format(index,r))
            f.Xor([C[111+r-66], C[r],-t3])
            if r >= warmup:
                f.Xor([ t1, t2, t3 * (-1)**(1-stream[r]) ])
            # we don't need the last 66/69 bits of internal state
            # since they won't be used for the keystream
            if r < l-69:
                a1 = f.Var("And",[A[ 93+r- 92],A[ 93+r- 91]])
                f.Xor([-B[ 84+r], t1, a1, B[ 84+r-78]])
            if r < l-66:
                a2 = f.Var("And",[B[ 84+r- 83],B[ 84+r- 82]])
                f.Xor([-C[111+r], t2, a2, C[111+r-87]])
                a3 = f.Var("And",[C[111+r-110],C[111+r-109]])
                f.Xor([-A[ 93+r], t3, a3, A[ 93+r-69]])
            

        if 0:
            # Differenciation
            f.Or([A[93-80+i]*(-1)**(key[i]) for i in range(80)])
    
    print(f)

        
