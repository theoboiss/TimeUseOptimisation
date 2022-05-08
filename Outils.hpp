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
class Outils
{
public:

	static int CodageLineaire(double a, int x, int b, int modulo);

	static bool Comparer(pair<int, int>& a, pair<int, int>& b);

	static int i_Calcul_Valeur_Fonction_Objectif(Solution* uneSolution, Instance* instance);

	static int i_Calcul_Penalisation_Fonction_Objectif(Solution* uneSolution, Instance* instance, float coeff_Valeur_FO_Contrainte);

	static Solution* CopieSolution(Solution* uneSolution);

	static list<int> Personne_Contraintes_Non_Respectes(Solution* uneSolution, Instance* instance);
	
};

