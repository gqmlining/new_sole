import m5
from m5.objects import *
m5.util.addToPath('../common')

binary_dir = '/home/lining/CPU2006/'
data_dir = '/home/lining/CPU2006/'
output_dir = '/home/lining/CPU2006/'
prefix = '/run/run_base_test_riscv.0001/'
#====================
#400.perlbench
perlbench = Process()
perlbench.executable =  binary_dir+'400.perlbench/exe/perlbench_base.riscv'
data=data_dir+'400.perlbench/data/test/input/makerand.pl'
perlbench.cwd = output_dir+'400.perlbench'+prefix
perlbench.cmd = [perlbench.executable] + [data]
perlbench.output = output_dir+'400.perlbench/makerand.out'
perlbench.errout = output_dir+'400.perlbench/makerand.err'
#====================
#401.bzip2
bzip2 = Process()
bzip2.executable =  binary_dir+'401.bzip2/exe/bzip2_base.riscv'
data=data_dir+'401.bzip2/data/all/input/input.program'
bzip2.cmd = [bzip2.executable] + [data, '1']
bzip2.cwd = output_dir+'401.bzip2'+prefix
bzip2.output = output_dir+'401.bzip2/input.program.out'
bzip2.errout = output_dir+'401.bzip2/input.program.err'

#====================
#403.gcc
gcc = Process()
gcc.executable =  binary_dir+'403.gcc/exe/gcc_base.riscv'
data=data_dir+'403.gcc/data/test/input/cccp.i'
gcc_output = output_dir+"403.gcc/cccp.o"
gcc.cwd = output_dir+'403.gcc'+prefix
gcc.cmd = [gcc.executable] + [data]+['-o',gcc_output]
gcc.output = output_dir+'403.gcc/cccp.out'
gcc.errout = output_dir+'403.gcc/cccp.err'
#====================
#429.mcf
mcf = Process()
mcf.executable =  binary_dir+'429.mcf/exe/mcf_base.riscv'
data=data_dir+'429.mcf/data/test/input/inp.in'
mcf.cwd = output_dir+'429.mcf'+prefix
mcf.cmd = [mcf.executable] + [data]
mcf.output = output_dir+'429.mcf/inp.out'
mcf.errout = output_dir+'429.mcf/inp.err'
#====================

#445.gobmk
gobmk=Process()
gobmk.executable =  binary_dir+'445.gobmk/exe/gobmk_base.riscv'
stdin=data_dir+'445.gobmk/data/test/input/capture.tst'
gobmk.cwd = output_dir+'445.gobmk'+prefix
gobmk.cmd = [gobmk.executable]+['--quiet','--mode','gtp']
gobmk.input=stdin
gobmk.output = output_dir+'445.gobmk/capture.out'
gobmk.errout = output_dir+'445.gobmk/capture.err'
#====================
#456.hmmer
hmmer=Process()
hmmer.executable =  binary_dir+'456.hmmer/exe/hmmer_base.riscv'
data=data_dir+'456.hmmer/data/test/input/bombesin.hmm'
hmmer.cwd = output_dir+'456.hmmer'+prefix
print(hmmer.cwd)
hmmer.cmd = [hmmer.executable]+['--fixed', '0', '--mean', '325', '--num', '45000', '--sd', '200', '--seed', '0', data]
#hmmer.output = output_dir+'456.hmmer/bombesin.out'
#hmmer.errout = output_dir+'456.hmmer/bombesin.err'

#458.sjeng
sjeng=Process()
sjeng.executable =  binary_dir+'458.sjeng/exe/sjeng_base.riscv'
sjeng.cwd = output_dir+'458.sjeng'+prefix
data=data_dir+'458.sjeng/data/test/input/test.txt'
sjeng.cmd = [sjeng.executable]+[data]
sjeng.output = output_dir+'458.sjeng/test.out'
sjeng.errout = output_dir+'458.sjeng/test.err'


#462.libquantum
libquantum=Process()
libquantum.executable =  binary_dir+'462.libquantum/exe/libquantum_base.riscv'
libquantum.cwd = output_dir+'462.libquantum'+prefix
libquantum.cmd = [libquantum.executable],'33','5'
libquantum.output = output_dir+'462.libquantum/test.out'
libquantum.errout = output_dir+'462.libquantum/test.out'

#464.h264ref
h264ref=Process()
h264ref.cwd = output_dir+'464.h264ref'+prefix
data= output_dir+'464.h264ref/data/test/input/foreman_test_encoder_baseline.cfg'
h264ref.executable = binary_dir+ '464.h264ref/exe/h264ref_base.riscv'
h264ref.cmd = [h264ref.executable]+['-d',data]
h264ref.output = output_dir + '464.h264ref/foreman_test_encoder_baseline.out'
h264ref.errout = output_dir + '464.h264ref/foreman_test_encoder_baseline.err'

#471.omnetpp
omnetpp=Process()
omnetpp.executable =  binary_dir+'471.omnetpp/exe/omnetpp_base.riscv'
data=data_dir+'471.omnetpp/data/test/input/omnetpp.ini'
omnetpp.cwd = output_dir+'471.omnetpp'+prefix
omnetpp.cmd = [omnetpp.executable]+[data]
omnetpp.output = output_dir+'471.omnetpp/omnetpp.out'
omnetpp.errout = output_dir+'471.omnetpp/omnetpp.err'

#====================
#473.astar
astar=Process()
astar.executable =  binary_dir+'473.astar/exe/astar_base.riscv'
astar.cwd = output_dir+'473.astar'+prefix
astar.cmd = [astar.executable]+['lake.cfg']
astar.output = output_dir+'473.astar/lake.out'
#astar.errout = output_dir+'473.astar/lake.err'

#====================
#483.xalancbmk
xalancbmk=Process()
xalancbmk.executable =  binary_dir+'483.xalancbmk/exe/Xalan_base.riscv'
xalancbmk.cwd = output_dir+'483.xalancbmk'+prefix
xalancbmk.cmd = [xalancbmk.executable]+['-v','test.xml','xalanc.xsl']
xalancbmk.output = output_dir+'483.xalancbmk/test.out'
xalancbmk.errout = output_dir+'483.xalancbmk/test.err'

#998.specrand
#specrand_i=Process()
#specrand_i.executable = binary_dir+'998.specrand/exe/specrand_base.riscv'
#specrand.cwd = output_dir+'998.specrand'+prefix
#specrand_i.cmd = [specrand_i.executable] + ['324342','24239']
#specrand_i.output = output_dir+'998.specrand/rand.24239.out'
#specrand_i.errout = output_dir+'998.specrand/rand.24239.err'

