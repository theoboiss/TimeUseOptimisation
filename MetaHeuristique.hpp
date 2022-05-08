#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <list>
#include <algorithm>
#include "Instance.hpp"
#include "Solution.hpp"
#include "Heuristique.hpp"
#include "Outils.hpp"

using namespace std;

#pragma once
class MetaHeuristique
{
public:

	static Solution* MeilleureSolution(Solution solutionRealisable, Instance* instance, float coeff_Valeur_FO_Contrainte, chrono::time_point<chrono::system_clock> chrono_start);

	static Solution* RechercheVoisinageVariable(Solution solutionRealisable, Instance* instance, int k, float coeff_Valeur_FO_Contrainte, chrono::time_point<chrono::system_clock> chrono_start);

	static void OperationOperateurModificationShift(Solution* solutionRealisable, Solution** candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
		int personne, int jour, int nombre_shift,
		void OperateurModificationShift(Solution*, int, int, int));

	static void OperateurModificationShift(Solution* uneSolution, int nombre_shift, int personne, int jour);

	static void OperateurModificationShiftAleatoire(Solution* uneSolution, int nombre_shiftPlusUn, int personne, int jour);

	static void OperationOperateurSwap(Solution* solutionRealisable, Solution** candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
		int personne, int jour,
		int* OperateurSwap(Solution*, int, int, int, int), int modulo, int b);

	static int* OperateurSwapJour(Solution* uneSolution, int nombre_jour, int personne, int jour, int b);

	static int* OperateurSwapPersonne(Solution* uneSolution, int nombre_personne, int personne, int jour, int b);

};

