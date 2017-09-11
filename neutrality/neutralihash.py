import sys

def neutralihash(name):

    if len(name) < 100:
        kreplacements = dict()
        kreplacements['00'] = '0'
        kreplacements['01'] = '1'
        kreplacements['10'] = 'c'
        kreplacements['11'] = 'n'
        
        xreplacements = dict()
        xreplacements['00'] = '0'
        xreplacements['01'] = '1'
        xreplacements['11'] = 'r'
    
        kpart = ""
        kname = (bin(int(name[:40],16))[2:].zfill(170))[10:]
        for i in range(80):
            kpart += kreplacements[kname[2*i:2*(i+1)]]
            
        xpart = ""
        xname = (bin(int(name[41:81],16))[2:].zfill(170))[10:]
        for i in range(80):
            xpart += xreplacements[xname[2*i:2*(i+1)]]

        return kpart + "_" + xpart + name[81:]
    
    else:
        replacements = dict()
        replacements['0'] = "00"
        replacements['1'] = "01"
        replacements['c'] = "10"
        replacements['n'] = "11"
        replacements['r'] = "11"
    
        kpart = ""
        for i in range(80):
            kpart += replacements[name[i]]
        kpart = (hex(int(kpart,2))[2:].zfill(50))[9:49]

        xpart = ""
        for i in range(80):
            xpart += replacements[name[81+i]]
        xpart = (hex(int(xpart,2))[2:].zfill(50))[9:49]

        return kpart + "_" + xpart + name[161:]

if __name__ == "__main__":
    print neutralihash(sys.argv[1])
