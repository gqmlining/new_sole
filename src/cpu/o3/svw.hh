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
    int depCheckShift = 2;
    class svwItem{
        public:
            bool VAILD;
            SVWTag_t TAG;
            SVWStoreSeqNum_t SSN;
            //uint8_t bitEnable;
            //uint8_t* data;
        svwItem(bool valid,SVWTag_t tag,SVWStoreSeqNum_t ssn)
              //  uint8_t bit_enable, uint8_t* data)
        :VAILD(valid),TAG(tag),SSN(ssn)/*,bitEnable(bit_enable)*/{
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
          (sz,list<svwItem> (assoc,svwItem(0,0,0/*,0,NULL*/)));
    }


    void insert(SVWKey_t key, SVWTag_t tag, SVWStoreSeqNum_t ssn
               /* uint8_t bit_enable, uint8_t* data*/){
        svwItems[key].pop_back();
        svwItems[key].push_front(svwItem(true,tag,ssn/*,bit_enable,data*/));
    }

    void insert(DynInstPtr &inst){
      if (inst->phtEffAddrLow == 0)
        return;
      auto inst_eff_addr1 = inst->phyEffAddrLow >> depCheckShift;
      auto inst_eff_addr2 =
        (inst->phyEffAddrLow + inst->effSize - 1) >> depCheckShift;
      /** from the front to end, the store is youngest to oldest. */
      for (auto addr = inst_eff_addr1; addr <= inst_eff_addr2; addr++){
        SVWKey_t key = addr % size;
        SVWTag_t tag = addr / size;
        insert(key,tag,inst->SSN/*,inst->effSize,inst->memData*/);
      }
    }

    SVWStoreSeqNum_t search(SVWKey_t key,SVWTag_t tag){
        for (auto i:svwItems[key]){
            if (i.VAILD && i.TAG == tag){
                return i.ssn;
        }
        return svwItems[[key].front().ssn;
    }

    SVWStoreSeqNum_t getSSN(DynInstPtr& inst)
    {
        auto addr1 = inst->phyEffAddrLow >> depCheckShift;
        SVWKey_t key = addr % size;
        SVWTag_t tag = addr / size;
        return search(key, tag);
    }

    bool violation(DynInstPtr &inst/*, uint8_t** memData*/){
      if (inst->phyEffAddrLow == 0)
        return false;
      auto inst_eff_addr1 = inst->phyEffAddrLow >> depCheckShift;
      auto inst_eff_addr2 =
        (inst->ptyEffAddrLow + inst->effSize - 1) >> depCheckShift;
      for (auto addr = inst_eff_addr1; addr <= inst_eff_addr2; addr++){
        SVWKey_t key = addr % size;
        SVWTag_t tag = addr / size;
        SVWStoreSeqNum_t ssn = search(key, tag);
        /** judge if ssn equals. */
        if (ssn != inst->SSN){
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
