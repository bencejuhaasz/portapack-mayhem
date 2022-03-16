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

      my_gmsk_button.on_select = [&nav](Button &){
        int number = my_gmsk_numberfield.value();
        number++;
        my_gmsk_numberfield.set_value(number);
      };
    }

    void GMSKView::update()                   // Every time you get a DisplayFrameSync message this function will be ran
    {
         // Message code
    }
}
