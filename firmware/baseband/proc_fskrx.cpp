#include "proc_fskrx.hpp"
#include "portapack_shared_memory.hpp"
#include "sine_table_int8.hpp"
#include "event_m4.hpp"

#include <cstdint>
#include <vector>
#include <numeric>


void FSKRXProcessor::execute(const buffer_c8_t& buffer) {
  std::vector<bool> v;
  for(int i=1;i<buffer.count;i++) {
    if(i==18446744073709551610){
      r_avg_bit_time=0;
      i_avg_bit_time=0;
      i = 0;
      j = 0;
    }
    if(j==18446744073709551610){
      r_avg_bit_time=0;
      i_avg_bit_time=0;
      i = 0;
      j = 0;
    }
    //zero crossing
    if (buffer.p[i].real()*buffer.p[i-1].real()<0) {
      if (real_last_zero_crossing_time!=0) {
        double next_avg = ((r_avg_bit_time * (i - 1)) + (time-real_last_zero_crossing_time)) / i;
        if((r_avg_bit_time*1.4)>next_avg){
          r_avg_bit_time=next_avg;
        }
        r_avg_bit_time
        i++;
      }
      real_last_zero_crossing_time=time;
    }
    if (buffer.p[i].imag()*buffer.p[i-1].imag()<0) {
      if (imag_last_zero_crossing_time!=0) {
        double next_avg = ((i_avg_bit_time * (j - 1)) + (time-imag_last_zero_crossing_time)) / j;
        if((i_avg_bit_time*1.4)>next_avg){
          i_avg_bit_time=next_avg;
        }
        j++;
      }
      imag_last_zero_crossing_time=time;
    }


    //bit time
    if((double)(time-real_last_zero_crossing_time)-r_avg_bit_time<1) {
      if(buffer.p[i].real()>0) {
        v.push_back(true);
      } else {
        v.push_back(false);
      }
    }
    if ((double)(time-imag_last_zero_crossing_time)-i_avg_bit_time<1) {
      if(buffer.p[i].imag()>0) {
        v.push_back(true);
      } else {
        v.push_back(false);
      }
    }



  }
  if (v.size()>0) {
    uint32_t a = accumulate(v.rbegin(), v.rend(), 0, [](int x, int y) { return (x << 1) + y; });
    data_message.is_data=true;
    data_message.value=a;
    shared_memory.application_queue.push(data_message);
  }
  time++;
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
