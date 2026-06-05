# MIRA A1 FP1 — Space Invider

Projet final de première année en prépa intégrée d'ingénierie robotique chez MIRA.

Ce jeu est une version terminal du classique "Space Invaders" qui se joue en console Windows.

## Présentation

- Nom du projet : **Space Invider**
- Langage : **C**
- Plateforme cible : **Windows** (console ANSI + API Windows)
- Objectif : tirer sur des envahisseurs qui descendent vers le joueur tout en survivant le plus longtemps possible.

## Fonctionnalités

- Interface console simple avec cadre de jeu
- Déplacement du joueur à gauche/droite
- Tir de balles depuis le joueur
- Apparition et déplacement automatique des envahisseurs
- Système de score
- Sauvegarde du meilleur score dans le fichier `best_score`
- Menu de démarrage et écran de fin de partie

## Structure du projet

- `src/main.c` : logique principale du jeu, affichage et boucle de jeu
- `libs/list.c` / `libs/list.h` : implémentation d'une liste doublement chaînée utilisée pour les projectiles et les envahisseurs
- `makefile` : instructions de compilation
- `best_score` : fichier de sauvegarde du meilleur score

## Compilation

Le projet utilise un `Makefile` pour construire le binaire.

Depuis le dossier racine du projet :

```powershell
make
```

Le programme compilé est généré dans :

```text
build/programme
```

> Remarque : le code utilise l'API Windows (`windows.h`, `GetTickCount`, `_kbhit`, `_getch`) et est conçu pour être compilé sous Windows avec GCC.

## Exécution

Lancer le jeu depuis le dossier racine :

```powershell
build\programme
```

Si la console affiche des caractères étranges, vérifie que tu utilises une console Windows moderne avec support ANSI.

## Commandes du jeu

- `←` : déplacer le joueur vers la gauche
- `→` : déplacer le joueur vers la droite
- `Espace` : tirer
- `q` : quitter le jeu
- `Entrée` : sélectionner dans les menus

## Règles

- Le joueur contrôle le vaisseau représenté par `^`
- Les envahisseurs sont représentés par `$`
- Les balles sont représentées par `*`
- Chaque envahisseur touché augmente le score
- Si un envahisseur atteint le bas du terrain de jeu, le joueur perd une vie
- La partie se termine lorsque la vie du joueur atteint 0
- Le meilleur score est sauvegardé automatiquement

## Améliorations possibles

Quelques pistes pour aller plus loin :

- ajout de différents types d'envahisseurs
- animations de départ et de fin
- vitesse progressive des envahisseurs
- ajout de niveaux
- gestion d'un menu de pause
