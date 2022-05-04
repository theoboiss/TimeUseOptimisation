#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <list>
#include <algorithm>
#include "Instance.hpp"
#include "Solution.hpp"

using namespace std;

#pragma once
class Heuristique
{
public:

	static Solution* GenerationSolutionRealisable(Instance* instance);

	void static InitValeurFonctionObjectif(Solution* uneSolution, Instance* instance, float coeff_Valeur_FO_Contrainte = 0.85);
	
	int static i_Calcul_Valeur_Fonction_Objectif(Solution* uneSolution, Instance* instance);

	int static i_Calcul_Penalisation_Fonction_Objectif(Solution* uneSolution, Instance* instance, float coeff_Valeur_FO_Contrainte);

	bool static is_Peut_Reprendre_Travail(Instance* instance, list<int>& l_Jour_OFF_Proche_WE_Personne, int personne, int jour, int Nbre_WE_Travaille_Personne, int duree_totale_shift_personne, int duree_Min_Shift_Consecutif_Personne);

	bool static is_Peut_Travailler_WE_Semaine(list<int>& l_Jour_OFF_Proche_WE_Personne, int nbre_WE_Travaille_Personne, int nbre_WE_Max_Personne, int semaine);

	bool static is_Jour_OFF_Proche_WE(Instance* instance, int jour, int i_Nbre_Jour_OFF_Consecutif_Min, int i_Nbre_Shift_Consecutif_Min);

	int static Duree_Minimum_Combinaison_Shift_Possible(Instance* instance, int personne);

	bool static Comparer(pair<int, int>& a, pair<int, int>& b);

};

