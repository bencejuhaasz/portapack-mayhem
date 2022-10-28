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
  BasebandThread baseband_thread { 2000, this, NORMALPRIO+20, baseband::Direction::Receive };
	//RSSIThread rssi_thread { NORMALPRIO + 10 };
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
