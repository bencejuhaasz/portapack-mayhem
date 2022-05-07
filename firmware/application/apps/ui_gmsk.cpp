#include "ui_gmsk.hpp"
#include "portapack.hpp"
#include <cstring>
#include "file.hpp"
#include "sd_card.hpp"

using namespace portapack;

namespace ui
{

    GMSKView::GMSKView(NavigationView &nav) // Application Main
    {
      prev_reg = 0;
      add_children({
        &my_gmsk_button,
        &my_gmsk_numberfield
      });

      my_gmsk_button.on_select = [this](Button &){
        int number = my_gmsk_numberfield.value();
        number++;
        my_gmsk_numberfield.set_value(number);
        start_rx();
      };
      create_file(u"", "ASD.txt");
      baseband::run_image(portapack::spi_flash::image_tag_afsk_rx);
    }

    void GMSKView::update()                   // Every time you get a DisplayFrameSync message this function will be ran
    {
         // Message code
    }

    void GMSKView::start_rx()
    {
      // persistent_memory::set_modem_baudrate(def_bell202->baudrate);
      //baseband::set_afsk(persistent_memory::modem_baudrate(), 8, 0, false);      
      baseband::set_afsk(9600, 8, 0, false);
      receiver_model.set_tuning_frequency(5000000);
      receiver_model.set_sampling_rate(2000000);
      receiver_model.set_baseband_bandwidth(240000);
      receiver_model.set_modulation(ReceiverModel::Mode::NarrowbandFMAudio);
      receiver_model.enable();


    }

    void GMSKView::stop_rx()
    {
      receiver_model.disable();
      baseband::shutdown();
    }

    void GMSKView::on_data(const uint32_t * value, const bool * is_data)
    {
      if(is_data) {
            // RX data handling Logic
	    //my_gmsk_numberfield.set_value(*value);
	      if (prev_reg!=*value) {
		    int number = my_gmsk_numberfield.value();
		    if (number==255) {
			    number=0;
                    }
		            number++;
			            my_gmsk_numberfield.set_value(number);
				    prev_reg=*value;
	    }
	    write_file(u"", "ASD.TXT", value);
         }
    }

    void GMSKView::on_rx_progress(const uint32_t progress, const bool done)  // Function logic for when the message handler       
    {                                                                          // sends a TXProgressMess
       if(done) {
          stop_rx();
       } else {
								                    // UI logic, update ProgressBar with progress 
       }									    //          }
    }

    bool GMSKView::create_file(const std::filesystem::path& path, std::string name) {
	        File file = { };                                                                // Create File object
		    Optional<File::Error> sucess = file.create(path.string() + "/" + name);         // Create File
		        return !(sucess.is_valid());                                                    // 0 is success
    }

    bool GMSKView::write_file(const std::filesystem::path& path, std::string name, const void * data) {
	        File file;                                                                     // Create File object
		    auto sucess = file.append(path.string() + "/" + name);                         // Open file
		        if(!sucess.is_valid()) {                                                       // 0 is success
				        file.write(data,1);
					        return true;
						    } else {
							            return false;
								        }
    }
}
