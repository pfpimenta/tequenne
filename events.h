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
	irr::scene::ICameraSceneNode* cam;
	bool OnEvent(const irr::SEvent &event);
	bool is_mouse_pressed(int &x, int &y);
};

#endif
