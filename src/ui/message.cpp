// ASEPRITE gui library
// Copyright (C) 2001-2013  David Capello
//
// This source file is distributed under a BSD-like license, please
// read LICENSE.txt for more information.

#include "config.h"

#include <allegro/keyboard.h>
#include <string.h>

#include "base/memory.h"
#include "ui/manager.h"
#include "ui/message.h"
#include "ui/rect.h"
#include "ui/widget.h"

namespace ui {

static int registered_messages = JM_REGISTERED_MESSAGES;

int ji_register_message_type()
{
  return registered_messages++;
}

Message* jmessage_new(int type)
{
  Message* msg = new Message;

  memset(msg, 0, sizeof(Message));

  msg->type = type;
  msg->any.widgets = new WidgetsList;
  msg->any.shifts =
    (key[KEY_LSHIFT] || key[KEY_RSHIFT] ? KB_SHIFT_FLAG: 0) |
    (key[KEY_LCONTROL] || key[KEY_RCONTROL] ? KB_CTRL_FLAG: 0) |
    (key[KEY_ALT] ? KB_ALT_FLAG: 0);

  // printf("type=%02d ", type);
  // if (msg->any.shifts & KB_SHIFT_FLAG) printf("KB_SHIFT_FLAG ");
  // if (msg->any.shifts & KB_CTRL_FLAG) printf("KB_CTRL_FLAG ");
  // if (msg->any.shifts & KB_ALT_FLAG) printf("KB_ALT_FLAG ");
  // printf("\n");
  // fflush(stdout);

  return msg;
}

Message* jmessage_new_key_related(int type, int readkey_value)
{
  Message* msg = jmessage_new(type);

  msg->key.scancode = (readkey_value >> 8) & 0xff;
  msg->key.ascii = readkey_value & 0xff;
  msg->key.repeat = 0;
  msg->key.propagate_to_children = false;
  msg->key.propagate_to_parent = true;

#if 0
  printf("%s: %i %i [%c]\n", type == JM_KEYPRESSED ? "JM_KEYPRESSED":
                                                     "JM_KEYRELEASED",
         msg->key.scancode, msg->key.ascii, msg->key.ascii);
  fflush(stdout);
#endif
  return msg;
}

Message* jmessage_new_copy(const Message* msg)
{
  Message* copy;

  ASSERT(msg != NULL);

  copy = new Message;
  if (!copy)
    return NULL;

  memcpy(copy, msg, sizeof(Message));

  copy->any.widgets = new WidgetsList(*msg->any.widgets);
  copy->any.used = false;

  return copy;
}

Message* jmessage_new_copy_without_dests(const Message* msg)
{
  ASSERT(msg != NULL);

  Message* copy = new Message;
  if (!copy)
    return NULL;

  memcpy(copy, msg, sizeof(Message));

  copy->any.widgets = new WidgetsList;
  copy->any.used = false;

  return copy;
}

void jmessage_free(Message* msg)
{
  ASSERT(msg != NULL);

  delete msg->any.widgets;
  delete msg;
}

void jmessage_add_dest(Message* msg, Widget* widget)
{
  ASSERT(msg != NULL);
  ASSERT_VALID_WIDGET(widget);

  msg->any.widgets->push_back(widget);
}

void jmessage_add_pre_dest(Message* msg, Widget* widget)
{
  ASSERT(msg != NULL);
  ASSERT_VALID_WIDGET(widget);

  msg->any.widgets->insert(msg->any.widgets->begin(), widget);
}

void jmessage_broadcast_to_children(Message* msg, Widget* widget)
{
  ASSERT(msg != NULL);
  ASSERT_VALID_WIDGET(widget);

  UI_FOREACH_WIDGET(widget->getChildren(), it)
    jmessage_broadcast_to_children(msg, *it);

  jmessage_add_dest(msg, widget);
}

void jmessage_broadcast_to_parents(Message* msg, Widget* widget)
{
  ASSERT(msg != NULL);
  ASSERT_VALID_WIDGET(widget);

  if (widget && widget->type != JI_MANAGER) {
    jmessage_add_dest(msg, widget);
    jmessage_broadcast_to_parents(msg, widget->getParent());
  }
}

} // namespace ui
