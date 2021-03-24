# Gomoku_AI
Intelligence artificielle réalisée au cours de mon cursus à EPITECH

Elle ne peut jouer qu'en mode standard avec les commandes MANDATORY du protocole du logiciel PISKVORK : [protocole](https://svn.code.sf.net/p/piskvork/code/trunk/source/doc/protocl2en.htm)

# Règles
Le gomoku se joue sur un plateau quadrillé de 19 lignes horizontales et 19 lignes verticales formant, comme un jeu de go, 361 intersections. Le nombre de pions est toutefois nettement inférieur, puisque les joueurs n'en reçoivent que 60, qu'ils posent un à un et à tour de rôle sur les intersections.

Le joueur qui a choisi ou obtenu par un tirage au sort les pions noirs (et que l'on appelle Noir par convention) joue toujours le premier en plaçant son premier pion sur l'intersection centrale du damier. Blanc doit alors poser son pion sur l'une des 8 intersections adjacentes au pion noir. Noir fait la même chose, et ainsi de suite, le but du jeu étant de prendre l'adversaire de vitesse et de réussir le premier à aligner 5 pions de sa couleur, dans les trois directions possibles : vertical, horizontal ou diagonal.

# Algorithme

L'IA est basée sur un algorithme plutôt commun dans ce cadre là : le min-max

Cependant j'ai rajouté plusieurs éléments afin de l'améliorer, ainsi on peut retrouver :
* La base principale (min max un chouïa optimisé).
* L'élagage alpha-bêta (ou alpha-beta pruning) qui est une technique permettant de réduire le nombre de nœuds évalués par l'algorithme minimax
* De l'heuristique (afin de reconnaître certains paternes "game breaker", un alignement de 4 pions par exemple)
* Des tables de transpositions (créées avec du Zobrist hashing)

Globalement l'IA est relativement forte (top 20 gomocup facile), mais elle plie face à des IA plus fortes comme Embryo, Pentazen voire  Wine
