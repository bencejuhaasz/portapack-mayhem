#include "ui_gmsk.hpp"
#include "portapack.hpp"
#include <cstring>

using namespace portapack;

namespace ui
{

    GMSKView::GMSKView(NavigationView &nav) // Application Main
    {
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

      baseband::run_image(portapack::spi_flash::image_tag_afsk_rx);
    }

    void GMSKView::update()                   // Every time you get a DisplayFrameSync message this function will be ran
    {
         // Message code
    }

    void NewAppView::start_rx()
    {
      persistent_memory::set_modem_baudrate(def_bell202->baudrate);
      baseband::set_afsk(persistent_memory::modem_baudrate(), 8, 0, false);
      receiver_model.set_tuning_frequency(933200000);
      receiver_model.set_sampling_rate(1000000);
      receiver_model.set_baseband_bandwidth(240000);
      receiver_model.set_modulation(ReceiverModel::Mode::NarrowbandFMAudio);
      receiver_model.enable();


    }

    void NewAppView::stop_rx()
    {
      receiver_model.disable();
      baseband::shutdown();
    }

    void NewAppView::on_data(uint32_t value, bool is_data)
    {
      if(is_data) {
            // RX data handling Logic
         }
    }


}
