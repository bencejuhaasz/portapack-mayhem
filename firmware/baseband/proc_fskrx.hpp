#ifndef __PROC_FSKRX_H__
#define __PROC_FSKRX_H__

#include "baseband_processor.hpp"
#include "baseband_thread.hpp"
#include "rssi_thread.hpp"
#include "dsp_decimate.hpp"
#include "dsp_demodulate.hpp"
#include "audio_output.hpp"
#include "fifo.hpp"
#include "message.hpp"
#include <vector>

class FSKRXProcessor: public BasebandProcessor {
public:
  void execute(const buffer_c8_t& buffer) override;
  void on_message(const Message* const p) override;
private:
  FSKDataMessage data_message { false, 0};
  BasebandThread baseband_thread { 3072000, this, NORMALPRIO+20, baseband::Direction::Receive };
  RSSIThread rssi_thread { NORMALPRIO + 10 };
  void configure(const FSKRxConfigureMessage& message);
  double r_avg_bit_time=0;
  double i_avg_bit_time=0;
  uint64_t i = 0;
  uint64_t j = 0;
  uint32_t time = 0;
  uint32_t real_last_zero_crossing_time=0;
  uint32_t imag_last_zero_crossing_time=0;

};

#endif