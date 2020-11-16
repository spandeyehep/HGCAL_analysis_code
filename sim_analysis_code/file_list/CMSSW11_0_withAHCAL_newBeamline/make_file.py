import os,sys

energy = [20,50,80,100,120,200,250,300]
PL = sys.argv[1]

for energy_ in energy:
    cmd = "cat sim_pion_xx.txt | sed -e s/PHYSICS_LIST/%s/g | sed -e s/ENERGY/%s/ > %s/sim_pion_%d.txt"%(PL,str(energy_),PL,energy_)
    print cmd
    os.system(cmd)
