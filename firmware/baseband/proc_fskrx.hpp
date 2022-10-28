#ifndef __PROC_FSKRX_H__
#define __PROC_FSKRX_H__

#include "baseband_processor.hpp"
#include "baseband_thread.hpp"
#include "dsp_decimate.hpp"

class FSKRXProcessor: public BasebandProcessor {
public:
  void execute(const buffer_c8_t& buffer) override;
  void on_message(const Message* const p) override;
private:
  void configure(const FSKRxConfigureMessage& message);
  std::array<complex16_t, 512> dst { };
  dsp::decimate::FIRC8xR16x24FS4Decim8 decim_0 { };
	dsp::decimate::FIRC16xR16x32Decim8 decim_1 { };
	dsp::decimate::FIRAndDecimateComplex channel_filter { };
  const buffer_c16_t dst_buffer {
		dst.data(),
		dst.size()
	};
};

#endif
