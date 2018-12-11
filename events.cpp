// events.cpp
#include <iostream>
#include <irrlicht.h>

#include "events.h"

#define MIN_DIST_P1_P2 23
#define MAX_DIST_P1_P2 200

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
    ic::vector3df p1_position;
    ic::vector3df p2_position;
    float distance;

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
	  p1_position = player1->getPosition() + vitesse * ic::vector3df(0, 0, 1);
	  distance = p1_position.getDistanceFrom(player2->getPosition());
	  if(distance > MIN_DIST_P1_P2 ){
	      player1->setPosition(p1_position);
	  }
	  new_animation = 'r';
          break;
        case KEY_KEY_Q: // marcher vers la gauche P1
	  p1_position = player1->getPosition() + vitesse * ic::vector3df(0, 0, -1);
	  distance = p1_position.getDistanceFrom(player2->getPosition());
	  if(distance < MAX_DIST_P1_P2 ){
	      player1->setPosition(p1_position);
	  }
	  new_animation = 'r';
	  break;
        case KEY_UP: // jump P2
	  new_animation = 'r';
          break;
        case KEY_DOWN: // crouch P2
	  new_animation = 'r';
          break;
        case KEY_RIGHT: // marcher vers la droite P2
	  p2_position = player2->getPosition() + vitesse * ic::vector3df(0, 0, 1);
	  distance = p2_position.getDistanceFrom(player1->getPosition());
	  if(distance < MAX_DIST_P1_P2 ){
	      player2->setPosition(p2_position);
	  }
	  new_animation = 'r';
          break;
        case KEY_LEFT: // marcher vers la gauche P2
	  p2_position = player2->getPosition() + vitesse * ic::vector3df(0, 0, -1);
	  distance = p2_position.getDistanceFrom(player1->getPosition());
	  if(distance > MIN_DIST_P1_P2 ){
	      player2->setPosition(p2_position);
	  }
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
