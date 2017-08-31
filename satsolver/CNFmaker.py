# ----------------------------------------------------------------------------
# "THE NO-WARE LICENSE" (Revision -0):
# Léo Barré wrote this file.  As long as you retain this notice you
# can do whatever you want with this stuff.
# ...That's it.
# ----------------------------------------------------------------------------
from math import log10,ceil

# The following should be a macro to
# simulate a "n choose k" pattern for
def nchoosek_macroform(n,k,iterators):
    __body__ = None

    if not k:
        __body__
    else:
        iterators = [-1 for i in range(k)]
        __k = 0
        while __k >= 0:
            iterators[__k] += 1
            while __k < k-1:
                __k += 1
                iterators[__k] = iterators[__k-1]+1
            __body__
            while __k >= 0 and iterators[__k] == n-k+__k:
                __k -= 1


####################
#                  #
#    TS18 CLASS    #
#                  #
####################

class logicVar(object):
    def __init__(self,formula,value,index):
        """create a logical variable to use in a logical formula, logically"""
        self._formula = formula
        self._value = value
        self._index = index

    def __repr__(self):
        return self._index

    def __str__(self):
        return str(self._value)

    def __neg__(self):
        return logicVar(self._formula,-self._value,self._index)

    def __mul__(self,x):
        return self._value * x

    def __lt__(self,x):
        return self._value < x._value
    
    def __gt__(self,x):
        return self._value > x._value

    def __le__(self,x):
        return self._value <= x._value

    def __ge__(self,x):
        return self._value >= x._value



    
####################
#                  #
#    TS18 CLASS    #
#                  #
####################
    
class TS18(logicVar):

    ##################
    # STATIC METHODS #
    ##################
    
    def And(f,s,xs):
        f.Implie(xs,[s])
        for x in xs:
            f.Implie([s],[x])

    def Nand(f,s,xs):
        f.Nand([s]+xs)
        for x in xs:
            f.Or([x,s])

    def Or(f,s,xs):
        f.Implie([s],xs)
        for x in xs:
            f.Implie([x],[s])

    def Nor(f,s,xs):
        f.Or([s]+xs)
        for x in xs:
            f.Nand([x,s])

    def Xor(f,s,xs):
        f.Xor([-s]+[x])
    
    Ops = {
        "And"     : And, 
        "Nand"    : Nand,
        "Or"      : Or,  
        "Nor"     : Nor, 
        "Xor"     : Xor, 
    }

    #################
    # CLASS METHODS #
    #################
    
    def __init__(self,formula,value,op,xs):
        if op not in TS18.Ops:
            raise ValueError('"' + op + '" operation not implemented yet or non existant')
        self._op = op
        self._xs = xs[:]
        self._xs.sort()
        super(TS18,self).__init__(formula,value,op + "(" + ",".join(list(map(repr,self._xs))) + ")")

    def link(self):
        TS18.Ops[self._op](self._formula,self,self._xs)

    def __iter__(self):
        return self._xs.__iter__()

    def __contains__(self,x):
        i = 0;
        while i < len(self._xs) and self._xs[i] < x._value :
            i+=1
        return i < len(self._xs) and self._xs[i] == x._value



######################
#                    #
#    DIMACS CLASS    #
#                    #
######################
    
