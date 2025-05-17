#include "supertux/menu/joybind_config_menu.hpp"
#include "control/keyboard_manager.hpp"
#include "supertux/gameconfig.hpp"
#include "supertux/globals.hpp"
#include "control/joystick_manager.hpp"
#include "control/joystick_config.hpp"
#include <SDL.h>
#include <vector>

#include <sstream>

JoybindConfigMenu::JoybindConfigMenu(InputManager& input_manager, int joystick_id, Control action):
    m_input_manager(input_manager),
    m_joystick_id(joystick_id),
    m_configured_action(action)
{
refresh();
}


void JoybindConfigMenu::menu_action(MenuItem& m){
  return;
}

void JoybindConfigMenu::refresh(){
  clear();
  add_label(_("Configure Control: " + Control_to_string(m_configured_action)));
    add_hl();
    std::vector<int> buttons = g_config->joystick_config.reversemap_joybutton(m_joystick_id, m_configured_action);
    std::vector<int> hats = g_config->joystick_config.reversemap_joyhat(m_joystick_id, m_configured_action);
    std::vector<int> axes = g_config->joystick_config.reversemap_joyaxis(m_joystick_id, m_configured_action);
    for(auto k : buttons){
      if(k == -1) continue;
      add_entry(get_button_name(k),
        [j=m_joystick_id,c=m_configured_action,k]{
          if(g_config->joystick_config.reversemap_joybutton(j,c).size()>1){  
          Dialog::show_confirmation(_("Delete this binding?"),[j,k]{
          g_config->joystick_config.erase_button_binding(j,k);
          MenuManager::instance().refresh();
        });
      }
      }
      );
    }

    for(auto k : hats){
      if(k == -1) continue;
      add_entry(get_hat_dir_name(k),
        [j=m_joystick_id,c=m_configured_action,k]{
          if(g_config->joystick_config.reversemap_joyhat(j,c).size()>1){  
          Dialog::show_confirmation(_("Delete this binding?"),[j,k]{
          g_config->joystick_config.erase_joyhat_binding(j,k);
          MenuManager::instance().refresh();
        });
      }
      }
      );
    }

    for(auto k : axes){
      if(k == -1) continue;
      add_entry(get_axis_name(k),
        [j=m_joystick_id,c=m_configured_action,k]{
          if(g_config->joystick_config.reversemap_joyaxis(j,c).size()>1){  
          Dialog::show_confirmation(_("Delete this binding?"),[j,k]{
          g_config->joystick_config.erase_joyaxis_binding(j,k);
          MenuManager::instance().refresh();
        });
      }
      }
      );
    }

    add_hl();
    add_entry(_("Add new binding"),
      [j=m_joystick_id, c=m_configured_action, &i=m_input_manager] {
        Dialog::show_message("Press any key");
       i.joystick_manager->bind_next_event_to(j, static_cast<Control>(c));
      });
add_entry(_("Clear bindings"),
      [j=m_joystick_id, c=m_configured_action] {
        Dialog::show_confirmation(_("Clear bindings for this action?"), [j,c]{
        g_config->joystick_config.unbind_joystick_control(j,c);
        MenuManager::instance().refresh();
        });
       
      });
    add_hl();
    add_back(_("Back"));

}

std::string
JoybindConfigMenu::get_button_name(int button) const
{
  if (button < 0)
  {
    return _("None");
  }
  else
  {
    std::ostringstream name;
    name << "Button " << button;
    return name.str();
  }
}

std::string
JoybindConfigMenu::get_hat_dir_name(int hat_dir) const
{
std::string name;

    switch (hat_dir)
    {
      case SDL_HAT_UP:
        name = _("Hat Up");
        break;

      case SDL_HAT_DOWN:
        name = _("Hat Down");
        break;

      case SDL_HAT_LEFT:
        name = _("Hat Left");
        break;

      case SDL_HAT_RIGHT:
        name = _("Hat Right");
        break;

      default:
        name = "Unknown hat_dir";
        break;
    }

    return name;
}


std::string
JoybindConfigMenu::get_axis_name(int axis) const
{
  std::ostringstream name;

    name << _("Axis ");

    if (axis < 0)
      name << _("-");
    else
      name << _("+");

    if (abs(axis) == 1)
      name << _("X");
    else if (abs(axis) == 2)
      name << _("Y");
    else if (abs(axis) == 3)
      name << _("X2");
    else if (abs(axis) == 4)
      name << _("Y2");
    else
      name << abs(axis);


    return name.str();
}