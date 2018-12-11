// events.h

#ifndef GUI_H
#define GUI_H

#include <vector>

#include <irrlicht.h>


namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;


class Gui : public ig::IGUIEnvironment
{
	bool isActivated;
  public:
	Gui();
	std::vector<iv::ITexture*> gui_textures;
};

#endif
