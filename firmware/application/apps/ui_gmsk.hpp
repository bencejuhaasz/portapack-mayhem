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

            // Example 3 digit number starting at "000", ends at "255"
            NumberField my_gmsk_numberfield(
              {10, 10},          // Coordinates are: int:x (px), int:y (px)
              3,                 // Length
              {0, 255},          // MIN -> MAX Range
              1,                 // Step
              '0',               // Fill Char
              false              // Can Loop
            );

            Button my_gmsk_button(
              {30, 30, 100, 24}, // Coordinates are: int:x (px), int:y (px), int:width (px), int:height (px)
              "Increase"
            );
    };


}
