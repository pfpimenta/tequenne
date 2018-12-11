#include <iostream>
#include <irrlicht.h>

#define WIDTH 640
#define HEIGHT 480


#include "events.h"
#include "gui.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;


int main()
{
  // Le gestionnaire d'événements
  EventReceiver receiver;
  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(WIDTH, HEIGHT),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver  *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();
  // Le GUI
  //gui = device->getGUIEnvironment();
  ig::IGUIEnvironment *gui  = device->getGUIEnvironment();
  iv::ITexture *blueLP = driver->getTexture("data/gui/lifePointsBleuBorde.png");
  iv::ITexture *redLP = driver->getTexture("data/gui/lifePointsRouge.png");
  
  ig::IGUIImage *barre_p1 = gui->addImage(ic::rect<s32>(10,10, WIDTH/2 - 10,50));
  barre_p1->setScaleImage(true);
  barre_p1->setImage(blueLP);
  
  ig::IGUIImage *barre_p2 = gui->addImage(ic::rect<s32>(WIDTH/2,10, WIDTH - 10,50));
  barre_p2->setScaleImage(true);
  barre_p2->setImage(blueLP);

  // Ajout de l'archive qui contient entre autres un niveau complet
  //device->getFileSystem()->addFileArchive("data/data_tp/map-20kdm2.pk3");
  //device->getFileSystem()->addFileArchive("data/riotarena/riotarena.pk3"); // ne marche pas: 0 nodes
  device->getFileSystem()->addFileArchive("data/map_oxodm3/map_oxodm3.pk3"); // cs_assault
  //device->getFileSystem()->addFileArchive("data/DBZArena.zip"); // DBZ arena
  //device->getFileSystem()->addFileArchive("data/tig_den/tig_den.pk3"); // moche / bugged

  // On charge un bsp (un niveau) en particulier :
  //is::IAnimatedMesh *mesh = smgr->getMesh("20kdm2.bsp");
  //is::IAnimatedMesh *mesh = smgr->getMesh("riotarena.bsp"); // ne marche pas
  is::IAnimatedMesh *mesh = smgr->getMesh("oxodm3.bsp"); // cs_assault
  //is::IAnimatedMesh *mesh = smgr->getMesh("Tenkaichi_Budokai_Arena.obj"); // DBZ arena
  //is::IAnimatedMesh *mesh = smgr->getMesh("tig_den.bsp"); // moche / bugged
  is::ISceneNode *node;
  node = smgr->addOctreeSceneNode(mesh->getMesh(0), nullptr, -1, 1024);
  //node->setMaterialFlag(iv::EMF_LIGHTING, false); // decommenter en dependant du map
  //node->setMaterialType(iv::EMT_TRANSPARENT_ALPHA_CHANNEL); // decommenter en dependant du map
  // Translation pour que nos personnages soient dans le décor
  node->setPosition(core::vector3df(400,-23,-1200));
  

  // Chargement de nos players
  is::IAnimatedMesh *mesh_player1 = smgr->getMesh("data/data_tp/tris.md2");
  is::IAnimatedMesh *mesh_player2 = smgr->getMesh("data/data_tp/tris.md2");

  // Attachement du PLAYER 1 dans la scène
  is::IAnimatedMeshSceneNode *player1 = smgr->addAnimatedMeshSceneNode(mesh_player1);
  player1->setMaterialFlag(iv::EMF_LIGHTING, false);
  player1->setMD2Animation(is::EMAT_STAND);
  player1->setMaterialTexture(0, driver->getTexture("data/data_tp/blue_texture.pcx"));
  player1->setRotation(ic::vector3df(0, -90, 0));
  player1->setPosition(player1->getPosition() + ic::vector3df(0, 0, -50));

  // Attachement du PLAYER 2 dans la scène
  is::IAnimatedMeshSceneNode *player2 = smgr->addAnimatedMeshSceneNode(mesh_player2);
  player2->setMaterialFlag(iv::EMF_LIGHTING, false);
  player2->setMD2Animation(is::EMAT_STAND);
  player2->setMaterialTexture(0, driver->getTexture("data/data_tp/red_texture.pcx"));
  player2->setRotation(ic::vector3df(0, 90, 0));
  player2->setPosition(player2->getPosition() + ic::vector3df(0, 0, 50));

  // camera mode COMBAT
  is::ICameraSceneNode* camera_combat = smgr->addCameraSceneNode(nullptr, 30*ic::vector3df(2.3, 0.5, 0), ic::vector3df(0, 5, 0));
  // Mode FREELOOK "superman camera"
  is::ICameraSceneNode* camera_freelook = smgr->addCameraSceneNodeFPS();

  
  
  // send node and cam to EventReceiver class
  receiver.node = player1;
  receiver.player1 = player1;
  receiver.player2 = player2;
  receiver.cam_combat = camera_combat;
  receiver.cam_freelook = camera_freelook;
  
  // current camera mode flag
  char camera_mode = 'c'; // 'c' combat ou 'f' freelook
  smgr->setActiveCamera(camera_combat);

  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(100,150,200,255));
    
    float distance_players = player1->getPosition().getDistanceFrom(player2->getPosition());
    
    // make the camera follow the center of the fight
    if(camera_mode == 'c'){
	// direction de la camera
	ic::vector3df fight_center = (player1->getPosition() + player2->getPosition())/2.0f;
	smgr->getActiveCamera()->setTarget(fight_center);
	// distance de la camera
	ic::vector3df offset = ic::vector3df(2.3, 0.5, 0);
	ic::vector3df new_cam_pos = fight_center + offset*(30+distance_players/5);
	smgr->getActiveCamera()->setPosition(new_cam_pos);
    }
    
    // debug print camera position: 
    //ic::vector3df cam_pos = smgr->getActiveCamera()->getPosition();
    //std::cout <<"debug cam pos: " << cam_pos.X << ", " << cam_pos.Y  << ", " << cam_pos.Z <<std::endl;
      
    // change camera if C key was pressed
    if(receiver.change_cam == true){
      receiver.change_cam = false;
      if(camera_mode == 'f'){
	smgr->setActiveCamera(camera_combat);
	camera_mode = 'c';
      }else if(camera_mode == 'c'){
	smgr->setActiveCamera(camera_freelook);
	camera_mode = 'f';
      }
    }
    
    // Dessin de la scène :
    smgr->drawAll();
    //  dessin de la gui
    gui->drawAll();
    driver->endScene();
  }
  device->drop();

  return 0;
}
