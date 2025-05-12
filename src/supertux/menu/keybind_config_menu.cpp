#include "supertux/menu/keybind_config_menu.hpp"
#include "control/keyboard_manager.hpp"
#include "supertux/gameconfig.hpp"
#include "supertux/globals.hpp"

#include <SDL.h>
#include <vector>

KeybindConfigMenu::KeybindConfigMenu(InputManager& input_manager,int player_id, Control action):
    m_input_manager(input_manager),
    m_player_id(player_id),
    m_configured_action(action)
{
refresh();
}


void KeybindConfigMenu::menu_action(MenuItem& m){
  return;
}

void KeybindConfigMenu::refresh(){
  clear();
  add_label(_("Configure Control: " + Control_to_string(m_configured_action)));
    add_hl();
    std::vector<SDL_Keycode> keys = g_config->keyboard_config.reversemap_key(m_player_id, m_configured_action);

    for(auto k : keys){
      add_entry(get_key_name(k),
        [p=m_player_id,c=m_configured_action,k]{
          if(g_config->keyboard_config.reversemap_key(p,c).size()>1){  
          Dialog::show_confirmation(_("Delete this binding?"),[k]{
          g_config->keyboard_config.erase_binding(k);
          MenuManager::instance().refresh();
        });
      }
      }
      );
    }


    add_hl();
    add_entry(_("Add new binding"),
      [this] {
        Dialog::show_message("Press any key");
       m_input_manager.keyboard_manager->bind_next_event_to(m_player_id, static_cast<Control>(m_configured_action));
      });

    add_hl();
    add_back(_("Back"));

}

std::string
KeybindConfigMenu::get_key_name(SDL_Keycode key) const
{
  switch (key) {
    case SDLK_UNKNOWN:
      return _("None");
    case SDLK_UP:
      return _("Up cursor");
    case SDLK_DOWN:
      return _("Down cursor");
    case SDLK_LEFT:
      return _("Left cursor");
    case SDLK_RIGHT:
      return _("Right cursor");
    case SDLK_RETURN:
      return _("Return");
    case SDLK_SPACE:
      return _("Space");
    case SDLK_RSHIFT:
      return _("Right Shift");
    case SDLK_LSHIFT:
      return _("Left Shift");
    case SDLK_RCTRL:
      return _("Right Control");
    case SDLK_LCTRL:
      return _("Left Control");
    case SDLK_RALT:
      return _("Right Alt");
    case SDLK_LALT:
      return _("Left Alt");
    case SDLK_RGUI:
      return _("Right Command");
    case SDLK_LGUI:
      return _("Left Command");
    default:
      return SDL_GetKeyName(static_cast<SDL_Keycode>(key));
  }
}