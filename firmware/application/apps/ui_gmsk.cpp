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
        &data_status_label,
        &start_btn,
        &stop_btn,
        &field_frequency
      });




      start_btn.on_select = [this](Button &){
        int number = data_status_label.value();
        number++;
        data_status_label.set_value(number);
        start_rx();
      };

      //Create file we are recording to
      create_file(u"FSK", "REC.bin");
      //Start baseband RX process

      stop_btn.on_select = [this](Button &){
        stop_rx();
      };


      //Freq selector
      field_frequency.set_value(receiver_model.tuning_frequency());
      field_frequency.set_step(receiver_model.frequency_step());
      field_frequency.on_change = [this](rf::Frequency f) {
		this->on_tuning_frequency_changed(f);
      };
      field_frequency.on_edit = [this, &nav]() {
		// TODO: Provide separate modal method/scheme?
		auto new_view = nav.push<FrequencyKeypadView>(receiver_model.tuning_frequency());
		new_view->on_changed = [this](rf::Frequency f) {
			this->on_tuning_frequency_changed(f);
			this->field_frequency.set_value(f);
		};
      };

      field_frequency.on_show_options = [this]() {
		this->on_show_options_frequency();
      };
    }


    void GMSKView::on_tuning_frequency_changed(rf::Frequency f) {
	receiver_model.set_tuning_frequency(f);
    }

    void GMSKView::on_show_options_frequency() {
	auto widget = std::make_unique<FrequencyOptionsView>(options_view_rect, &style_options_group);

	widget->set_step(receiver_model.frequency_step());
	widget->on_change_step = [this](rf::Frequency f) {
		this->on_frequency_step_changed(f);
	};
	widget->set_reference_ppm_correction(persistent_memory::correction_ppb() / 1000);
	widget->on_change_reference_ppm_correction = [this](int32_t v) {
		this->on_reference_ppm_correction_changed(v);
	};

	set_options_widget(std::move(widget));
	field_frequency.set_style(&style_options_group);
     }

     void GMSKView::on_frequency_step_changed(rf::Frequency f) {
	receiver_model.set_frequency_step(f);
	field_frequency.set_step(f);
     }

     void GMSKView::on_reference_ppm_correction_changed(int32_t v) {
	persistent_memory::set_correction_ppb(v * 1000);
     }

    void GMSKView::update()                   // Every time you get a DisplayFrameSync message this function will be ran
    {
         // Message code
    }

    void GMSKView::start_rx()
    {
      // persistent_memory::set_modem_baudrate(def_bell202->baudrate);
      //baseband::set_afsk(persistent_memory::modem_baudrate(), 8, 0, false);
      baseband::run_image(portapack::spi_flash::image_tag_fsk_rx);
      //baseband::set_afsk(9600, 8, 0, false);
      receiver_model.set_tuning_frequency(5000000);
      receiver_model.set_sampling_rate(2000000);
      receiver_model.set_baseband_bandwidth(240000);
      //receiver_model.set_modulation(ReceiverModel::Mode::NarrowbandFMAudio);
      receiver_model.set_modulation(ReceiverModel::Mode::Capture);
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
		    int number = data_status_label.value();
		    if (number==255) {
			    number=0;
                    }
		            number++;
			            data_status_label.set_value(number);
				    prev_reg=*value;
	    }
	    if(buffer_cnt==1000) {
	    	write_file(u"FSK", "REC.bin", int_rec_buffer);
	    	buffer_cnt=0;
	    }
	    int_rec_buffer[buffer_cnt]=*value;
	    buffer_cnt++;

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
				        file.write(data,1000);
					        return true;
						    } else {
							            return false;
								        }
    }

    void GMSKView::set_options_widget(std::unique_ptr<Widget> new_widget) {
	/*remove_options_widget();

	if( new_widget ) {
		options_widget = std::move(new_widget);
	} else {
		// TODO: Lame hack to hide options view due to my bad paint/damage algorithm.
		options_widget = std::make_unique<Rectangle>(options_view_rect, style_options_group.background);
	}
	add_child(options_widget.get());*/
    }

   void GMSKView::remove_options_widget() {
	if( options_widget ) {
		remove_child(options_widget.get());
		options_widget.reset();
	}

	//field_lna.set_style(nullptr);
	//options_modulation.set_style(nullptr);
	field_frequency.set_style(nullptr);
   }

}
