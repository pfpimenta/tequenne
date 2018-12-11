// events.cpp
#include <iostream>
#include <irrlicht.h>

#include "events.h"

#define MIN_DIST_P1_P2 23
#define MAX_DIST_P1_P2 200
#define MUR_Z_DROITE 390
#define MUR_Z_GAUCHE -240

using namespace irr;


/**************************************************************************\
 * EventReceiver::EventReceiver                                           *
\**************************************************************************/
EventReceiver::EventReceiver()
  : button_pressed(false)
{
  //initialization du array keys
  for(int x=0; x< sizeof(irr::EKEY_CODE); x++) keys[x] = false;
}

/*------------------------------------------------------------------------*\
 * EventReceiver::keyboard                                                *
\*------------------------------------------------------------------------*/
bool EventReceiver::keyboard(const SEvent &event)
{
    ic::vector3df p1_position;
    ic::vector3df p2_position;
    float distance;
    
    if(event.EventType == irr::EET_KEY_INPUT_EVENT){
	keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
	if(keys[KEY_ESCAPE]){
           exit(0);
	}
	if( keys[KEY_KEY_C]){ // change camera: combat <-> freelook
	  change_cam = true;
	}
        if( keys[KEY_KEY_Z]){  // jump P1
	  new_animation = 'r';
	}
        if( keys[KEY_KEY_S]){  // crouch P1
	  new_animation = 'r';
	}
        if( keys[KEY_KEY_D]){ // marcher vers la droite P1
	  p1_position = player1->getPosition() + vitesse * ic::vector3df(0, 0, 1);
	  distance = p1_position.getDistanceFrom(player2->getPosition());
	  if(distance > MIN_DIST_P1_P2 ){
	      player1->setPosition(p1_position);
	  }
	  new_animation = 'r';
	}
        if( keys[KEY_KEY_Q]){ // marcher vers la gauche P1
	  p1_position = player1->getPosition() + vitesse * ic::vector3df(0, 0, -1);
	  distance = p1_position.getDistanceFrom(player2->getPosition());
	  if(distance < MAX_DIST_P1_P2 && p1_position.Z > MUR_Z_GAUCHE){
	      player1->setPosition(p1_position);
	  }
	  new_animation = 'r';
	}
        if( keys[KEY_UP]){ // jump P2
	  new_animation = 'r';
	}
        if( keys[KEY_DOWN]){ // crouch P2
	  new_animation = 'r';
	}
        if( keys[KEY_RIGHT]){ // marcher vers la droite P2
	  p2_position = player2->getPosition() + vitesse * ic::vector3df(0, 0, 1);
	  distance = p2_position.getDistanceFrom(player1->getPosition());
	  if(distance < MAX_DIST_P1_P2 && p2_position.Z < MUR_Z_DROITE){
	      player2->setPosition(p2_position);
	  }
	  new_animation = 'r';
	}
        if( keys[KEY_LEFT]){ // marcher vers la gauche P2
	  p2_position = player2->getPosition() + vitesse * ic::vector3df(0, 0, -1);
	  distance = p2_position.getDistanceFrom(player1->getPosition());
	  if(distance > MIN_DIST_P1_P2 ){
	      player2->setPosition(p2_position);
	  }
	  new_animation = 'r';
	}
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
