from multiprocessing import Pool
import os

def rungem5(name):
    path = "/home/lining/gem5/preacle/gem5/"
#    os.system('%sbuild/RISCV/gem5.opt %sconfigs/example/se_ln.py --cpu-type=DerivO3CPU --caches --benchmark=%s|grep "need_check" > trace/%s.trace'%(path,path,name,name))

    #os.system('%sbuild/RISCV/gem5.opt --stats-file=%sout_5y/%s.out %sconfigs/example/se_ln.py --maxinsts=500000000 --cpu-type=DerivO3CPU --caches --l2cache --l3cache --benchmark=%s|grep -aE "xxxxx" > out_5y/%s_5y.log '%(path,path,name,path,name,name))

    #os.system('%sbuild/RISCV/gem5.opt --stats-file=%swarm_out/%s_128_2.out %sconfigs/example/se_ln.py --standard-switch=1 --warmup-insts=100000000 --maxinsts=200000000 --cpu-type=DerivO3CPU --caches --l2cache --l3cache --benchmark=%s|grep -aE "reexecuteTime|reduce time" > warm_log/%s_128_2.log '%(path,path,name,path,name,name))

    os.system('%sbuild/RISCV/gem5.opt --stats-file=%sadd_out/%s_final.out %sconfigs/example/se_ln.py --maxinsts=500000000 --cpu-type=DerivO3CPU --caches --l2cache --l3cache --benchmark=%s|grep -aE "reexecuteTime" > log_5y/%s_final.log '%(path,path,name,path,name,name))

if __name__ == '__main__':
    names = ["perlbench","bzip2","gcc","mcf","gobmk","hmmer","sjeng","libquantum","h264ref","omnetpp","astar","xalancbmk"]
#    names = ["bzip2","gcc","gobmk","omnetpp","xalancbmk","sjeng"]
#    names = ["bzip2","gcc","sjeng"]
    pool = Pool(processes=len(names))
    for name in names:
        pool.apply_async(rungem5,(name,))
    pool.close()
    pool.join()
#    rungem5(names[1])
