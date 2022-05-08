#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <list>
#include <algorithm>
#include "Instance.hpp"
#include "Solution.hpp"
#include "Outils.hpp"

using namespace std;

#pragma once
class Heuristique
{
public:

	static Solution* GenerationSolutionRealisable(Instance* instance);

	static void InitValeurFonctionObjectif(Solution* uneSolution, Instance* instance, float coeff_Valeur_FO_Contrainte);
	
	static bool is_Peut_Reprendre_Travail(Instance* instance, list<int>& l_Jour_OFF_Proche_WE_Personne, int personne, int jour, int Nbre_WE_Travaille_Personne, int duree_totale_shift_personne, int duree_Min_Shift_Consecutif_Personne);

	static bool is_Peut_Travailler_WE_Semaine(list<int>& l_Jour_OFF_Proche_WE_Personne, int nbre_WE_Travaille_Personne, int nbre_WE_Max_Personne, int semaine);

	static bool is_Jour_OFF_Proche_WE(Instance* instance, int jour, int i_Nbre_Jour_OFF_Consecutif_Min, int i_Nbre_Shift_Consecutif_Min);

	static int Duree_Minimum_Combinaison_Shift_Possible(Instance* instance, int personne);

};

