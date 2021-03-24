# Gomoku_AI
Intelligence artificielle réalisée au cours de mon cursus à EPITECH

Elle ne peut jouer qu'en mode standard avec les commandes MANDATORY du protocole du logiciel PISKVORK : [protocole](https://svn.code.sf.net/p/piskvork/code/trunk/source/doc/protocl2en.htm)

# Algorithme

L'IA est basée sur un algorithme plutôt commun dans ce cadre là : le min-max

Cependant j'ai rajouté plusieurs éléments afin de l'améliorer, ainsi on peut retrouver :
* La base principale (min max un chouïa optimisé).
* L'élagage alpha-bêta (ou alpha-beta pruning) qui est une technique permettant de réduire le nombre de nœuds évalués par l'algorithme minimax
* De l'heuristique (afin de reconnaître certains paternes "game breaker", un alignement de 4 pions par exemple)
* Des tables de transpositions (créées avec du Zobrist hashing)

Globalement l'IA est relativement forte (top 20 gomocup facile), mais elle plie face à des IA plus fortes comme Embryo, Pentazen voire  Wine