class DIMACS:

    ##################
    # STATIC METHODS #
    ##################

    Formats = {
        "cnf" : None,
        #"sat" : None,
    }
    
    #################
    # CLASS METHODS #
    #################   
    
    _do_xlauses = False
    
    def __init__( self, form,
                  padding="  ",
                  vars_explained = True,
                  do_xlauses = False,
                  xorlimit = 6           ):

        if form not in DIMACS.Formats:
            raise ValueError('"' + form + '" format not implemented yet or non existant')
        self._format = form
        self._padding = padding
        self._vars_explained = vars_explained
        self._do_xlauses = do_xlauses
        self._xorlimit = xorlimit
        
        self._nbvars = 0
        self._nbclauses = 0
        self._vars = dict()
        self._clauses = []
        

    def __str__(self):
        intro = []
        if self._vars_explained:
            expl_form = "c {:>"+str(int(ceil(log10(self._nbvars+1)))) + "} -> {}"
            for v in self._vars:
                intro += [expl_form.format(self._vars[v],v)]
        intro.sort()
        intro += ["p " + self._format + " "
                  + str(self._nbvars) + " " + str(self._nbclauses)]
        return "\n".join(intro + self._clauses)
    
    
    def Var(self,p1=None,p2=None):
        # case 1 : .Var(symbol) => simple var
        if p2 == None:
            if p1 == None:
                p1 = str(self._nbvars+1)
            symbol = p1
            if not symbol in self._vars:
                v = logicVar(self,self._nbvars+1,symbol)
                self._nbvars += 1
                self._vars[symbol] = v
                return v
            return self._vars[symbol]
        # case 2 : .Var(op,xs) => Tseityn var
        op = p1
        xs = p2
        ts18 = TS18(self,self._nbvars+1,op,xs)
        if not repr(ts18) in self._vars:
            self._nbvars += 1
            self._vars[repr(ts18)] = ts18
            ts18.link()
            return ts18
        return self._vars[repr(ts18)]
        
    
    
    def Comment(self,comment):
        splitted_com = comment.split('\n')
        for com in splitted_com:
            self._clauses += ["c " + com]

            
    def True(self,x):
        """'x' is true"""
        self._nbclauses += 1
        self._clauses += [self._padding + str(x) + " 0"]

        
    def False(self,x):
        """'x' is false"""
        self._nbclauses += 1
        self._clauses += [self._padding + str(-x) + " 0"]

        
    def Or(self,xs):
        """one of the 'xs' is true"""
        self._nbclauses += 1
        self._clauses += [self._padding]
        for x in xs:
            self._clauses[-1] += str(x) + " "
        self._clauses[-1] += "0"

        
    def Nand(self,xs):
        """one of the 'xs' is false"""
        self._nbclauses += 1
        self._clauses += [self._padding]
        for x in xs:
            self._clauses[-1] += str(-x) + " "
        self._clauses[-1] += "0"

        
    def Implie(self,xs,ys):
        """if all 'xs' are true, then one of the 'ys' is so"""
        self._nbclauses += 1
        self._clauses += [self._padding]
        for x in xs:
            #print xs
            self._clauses[-1] += str(-x) + " "
        for y in ys:
            self._clauses[-1] += str(y) + " "
        self._clauses[-1] += "0"

    def Equal(self,xs):
        """all 'xs' have the same value (either true or false)"""
        l = len(xs)
        for i in range(l-1):
            for j in range(i+1,l):
                self.Xor([-xs[i],xs[j]])

    def __cnfXor(self,xs):
        for k in range(2**(len(xs))):
            if bin(k).count('1')%2 == 0:
                # C'EST D\'EGUEULASSE
                self.Or([xs[i]*(-1)**((k//(2**i))%2) for i in range(len(xs))])
        
    
    def Xor(self,xs):
        if self._do_xlauses:
            self._nbclauses += 1
            self._clauses += [self._padding + "x"]
            for x in xs:
                self._clauses[-1] += str(x) + " "
            self._clauses[-1] += "0"
        else:
            xs1 = xs
            while(len(xs1) > self._xorlimit):
                xs2 = [xs1[(self._xorlimit-1)*i:(self._xorlimit-1)*(i+1)]
                       for i in range(len(xs1)//self._xorlimit+1)]
                xs1 = []
                for x2 in xs2:
                    if (len(x2) > 1):
                        xxor = self.Var("(" + " ^ ".join(list(map(repr,x2))) + ")")
                        self.__cnfXor([-xxor]+x2)
                        xs1 += [xxor]
                    else:
                        xs1 += x2
            self.__cnfXor(xs1)



##############
#            #
#    TEST    #
#            #
##############

if __name__=='__main__':
    f = DIMACS("cnf")
    x1 = f.Var('x1')
    x2 = f.Var('x2')
    x3 = f.Var()
    x4 = f.Var("And",[x1,x2])
    x5 = f.Var("Or",[x2,x3,-x4])
    print(f)
    
