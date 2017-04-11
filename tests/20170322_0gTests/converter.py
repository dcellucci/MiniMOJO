import glob
import os
files = glob.glob("./*.txt")
#print [a[2:-4] for a in files]

for fil in files:
    with open(fil,'r') as ifil:
        with open('./raw/'+fil[2:-4]+'.dat','w') as ofil:
            for line in ifil.readlines()[3:]:
                #print line.split(',')
                ofil.write(''.join([chr(int(a)) for a in line.split(",")[:-1]]))
