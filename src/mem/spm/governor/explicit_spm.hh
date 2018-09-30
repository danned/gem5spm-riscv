/* This class implements an SPM governor which maps the allocation
 * requests to SPMs. It first finds the highest number
 * of free contiguous slots on the SPM and then maps as many pages
 * as possible to that region and repeats this process until no page is free
 * and then the rest would go off chip.
 *
 * Authors: Daniel
 * */

#ifndef __EXPLICIT_SPM_HH__
#define __EXPLICIT_SPM_HH__

#include "mem/spm/governor/random_spm.hh"
#include "params/ExplicitSPM.hh"

using namespace std;

class ExplicitSPM: public RandomSPM {

  public:
        ExplicitSPM(const Params *p);
    virtual ~ExplicitSPM();
    virtual void init();

    virtual void addPMMU(PMMU *p);

    // SPM APIs
    virtual int allocate(GOVRequest *gov_request);
    virtual int deAllocate(GOVRequest *gov_request);

  protected:
        map<PMMU *, pair<int,int>> pmmu_to_coord;
    virtual int allocation_helper_on_free_pages(GOVRequest *gov_request,
                                                HostInfo *host_info) override;

};

#endif  /* __EXPLICIT_SPM_HH__ */
