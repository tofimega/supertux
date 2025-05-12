#ifndef HEADER_SUPERTUX_SUPERTUX_MENU_KEYBIND_CONFIG_MENU_HPP
#define HEADER_SUPERTUX_SUPERTUX_MENU_KEYBIND_CONFIG_MENU_HPP
#include "gui/menu_item.hpp"
#include "gui/dialog.hpp"
#include "control/controller.hpp"
#include "control/input_manager.hpp"
#include <string>

class KeybindConfigMenu final : public Menu
{
public:
    KeybindConfigMenu(InputManager& input_manager, int player_id, Control action);
    std::string get_key_name(SDL_Keycode key) const;
    void menu_action(MenuItem& m) override;
    void refresh() override;

private: 
    int m_player_id;
    Control m_configured_action;
    InputManager& m_input_manager;

};

#endif