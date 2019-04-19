#ifndef __CPU_O3_SVW_HH__
#define __CPU_O3_SVW_HH__

#include <iostream>
#include <list>
#include <vector>

using namespace std;
typedef uint64_t SVWTag_t;
typedef uint64_t SVWStoreSeqNum_t;
typedef uint64_t SVWKey_t;

template <class Impl>
class SVW{
  public:
    typedef typename Impl::DynInstPtr DynInstPtr;
  public:
    int depCheckShift = 3;
    class svwItem{
        public:
            bool VAILD;
            SVWTag_t TAG;
            SVWStoreSeqNum_t SSN;
            uint8_t bitEnable;
            //uint8_t* data;
        svwItem(bool valid,SVWTag_t tag,SVWStoreSeqNum_t ssn,
                uint8_t bit_enable)//, uint8_t* data)
        :VAILD(valid),TAG(tag),SSN(ssn),bitEnable(bit_enable) {
           /* if (data == NULL)
                return;
            this->data = new uint8_t[bit_enable];
            memcpy(this->data, data, bit_enable);*/
        }
        ~svwItem() {
           // delete [] data;
        }
    };
    vector<list<svwItem> > svwItems;
    uint64_t size;
    uint64_t assoc;
public:
    SVW(size_t sz,size_t assoc):size(sz),assoc(assoc){
        svwItems = vector<list<svwItem>>
          (sz,list<svwItem> (assoc,svwItem(0,0,0,0/*,NULL*/)));
    }


    void insert(SVWKey_t key, SVWTag_t tag, SVWStoreSeqNum_t ssn,
                uint8_t bit_enable/*, uint8_t* data*/){
        svwItems[key].pop_back();
        svwItems[key].push_front(svwItem(true,tag,ssn,bit_enable/*,data*/));
    }

    void insert(DynInstPtr &inst){
      //uint64_t base = inst->readIntRegOperand(inst->staticInst.get(),0);
      //uint64_t offset = inst->staticInst->getOffset();
      //inst->effAddr = base + offset;
      if (inst->effAddr == 0)
      {
           uint64_t base = inst->readIntRegOperand(
                           inst->staticInst.get(),0);
           uint64_t offset = inst->staticInst->getOffset();
           inst->effAddr = base + offset;
           inst->bitEnable = 255;
      }
      auto inst_eff_addr1 = inst->effAddr >> depCheckShift;
      auto inst_eff_addr2 =
        (inst->effAddr + inst->effSize - 1) >> depCheckShift;
      /** from the front to end, the store is youngest to oldest. */
      for (auto addr = inst_eff_addr1; addr <= inst_eff_addr2; addr++){
        SVWKey_t key = addr % size;
        SVWTag_t tag = addr / size;
        insert(key,tag,inst->SSN,inst->bitEnable/*,inst->memData*/);
        std::cout << "svw update: addr is " << inst->effAddr << " index: " << key
                  << " tag: " << tag << " ssn: " << inst->SSN;inst->dump();
      }
    }

    void squashSVW(DynInstPtr &inst){
        auto addr = inst->effAddr >> depCheckShift;
        //std::cout << "getSSN: physical addr: " << inst->physEffAddrLow; inst->dump();
        SVWKey_t key = addr % size;
        for (auto &i:svwItems[key])
        {
            if (i.VAILD && i.SSN == inst->SSN)
            {
                i.VAILD = false;
            }
        }
    }

    SVWStoreSeqNum_t search(SVWKey_t key,SVWTag_t tag,uint8_t bitEnable){
       auto temp = svwItems[key].front();
       SVWStoreSeqNum_t res = temp.VAILD?temp.SSN:0;
       for (auto i:svwItems[key]){
            res = std::min(res, i.SSN);
            if (i.VAILD && i.TAG == tag)
            {
                uint8_t mask = 1;
                for (uint64_t t=0;t<8;t++)
                {
                    if (((mask & bitEnable) != 0) &&
                        ((mask & i.bitEnable) != 0))
                    {
                        return i.SSN;
                    }
                    mask = mask << 1;
                }
            }
        }
        return res;
    }

    SVWStoreSeqNum_t getSSN(DynInstPtr& inst) {
        auto addr = inst->effAddr >> depCheckShift;
        //std::cout << "getSSN: physical addr: " << inst->physEffAddrLow; inst->dump();
        SVWKey_t key = addr % size;
        SVWTag_t tag = addr / size;
        for (auto i:svwItems[key]){
            if (i.VAILD && i.TAG == tag)
            {
                bool bitFit = true;
                uint8_t mask = 1;
                for (uint64_t t=0;t<8;t++)
                {
                    if (((mask & inst->bitEnable) == 1) &&
                        ((mask & i.bitEnable) == 0))
                    {
                        bitFit = false;
                        break;
                    }
                    mask = mask << 1;
                }
                if (!bitFit)
                    continue;
                return i.SSN;
            }
        }
        if (svwItems[key].front().VAILD)
            return svwItems[key].front().SSN;
        else
            return 0;
        //return search(key, tag);
    }

    bool violation(DynInstPtr &inst/*, uint8_t** memData*/) {
      //std::cout << "debug: Enter svw filter!!\n";
      if (inst->effAddr == 0)
        return false;
      auto inst_eff_addr1 = inst->effAddr >> depCheckShift;
      //std::cout << "violation: physical addr: " << inst->physEffAddrLow; inst->dump();
      auto inst_eff_addr2 =
        (inst->effAddr + inst->effSize - 1) >> depCheckShift;
      //std::cout << "debug: addr1 is: " << inst_eff_addr1
        //        << " addr2 is: " << inst_eff_addr2 << std::endl;
      for (auto addr = inst_eff_addr1; addr <= inst_eff_addr2; addr++){
        SVWKey_t key = addr % size;
        SVWTag_t tag = addr / size;
        SVWStoreSeqNum_t ssn = search(key, tag, inst->bitEnable);
        std::cout << "svw violation: addr is: " << inst->effAddr << " index: " << key
                  << " tag: " << tag << " ssn is: " << ssn
                  << " forward ssn is: " << inst->forwardSSN;inst->dump();
        /** judge if ssn equals. */
        if (inst->isForward)
        {
            if (ssn != inst->forwardSSN){
            /** judge if the same address and data size, which used
            * to determind if still need reexecution from memory.
            */
            /*if (item.bitEnable != inst->effSize || item.TAG != inst->TAG) {
                return true;
              }
             else {
               *memData = new uint8_t[item.bitEnable];
               memcpy(*memData,item.data,item.bitEnable);
               return true;
              }*/
                 return true;
            }
        }
        else if (ssn > inst->forwardSSN)
             return true;
      }
      return false;
    }


    void printKey(SVWKey_t key){
        for (auto i:svwItems[key]){
            std::cout<<i.valid<<" "<<i.TAG<<" "<<i.SSN<<std::endl;
        }
        std::cout<<"---------------------------"<<std::endl;
    }
};
#endif //__CPU_O3_SVW_HH__
