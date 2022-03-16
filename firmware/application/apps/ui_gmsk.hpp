#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "string_format.hpp"

namespace ui
{
    class GMSKView : public View                                // App class declaration
    {
    public:
        GMSKView(NavigationView &nav);                          // App class init function declaration
        std::string title() const override { return "GMSK Demod App"; }; // App title

    private:
        void update();                                            // Function declaration
        MessageHandlerRegistration message_handler_update{        // Example, not required: MessageHandlerRegistration class
            Message::ID::DisplayFrameSync,                        // relays messages to your app code from baseband. Every time you
            [this](const Message *const) {                        // get a  DisplayFrameSync message the update() function will
                this->update();                                   // be triggered.
            }};

            NumberField my_gmsk_numberfield{{10, 10},3,{0, 255},1,'0',false};

            Button my_gmsk_button{{30, 30, 100, 24},"Increase"};
    };


}
