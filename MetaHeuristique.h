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

	int static CodageLineaire(int a, int x, int b, int modulo);

	static Solution* OperateurSwapCodageLineaire(Solution* uneSolution, int a, int b);

};

