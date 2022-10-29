#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "ui_receiver.hpp"
#include "string_format.hpp"
#include "encoders.hpp"
#include "baseband_api.hpp"
#include "file.hpp"
#include "sd_card.hpp"
#include "portapack_persistent_memory.hpp"

namespace ui
{
    class GMSKView : public View                                // App class declaration
    {
    public:
        GMSKView(NavigationView &nav);                          // App class init function declaration
        std::string title() const override { return "GMSK-REC"; }; // App title

    private:
	uint32_t prev_reg;
	uint32_t int_rec_buffer[1000];
	int buffer_cnt=0;
        void update();                                            // Function declaration
        MessageHandlerRegistration message_handler_update{        // Example, not required: MessageHandlerRegistration class
            Message::ID::DisplayFrameSync,                        // relays messages to your app code from baseband. Every time you
            [this](const Message *const) {                        // get a  DisplayFrameSync message the update() function will
                this->update();                                   // be triggered.
            }};

            NumberField data_status_label{{10, 10},3,{0, 255},1,'0',false};

            FrequencyField field_frequency {
		{60, 10},
	    };

            //freq change events
            void on_tuning_frequency_changed(rf::Frequency f);
            void on_show_options_frequency();
            void on_edit_frequency();
            void on_reference_ppm_correction_changed(int32_t);
            void on_frequency_step_changed(rf::Frequency);
            void set_options_widget(std::unique_ptr<ui::Widget>);

            Button start_btn{{60, 60, 100, 100},"Start"};
            Button stop_btn{{60, 200, 100, 100},"Stop"};

            //??
            const Rect options_view_rect { 0 * 8, 1 * 16, 30 * 8, 1 * 16 };
            Style style_options_group {
	      .font = font::fixed_8x16,
	      .background = Color::blue(),
	      .foreground = Color::white(),
            };
            std::unique_ptr<Widget> options_widget { };
            void remove_options_widget();




            //RX functions
            void start_rx();
            void stop_rx();
            void on_data(const uint32_t*, const bool*);
            void on_rx_progress(const uint32_t progress, const bool done);

            MessageHandlerRegistration message_handler_tx_progress {
              Message::ID::TXProgress,
              [this](const Message* const p) {
                const auto message = *reinterpret_cast<const TXProgressMessage*>(p);
                this->on_rx_progress(message.progress, message.done);

             }};


	//File handling
  bool write_file(const std::filesystem::path& path, std::string name, const void * data);
	bool create_file(const std::filesystem::path& path, std::string name);

        MessageHandlerRegistration message_handler_packet {
          Message::ID::FSKData,
          [this](Message* const p) {
            const auto message = static_cast<const FSKDataMessage*>(p);
            this->on_data(&message->value, &message->is_data);
        }};
    };


}
