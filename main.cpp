#include <iostream>
#include <irrlicht.h>
#include "events.h"
#include "gui.h"


#define WIDTH 640
#define HEIGHT 480
#define MIN_DIST_P1_P2 35
#define MAX_DIST_P1_P2 250
#define MUR_Z_DROITE 390
#define MUR_Z_GAUCHE -240
#define BARRES_HAUT 10
#define BARRES_BAS 50
#define BARRES_BORDE 3
#define BARRE_TAILLE_H WIDTH/2 - 20



using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

char new_animation = 's'; // 's': stand, 'r': run, 'b': back
char old_animation = 's';
bool change_cam = false;
float vitesse = 1.25;    
ic::vector3df p1_position;
ic::vector3df p2_position;
float distance;

float points_vie_p1 = 100.0f;
float points_vie_p2 = 100.0f;
float taille_barre_rouge_p1 = (100.0f - points_vie_p1) * BARRE_TAILLE_H;
float taille_barre_rouge_p2 = (100.0f - points_vie_p2) * BARRE_TAILLE_H;



int main(int argc, char **argv)
{
  /*************************
   * Parseur
   ************************/
  bool debug = false;
  if (argc > 1)
  {
    if (std::string(argv[1]) == "debug")
    {
      debug = true;
    }
  }
  std::cout << argc << ", " << argv[argc-1] << std::endl;
  std::cout << "debug = " << debug << std::endl;

  /***************************
   * Creation de la scene
   ***************************/
  // Le gestionnaire d'événements
  EventReceiver receiver;
  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(WIDTH, HEIGHT),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver  *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();
  
  /***************************
   * Creation du GUI
   ***************************/
  ig::IGUIEnvironment *gui  = device->getGUIEnvironment();
  iv::ITexture *blueLP = driver->getTexture("data/gui/lifePointsBleuBorde.png");
  iv::ITexture *redLP = driver->getTexture("data/gui/lifePointsRouge.png");
  
  
  ig::IGUIImage *barre_bleu_p1 = gui->addImage(ic::rect<s32>(10,BARRES_HAUT, WIDTH/2 - 10,BARRES_BAS));
  barre_bleu_p1->setScaleImage(true);
  barre_bleu_p1->setImage(blueLP);
  
  ig::IGUIImage *barre_bleu_p2 = gui->addImage(ic::rect<s32>(WIDTH/2,BARRES_HAUT, WIDTH - 10,BARRES_BAS));
  barre_bleu_p2->setScaleImage(true);
  barre_bleu_p2->setImage(blueLP);
  
  ig::IGUIImage *barre_rouge_p1 = gui->addImage(ic::rect<s32>(14,BARRES_HAUT+BARRES_BORDE,
							      14+taille_barre_rouge_p1,45));
  barre_rouge_p1->setScaleImage(true);
  barre_rouge_p1->setImage(redLP);
  barre_rouge_p1->setVisible(false);
  
  ig::IGUIImage *barre_rouge_p2 = gui->addImage(ic::rect<s32>(WIDTH/2 + 4,BARRES_HAUT+BARRES_BORDE,
							      WIDTH/2 + 4 + taille_barre_rouge_p2,45));
  barre_rouge_p2->setScaleImage(true);
  barre_rouge_p2->setImage(redLP);
  //barre_rouge_p2->setVisible(false);
  // TODO finir ca 
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  barre_rouge_p2->setRelativePosition(ic::rect<s32>(WIDTH/2 + 4,15, WIDTH - 100,45));
  
  
  /***************************
   * Creation du monde
   ***************************/
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
  

  /***************************
   * Creation des personnages
   ***************************/
  // Chargement du mesh (une seule fois)
  is::IAnimatedMesh *mesh_player = smgr->getMesh("data/personnages/Heihachi/heihachi.x");

  // Attachement du PLAYER 1 dans la scène
  is::IAnimatedMeshSceneNode *player1 = smgr->addAnimatedMeshSceneNode(mesh_player);
  player1->setMaterialFlag(iv::EMF_LIGHTING, false);
  player1->setScale(ic::vector3df(15.0f, 15.0f, 15.0f));
  player1->setRotation(ic::vector3df(0, -180, 0));
  player1->setPosition(player1->getPosition() + ic::vector3df(0, -23.5f, -50));
  player1->setAnimationSpeed(24);
  player1->setFrameLoop(0, 12);

  // Attachement du PLAYER 2 dans la scène
  is::IAnimatedMeshSceneNode *player2 = smgr->addAnimatedMeshSceneNode(mesh_player);
  player2->setMaterialFlag(iv::EMF_LIGHTING, false);
  player2->setScale(ic::vector3df(15.0f, 15.0f, 15.0f));
  player2->setRotation(ic::vector3df(0, 0, 0));
  player2->setPosition(player2->getPosition() + ic::vector3df(0, -23.5f, 50));
  player2->setAnimationSpeed(24);
  player2->setFrameLoop(0, 12);


  /***************************
   * Creation de la camera
   ***************************/
  is::ICameraSceneNode* camera;
  if (debug)
  {
    // Mode FREELOOK "superman camera"
    camera = smgr->addCameraSceneNodeFPS();
  }
  else
  {
    // camera mode COMBAT
    camera = smgr->addCameraSceneNode(nullptr, 30*ic::vector3df(2.3, 0.5, 0), ic::vector3df(0, 5, 0));
  }
  smgr->setActiveCamera(camera);
  
  
  // send nodes and cam to EventReceiver class
  receiver.player1 = player1;
  receiver.player2 = player2;
  receiver.cam = camera;

  // Animation de déplacement
  int frame_id = 0;
  int frame = 36;

  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(100,150,200,255));
    
    float distance_players = player1->getPosition().getDistanceFrom(player2->getPosition());

    /******** CAMERA **********/
    if(debug) // Free look
    {
      // debug print camera position: 
      ic::vector3df cam_pos = smgr->getActiveCamera()->getPosition();
      std::cout <<"debug cam pos: " << cam_pos.X << ", " << cam_pos.Y  << ", " << cam_pos.Z <<std::endl;
    }
    else // make the camera follow the center of the fight
    {
      // direction de la camera
      ic::vector3df fight_center = (player1->getPosition() + player2->getPosition())/2.0f;
      fight_center.Y = 45.0f;
      smgr->getActiveCamera()->setTarget(fight_center);
      // distance de la camera
      ic::vector3df offset = ic::vector3df(2.3, 0.5, 0);
      ic::vector3df new_cam_pos = fight_center + offset*(30+distance_players/5);
      smgr->getActiveCamera()->setPosition(new_cam_pos);
    }
    
    if(receiver.keys[KEY_ESCAPE])
    {
      exit(0);
    }

    /******** Personnage 1 *******/
    if( receiver.keys[KEY_KEY_Z]){  // jump P1
      new_animation = 'r';
    }

    if( receiver.keys[KEY_KEY_S]){  // crouch P1
      new_animation = 'r';
    }

    if( receiver.keys[KEY_KEY_D]){ // marcher vers la droite P1
      frame = frame_id % 13 + 36;
      frame_id ++; // ADD TIME ELAPSED CONTROL
      player1->setCurrentFrame(frame);
      p1_position = player1->getPosition() + vitesse * ic::vector3df(0, 0, 1);
      distance = p1_position.getDistanceFrom(player2->getPosition());
      if(distance > MIN_DIST_P1_P2 )
      {
        player1->setPosition(p1_position);
      }
    }

    if( receiver.keys[KEY_KEY_Q]) // marcher vers la gauche P1
    {
      player1->setFrameLoop(36, 48);
      player1->setLoopMode(false);
      p1_position = player1->getPosition() + vitesse * ic::vector3df(0, 0, -1);
      distance = p1_position.getDistanceFrom(player2->getPosition());
      if(distance < MAX_DIST_P1_P2 && p1_position.Z > MUR_Z_GAUCHE)
      {
        player1->setPosition(p1_position);
      }
    }

    /******** Personnage 2 *******/
    if( receiver.keys[KEY_UP]) // jump P2
    {
      new_animation = 'r';
    }
    if( receiver.keys[KEY_DOWN]) // crouch P2
    {
      new_animation = 'r';
    }

    if( receiver.keys[KEY_RIGHT]) // marcher vers la droite P2
    {
      player2->setFrameLoop(36, 48);
      player2->setLoopMode(false);
      p2_position = player2->getPosition() + vitesse * ic::vector3df(0, 0, 1);
      distance = p2_position.getDistanceFrom(player1->getPosition());
      if(distance < MAX_DIST_P1_P2 && p2_position.Z < MUR_Z_DROITE)
      {
        player2->setPosition(p2_position);
      }
    }

    if( receiver.keys[KEY_LEFT]) // marcher vers la gauche P2
    {
      player2->setFrameLoop(36, 48);
      player2->setLoopMode(false);
      p2_position = player2->getPosition() + vitesse * ic::vector3df(0, 0, -1);
      distance = p2_position.getDistanceFrom(player1->getPosition());
      if(distance > MIN_DIST_P1_P2 )
      {
        player2->setPosition(p2_position);
      }
    }
    
    // Dessin de la scene :
    smgr->drawAll();
    //  dessin du gui
    gui->drawAll();
    driver->endScene();
  }
  device->drop();

  return 0;
}
