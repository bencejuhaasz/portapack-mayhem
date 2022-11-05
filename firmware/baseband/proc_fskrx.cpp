#include "proc_fskrx.hpp"
#include "portapack_shared_memory.hpp"
#include "sine_table_int8.hpp"
#include "event_m4.hpp"

#include <cstdint>
#include <vector>


void FSKRXProcessor::execute(const buffer_c8_t& buffer) {
  //create preprocess output vector
  std::vector<std::complex<float>> prep_output;
  //DFT on buffer
  for (size_t i = 0; i < buffer.count; i++) {
    prep_output.push_back(DFT(buffer.p[i], i,buffer));
  }

  //bit vector
  v.push_back(true);
  //process DFT result
  for (size_t i = 0; i < prep_output.size(); i++) {
    /* code */
  }
  //return data
  data_message.is_data=true;
  data_message.value=32;
  shared_memory.application_queue.push(data_message);
}

std::complex<float> FSKRXProcessor::DFT(std::complex<signed char> in, int k,const buffer_c8_t& buffer) {
    float a = 0;
    float b = 0;
    int N = buffer.count;
    for(int n = 0; n < N; n++)
    {
        a+= cos((2 * M_PI * k * n) / N) * buffer.p[n];
        b+= -sin((2 * M_PI * k * n) / N) * buffer.p[n];
    }
    std::complex<float> temp(a, b);
    return temp;
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
