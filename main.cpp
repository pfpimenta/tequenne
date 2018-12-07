#include <iostream>
#include <irrlicht.h>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

irr::s32 old_mouse_X;
char new_animation = 's'; // 's': stand, 'r': run, 'b': back
char old_animation = 's';

float vitesse = 1.75;

struct MyEventReceiver : IEventReceiver
{
  bool OnEvent(const SEvent &event)
  {
    // Si l'événement est de type souris (MOUSE_INPUT)
    // et du genre pressage de touche
    if (event.EventType == EET_MOUSE_INPUT_EVENT)// &&
        //event.MouseInput.Event==EMIE_MOUSE_MOVED)
    {
      ic::vector3df rotation = node->getRotation();
      irr::s32 x = event.MouseInput.X;
      //event.MouseInput.Event
      if( (x-old_mouse_X) < 0){
	  // Tourne à gauche
	 rotation.Y -= 10;
      }else{
	  // Tourne à droite
	 rotation.Y += 10;
      }
    old_mouse_X = x;
      
    node->setRotation(rotation);
    }
    // Si l'événement est de type clavier (KEY_INPUT)
    // et du genre pressage de touche
    // et que la touche est escape, on sort du programme
    if (event.EventType == EET_KEY_INPUT_EVENT &&
        event.KeyInput.PressedDown)
    {
      ic::vector3df position = node->getPosition();
      ic::vector3df rotation = node->getRotation();

      switch (event.KeyInput.Key)
      {
        case KEY_ESCAPE:
          exit(0);
        case KEY_KEY_Z: // Avance
          position.X += vitesse * cos(rotation.Y * M_PI / 180.0);
          position.Z += -vitesse * sin(rotation.Y * M_PI / 180.0);
	  new_animation = 'r';
          break;
        case KEY_KEY_S: // Recule
          position.X += -vitesse * cos(rotation.Y * M_PI / 180.0);
          position.Z += vitesse * sin(rotation.Y * M_PI / 180.0);
	  new_animation = 'b';
          break;
        case KEY_KEY_D: // Tourne à droite
          rotation.Y += 10;
	  new_animation = 's';
          break;
        case KEY_KEY_Q: // Tourne à gauche
          rotation.Y -= 10;
	  new_animation = 's';
          break;
        default:;
      }
    node->setPosition(position);
    node->setRotation(rotation);
    }else{
	new_animation = 's'; // stop running
    }
    return false;
  }
  is::IAnimatedMeshSceneNode *node;
};

int main()
{
  // Le gestionnaire d'événements
  MyEventReceiver receiver;
  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(640, 480),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver  *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();

  // Ajout de l'archive qui contient entre autres un niveau complet
  device->getFileSystem()->addFileArchive("data/map-20kdm2.pk3");

  // On charge un bsp (un niveau) en particulier :
  is::IAnimatedMesh *mesh = smgr->getMesh("20kdm2.bsp");
  is::ISceneNode *node;
  node = smgr->addOctreeSceneNode(mesh->getMesh(0), nullptr, -1, 1024);
  // Translation pour que nos personnages soient dans le décor
  node->setPosition(core::vector3df(-1300,-104,-1249));

  // Chargement de notre personnage (réutilisé plusieurs fois)
  is::IAnimatedMesh *mesh_perso = smgr->getMesh("data/tris.md2");

  // Attachement de notre personnage dans la scène
  is::IAnimatedMeshSceneNode *perso = smgr->addAnimatedMeshSceneNode(mesh_perso);
  receiver.node = perso;
  perso->setMaterialFlag(iv::EMF_LIGHTING, false);
  perso->setMD2Animation(is::EMAT_STAND);
  perso->setMaterialTexture(0, driver->getTexture("data/blue_texture.pcx"));

  // Un deuxième personnage, en rouge et qui cours entre deux points
  is::IAnimatedMeshSceneNode *perso_cours = smgr->addAnimatedMeshSceneNode(mesh_perso);
  perso_cours->setMaterialFlag(iv::EMF_LIGHTING, false);
  perso_cours->setMD2Animation(is::EMAT_RUN);
  perso_cours->setMaterialTexture(0, driver->getTexture("data/red_texture.pcx"));
  is::ISceneNodeAnimator *anim =
            smgr->createFlyStraightAnimator(ic::vector3df(-100,0,60),
            ic::vector3df(100,0,60), 3500, true);
  perso_cours->addAnimator(anim);

  // Un troisième personnage, qui saute en rond
  is::IAnimatedMeshSceneNode *perso_cercle = smgr->addAnimatedMeshSceneNode(mesh_perso);
  perso_cercle->setMaterialFlag(iv::EMF_LIGHTING, false);
  perso_cercle->setMD2Animation(is::EMAT_JUMP);
  perso_cercle->setMaterialTexture(0, driver->getTexture("data/red_texture.pcx"));
  is::ISceneNodeAnimator *anim_cercle =
            smgr->createFlyCircleAnimator(core::vector3df(30,0,0), 20.0f);
  perso_cercle->addAnimator(anim_cercle);

  // Camera fixe
  //smgr->addCameraSceneNode(nullptr, ic::vector3df(0, 30, -70), ic::vector3df(0, 5, 0));
  // camera FPS v2
  smgr->addCameraSceneNode(perso, ic::vector3df(-70, 30, 0), ic::vector3df(0, 0, 0));
  // Ou une caméra "jeu video à la première personne"
  //smgr->addCameraSceneNodeFPS();
  //smgr->addCameraSceneNodeFPS(perso);//,       // le noeud parent de la caméra
// 	f32 rotateSpeed,           // la vitesse de rotation de la caméra
// 	f32 moveSpeed,             // la vitesse de déplacement
// 	s32 id,                    // numéro d'identification du noeud
// 	SKeyMap * keyMapArray,     // une map permettant de re-affecter les touches
// 	s32 keyMapSize,            // taille de la keyMap
// 	bool noVerticalMovement,   // autorise ou non les mouvements sur l'axe vertical
// 	f32 jumpSpeed,             // vitesse de déplacement lors d'un saut
// 	bool invertMouse,          // inverse ou non la rotation de la caméra
// 	bool makeActive)           // indique si la caméra doit être active ou non

	

  // Ou une caméra type modelleur
  //smgr->addCameraSceneNodeMaya();

  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(100,150,200,255));

    // make the camera follow the blue skeleton
    smgr->getActiveCamera()->setTarget(perso->getPosition());
    
    // character animation 
    if(new_animation != old_animation){
      switch(new_animation){
	case 's':
	  perso->setMD2Animation(is::EMAT_STAND);
	  break;
	case 'r':
	  perso->setMD2Animation(is::EMAT_RUN);
	  break;
	case 'b':
	  perso->setMD2Animation(is::EMAT_BOOM);
	  break;  
      }
      old_animation = new_animation;
    }
    
    // Dessin de la scène :
    smgr->drawAll();
    // 
    driver->endScene();
  }
  device->drop();

  return 0;
}
