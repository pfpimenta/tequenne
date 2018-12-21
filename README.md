# tequenne
Code du projet du module Jeux Vidéo à CPE Lyon: implementer un jeu vidéo.
Le jeu et un jeu de combat ne s’inspirant pas du tout du jeu Tekken 3 sorti par 
Namco sur Playstation en 1998. 

Les auteurs:
* COUDERC, Corentin
* FOLETTO PIMENTA, Pedro


## Comment le compiler
Dans le dossier des sources du jeu (là où se trouve le Makefile), compiler avec la commande `make`. Lancer ensuite l’exécutable `./tequenne`.  

## Comment le jouer
Le jeu est designé pour deux joueurs (pas encore d’IA disponible) et se joue avec les touches du clavier.
Dans le jeu on peut se déplacer à gauche ou à droite, sauter pour se déplacer plus vite, se baisser pour esquiver, et donner des coups de poing et coups de pied. 

## Mode debug 
Pour lancer le jeu en mode debug (caméra libre), lancer l’exécutable avec  l’argument debug : `./tequenne debug`
Le mode debug utilise une caméra de type FPS qui permet de se balader dans le monde avec la souris et les flèches directionnelles.

## Divers détails 
* Chaque joueur commence avec 100 points de vie. Toucher un adversaire avec le poing lui enlève 10 points de vie, tandis que le coup de pied, qui a un peu plus de portée, lui en enlève 5. 
* Sauter et se déplacer sur les côtés nous permet de se mouvoir plus rapidement (reculer pour s’éloigner du combat par exemple). 
