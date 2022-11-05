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

class FSKRXProcessor: public BasebandProcessor {
public:
  void execute(const buffer_c8_t& buffer) override;
  void on_message(const Message* const p) override;
private:
  float M_PI = 3.14159265359;
  std::vector<bool> v;
  FSKDataMessage data_message { false, v, 0};
  BasebandThread baseband_thread { 3072000, this, NORMALPRIO+20, baseband::Direction::Receive };
	RSSIThread rssi_thread { NORMALPRIO + 10 };
  void configure(const FSKRxConfigureMessage& message);
  std::complex<float> DFT(std::complex<signed char> in, int k,const buffer_c8_t& buffer);
};

#endif
