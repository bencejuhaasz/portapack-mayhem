#include "proc_fskrx.hpp"
#include "portapack_shared_memory.hpp"
#include "sine_table_int8.hpp"
#include "event_m4.hpp"

#include <cstdint>


void FSKRXProcessor::execute(const buffer_c8_t& buffer) {
  uint32_t ret = 0;
  for (size_t i = 0; i < buffer.count; i++) {
    complex8_t cmp = buffer.p[i];
    ret+=cmp.real();
    ret+=cmp.imag();
  }
  data_message.is_data = true;
  data_message.value = ret;
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
