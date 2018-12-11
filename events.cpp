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
  
}

/*------------------------------------------------------------------------*\
 * EventReceiver::keyboard                                                *
\*------------------------------------------------------------------------*/
bool EventReceiver::keyboard(const SEvent &event)
{
    // Si l'événement est de type clavier (KEY_INPUT)
    // et du genre pressage de touche
    // et que la touche est escape, on sort du programme
    if (event.EventType == EET_KEY_INPUT_EVENT &&
        event.KeyInput.PressedDown)
    {
      switch (event.KeyInput.Key)
      {
        case KEY_ESCAPE:
          exit(0);
	case KEY_KEY_C: // change camera: combat <-> freelook
	  change_cam = true;
          break;
        case KEY_KEY_Z: // jump P1
	  new_animation = 'r';
          break;
        case KEY_KEY_S: // crouch P1
	  new_animation = 'r';
          break;
        case KEY_KEY_D: // marcher vers la droite P1
	  new_animation = 'r';
          break;
        case KEY_KEY_Q: // marcher vers la gauche P1
	  new_animation = 'r';
	  break;
        default:;
      }
    }else{
	new_animation = 's'; // stop running
    }
    
        // character animation // TODO faire ca pour P1 et P2
//     if(new_animation != old_animation){
//       switch(new_animation){
// 	case 's':
// 	  perso->setMD2Animation(is::EMAT_STAND);
// 	  break;
// 	case 'r':
// 	  perso->setMD2Animation(is::EMAT_RUN);
// 	  break;
// 	case 'b':
// 	  perso->setMD2Animation(is::EMAT_BOOM);
// 	  break;  
//       }
//       old_animation = new_animation;
//     }


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
