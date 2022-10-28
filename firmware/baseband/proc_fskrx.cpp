#include "proc_fskrx.hpp"
#include "portapack_shared_memory.hpp"
#include "sine_table_int8.hpp"
#include "event_m4.hpp"

#include <cstdint>


void FSKRXProcessor::execute(const buffer_c8_t& buffer) {
  const auto decim_0_out = decim_0.execute(buffer, dst_buffer);				// 2048 / 8 = 256 (512 I/Q samples)
	const auto decim_1_out = decim_1.execute(decim_0_out, dst_buffer);			// 256 / 8 = 32 (64 I/Q samples)
	const auto channel_out = channel_filter.execute(decim_1_out, dst_buffer);	// 32 / 2 = 16 (32 I/Q samples)
  uint32_t ret = channel_out.p[0].real();
  for (size_t i = 0; i < channel_out.count; i++) {
    ret += channel_out.p[i].real();
  }
  for (size_t i = 0; i < channel_out.count; i++) {
    ret += channel_out.p[i].imag();
  }
  FSKDataMessage data_message(true,ret);
  shared_memory.application_queue.push(data_message);
}

void FSKRXProcessor::on_message(const Message* const message) {
  if (message->id == Message::ID::FSKRxConfigure)
		configure(*reinterpret_cast<const FSKRxConfigureMessage*>(message));
}

void FSKRXProcessor::configure(const FSKRxConfigureMessage& message) {

}

int main() {
	EventDispatcher event_dispatcher { std::make_unique<FSKRXProcessor>() };
	event_dispatcher.run();
	return 0;
}
