// events.h

#ifndef EVENTS_H
#define EVENTS_H

#include <vector>

#include <irrlicht.h>


namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;


class EventReceiver : public irr::IEventReceiver
{
	bool button_pressed;
	int old_x, old_y;
	
	float vitesse = 1.75;    

	char new_animation = 's'; // 's': stand, 'r': run, 'b': back
	char old_animation = 's';
	


	bool keyboard(const irr::SEvent &event);
	bool mouse(const irr::SEvent &event);
  public:
	EventReceiver();
	is::IAnimatedMeshSceneNode *node;
	is::IAnimatedMeshSceneNode *player1;
	is::IAnimatedMeshSceneNode *player2;
	irr::scene::ICameraSceneNode* cam_combat;
	irr::scene::ICameraSceneNode* cam_freelook;
	bool OnEvent(const irr::SEvent &event);
	bool is_mouse_pressed(int &x, int &y);
	bool change_cam = false;
	bool keys[sizeof(irr::EKEY_CODE)];
};

#endif
