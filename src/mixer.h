#ifndef CYFCO_SRC_MIXER_H_
#define CYFCO_SRC_MIXER_H_

#include <string>
#include "cyclus.h"

namespace cyfco {

/// Mixer mixes N streams with fixed, static, user-specified
/// ratios into a single output stream. The Mixer has N input inventories:
/// one for each streams to be mixed, and one output stream. The supplying of
/// mixed material is constrained by available inventory of mixed material
/// quantities.
class Mixer : public cyclus::Facility {
#pragma cyclus note {   	  \
    "niche": "mixing facility",				  \
    "doc": "Mixer mixes N streams with fixed, static, user-specified" \
           " ratios into a single output stream. The Mixer has N input"\
           " inventories: one for each streams to be mixed, and one output"\
           " stream. The supplying of mixed material is constrained by "\
           " available inventory of mixed material quantities.", \
    }

  friend class MixerTest;

 public:
  Mixer(cyclus::Context* ctx);
  virtual ~Mixer(){};

  virtual void Tick();
  virtual void Tock();
  virtual void EnterNotify();

  virtual void AcceptMatlTrades(
      const std::vector<std::pair<cyclus::Trade<cyclus::Material>,
                                  cyclus::Material::Ptr> >& responses);

  virtual std::set<cyclus::RequestPortfolio<cyclus::Material>::Ptr>
  GetMatlRequests();

#pragma cyclus clone
#pragma cyclus initfromcopy
#pragma cyclus infiletodb
#pragma cyclus initfromdb
#pragma cyclus schema
#pragma cyclus annotations
#pragma cyclus snapshot
  // the following pragmas are ommitted and the functions are written
  // manually in order to handle the vector of resource buffers:
  //
  //     #pragma cyclus snapshotinv
  //     #pragma cyclus initinv

  virtual cyclus::Inventories SnapshotInv();
  virtual void InitInv(cyclus::Inventories& inv);

 protected:
#pragma cyclus var { \
    "alias": ["in_streams", [ "stream", [ "info", "mixing_ratio", "buf_size"], [ "commodities", "commodity", "pref"]]], \
    "uitype": ["oneormore", [ "pair", ["pair", "double", "double"], ["oneormore", "incommodity", "double"]]], \
    "uilabel": "", \
    "doc": "", \
  }
  std::vector<std::pair<std::pair<double, double>, std::map<std::string, double> > > streams_;

  std::vector<std::map<std::string, double> > in_commods;
  std::vector<double> in_buf_sizes;
  std::vector<double> mixing_ratios;

  // custom SnapshotInv and InitInv and EnterNotify are used to persist this
  // state var.
  std::map<std::string, cyclus::toolkit::ResBuf<cyclus::Material> > streambufs;


#pragma cyclus var {                                                 \
  "doc" : "Commodity on which to offer/supply mixed fuel material.", \
  "uilabel" : "Output Commodity", "uitype" : "outcommodity", }
  std::string out_commod;

#pragma cyclus var { \
    "doc" : "Maximum amount of mixed material that can be stored." \
            " If full, the facility halts operation until space becomes" \
            " available.", \
    "uilabel": "Maximum Leftover Inventory", \
    "default": 1e299, \
    "uitype": "range", \
    "range": [0.0, 1e299], \
    "units": "kg", \
  }
  double out_buf_size;

#pragma cyclus var { "capacity" : "out_buf_size", }
  cyclus::toolkit::ResBuf<cyclus::Material> output;

#pragma cyclus var { \
    "default": 1e299, \
    "doc": "Maximum number of kg of fuel material that can be mixed per time step.", \
    "uilabel": "Maximum Throughput", \
    "uitype": "range", \
    "range": [0.0, 1e299], \
    "units": "kg", \
  }
  double throughput;

  #pragma cyclus var {"default": 0,\
                      "tooltip":"If true try to feed buffer > 1 only if buffer 0 is full", \
                      "doc":" Request commods for the buffer other than the first one," \
                      " depending of the status of the first buffer: if there is enough"\
                      " material in the first buffer to fill the throughput then try to"\
                      " fill the other one...",\
                      "uilabel":"Batch Handling"}
  int constrain_request;

  bool  request_other_buffer;
  // intra-time-step state - no need to be a state var
  // map<request, inventory name>
  std::map<cyclus::Request<cyclus::Material>*, std::string> req_inventories_;

  //// A policy for sending material
  cyclus::toolkit::MatlSellPolicy sell_policy;
};

}  // namespace cyfco

#endif  // CYFCO_SRC_MIXER_H_
