#include "Instance.hpp"
#include "Solution.hpp"
#include "Heuristique.hpp"
#include "Outils.hpp"

using namespace std;

#pragma once
class MetaHeuristique
{
public:


// ################### Algorithmes généraux pour améliorer la solution initiale ###################

	/// <summary>
	/// Calcule et renvoie une meilleure solution si elle est trouvée que celle initiale, à partir d'une méta-heuristique à aproche trajectoire et de type Recherche à voisinage variable (RVV).
	/// Parcourt progressivement l'espace des solutions en s'extrayant à chaque changement d'opérateur d'un optimal local qui n'est pas global.
	/// </summary>
	/// 
	/// <param name="solutionRealisable"></param>
	/// <param name="instance"></param>
	/// <param name="coeff_Valeur_FO_Contrainte"></param>
	/// <param name="chrono_start"></param>
	/// <returns>Une meilleure solution que celle initiale ou rien</returns>
	static Solution* Descente_Voisinage_Variable(Solution solutionInitiale, Instance* instance, float coeff_Valeur_FO_Contrainte, chrono::time_point<chrono::system_clock> chrono_start);


	/// <summary>
	/// Calcule et renvoie une meilleure solution si elle est trouvée, en explorant le voisinage d'une solution réalisable avec un opérateur donné et en retire l'optimal local si le temps le permet.
	/// </summary>
	/// 
	/// <param name="solutionRealisable"></param>
	/// <param name="instance"></param>
	/// <param name="k"></param>
	/// <param name="coeff_Valeur_FO_Contrainte"></param>
	/// <param name="chrono_start"></param>
	/// <returns>Une meilleure solution que celle du voisinage précédent ou rien</returns>
	static Solution* Recherche_Meilleure_Solution_Voisinage(Solution solutionRealisable, Instance* instance, int k, float coeff_Valeur_FO_Contrainte, chrono::time_point<chrono::system_clock> chrono_start);


// ################### Opérations paramétrables qui éxécutent les opérateurs pour explorer le voisinage d'une solution ###################

	/// <summary>
	/// Associe un candidat à une meilleure solution voisine si elle existe, en fonction de l'opérateur de modification de shift choisi.
	/// Est paramétrée lors de la recherche de meilleure solution du voisinage.
	/// </summary>
	/// 
	/// <param name="solutionRealisable"></param>
	/// <param name="candidat"></param>
	/// <param name="solutionVoisine"></param>
	/// <param name="instance"></param>
	/// <param name="coeff_Valeur_FO_Contrainte"></param>
	/// <param name="personne"></param>
	/// <param name="jour"></param>
	/// <param name="nombre_shift"></param>
	/// <param name="OperateurModificationShift"></param>
	static void Operation_Operateur_Modification_Shift(Solution* solutionRealisable, Solution** candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
		int personne, int jour, int nombre_shift,
		void OperateurModificationShift(Solution*, int, int, int));


	/// <summary>
	/// Associe un candidat à une meilleure solution voisine si elle existe, en fonction de l'opérateur de swap choisi.
	/// Est paramétrée lors de la recherche de meilleure solution du voisinage.
	/// </summary>
	/// 
	/// <param name="solutionRealisable"></param>
	/// <param name="candidat"></param>
	/// <param name="solutionVoisine"></param>
	/// <param name="instance"></param>
	/// <param name="coeff_Valeur_FO_Contrainte"></param>
	/// <param name="personne"></param>
	/// <param name="jour"></param>
	/// <param name="OperateurSwap"></param>
	/// <param name="modulo"></param>
	/// <param name="deplacement"></param>
	static void Operation_Operateur_Swap(Solution* solutionRealisable, Solution** candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
		int personne, int jour,
		void OperateurSwap(Solution*, int, int, int, int, int), int moduloX, int moduloY, int deplacement);


// ################### Opérateurs qui crée un nouveau voisin d'une solution ###################
	
	/// <summary>
	/// Fait varier incrémentalement un jour de travail en un autre jour de travail.
	/// </summary>
	/// 
	/// <param name="uneSolution"></param>
	/// <param name="nombre_shift"></param>
	/// <param name="personne"></param>
	/// <param name="jour"></param>
	static void Operateur_Modification_Shift_Incrementation(Solution* uneSolution, int nombre_shift, int personne, int jour);


	/// <summary>
	/// Fait varier les jours de façon relativement aléatoire.
	/// Donne une probabilité d'environ 2/7 de changer d'état entre jour de travail et jour de repos, et associe un jour de travail aléatoire.
	/// </summary>
	/// 
	/// <param name="uneSolution"></param>
	/// <param name="nombre_shiftPlusUn"></param>
	/// <param name="personne"></param>
	/// <param name="jour"></param>
	static void Operateur_Modification_Shift_Aleatoire(Solution* uneSolution, int nombre_shiftPlusUn, int personne, int jour);


	/// <summary>
	/// Echange les shifts entre deux jours d'une personne donnée.
	/// </summary>
	/// 
	/// <param name="uneSolution"></param>
	/// <param name="nombre_jour"></param>
	/// <param name="personne"></param>
	/// <param name="jour"></param>
	/// <param name="b"></param>
	static void Operateur_Swap_Jour(Solution* uneSolution, int nombre_personne, int nombre_jour, int personne, int jour, int deplacement);


	/// <summary>
	/// Echange les shifts entre deux personnes d'un jour donné.
	/// </summary>
	/// 
	/// <param name="uneSolution"></param>
	/// <param name="nombre_personne"></param>
	/// <param name="personne"></param>
	/// <param name="jour"></param>
	/// <param name="deplacement"></param>
	static void Operateur_Swap_Personne(Solution* uneSolution, int nombre_personne, int nombre_jour, int personne, int jour, int deplacement);

	/// <summary>
	/// Echange les shifts entre deux personnes et deux jours.
	/// </summary>
	/// 
	/// <param name="uneSolution"></param>
	/// <param name="nombre_personne"></param>
	/// <param name="nombre_jour"></param>
	/// <param name="personne"></param>
	/// <param name="jour"></param>
	/// <param name="deplacement"></param>
	static void Operateur_Swap_Personne_Jour(Solution* uneSolution, int nombre_personne, int nombre_jour, int personne, int jour, int deplacement);

};
