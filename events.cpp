// events.cpp
#include <iostream>
#include <irrlicht.h>

#include "events.h"


using namespace irr;


/**************************************************************************\
 * EventReceiver::EventReceiver                                           *
\**************************************************************************/
EventReceiver::EventReceiver()
  : button_pressed(false)
{
  //initialization du array keys
  for(int x=0; x < irr::KEY_KEY_CODES_COUNT; x++) keys[x] = false;
}

/*------------------------------------------------------------------------*\
 * EventReceiver::keyboard                                                *
\*------------------------------------------------------------------------*/
bool EventReceiver::keyboard(const SEvent &event)
{
  if(event.EventType == irr::EET_KEY_INPUT_EVENT)
  {   
    // capturer les touches qui ont ete appuyees:
    keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
  }
  
  return false;
}

/*------------------------------------------------------------------------*\
 * EventReceiver::mouse                                                   *
\*------------------------------------------------------------------------*/

bool EventReceiver::mouse(const SEvent &event)
{
  return false;
}

/**************************************************************************\
 * EventReceiver::OnEvent                                                 *
\**************************************************************************/
bool EventReceiver::OnEvent(const SEvent &event)
{
  switch (event.EventType)
  {
    case EET_KEY_INPUT_EVENT:
      return keyboard(event);
    case EET_MOUSE_INPUT_EVENT:
      return mouse(event);
    default:;
  }

  return false;
}

/**************************************************************************\
 * EventReceiver::is_mouse_pressed                                        *
\**************************************************************************/
bool EventReceiver::is_mouse_pressed(int &x, int &y)
{
  if (button_pressed)
  {
    x = old_x;
    y = old_y;
    return true;
  }
  return false;
}
