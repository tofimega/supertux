//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>,
//           2007,2014 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "supertux/menu/joystick_menu.hpp"

#include <iostream>
#include <sstream>
#include "control/joystick_manager.hpp"
#include "gui/item_controlfield.hpp"
#include "gui/item_toggle.hpp"
#include "supertux/gameconfig.hpp"
#include "supertux/globals.hpp"
#include "util/gettext.hpp"
#include "control/game_controller_manager.hpp"

namespace {

enum {
  MNID_JUMP_WITH_UP = static_cast<int>(Control::CONTROLCOUNT),
  MNID_SCAN_JOYSTICKS,
  MNID_AUTO_JOYSTICK_CFG
};

} // namespace

JoystickMenu::JoystickMenu(InputManager& input_manager) :
  m_input_manager(input_manager),
  m_joysticks_available(false),
  m_auto_joystick_cfg(!m_input_manager.use_game_controller()),
  m_joy_id(0)
{
  recreate_menu();
}

JoystickMenu::~JoystickMenu()
{}

void
JoystickMenu::recreate_menu()
{
  clear();
  add_label(_("Setup Joystick"));
  add_hl();

  add_toggle(MNID_AUTO_JOYSTICK_CFG, _("Manual Configuration"),
             &m_auto_joystick_cfg)
    .set_help(_("Use manual configuration instead of SDL2's automatic GameController support"));

  if (m_input_manager.use_game_controller())
  {
    m_joysticks_available = false;
  }
  else
  {
    if (m_input_manager.joystick_manager->get_num_joysticks() > 0)
    {
      m_joysticks_available = true;

      add_hl();
      add_label(_("Test"));
      add_hl();

      add_controlfield(static_cast<int>(Control::UP),          _("Up"));
      add_controlfield(static_cast<int>(Control::DOWN),        _("Down"));
      add_controlfield(static_cast<int>(Control::LEFT),        _("Left"));
      add_controlfield(static_cast<int>(Control::RIGHT),       _("Right"));
      add_controlfield(static_cast<int>(Control::JUMP),        _("Jump"));
      add_controlfield(static_cast<int>(Control::ACTION),      _("Action"));
      add_controlfield(static_cast<int>(Control::GRAB),        _("Grab"));
      add_controlfield(static_cast<int>(Control::INTERACT),    _("Interact")); 
      add_controlfield(static_cast<int>(Control::ITEM),        _("Item Pocket"));
      add_controlfield(static_cast<int>(Control::START),       _("Pause/Menu"));
      add_controlfield(static_cast<int>(Control::PEEK_LEFT),   _("Peek Left"));
      add_controlfield(static_cast<int>(Control::PEEK_RIGHT),  _("Peek Right"));
      add_controlfield(static_cast<int>(Control::PEEK_UP),     _("Peek Up"));
      add_controlfield(static_cast<int>(Control::PEEK_DOWN),   _("Peek Down"));
      if (g_config->developer_mode) {
        add_controlfield(static_cast<int>(Control::CONSOLE), _("Console"));
        add_controlfield(static_cast<int>(Control::CHEAT_MENU), _("Cheat Menu"));
        add_controlfield(static_cast<int>(Control::DEBUG_MENU), _("Debug Menu"));
      }
      add_hl();
      add_toggle(MNID_JUMP_WITH_UP, _("Jump with Up"), &g_config->joystick_config.m_jump_with_up_joy);
      add_toggle(MNID_JUMP_WITH_UP+1, _("Interact with Up"), &g_config->joystick_config.m_interact_with_up_joy);
      add_toggle(MNID_JUMP_WITH_UP+1, _("Grab with Action"), &g_config->joystick_config.m_grab_with_action_joy);

      if(InputManager::current()->m_use_game_controller){
        auto cont_map=m_input_manager.game_controller_manager->get_controller_mapping();
        if(cont_map.size()>1){
        add_hl() ;
        for(auto c: cont_map){
          SDL_GameController* cont=c.first;
          SDL_JoystickID i = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(cont));
          if(i==m_joy_id) continue;
          add_entry(_(SDL_GameControllerName(cont)),
            [this, i]{
              m_joy_id=i;
              recreate_menu();
            });
        }
      }
      }
      else{
      auto joy_map=m_input_manager.joystick_manager->get_joystick_mapping();
        if(joy_map.size()>1){
        add_hl() ;
        for(auto c: joy_map){
          SDL_Joystick* cont=c.first;
          SDL_JoystickID i=SDL_JoystickInstanceID(cont);
          if(i==m_joy_id) continue;
          add_entry(_(SDL_JoystickName(cont)),
            [this, i]{
              m_joy_id=i;
              recreate_menu();
            });
        }
      }



    }
    }
    else
    {
      m_joysticks_available = false;

      add_inactive(_("No Joysticks found"));
      add_entry(MNID_SCAN_JOYSTICKS, _("Scan for Joysticks"));
    }
  }

  add_hl();
  add_back(_("Back"));
  refresh();
}

