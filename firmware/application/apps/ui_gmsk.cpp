#include "ui_gmsk.hpp"
#include "portapack.hpp"
#include <cstring>

using namespace portapack;

namespace ui
{

    GMSKView::GMSKView(NavigationView &nav) // Application Main
    {
      add_children({
        &my_button,
        &my_numberfield
      });
    }

    void NewAppView::update()                   // Every time you get a DisplayFrameSync message this function will be ran
    {
         // Message code
    }

    my_button.on_select = [&nav](Button &){
      int number = my_numberField.value();
      number++;
      my_numberField.set_value(number);
    }
}
