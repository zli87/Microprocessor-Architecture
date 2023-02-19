import os

#os.system("some_command < input_file | another_command > output_file")
#mv stats.* {new name}

print("[info] script start")

# 3.1 Perfect

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=128 --lsq=64 --iq=32 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] perfALL.config1 finish")
os.system("mv stats.* Reportstats/perfALL.config1") 
os.system("mv phase.* Reportphase/perfALL.config1") 


os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=256 --lsq=128 --iq=64 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] perfALL.config2 finish")
os.system("mv stats.* Reportstats/perfALL.config2") 
os.system("mv phase.* Reportphase/perfALL.config2") 


os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=512 --lsq=256 --iq=128 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] perfALL.config3 finish")
os.system("mv stats.* Reportstats/perfALL.config3") 
os.system("mv phase.* Reportphase/perfALL.config3") 


os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=1024 --lsq=512 --iq=256 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] perfALL.config4 finish")
os.system("mv stats.* Reportstats/perfALL.config4") 
os.system("mv phase.* Reportphase/perfALL.config4") 

# 3.2 Real D Cache

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=1,0,1,1 -t --fq=64 --cp=64 --al=128 --lsq=64 --iq=32 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] RealDS.config1 finish")
os.system("mv stats.* Reportstats/realDS.config1") 
os.system("mv phase.* Reportphase/realDS.config1") 


os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=1,0,1,1 -t --fq=64 --cp=64 --al=256 --lsq=128 --iq=64 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] RealDS.config2 finish")
os.system("mv stats.* Reportstats/realDS.config2") 
os.system("mv phase.* Reportphase/realDS.config2") 


os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=1,0,1,1 -t --fq=64 --cp=64 --al=512 --lsq=256 --iq=128 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] RealDS.config3 finish")
os.system("mv stats.* Reportstats/realDS.config3") 
os.system("mv phase.* Reportphase/realDS.config3") 


os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=1,0,1,1 -t --fq=64 --cp=64 --al=1024 --lsq=512 --iq=256 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] RealDS.config4 finish")
os.system("mv stats.* Reportstats/realDS.config4") 
os.system("mv phase.* Reportphase/realDS.config4") 

# 3.3 Real BP

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=0,1,1,1 -t --fq=64 --cp=64 --al=128 --lsq=64 --iq=32 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] RealDS.config1 finish")
os.system("mv stats.* Reportstats/realBP.config1") 
os.system("mv phase.* Reportphase/realBP.config1") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=0,1,1,1 -t --fq=64 --cp=64 --al=256 --lsq=128 --iq=64 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] RealDS.config2 finish")
os.system("mv stats.* Reportstats/realBP.config2") 
os.system("mv phase.* Reportphase/realBP.config2") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=0,1,1,1 -t --fq=64 --cp=64 --al=512 --lsq=256 --iq=128 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] RealDS.config3 finish")
os.system("mv stats.* Reportstats/realBP.config3") 
os.system("mv phase.* Reportphase/realBP.config3") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=0,1,1,1 -t --fq=64 --cp=64 --al=1024 --lsq=512 --iq=256 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] RealDS.config4 finish")
os.system("mv stats.* Reportstats/realBP.config4") 
os.system("mv phase.* Reportphase/realBP.config4") 

# 3.4 no T Cache

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=1,1,1,0 --fq=64 --cp=64 --al=128 --lsq=64 --iq=32 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] noTS.config1 finish")
os.system("mv stats.* Reportstats/noTS.config1") 
os.system("mv phase.* Reportphase/noTS.config1") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=1,1,1,0 --fq=64 --cp=64 --al=256 --lsq=128 --iq=64 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] noTS.config2 finish")
os.system("mv stats.* Reportstats/noTS.config2") 
os.system("mv phase.* Reportphase/noTS.config2") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=1,1,1,0 --fq=64 --cp=64 --al=512 --lsq=256 --iq=128 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] noTS.config3 finish")
os.system("mv stats.* Reportstats/noTS.config3") 
os.system("mv phase.* Reportphase/noTS.config3") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=1,0,0 --perf=1,1,1,0 --fq=64 --cp=64 --al=1024 --lsq=512 --iq=256 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] noTS.config4 finish")
os.system("mv stats.* Reportstats/noTS.config4") 
os.system("mv phase.* Reportphase/noTS.config4") 

