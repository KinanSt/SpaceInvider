# MIRA A1 FP1 — Space Invider

Projet final de première année en prépa intégrée d'ingénierie robotique chez MIRA.

Space Invider est une version graphique du classique "Space Invaders" développée en **C** avec **SDL3**.

## Présentation

- Nom du projet : **Space Invider**
- Langage : **C**
- Technologies : **SDL3**, **SDL3_ttf**, **SDL3_image**
- Plateforme cible : **Windows**
- Objectif : piloter un vaisseau, tirer sur des envahisseurs et atteindre le meilleur score

## Fonctionnalités

- Affichage graphique SDL3 dans une fenêtre redimensionnable
- Textures pour le joueur et les envahisseurs
- Texte rendu avec `TTF_OpenFont`
- Déplacement du joueur à gauche/droite
- Tir de projectiles
- Apparition et déplacement automatique des envahisseurs
- Système de score et d'affichage de la vie
- Sauvegarde du meilleur score dans le fichier `best_score`
- Menu de démarrage et écran de fin de partie

## Structure du projet

- `src/main.c` : logique de jeu, boucle principale et gestion des événements
- `src/visual.c` / `src/visual.h` : rendu SDL, texte et textures
- `src/player.h` : structure et mouvements du joueur
- `src/invider.h` : structure des envahisseurs
- `src/list.h` / `libs/list.c` : liste doublement chaînée pour les entités dynamiques
- `makefile` : compilation
- `best_score` : sauvegarde du record
- `fonts/arial.ttf` : police utilisée pour le texte
- `imgs/player.png` / `imgs/invader.png` : textures du jeu

## Dépendances

- GCC pour Windows
- SDL3
- SDL3_ttf
- SDL3_image
- Fichiers de ressources présentés dans `fonts/` et `imgs/`

## Compilation

Depuis le dossier racine du projet :

```powershell
make
```

Le binaire est généré dans :

```text
build/programme.exe
```

## Exécution

Lancer le jeu depuis le dossier racine :

```powershell
build\programme.exe
```

> Si la fenêtre ne s'ouvre pas ou si le texte n'apparaît pas, vérifie que les fichiers `fonts/arial.ttf`, `imgs/player.png` et `imgs/invader.png` sont présents.

## Commandes du jeu

- `←` : déplacer le joueur vers la gauche
- `→` : déplacer le joueur vers la droite
- `Espace` : tirer
- `Entrée` : valider dans les menus
- `q` : quitter le jeu

## Règles

- Le joueur contrôle un vaisseau en bas de l'écran
- Les envahisseurs descendent depuis le haut
- Chaque tir qui touche un envahisseur augmente le score
- Si un envahisseur atteint le bas, le joueur perd une vie
- La partie se termine lorsque toutes les vies sont perdues
- Le meilleur score est sauvegardé dans `best_score`

## Notes

- Le projet utilise une boucle SDL et la gestion d'événements clavier
- Le rendu graphique est géré dans `src/visual.c`
- Les ressources images et police doivent rester dans leurs dossiers respectifs pour que le jeu fonctionne
