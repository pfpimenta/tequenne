#include <iostream>
#include <irrlicht.h>
#include "events.h"


#define WIDTH 850
#define HEIGHT 480
#define MIN_DIST_P1_P2 35
#define MAX_DIST_P1_P2 300
#define MUR_Z_DROITE 390
#define MUR_Z_GAUCHE -225
#define BARRES_HAUT 10
#define BARRES_BAS 30
#define BARRES_BORDS 3
#define BARRE_TAILLE_W float(WIDTH/2 - 58)
#define MIN_DIST_PUNCH 45
#define MIN_DIST_KICK 52



using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

// Variables qui gerent la position des personnages
float vitesse = 1.25f;    
ic::vector3df p1_position;
ic::vector3df p2_position;
float distance;

// Variables qui gerent les points de vie du personnage (GUI)
int chrono = 40;
int round_id = 1;
bool reset_round = false;
bool end_timer = false;
bool end_round = false;
bool end_game = false;
bool start_game = false;
bool display_win = true;
bool win_p1 = false;
bool win_p2 = false;
int nb_win_p1 = 0;
int nb_win_p2 = 0;
float points_vie_total_p1 = 100.0f;
float points_vie_manquant_p1 = 0.0f;
float points_vie_total_p2 = 100.0f;
float points_vie_manquant_p2 = 0.0f;
float taille_barre_rouge_p1 = (points_vie_manquant_p1 / points_vie_total_p1) * BARRE_TAILLE_W;
float taille_barre_rouge_p2 = (points_vie_manquant_p2 / points_vie_total_p2) * BARRE_TAILLE_W;

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
  ig::IGUIEnvironment *gui  = device->getGUIEnvironment();
  irr::ITimer *timer = device->getTimer();
  

  /***************************
   * Creation du GUI
   ***************************/
  iv::ITexture *blueLP = driver->getTexture("data/gui/lifePointsBleuBorde.png");
  iv::ITexture *redLP = driver->getTexture("data/gui/lifePointsRouge.png");
  
  /********* Life Points ********/
  ig::IGUIImage *barre_bleu_p1 = gui->addImage(ic::rect<s32>(10,           BARRES_HAUT, 
                                                            WIDTH/2 - 40,  BARRES_BAS));
  barre_bleu_p1->setScaleImage(true);
  barre_bleu_p1->setImage(blueLP);
  
  ig::IGUIImage *barre_bleu_p2 = gui->addImage(ic::rect<s32>(WIDTH/2 + 40, BARRES_HAUT, 
                                                            WIDTH-10,      BARRES_BAS));
  barre_bleu_p2->setScaleImage(true);
  barre_bleu_p2->setImage(blueLP);

  ig::IGUIImage *barre_rouge_p1 = gui->addImage(ic::rect<s32>(14,                       BARRES_HAUT+BARRES_BORDS,
                                                              14+taille_barre_rouge_p1, BARRES_BAS-BARRES_BORDS));
  barre_rouge_p1->setScaleImage(true);
  barre_rouge_p1->setImage(redLP);
  barre_rouge_p1->setVisible(false);
  
  ig::IGUIImage *barre_rouge_p2 = gui->addImage(ic::rect<s32>(WIDTH/2 + 44,                         BARRES_HAUT+BARRES_BORDS,
                                                              WIDTH/2 + 44 + taille_barre_rouge_p2, BARRES_BAS-BARRES_BORDS));
  barre_rouge_p2->setScaleImage(true);
  barre_rouge_p2->setImage(redLP);
  barre_rouge_p2->setVisible(false);

  /******* Names ********/
  iv::ITexture *name = driver->getTexture("data/gui/heihachi_name.png");

  ig::IGUIImage *name_p1 = gui->addImage(ic::rect<s32>(10,  BARRES_BAS + 3, 
                                                       110, BARRES_BAS + 18));
  name_p1->setScaleImage(true);
  name_p1->setImage(name);

  ig::IGUIImage *name_p2 = gui->addImage(ic::rect<s32>(WIDTH - 110, BARRES_BAS + 3, 
                                                       WIDTH - 10,  BARRES_BAS + 18));
  name_p2->setScaleImage(true);
  name_p2->setImage(name);

  /******* Rounds *******/
  iv::ITexture *empty_count = driver->getTexture("data/gui/empty_count.png");
  iv::ITexture *full_count = driver->getTexture("data/gui/full_count.png");

  ig::IGUIImage *p1_round1 = gui->addImage(ic::rect<s32>(WIDTH/2 - 60, BARRES_BAS + 3, 
                                                         WIDTH/2 - 40, BARRES_BAS + 18));
  p1_round1->setScaleImage(true);
  p1_round1->setImage(empty_count);

  ig::IGUIImage *p1_round2 = gui->addImage(ic::rect<s32>(WIDTH/2 - 80, BARRES_BAS + 3, 
                                                         WIDTH/2 - 60, BARRES_BAS + 18));
  p1_round2->setScaleImage(true);
  p1_round2->setImage(empty_count);

  ig::IGUIImage *p2_round1 = gui->addImage(ic::rect<s32>(WIDTH/2 + 40, BARRES_BAS + 3, 
                                                         WIDTH/2 + 60, BARRES_BAS + 18));
  p2_round1->setScaleImage(true);
  p2_round1->setImage(empty_count);

  ig::IGUIImage *p2_round2 = gui->addImage(ic::rect<s32>(WIDTH/2 + 60, BARRES_BAS + 3, 
                                                         WIDTH/2 + 80, BARRES_BAS + 18));
  p2_round2->setScaleImage(true);
  p2_round2->setImage(empty_count);

  /****** Timer *******/
  iv::ITexture *digits[10];
  digits[0] = driver->getTexture("data/gui/0.png");
  digits[1] = driver->getTexture("data/gui/1.png");
  digits[2] = driver->getTexture("data/gui/2.png");
  digits[3] = driver->getTexture("data/gui/3.png");
  digits[4] = driver->getTexture("data/gui/4.png");
  digits[5] = driver->getTexture("data/gui/5.png");
  digits[6] = driver->getTexture("data/gui/6.png");
  digits[7] = driver->getTexture("data/gui/7.png");
  digits[8] = driver->getTexture("data/gui/8.png");
  digits[9] = driver->getTexture("data/gui/9.png");

  ig::IGUIImage *chrono_10 = gui->addImage(ic::rect<s32>(WIDTH/2 - 35, 10,
                                                         WIDTH/2,      50)); 
  chrono_10->setScaleImage(true);
  ig::IGUIImage *chrono_1  = gui->addImage(ic::rect<s32>(WIDTH/2,      10, 
                                                         WIDTH/2 + 35, 50)); 
  chrono_1->setScaleImage(true);

  /******* Annonces *******/
  iv::ITexture *round1 = driver->getTexture("data/gui/round_1.png");
  iv::ITexture *round2 = driver->getTexture("data/gui/round_2.png");
  iv::ITexture *finalround = driver->getTexture("data/gui/final_round.png");
  iv::ITexture *ready = driver->getTexture("data/gui/ready.png");
  iv::ITexture *fight = driver->getTexture("data/gui/fight.png");

  ig::IGUIImage *rounds = gui->addImage(ic::rect<s32>(WIDTH/2 - 250, HEIGHT/2 - 100, 
                                                      WIDTH/2 + 250, HEIGHT/2 - 40));
  rounds->setScaleImage(true);
  rounds->setImage(round1);
  rounds->setVisible(false);

  ig::IGUIImage *ready_text = gui->addImage(ic::rect<s32>(WIDTH/2 - 250, HEIGHT/2 - 100, 
                                                          WIDTH/2 + 250, HEIGHT/2 - 40));
  ready_text->setScaleImage(true);
  ready_text->setImage(ready);
  ready_text->setVisible(false);

  ig::IGUIImage *fight_text = gui->addImage(ic::rect<s32>(WIDTH/2 - 250, HEIGHT/2 - 100, 
                                                          WIDTH/2 + 250, HEIGHT/2 - 40));
  fight_text->setScaleImage(true);
  fight_text->setImage(fight);
  fight_text->setVisible(false);

  iv::ITexture *ko = driver->getTexture("data/gui/KO.png");
  ig::IGUIImage *ko_text = gui->addImage(ic::rect<s32>(WIDTH/2 - 250, HEIGHT/2 - 10, 
                                                       WIDTH/2 + 250, HEIGHT/2 + 40));
  ko_text->setScaleImage(true);
  ko_text->setImage(ko);
  ko_text->setVisible(false);

  iv::ITexture *p1win = driver->getTexture("data/gui/P1wins.png");
  iv::ITexture *p2win = driver->getTexture("data/gui/P2wins.png");
  iv::ITexture *draw = driver->getTexture("data/gui/draw.png");
  ig::IGUIImage *winner = gui->addImage(ic::rect<s32>(WIDTH/2 - 250, HEIGHT/2 - 100, 
                                                      WIDTH/2 + 250, HEIGHT/2 - 40));
  winner->setScaleImage(true);
  winner->setImage(p1win);
  winner->setVisible(false);

  iv::ITexture *UI = driver->getTexture("data/gui/UI.png");
  ig::IGUIImage *menu = gui->addImage(ic::rect<s32>(0,    0, 
                                                    WIDTH, HEIGHT));
  menu->setScaleImage(true);
  menu->setImage(UI);
  menu->setVisible(true);

  
  /***************************
   * Creation du monde
   ***************************/
  // Ajout de l'archive qui contient entre autres un niveau complet
  device->getFileSystem()->addFileArchive("data/map_oxodm3/map_oxodm3.pk3"); // map "cs_assault"

  // On charge un bsp (un niveau) en particulier :
  is::IAnimatedMesh *mesh = smgr->getMesh("oxodm3.bsp");
  is::ISceneNode *node;
  node = smgr->addOctreeSceneNode(mesh->getMesh(0), nullptr, -1, 1024);
  // Translation pour que nos personnages soient dans le décor
  node->setPosition(ic::vector3df(400,-23,-1200));


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
  bool key_callback_p1 = false;
  bool key_callback_p2 = false;


  /*************************
   * MAIN LOOP
   *************************/
  bool annonces = true; // boolean qui est true quand les annonces se passent
  unsigned int previous_time = 0;
  unsigned int end_annonce = 6000;
  unsigned int reset_time = 0;
  
  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(100,150,200,255));

    // Close window
    if(receiver.keys[KEY_ESCAPE])
    {
      exit(0);
    }

    if (debug)
    {
      menu->setVisible(false);
      start_game = true;
    }

    if (!start_game)
    {
      if (receiver.keys[KEY_SPACE]) // Menu des touches a utiliser (avant le debut du jeu)
      {
        menu->setVisible(false);
        start_game = true;
        timer->start();
        previous_time = timer->getTime();
        chrono = 40;
        chrono_10->setImage(digits[4]);
        chrono_1->setImage(digits[0]);
      }
    }
    else // Le jeu commence
    {
      if (reset_round && !end_game) //RESET a la fin de chaque round
      {
        if (nb_win_p1 == 1)
          p1_round1->setImage(full_count);
        if (nb_win_p2 == 1)
          p2_round1->setImage(full_count);
        
        if (win_p1 || win_p2) // Sinon on rejoue le round
          round_id ++;
        if (round_id == 2)
        {
          rounds->setImage(round2);
        }
        else if (round_id == 3)
        {
          rounds->setImage(finalround);
        }

        // Annonce
        reset_round = false;
        end_round = false;
        end_timer = false;
        display_win = true;
        timer->setTime(0);
        previous_time = timer->getTime();
        chrono = 40;
        chrono_10->setImage(digits[4]);
        chrono_1->setImage(digits[0]);

        // Animation
        animEnd1.enable_action = true;
        animEnd1.enable_movement = true;
        animEnd1.is_dead = false;
        key_callback_p1 = false;
        animEnd2.enable_action = true;
        animEnd2.enable_movement = true;
        animEnd2.is_dead = false;
        key_callback_p2 = false;
        player1->setFrameLoop(0, 12);
        player1->setLoopMode(true);
        player2->setFrameLoop(0, 12);
        player2->setLoopMode(true);

        // Gui
        points_vie_manquant_p1 = 0.0f;
        points_vie_manquant_p2 = 0.0f;
        taille_barre_rouge_p1 = (points_vie_manquant_p1 / points_vie_total_p1) * BARRE_TAILLE_W;
        barre_rouge_p1->setRelativePosition(ic::rect<s32>(14, BARRES_HAUT+BARRES_BORDS, 
                                                          14 + taille_barre_rouge_p1, BARRES_BAS-BARRES_BORDS));
        barre_rouge_p1->setVisible(false);
        taille_barre_rouge_p2 = (points_vie_manquant_p2 / points_vie_total_p2) * BARRE_TAILLE_W;
        barre_rouge_p2->setRelativePosition(ic::rect<s32>(WIDTH - 14 - taille_barre_rouge_p2, BARRES_HAUT+BARRES_BORDS, 
                                                          WIDTH - 14, BARRES_BAS-BARRES_BORDS));
        barre_rouge_p2->setVisible(false);
        win_p1 = false;
        win_p2 = false;
        winner->setVisible(false);
        ko_text->setVisible(false);

        // Position
        player1->setPosition(core::vector3df(0,-23.5,-50));
        player2->setPosition(core::vector3df(0,-23.5, 50));
      }
      else
      {
        /****** ANNONCES ******/
        if (timer->getTime() < end_annonce)
        {
	  annonces = true;
	  if (timer->getTime() < end_annonce/3)
          {
            rounds->setVisible(true);
          }
          else if (timer->getTime() < 2*end_annonce/3)
          {
            rounds->setVisible(false);
            ready_text->setVisible(true);
          }
          else
          {
            ready_text->setVisible(false);
            fight_text->setVisible(true);
          }
        }
        else if (timer->getTime() >= end_annonce && timer->getTime() < end_annonce + 30) // environ une seule fois
        {
          key_callback_p1 = true;
          key_callback_p2 = true;
          fight_text->setVisible(false);
        }else{
	  annonces = false;
	}
        
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
          ic::vector3df fight_center = (player1->getPosition() + player2->getPosition())/2.0f;
          fight_center.Y = 35.0f;
	  ic::vector3df offset = ic::vector3df(2.3, 0.5, 0);
	  ic::vector3df new_cam_pos;
	  
	  if(annonces == true) //rotation de la camera pendant la duree des annonces
	  {
	    float rotation = ((float)timer->getTime() / (float)end_annonce) * 359;
	    new_cam_pos = fight_center + offset*(30 + (359 - rotation)/7 + distance/5);
	    new_cam_pos.rotateXZBy(-(180 + rotation/2), fight_center); // demi tourne
	    //new_cam_pos.rotateXZBy(120 + 2*rotation/3, fight_center); // 2/3 de tourne
	    //new_cam_pos.rotateXZBy(rotation, fight_center); // tourne complet
	  }else{ // camera "normale" pour le jeu apres les annonces
	    new_cam_pos = fight_center + offset*(30+distance/5);
	  }
	  // direction de la camera
	  smgr->getActiveCamera()->setTarget(fight_center);
	  // distance de la camera
          smgr->getActiveCamera()->setPosition(new_cam_pos);
	}

        /******** Personnage 1 *******/
        if (!animEnd1.is_dead)
        {
          if (key_callback_p1)
          {
            // FIGHT
            if(receiver.keys[KEY_KEY_V]) // Coup de poing
            {
              if (animEnd1.enable_action)
              {
                if(distance < MIN_DIST_PUNCH && !receiver.keys[KEY_DOWN] && !end_round)
                {
                  // Gestion des points de vie
                  barre_rouge_p2->setVisible(true);
                  points_vie_manquant_p2 += 10.0f;
                  if (points_vie_manquant_p2 >= points_vie_total_p2)
                  {
                    // P2 mort
                    points_vie_manquant_p2 = 100.0f;
                    // Animation mort
                    if (!animEnd2.is_dead)
                    {
                      player2->setFrameLoop(84, 108);
                      player2->setLoopMode(false);
                    }
                    animEnd2.enable_action = false;
                    animEnd2.enable_movement = false;
                    animEnd2.is_dead = true;
                    key_callback_p2 = false;
                    reset_time = timer->getTime();
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
                if(distance < MIN_DIST_KICK && !receiver.keys[KEY_DOWN] && !end_round)
                {
                  // Gestion des points de vie
                  barre_rouge_p2->setVisible(true);
                  points_vie_manquant_p2 += 5.0f;
                  if (points_vie_manquant_p2 >= points_vie_total_p2) // P2 mort
                  {
                    points_vie_manquant_p2 = 100.0f;
                    // Animation mort
                    if (!animEnd2.is_dead)
                    {
                      player2->setFrameLoop(84, 108);
                      player2->setLoopMode(false);
                    }
                    animEnd2.enable_action = false;
                    animEnd2.enable_movement = false;
                    animEnd2.is_dead = true;
                    key_callback_p2 = false;
                    reset_time = timer->getTime();
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
        }
        else
        {
          end_round = true;
          win_p2 = true;
        }


        /******** Personnage 2 *******/
        if (!animEnd2.is_dead)
        {
          if (key_callback_p2)
          {
            // FIGHT
            if(receiver.keys[KEY_KEY_L]) // Coup de poing
            {
              if (animEnd2.enable_action)
              {
                if(distance < MIN_DIST_PUNCH && !receiver.keys[KEY_KEY_S] && !end_round) // Collision detectee si assez proche
                {
                  // Gestion des points de vie
                  barre_rouge_p1->setVisible(true);
                  points_vie_manquant_p1 += 10.0f;
                  if (points_vie_manquant_p1 >= points_vie_total_p1) // P1 mort
                  {
                    points_vie_manquant_p1 = 100.0f;
                    // Animation mort
                    if (!animEnd1.is_dead)
                    {
                      player1->setFrameLoop(84, 108);
                      player1->setLoopMode(false);
                    }
                    animEnd1.enable_action = false;
                    animEnd1.enable_movement = false;
                    animEnd1.is_dead = true;
                    key_callback_p1 = false;
                    reset_time = timer->getTime();
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
                if(distance < MIN_DIST_KICK && !receiver.keys[KEY_KEY_S] && !end_round)
                {
                  // Gestion des points de vie
                  barre_rouge_p1->setVisible(true);
                  points_vie_manquant_p1 += 5.0f;
                  if (points_vie_manquant_p1 >= points_vie_total_p1) // P1 mort
                  {
                    points_vie_manquant_p1 = 100.0f;
                    // Animation mort
                    if (!animEnd1.is_dead)
                    {
                      player1->setFrameLoop(84, 108);
                      player1->setLoopMode(false);
                    }
                    animEnd1.enable_action = false;
                    animEnd1.enable_movement = false;
                    animEnd1.is_dead = true;
                    key_callback_p1 = false;
                    reset_time = timer->getTime();
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
        }
        else // P2 est mort
        {
          end_round = true;
          win_p1 = true;
        }

        if (nb_win_p1 == 2)
          p1_round2->setImage(full_count);
        if (nb_win_p2 == 2)
          p2_round2->setImage(full_count);
        
        // Dessin de la scene :
        smgr->drawAll();
      }
    }

    // Chronometre
    if (animEnd1.is_dead || animEnd2.is_dead)
    {
      end_timer = true;
    }
    unsigned int now = timer->getTime();
    if (!end_timer && (now - previous_time >= 1000) && now > end_annonce)
    {
      previous_time = now;
      chrono--;
      if (chrono <= 0) // Fin du temps
      {
        end_timer = true;
        chrono_10->setImage(digits[0]);
        chrono_1->setImage(digits[0]);
        if (points_vie_manquant_p1 > points_vie_manquant_p2)
        {
          key_callback_p1 = false;
          animEnd1.is_dead = true;
          end_round = true;
          win_p2 = true;
        }
        else if (points_vie_manquant_p1 < points_vie_manquant_p2)
        {
          key_callback_p2 = false;
          animEnd2.is_dead = true;
          end_round = true;
          win_p1 = true;
        }
        else
          end_round = true;
        
        reset_time = timer->getTime();
      }
      else
      {
        chrono_10->setImage(digits[(chrono / 10) % 10]);
        chrono_1->setImage(digits[(chrono / 1) % 10]);
      }
    }

    // Annonce vainqueur
    if (end_round)
    {
      if ((timer->getTime() - reset_time >= 5000))
      {
        reset_round = true;
      }
      else if (display_win)
      {
        display_win = false;
        if (win_p1)
        {
          winner->setImage(p1win);
          winner->setVisible(true);
          ko_text->setVisible(true);
          nb_win_p1 ++;
          if (nb_win_p1 >= 2)
            end_game = true;
        }
        else if (win_p2)
        {
          winner->setImage(p2win);
          winner->setVisible(true);
          ko_text->setVisible(true);
          nb_win_p2 ++;
          if (nb_win_p2 >= 2)
            end_game = true;
        }
        else // Draw
        {
          winner->setImage(draw);
          winner->setVisible(true);
        }
      }
    }

    if (!debug)
    {
      // Dessin du gui :
      gui->drawAll();
    }
    
    driver->endScene();
  }
  device->drop();

  return 0;
}