# 3.5.1 real-disambig-nonspec

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=0,0,0 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=128 --lsq=64 --iq=32 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] rdn.config1 finish")
os.system("mv stats.* Reportstats/rdn.config1") 
os.system("mv phase.* Reportphase/rdn.config1") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=0,0,0 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=256 --lsq=128 --iq=64 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] rdn.config2 finish")
os.system("mv stats.* Reportstats/rdn.config2") 
os.system("mv phase.* Reportphase/rdn.config2") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=0,0,0 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=512 --lsq=256 --iq=128 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] rdn.config3 finish")
os.system("mv stats.* Reportstats/rdn.config3") 
os.system("mv phase.* Reportphase/rdn.config3") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=0,0,0 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=1024 --lsq=512 --iq=256 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] rdn.config4 finish")
os.system("mv stats.* Reportstats/rdn.config4") 
os.system("mv phase.* Reportphase/rdn.config4") 

# 3.5.2 real-disambig-spec

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=0,1,0 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=128 --lsq=64 --iq=32 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] rds.config1 finish")
os.system("mv stats.* Reportstats/rds.config1") 
os.system("mv phase.* Reportphase/rds.config1") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=0,1,0 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=256 --lsq=128 --iq=64 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] rds.config2 finish")
os.system("mv stats.* Reportstats/rds.config2") 
os.system("mv phase.* Reportphase/rds.config2") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=0,1,0 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=512 --lsq=256 --iq=128 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] rds.config3 finish")
os.system("mv stats.* Reportstats/rds.config3") 
os.system("mv phase.* Reportphase/rds.config3") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=0,1,0 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=1024 --lsq=512 --iq=256 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] rds.config4 finish")
os.system("mv stats.* Reportstats/rds.config4") 
os.system("mv phase.* Reportphase/rds.config4") 

# 3.5.3 real-disambig-spec-constr

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=0,1,1 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=128 --lsq=64 --iq=32 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] rdsc.config1 finish")
os.system("mv stats.* Reportstats/rdsc.config1") 
os.system("mv phase.* Reportphase/rdsc.config1") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=0,1,1 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=256 --lsq=128 --iq=64 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] rdsc.config2 finish")
os.system("mv stats.* Reportstats/rdsc.config2") 
os.system("mv phase.* Reportphase/rdsc.config2") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=0,1,1 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=512 --lsq=256 --iq=128 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] rdsc.config3 finish")
os.system("mv stats.* Reportstats/rdsc.config3") 
os.system("mv phase.* Reportphase/rdsc.config3") 

os.system("make cleanrun SIM_FLAGS_EXTRA='-e100000000 --disambig=0,1,1 --perf=1,1,1,1 -t --fq=64 --cp=64 --al=1024 --lsq=512 --iq=256 --iqnp=4 --fw=16 --dw=16 --iw=16 --rw=16 --lane=0xffff:0xffff:0xffff:0xffff:0xffff:0xffff:0xffff --lat=1:1:1:1:1:1:1 -b'") 
print("[info] rdsc.config4 finish")
os.system("mv stats.* Reportstats/rdsc.config4") 
os.system("mv phase.* Reportphase/rdsc.config4") 



'''
os.system("echo > stats.2") 
os.system("mv stats.* Report/test2") 
os.system("echo > stats.3") 
os.system("mv stats.* Report/test3") 
os.system("echo > stats.4") 
os.system("mv stats.* Report/test4") 
'''
