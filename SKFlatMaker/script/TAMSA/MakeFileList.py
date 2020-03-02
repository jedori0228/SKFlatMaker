import os,subprocess

Year = "2018"

masses = open('mass.txt').readlines()
#masses = open('mass_Full.txt').readlines()

for mass in masses:

  mass = mass.strip('\n')

  if Year=="2016":
    #### Check dir number
    #out_timestamp = subprocess.check_output('ls -1d /gv0/Users/jskim/MiniAOD//2016Run_WR_Dilep/'+mass+'/CMSSW_9_4_6_patch1__MINIAOD/*',shell=True)
    #if len( out_timestamp.split('\n') ) != 2:
    #  print mass
    #out_outdir = subprocess.check_output('ls -1d /gv0/Users/jskim/MiniAOD//2016Run_WR_Dilep/'+mass+'/CMSSW_9_4_6_patch1__MINIAOD/*/*',shell=True)
    #if len( out_outdir.split('\n') ) != 2:
    #  print mass

    cmd = 'ls -1 /gv0/Users/jskim/MiniAOD//2016Run_WR_Dilep/'+mass+'/CMSSW_9_4_6_patch1__MINIAOD/*/*/*.root &> filelist/'+Year+'/'+mass+'.txt'

  elif Year=="2017":

    cmd = 'ls -1 /gv0/Users/jskim/MiniAOD//2017Run_WR_Dilep/'+mass+'/*.root &> filelist/'+Year+'/'+mass+'.txt'

  elif Year=="2018":

    cmd = 'ls -1 /data9/Users/jskim/MiniAOD/2018Run_WR_Dilep/'+mass+'/CMSSW_10_2_11__MINIAOD/*.root &> filelist/'+Year+'/'+mass+'.txt'

  os.system(cmd)
