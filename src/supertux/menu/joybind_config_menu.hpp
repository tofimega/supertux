#ifndef HEADER_SUPERTUX_SUPERTUX_MENU_JOYBIND_CONFIG_MENU_HPP
#define HEADER_SUPERTUX_SUPERTUX_MENU_JOYBIND_CONFIG_MENU_HPP

#include "gui/menu_item.hpp"
#include "gui/dialog.hpp"
#include "control/controller.hpp"
#include "control/input_manager.hpp"
#include <string>
#include <SDL.h>

class JoybindConfigMenu final : public Menu
{
public:

using JoystickID = SDL_JoystickID;

    JoybindConfigMenu(InputManager& input_manager, JoystickID joystick_id, Control action);
    void menu_action(MenuItem& m) override;
    void refresh() override;
    std::string get_button_name(int button) const;
    std::string get_axis_name(int button) const;
    std::string get_hat_dir_name(int button) const;
  
private: 
    JoystickID m_joystick_id;
    Control m_configured_action;
    InputManager& m_input_manager;

};




#endif