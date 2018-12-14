// events.h

#ifndef EVENTS_H
#define EVENTS_H

#include <vector>

#include <irrlicht.h>



namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;


class MyAnimationEndCallback : public is::IAnimationEndCallBack
{
	public:
		void OnAnimationEnd(is::IAnimatedMeshSceneNode *node)
		{
			if (crouch)
			{
				node->setLoopMode(false);
				node->setCurrentFrame(54);
			}
			else if (is_dead)
			{
				node->setLoopMode(false);
				node->setCurrentFrame(96);
			}
			else
			{
				node->setLoopMode(true);
				node->setFrameLoop(0, 12); // Idle
			}
			enable_action = true;
			enable_movement = true;
			return;
		}
		bool enable_action;
		bool enable_movement;
		bool crouch;
		bool is_dead;
};

class EventReceiver : public irr::IEventReceiver
{
	bool button_pressed;
	int old_x, old_y;

	bool keyboard(const irr::SEvent &event);
	bool mouse(const irr::SEvent &event);

	public:
		EventReceiver();
		is::IAnimatedMeshSceneNode *player1;
		is::IAnimatedMeshSceneNode *player2;
		irr::scene::ICameraSceneNode* cam;
		bool OnEvent(const irr::SEvent &event);
		bool is_mouse_pressed(int &x, int &y);
		bool keys[irr::KEY_KEY_CODES_COUNT];
		char new_animationP1;
		char old_animationP1;
		char new_animationP2;
		char old_animationP2;
		bool actionP1 = false;
		bool actionP2 = false;
};

#endif
