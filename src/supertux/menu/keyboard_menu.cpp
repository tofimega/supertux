//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>,
//                2007 Ingo Ruhnke <grumbel@gmail.com>
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

#include "supertux/menu/keyboard_menu.hpp"

#include "control/input_manager.hpp"
#include "control/keyboard_manager.hpp"
#include "gui/item_controlfield.hpp"
#include "gui/menu_manager.hpp"
#include "supertux/gameconfig.hpp"
#include "supertux/globals.hpp"
#include "util/gettext.hpp"

#include "supertux/menu/keybind_config_menu.hpp"
#include <fmt/format.h>

KeyboardMenu::KeyboardMenu(InputManager& input_manager, int player_id) :
  m_input_manager(input_manager),
  m_player_id(player_id)
{
  add_label(_("Setup Keyboard"));
  add_hl();
  
  add_entry(static_cast<int>(Control::UP),         _("Up"));
  add_entry(static_cast<int>(Control::DOWN),       _("Down"));
  add_entry(static_cast<int>(Control::LEFT),       _("Left"));
  add_entry(static_cast<int>(Control::RIGHT),      _("Right"));
  add_entry(static_cast<int>(Control::JUMP),       _("Jump"));
  add_entry(static_cast<int>(Control::ACTION),     _("Action"));
  add_entry(static_cast<int>(Control::GRAB),       _("Grab"));
  add_entry(static_cast<int>(Control::INTERACT),   _("Interact"));
  add_entry(static_cast<int>(Control::ITEM),       _("Item Pocket"));

  add_entry(static_cast<int>(Control::PEEK_LEFT),  _("Peek Left"));
  add_entry(static_cast<int>(Control::PEEK_RIGHT), _("Peek Right"));
  add_entry(static_cast<int>(Control::PEEK_UP),    _("Peek Up"));
  add_entry(static_cast<int>(Control::PEEK_DOWN),  _("Peek Down"));


  if (m_player_id == 0)
  {
    if (g_config->developer_mode) {
      add_entry(static_cast<int>(Control::CONSOLE),    _("Console"));
      add_entry(static_cast<int>(Control::CHEAT_MENU), _("Cheat Menu"));
      add_entry(static_cast<int>(Control::DEBUG_MENU), _("Debug Menu"));
    }
  }

  add_hl();
  add_toggle(static_cast<int>(Control::CONTROLCOUNT), _("Jump with Up"), &g_config->keyboard_config.m_jump_with_up_kbd);
  add_toggle(static_cast<int>(Control::CONTROLCOUNT)+1, _("Interact with Up"), &g_config->keyboard_config.m_interact_with_up_kbd);
  add_toggle(static_cast<int>(Control::CONTROLCOUNT)+2, _("Grab with Action"), &g_config->keyboard_config.m_grab_with_action_kbd);
  
  add_hl();
  add_entry(_("Reset bindings"), [p=m_player_id]{

  Dialog::show_confirmation(_("Reset bindings for this player?"),
  [p] {
    g_config->keyboard_config.reset_bindings(p);
  }
);
});

  if (m_input_manager.get_num_users()>1)
  {
    add_hl();
    for (int id = 0; id < m_input_manager.get_num_users(); id++)
    {
      if(id==m_player_id) continue;
      add_entry(fmt::format(fmt::runtime(_("Player {}")), std::to_string(id + 1)),
      [&input_manager, id] {
        MenuManager::instance().set_menu(std::make_unique<KeyboardMenu>(input_manager, id));
      });
    }
    add_hl();
  }
else add_hl();

  add_back(_("Back"));
  refresh();
}

std::string
KeyboardMenu::get_key_name(SDL_Keycode key) const
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

void
KeyboardMenu::menu_action(MenuItem& item)
{
  if (item.get_id() >= 0 && item.get_id() < static_cast<int>(Control::CONTROLCOUNT))
  {
    MenuManager::instance().push_menu(std::make_unique<KeybindConfigMenu>(m_input_manager, m_player_id, static_cast<Control>(item.get_id())));
  }
}


/* EOF */
