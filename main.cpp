#include <iostream>
#include <irrlicht.h>

#include "events.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

int main()
{
  // Le gestionnaire d'événements
  EventReceiver receiver;
  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(640, 480),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver  *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();

  // Ajout de l'archive qui contient entre autres un niveau complet
  //device->getFileSystem()->addFileArchive("data/data_tp/map-20kdm2.pk3");
  //device->getFileSystem()->addFileArchive("data/riotarena/riotarena.pk3"); // ne marche pas: 0 nodes
  device->getFileSystem()->addFileArchive("data/map_oxodm3/map_oxodm3.pk3"); // cs_assault
  //device->getFileSystem()->addFileArchive("data/tig_den/tig_den.pk3"); // moche / bugged

  // On charge un bsp (un niveau) en particulier :
  //is::IAnimatedMesh *mesh = smgr->getMesh("20kdm2.bsp");
  //is::IAnimatedMesh *mesh = smgr->getMesh("riotarena.bsp"); // ne marche pas
  is::IAnimatedMesh *mesh = smgr->getMesh("oxodm3.bsp"); // cs_assault
  //is::IAnimatedMesh *mesh = smgr->getMesh("tig_den.bsp"); // moche / bugged
  is::ISceneNode *node;
  node = smgr->addOctreeSceneNode(mesh->getMesh(0), nullptr, -1, 1024);
  // Translation pour que nos personnages soient dans le décor
  //node->setPosition(core::vector3df(-1300,-104,-1249));

  // Chargement de notre personnage (réutilisé plusieurs fois)
  is::IAnimatedMesh *mesh_player1 = smgr->getMesh("data/data_tp/tris.md2");
  is::IAnimatedMesh *mesh_player2 = smgr->getMesh("data/data_tp/tris.md2");

  // Attachement du PLAYER 1 dans la scène
  is::IAnimatedMeshSceneNode *player1 = smgr->addAnimatedMeshSceneNode(mesh_player1);
  player1->setMaterialFlag(iv::EMF_LIGHTING, false);
  player1->setMD2Animation(is::EMAT_STAND);
  player1->setMaterialTexture(0, driver->getTexture("data/data_tp/blue_texture.pcx"));

  // Attachement du PLAYER 2 dans la scène
  is::IAnimatedMeshSceneNode *player2 = smgr->addAnimatedMeshSceneNode(mesh_player2);
  player2->setMaterialFlag(iv::EMF_LIGHTING, false);
  player2->setMD2Animation(is::EMAT_STAND);
  player2->setMaterialTexture(0, driver->getTexture("data/data_tp/red_texture.pcx"));



  // camera mode COMBAT // TODO
  is::ICameraSceneNode* camera_combat = smgr->addCameraSceneNode(nullptr, ic::vector3df(0, 30, -70), ic::vector3df(0, 5, 0));
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

  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(100,150,200,255));

    
    // make the camera follow the center of the fight
    //ic::vector3df fight_center = ic::vector3df(0, 0, 0); //TODO
    //smgr->getActiveCamera()->setTarget(fight_center);
    
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
    // 
    driver->endScene();
  }
  device->drop();

  return 0;
}
