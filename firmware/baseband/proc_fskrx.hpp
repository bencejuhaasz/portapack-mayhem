#ifndef __PROC_FSKRX_H__
#define __PROC_FSKRX_H__

#include "baseband_processor.hpp"
#include "baseband_thread.hpp"

class FSKRXProcessor: public BasebandProcessor {
public:
  void execute(const buffer_c8_t& buffer) override;
  void on_message(const Message* const p) override;
};

#endif
