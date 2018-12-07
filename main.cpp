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
  is::IAnimatedMesh *mesh_perso = smgr->getMesh("data/data_tp/tris.md2");

  // Attachement de notre personnage dans la scène
  is::IAnimatedMeshSceneNode *perso = smgr->addAnimatedMeshSceneNode(mesh_perso);
  perso->setMaterialFlag(iv::EMF_LIGHTING, false);
  perso->setMD2Animation(is::EMAT_STAND);
  perso->setMaterialTexture(0, driver->getTexture("data/data_tp/blue_texture.pcx"));



  // camera mode COMBAT // TODO
  //is::ICameraSceneNode* camera = smgr->addCameraSceneNode(nullptr, ic::vector3df(0, 30, -70), ic::vector3df(0, 5, 0));
  // Mode FREELOOK "superman camera"
  is::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();

  
  
  // send node and cam to EventReceiver class
  receiver.node = perso;
  receiver.cam = camera;

  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(100,150,200,255));

    
    // make the camera follow the center of the fight
    //ic::vector3df fight_center = ic::vector3df(0, 0, 0); //TODO
    //smgr->getActiveCamera()->setTarget(fight_center);
    
    
    // Dessin de la scène :
    smgr->drawAll();
    // 
    driver->endScene();
  }
  device->drop();

  return 0;
}
