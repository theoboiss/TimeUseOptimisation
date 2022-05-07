#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include "Instance.hpp"
#include "Solution.hpp"
#include "Heuristique.hpp"
#include "Outils.hpp"

using namespace std;

#pragma once
class MetaHeuristique
{
public:

	static Solution* MeilleureSolution(Solution solutionRealisable, Instance* instance, float coeff_Valeur_FO_Contrainte);

	static Solution* RechercheVoisinageVariable(Solution solutionRealisable, Instance* instance, int k, float coeff_Valeur_FO_Contrainte);

	static void OperationOperateurModificationShift(Solution* solutionRealisable, Solution** candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte, int personne, int jour, int nombre_personne = 0, int nombre_jour = 0, int nombre_shift = 0);

	static void OperateurModificationShift(Solution* uneSolution, int nombre_shift, int personne, int jour, bool inverse = false);

	static void OperationOperateurSwapCodageLineaire(Solution* solutionRealisable, Solution** candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
		int personne, int jour, int modulo,
		int* OperateurSwapCodageLineaire(Solution*, int, int, int, int, int), int a, int b);

	static int* OperateurSwapJourCodageLineaire(Solution* uneSolution, int nombre_jour, int personne, int jour, int a, int b);

	static int* OperateurSwapPersonneCodageLineaire(Solution* uneSolution, int nombre_personne, int personne, int jour, int a, int b);

};

