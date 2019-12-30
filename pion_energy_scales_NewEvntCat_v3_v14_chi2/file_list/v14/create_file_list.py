import sys, os

#energy = int(sys.argv[1])
energy = sys.argv[1]
print energy
# sys.exit(0)

energy_list = {
    "20" : [679,680,681,682,683,684,685,686,687,688,689],
    "50" : [690,691,692,693,694,695,696],
    "80" : range(525,540),
    "100" : range(552,563),
    "120" : range(584,594),
    "200" : range(563,578),
    "250" : range(541,552),
    "300" : range(512,524),
    "420" : [698,699,700,701,702,704,705,708,709,710,712,713,714,715,717,718,721,722]
}


# list_20 = [679,680,681,682,683,684,685,686,687,688,689] 
# list_50 = [690,691,692,693,694,695,696]
# list_80 = range(525,540) #[525,526,527,528,529,530,]
# list_100 = range(552,563) 
# list_120 = range(584,594)
# list_250 = range(541,552)  
# list_300 = range(512,524)  

#muon_list_200 = [698,699,700,701,702,704,705,708,709,710,712,713,714,715,717,718,719,721,722]
muon_list_200 = [698,699,700,701,702,704,705,708,709,710,712,713,714,715,717,718,721,722]
destination_path = "/afs/cern.ch/work/s/spandey/public/hgcal/2018_TB/OctoberTB/hgcal_ahcal_samples/v3_v14/pion%s"%(energy)
count = 0
file_name = "pion%s_config1.txt"%(energy)

if(int(energy) == 420):
    destination_path = "/afs/cern.ch/work/s/spandey/public/hgcal/2018_TB/OctoberTB/hgcal_ahcal_samples/v3_v14/muon200"
    file_name = "muon200_config1.txt"
file_ = open(file_name,'w')
for sample in energy_list[energy]:
    line = destination_path+"/fulltree_"+str(sample)+".root"
    print line
    file_.write(line+"\n")
    # print cmd
    # os.system(cmd)
    count += 1

file_.close()
print "file:%s Written"%(file_name)
