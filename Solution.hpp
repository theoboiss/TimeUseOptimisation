#ifndef Solution_hpp
#define Solution_hpp

#include "Instance.hpp"
#include <stdio.h>
#include <vector>

using namespace std;

class Solution
{
    public:// /!\ Normalement pour coder "proprepement" il faudrait mettre ces attributs en privée et passer par des fonctions setters/getters pour les modifier. Cependant, dans une méthode de résolution, les opérations sur les solutions (ajout d'id POI, supression d'ID, etc.) sont très très nombreuses, passer par des setters/getters (qui en plus vérifie certaines opérations) est très coûteux en temps de calcul. En optimisation, on évite et manipule directement les solutions (autres méthodes, passer par les inline mais ça reste lourd).
       
        vector<vector<int>> v_v_IdShift_Par_Personne_et_Jour;   // v_v_IdShift_Par_Personne_et_Jour[p][j] Donne l'Id Shift (ou -1 si ne travaille pas) pour la personne p le jour j. La taille de la première dimension du tableau est égale au nombre de personne puis la deuxième dimension est égale au nombre de jour.
        
        int i_valeur_fonction_objectif;                 // Valeur de la fonction objectif (somme des scores des POIs visités
    
    public:
    /* Constructeurs et destructeur  */
        Solution();
        virtual ~Solution();
    bool Verification_Solution(Instance *instance);
};

#endif
