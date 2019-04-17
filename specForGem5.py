from multiprocessing import Pool
import os

def rungem5(name):
    path = "/home/lining/gem5/preacle/gem5/"
#    os.system('%sbuild/RISCV/gem5.opt %sconfigs/example/se_lgy.py --cpu-type=DerivO3CPU --caches --benchmark=%s|grep "need_check" > trace/%s.trace'%(path,path,name,name))

    os.system('%sbuild/RISCV/gem5.opt --stats-file=%s.out %sconfigs/example/se_lgy.py --maxinsts=100000000 --cpu-type=DerivO3CPU --caches --benchmark=%s|grep -aE "check:Addr|Commit inst" > spec_log/%s.log '%(path,name,path,name,name))


if __name__ == '__main__':
#    names = ["perlbench","bzip2","gcc","mcf","gobmk","hmmer","sjeng","libquantum","h264ref","omnetpp","astar","xalancbmk"]
    names = ["perlbench","bzip2","gcc","mcf","gobmk","hmmer","sjeng","libquantum","h264ref","omnetpp","astar","xalancbmk"]
#    names = ["bzip2","gcc","sjeng"]
    pool = Pool(processes=len(names))
    for name in names:
        pool.apply_async(rungem5,(name,))
    pool.close()
    pool.join()
#    rungem5(names[1])
