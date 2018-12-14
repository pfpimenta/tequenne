#include <iostream>
#include <irrlicht.h>
#include "events.h"
#include "gui.h"


#define WIDTH 640
#define HEIGHT 480
#define MIN_DIST_P1_P2 35
#define MAX_DIST_P1_P2 300
#define MUR_Z_DROITE 390
#define MUR_Z_GAUCHE -225
#define BARRES_HAUT 10
#define BARRES_BAS 30
#define BARRES_BORDS 3
#define BARRE_TAILLE_W float(WIDTH/2 - 38)
#define MIN_DIST_PUNCH 45
#define MIN_DIST_KICK 52



using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

float vitesse = 1.0f;    
ic::vector3df p1_position;
ic::vector3df p2_position;
float distance;

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

  /***************************
   * Creation de la scene
   ***************************/

  // Le gestionnaire d'événements
  EventReceiver receiver;
  // Le gestionnaire de fin d'animation
  MyAnimationEndCallback animEnd1;
  MyAnimationEndCallback animEnd2;
  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(WIDTH, HEIGHT),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver  *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();
  // irr::ITimer *timer = device->getTimer();
  
  /***************************
   * Creation du GUI
   ***************************/
  ig::IGUIEnvironment *gui  = device->getGUIEnvironment();
  iv::ITexture *blueLP = driver->getTexture("data/gui/lifePointsBleuBorde.png");
  iv::ITexture *redLP = driver->getTexture("data/gui/lifePointsRouge.png");
  
  
  /********* Life Points ********/
  float points_vie_total_p1 = 100.0f;
  float points_vie_manquant_p1 = 0.0f;
  float points_vie_total_p2 = 100.0f;
  float points_vie_manquant_p2 = 0.0f;
  float taille_barre_rouge_p1 = (points_vie_manquant_p1 / points_vie_total_p1) * BARRE_TAILLE_W;
  float taille_barre_rouge_p2 = (points_vie_manquant_p2 / points_vie_total_p2) * BARRE_TAILLE_W;

  ig::IGUIImage *barre_bleu_p1 = gui->addImage(ic::rect<s32>(10,           BARRES_HAUT, 
                                                             WIDTH/2 - 20, BARRES_BAS));
  barre_bleu_p1->setScaleImage(true);
  barre_bleu_p1->setImage(blueLP);
  
  ig::IGUIImage *barre_bleu_p2 = gui->addImage(ic::rect<s32>(WIDTH/2 + 20, BARRES_HAUT, 
                                                             WIDTH-10,     BARRES_BAS));
  barre_bleu_p2->setScaleImage(true);
  barre_bleu_p2->setImage(blueLP);
  
  ig::IGUIImage *barre_rouge_p1 = gui->addImage(ic::rect<s32>(14, BARRES_HAUT+BARRES_BORDS,
							                                                14+taille_barre_rouge_p1, BARRES_BAS-BARRES_BORDS));
  barre_rouge_p1->setScaleImage(true);
  barre_rouge_p1->setImage(redLP);
  barre_rouge_p1->setVisible(false);
  
  ig::IGUIImage *barre_rouge_p2 = gui->addImage(ic::rect<s32>(WIDTH/2 + 24, BARRES_HAUT+BARRES_BORDS,
							                                                WIDTH/2 + 24 + taille_barre_rouge_p2, BARRES_BAS-BARRES_BORDS));
  barre_rouge_p2->setScaleImage(true);
  barre_rouge_p2->setImage(redLP);
  barre_rouge_p2->setVisible(false);

  
  /***************************
   * Creation du monde
   ***************************/
  // Ajout de l'archive qui contient entre autres un niveau complet
  device->getFileSystem()->addFileArchive("data/map_oxodm3/map_oxodm3.pk3"); // cs_assault
  //device->getFileSystem()->addFileArchive("data/DBZArena.zip"); // DBZ arena

  // On charge un bsp (un niveau) en particulier :
  is::IAnimatedMesh *mesh = smgr->getMesh("oxodm3.bsp"); // cs_assault
  //is::IAnimatedMesh *mesh = smgr->getMesh("Tenkaichi_Budokai_Arena.obj"); // DBZ arena
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
  player1->setAnimationEndCallback(&animEnd1);

  // Attachement du PLAYER 2 dans la scène
  is::IAnimatedMeshSceneNode *player2 = smgr->addAnimatedMeshSceneNode(mesh_player);
  player2->setMaterialFlag(iv::EMF_LIGHTING, false);
  player2->setScale(ic::vector3df(15.0f, 15.0f, 15.0f));
  player2->setRotation(ic::vector3df(0, 0, 0));
  player2->setPosition(player2->getPosition() + ic::vector3df(0, -23.5f, 50));
  player2->setAnimationSpeed(24);
  player2->setFrameLoop(0, 12);
  player2->setAnimationEndCallback(&animEnd2);


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

  /****************************
  * Creation de la lumiere
  ***************************/
  // PENSER A ENLEVER EMAT LIGHT FALSE SI LUMIERE DANS LA SCENE
  // is::ILightSceneNode* light1 = smgr->addLightSceneNode(0, ic::vector3df(0,0,0),
  //                                                       iv::SColorf(1.0f, 1.0f, 1.0f, 0.0f), 800.0f);
  
  
  // Envoyer les noeuds et la camera a la classe EventReceiver
  receiver.player1 = player1;
  receiver.player2 = player2;
  receiver.cam = camera;

  // Booleens pour l animation
  animEnd1.enable_action = true;
  animEnd1.enable_movement = true;
  animEnd1.crouch = false;
  animEnd1.is_dead = false;
  animEnd2.enable_action = true;
  animEnd2.enable_movement = true;
  animEnd2.crouch = false;
  animEnd2.is_dead = false;


  /*************************
   * MAIN LOOP
   *************************/
  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(100,150,200,255));
    
    distance = player1->getPosition().getDistanceFrom(player2->getPosition());

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
      fight_center.Y = 35.0f;
      smgr->getActiveCamera()->setTarget(fight_center);
      // distance de la camera
      ic::vector3df offset = ic::vector3df(2.3, 0.5, 0);
      ic::vector3df new_cam_pos = fight_center + offset*(30+distance/5);
      smgr->getActiveCamera()->setPosition(new_cam_pos);
    }
    
    // Close window
    if(receiver.keys[KEY_ESCAPE])
    {
      exit(0);
    }


    /******** Personnage 1 *******/
    if (!animEnd1.is_dead)
    {
      // FIGHT
      if(receiver.keys[KEY_KEY_V]) // Coup de poing
      {
        if (animEnd1.enable_action)
        {
          if(distance < MIN_DIST_PUNCH && !receiver.keys[KEY_DOWN])
          {
            // Gestion des points de vie
            barre_rouge_p2->setVisible(true);
            points_vie_manquant_p2 += 5.0f;
            if (points_vie_manquant_p2 >= points_vie_total_p2)
            {
              // P2 mort
              points_vie_manquant_p2 = 100.0f;
              // Animation mort
              animEnd2.enable_action = false;
              animEnd2.enable_movement = false;
              animEnd2.is_dead = true;
              player2->setFrameLoop(84, 96);
              player2->setLoopMode(false);
            }
            else
            {
              // Animation blesse
              animEnd2.enable_action = false;
              animEnd2.enable_movement = false;
              player2->setFrameLoop(72, 84);
              player2->setLoopMode(false); 
            }
            taille_barre_rouge_p2 = (points_vie_manquant_p2 / points_vie_total_p2) * BARRE_TAILLE_W;
            barre_rouge_p2->setRelativePosition(ic::rect<s32>(WIDTH - 14 - taille_barre_rouge_p2, BARRES_HAUT+BARRES_BORDS, 
                                                              WIDTH - 14, BARRES_BAS-BARRES_BORDS));
          }

          // Animation coup de poing
          animEnd1.enable_action = false;
          animEnd1.enable_movement = false;
          player1->setFrameLoop(12, 24);
          player1->setLoopMode(false);
        }
      }
      if(receiver.keys[KEY_KEY_B]) // Coup de pied
      {
        if (animEnd1.enable_action)
        {
          if(distance < MIN_DIST_KICK && !receiver.keys[KEY_DOWN])
          {
            // Gestion des points de vie
            barre_rouge_p2->setVisible(true);
            points_vie_manquant_p2 += 10.0f;
            if (points_vie_manquant_p2 >= points_vie_total_p2) // P2 mort
            {
              points_vie_manquant_p2 = 100.0f;
              // Animation mort
              animEnd2.enable_action = false;
              animEnd2.enable_movement = false;
              animEnd2.is_dead = true;
              player2->setFrameLoop(84, 96);
              player2->setLoopMode(false);
            }
            else // P2 blesse
            {
              // Animation blesse
              animEnd2.enable_action = false;
              animEnd2.enable_movement = false;
              player2->setFrameLoop(72, 84);
              player2->setLoopMode(false);
            }
            taille_barre_rouge_p2 = (points_vie_manquant_p2 / points_vie_total_p2) * BARRE_TAILLE_W;
            barre_rouge_p2->setRelativePosition(ic::rect<s32>(WIDTH - 14 - taille_barre_rouge_p2, BARRES_HAUT+BARRES_BORDS, 
                                                              WIDTH - 14, BARRES_BAS-BARRES_BORDS)); 
          }

          // Animation coup de pied
          animEnd1.enable_action = false;
          animEnd1.enable_movement = false;
          player1->setFrameLoop(24, 36);
          player1->setLoopMode(false);
        }
      }

      // MOVE
      if(receiver.keys[KEY_KEY_Z]) // jump
      {
        if (animEnd1.enable_action)
        {
          // Animation
          animEnd1.enable_action = false;
          animEnd1.enable_movement = false;
          player1->setFrameLoop(60, 72);
          player1->setLoopMode(false);
        }
      }
      if(receiver.keys[KEY_KEY_S]) // crouch
      {
        if (!animEnd1.crouch)
        {
          if (animEnd1.enable_action)
          {
            // Animation
            animEnd1.enable_action = false;
            animEnd1.enable_movement = false;
            animEnd1.crouch = true;
            
            player1->setFrameLoop(48, 54);
            player1->setLoopMode(false);
          }
        }
      }
      else
      {
        animEnd1.crouch = false;
      }
      if(receiver.keys[KEY_KEY_D]) // marcher vers la droite P1
      {
        if (!animEnd1.enable_action)
          p1_position = player1->getPosition() + vitesse/3.0 * ic::vector3df(0, 0, 1);
        else
          p1_position = player1->getPosition() + vitesse * ic::vector3df(0, 0, 1);
        if (receiver.keys[KEY_KEY_Z])
          p1_position = player1->getPosition() + vitesse*1.5f * ic::vector3df(0, 0, 1);
        distance = p1_position.getDistanceFrom(player2->getPosition());
        if(distance > MIN_DIST_P1_P2 )
        {
          player1->setPosition(p1_position);
        }
        if (animEnd1.enable_movement)
        {
          // Animation
          animEnd1.enable_movement = false;
          player1->setFrameLoop(36, 48);
          player1->setLoopMode(false);
        }
      }
      if(receiver.keys[KEY_KEY_Q]) // marcher vers la gauche P1
      {
        if (!animEnd1.enable_action)
          p1_position = player1->getPosition() + vitesse/3.0 * ic::vector3df(0, 0, -1);
        else
          p1_position = player1->getPosition() + vitesse * ic::vector3df(0, 0, -1);
        if (receiver.keys[KEY_KEY_Z])
          p1_position = player1->getPosition() + vitesse*1.5f * ic::vector3df(0, 0, -1);
        distance = p1_position.getDistanceFrom(player2->getPosition());
        if(distance < MAX_DIST_P1_P2 && p1_position.Z > MUR_Z_GAUCHE)
        {
          player1->setPosition(p1_position);
        }
        if (animEnd1.enable_movement)
        {
          // Animation
          animEnd1.enable_movement = false;
          player1->setFrameLoop(36, 48);
          player1->setLoopMode(false);
        }
      }
    }
    else
    {
      std::cout << "P2 wins" << std::endl;
    }



    if (!animEnd2.is_dead)
    {
      /******** Personnage 2 *******/
      // FIGHT
      if(receiver.keys[KEY_KEY_L]) // Coup de poing
      {
        if (animEnd2.enable_action)
        {
          if(distance < MIN_DIST_PUNCH && !receiver.keys[KEY_KEY_S]) // Collision detectee si assez proche
          {
            // Gestion des points de vie
            barre_rouge_p1->setVisible(true);
            points_vie_manquant_p1 += 5.0f;
            if (points_vie_manquant_p1 >= points_vie_total_p1) // P1 mort
            {
              points_vie_manquant_p1 = 100.0f;
              // Animation mort
              animEnd1.enable_action = false;
              animEnd1.enable_movement = false;
              animEnd1.is_dead = true;
              player1->setFrameLoop(84, 96);
              player1->setLoopMode(false);
            }
            else // P1 blesse
            {
              // Animation blesse
              animEnd1.enable_action = false;
              animEnd1.enable_movement = false;
              player1->setFrameLoop(72, 84);
              player1->setLoopMode(false); 
            }
            taille_barre_rouge_p1 = (points_vie_manquant_p1 / points_vie_total_p1) * BARRE_TAILLE_W;
            barre_rouge_p1->setRelativePosition(ic::rect<s32>(14, BARRES_HAUT+BARRES_BORDS, 
                                                              14 + taille_barre_rouge_p1, BARRES_BAS-BARRES_BORDS));                                                     
          }

          // Animation coup de poing
          animEnd2.enable_action = false;
          animEnd2.enable_movement = false;
          player2->setFrameLoop(12, 24);
          player2->setLoopMode(false);
        }
      }
      if(receiver.keys[KEY_KEY_M]) // Coup de pied
      {
        if (animEnd2.enable_action)
        {
          if(distance < MIN_DIST_KICK && !receiver.keys[KEY_KEY_S])
          {
            // Gestion des points de vie
            barre_rouge_p1->setVisible(true);
            points_vie_manquant_p1 += 10.0f;
            if (points_vie_manquant_p1 >= points_vie_total_p1) // P1 mort
            {
              points_vie_manquant_p1 = 100.0f;
              // Animation mort
              animEnd1.enable_action = false;
              animEnd1.enable_movement = false;
              animEnd1.is_dead = true;
              player1->setFrameLoop(84, 96);
              player1->setLoopMode(false);
            }
            else // P1 blesse
            {
              // Animation blesse
              animEnd1.enable_action = false;
              animEnd1.enable_movement = false;
              player1->setFrameLoop(72, 84);
              player1->setLoopMode(false); 
            }
            taille_barre_rouge_p1 = (points_vie_manquant_p1 / points_vie_total_p1) * BARRE_TAILLE_W;
            barre_rouge_p1->setRelativePosition(ic::rect<s32>(14, BARRES_HAUT+BARRES_BORDS, 
                                                              14 + taille_barre_rouge_p1, BARRES_BAS-BARRES_BORDS));
          }

          // Animation coup de pied
          animEnd2.enable_action = false;
          animEnd2.enable_movement = false;
          player2->setFrameLoop(24, 36);
          player2->setLoopMode(false);
        }
      }

      // MOVE
      if(receiver.keys[KEY_UP]) // jump
      {
        if (animEnd2.enable_action)
        {
          // Animation
          animEnd2.enable_action = false;
          animEnd2.enable_movement = false;
          player2->setFrameLoop(60, 72);
          player2->setLoopMode(false);
        }
      }
      if(receiver.keys[KEY_DOWN]) // crouch
      {
        if (!animEnd2.crouch)
        {
          if (animEnd2.enable_action)
          {
            // Animation
            animEnd2.enable_action = false;
            animEnd2.enable_movement = false;
            animEnd2.crouch = true;
            player2->setFrameLoop(48, 54);
            player2->setLoopMode(false);
          }
        }
      }
      else
      {
        animEnd2.crouch = false;
      }
      if(receiver.keys[KEY_RIGHT]) // marcher vers la droite P2
      {
        if(!animEnd2.enable_action)
          p2_position = player2->getPosition() + vitesse/3.0f * ic::vector3df(0, 0, 1);
        else
          p2_position = player2->getPosition() + vitesse * ic::vector3df(0, 0, 1);
        if (receiver.keys[KEY_UP])
          p2_position = player2->getPosition() + vitesse*1.5f * ic::vector3df(0, 0, 1);
        distance = p2_position.getDistanceFrom(player1->getPosition());
        if(distance < MAX_DIST_P1_P2 && p2_position.Z < MUR_Z_DROITE)
        {
          player2->setPosition(p2_position);
        }
        if (animEnd2.enable_movement)
        {
          // Animation
          animEnd2.enable_movement = false;
          player2->setFrameLoop(36, 48);
          player2->setLoopMode(false);
        }
      }
      if(receiver.keys[KEY_LEFT]) // marcher vers la gauche P2
      {
        if(!animEnd2.enable_action)
          p2_position = player2->getPosition() + vitesse/3.0f * ic::vector3df(0, 0, -1);
        else
          p2_position = player2->getPosition() + vitesse * ic::vector3df(0, 0, -1);
        if (receiver.keys[KEY_UP])
          p2_position = player2->getPosition() + vitesse*1.5f * ic::vector3df(0, 0, -1);
        distance = p2_position.getDistanceFrom(player1->getPosition());
        if(distance > MIN_DIST_P1_P2 )
        {
          player2->setPosition(p2_position);
        }
        if (animEnd2.enable_movement)
        {
          // Animation
          animEnd2.enable_movement = false;
          player2->setFrameLoop(36, 48);
          player2->setLoopMode(false);
        }
      }
    }
    else // P2 est mort
    {
      std::cout << "P1 wins" << std::endl;
    }
    
    // Dessin de la scene :
    smgr->drawAll();
    // Dessin du gui :
    gui->drawAll();
    driver->endScene();
  }
  device->drop();

  return 0;
}