std::string
JoystickMenu::get_button_name(int button) const
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

void
JoystickMenu::menu_action(MenuItem& item)
{
  return;
  if (0 <= item.get_id() && item.get_id() < static_cast<int>(Control::CONTROLCOUNT))
  {
    ItemControlField& field = static_cast<ItemControlField&>(item);
    field.change_input(_("Press Button"));
    m_input_manager.joystick_manager->bind_next_event_to(m_joy_id , static_cast<Control>(item.get_id()));
  }
  else if (item.get_id() == MNID_AUTO_JOYSTICK_CFG)
  {
    //m_input_manager.use_game_controller(!item.toggled);
    m_input_manager.use_game_controller(!m_auto_joystick_cfg);
    m_input_manager.reset();
    recreate_menu();
  }
  else if (item.get_id() == MNID_SCAN_JOYSTICKS)
  {
    m_input_manager.reset();
    recreate_menu();
  }
}

void
JoystickMenu::refresh_menu_item(Control id)
{
  ItemControlField* itemcf = dynamic_cast<ItemControlField*>(&get_item_by_id(static_cast<int>(id)));
  if (!itemcf) {
    return;
  }

  int button  = g_config->joystick_config.reversemap_joybutton(m_joy_id,id)[0];
  int axis    = g_config->joystick_config.reversemap_joyaxis(m_joy_id,id)[0];
  int hat_dir = g_config->joystick_config.reversemap_joyhat(m_joy_id,id)[0];

  if (button != -1)
  {
    itemcf->change_input(get_button_name(button));
  }
  else if (axis != 0)
  {
    

    itemcf->change_input(get_axis_name(axis));
  }
  else if (hat_dir != -1)
  {
    

    itemcf->change_input(get_hat_dir_name(hat_dir));
  }
  else
  {
    itemcf->change_input(_("None"));
  }
}

std::string
JoystickMenu::get_hat_dir_name(int hat_dir) const
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
JoystickMenu::get_axis_name(int axis) const
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


void
JoystickMenu::refresh()
{
  if (m_joysticks_available)
  {
    refresh_menu_item(Control::UP);
    refresh_menu_item(Control::DOWN);
    refresh_menu_item(Control::LEFT);
    refresh_menu_item(Control::RIGHT);

    refresh_menu_item(Control::JUMP);
    refresh_menu_item(Control::ACTION);
    refresh_menu_item(Control::GRAB);
    refresh_menu_item(Control::INTERACT);
    refresh_menu_item(Control::ITEM);
    refresh_menu_item(Control::START);
    refresh_menu_item(Control::PEEK_LEFT);
    refresh_menu_item(Control::PEEK_RIGHT);
    refresh_menu_item(Control::PEEK_UP);
    refresh_menu_item(Control::PEEK_DOWN);

    if (g_config->developer_mode) {
      refresh_menu_item(Control::CONSOLE);
      refresh_menu_item(Control::CHEAT_MENU);
      refresh_menu_item(Control::DEBUG_MENU);
    }
  }
}

/* EOF */
