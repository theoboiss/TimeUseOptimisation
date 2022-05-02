#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <list>
#include <algorithm>
#include "Instance.hpp"
#include "Solution.hpp"
#include "Heuristique.hpp"

using namespace std;

#pragma once
class MetaHeuristique
{
public:

	static Solution* MeilleureSolution(Solution solutionRealisable, Instance* instance);

	static Solution* RechercheVoisinageVariable(Solution solutionRealisable, Instance* instance, int k);

	static int CodageLineaire(int a, int x, int b, int modulo);

	static Solution* OperateurSwapJourCodageLineaire(Solution* uneSolution, int a, int b);

	static Solution* OperateurSwapPersonneCodageLineaire(Solution* uneSolution, int a, int b);

	static Solution* OperateurModificationShiftCodageLineaire(Solution* uneSolution, int a, int b, int c, int d, int e, int f, int nombreShift);

	static Solution* OperateurModificationShiftAleatoire(Solution* uneSolution);

};

