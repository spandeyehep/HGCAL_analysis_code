import os, sys

list_20 = [679,680,681,682,683,684,685,686,687,688,689]
list_50 = [690,691,692,693,694,695,696]
list_80 = range(525,540) #[525,526,527,528,529,530,]                                                                                        
list_100 = range(552,563)
list_120 = range(584,594)
list_250 = range(541,552)
list_300 = range(512,524)

outFile = open("pion250_config1.txt","w")
for sample in list_250:
    line = "/afs/cern.ch/work/s/spandey/public/hgcal/2018_TB/OctoberTB/hgcal_ahcal_samples/pion250/fulltree_"+str(sample)+".root\n"
    outFile.write(line)
    print line

outFile.close()
