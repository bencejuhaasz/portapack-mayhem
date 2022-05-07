#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "string_format.hpp"
#include "encoders.hpp"
#include "baseband_api.hpp"
#include "file.hpp" 
#include "sd_card.hpp"

namespace ui
{
    class GMSKView : public View                                // App class declaration
    {
    public:
        GMSKView(NavigationView &nav);                          // App class init function declaration
        std::string title() const override { return "GMSK Demod App"; }; // App title

    private:
	uint32_t prev_reg;
        void update();                                            // Function declaration
        MessageHandlerRegistration message_handler_update{        // Example, not required: MessageHandlerRegistration class
            Message::ID::DisplayFrameSync,                        // relays messages to your app code from baseband. Every time you
            [this](const Message *const) {                        // get a  DisplayFrameSync message the update() function will
                this->update();                                   // be triggered.
            }};

            NumberField my_gmsk_numberfield{{10, 10},3,{0, 255},1,'0',false};

            Button my_gmsk_button{{30, 30, 100, 24},"Increase"};

            //TX functions
            void start_tx(std::string& message);                                         // Function declarations
            void stop_tx();
            void on_rx_progress(const uint32_t progress, const bool done);

            MessageHandlerRegistration message_handler_tx_progress {
              Message::ID::TXProgress,
              [this](const Message* const p) {
                const auto message = *reinterpret_cast<const TXProgressMessage*>(p);
                this->on_rx_progress(message.progress, message.done);

             }};

        void start_rx();
        void stop_rx();
        void on_data(const uint32_t*, const bool*);	
        //void on_data(const uint32_t, const bool);
	
	//File handling
        bool write_file(const std::filesystem::path& path, std::string name, const void * data);
	bool create_file(const std::filesystem::path& path, std::string name);

        MessageHandlerRegistration message_handler_packet {
          Message::ID::AFSKData,
          [this](Message* const p) {
            const auto message = static_cast<const AFSKDataMessage*>(p); 
            this->on_data(&message->value, &message->is_data);
        }};
    };


}
