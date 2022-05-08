#include <algorithm>
#include <list>
#include "Instance.hpp"
#include "Solution.hpp"
#include "Outils.hpp"

using namespace std;

#pragma once
class Heuristique
{
public:


// ################### Initialisation de la solution initiale ###################

	/// <summary>
	/// Calcul et renvoie une solution initiale qui respecte un maximum de contraintes.
	/// </summary>
	/// 
	/// <param name="instance"></param>
	/// <returns>Une solution initiale</returns>
	static Solution* Generation_Solution_Initiale(Instance* instance);

	/// <summary>
	/// Calcul de la valeur de la fonction objectif.
	/// </summary>
	/// 
	/// <param name="uneSolution"></param>
	/// <param name="instance"></param>
	/// <param name="coeff_Valeur_FO_Contrainte"></param>
	static void Init_Valeur_FO_Indicative(Solution* uneSolution, Instance* instance, float coeff_Valeur_FO_Contrainte);
	

// ################### Indicateurs pour prendre des décisions lors de l'initialisation ###################

	/// <summary>
	/// Indique si une personne peut reprendre le travail pour un jour donné, après avoir était en repos.
	/// </summary>
	/// 
	/// <param name="instance"></param>
	/// <param name="l_Jour_OFF_Proche_WE_Personne"></param>
	/// <param name="personne"></param>
	/// <param name="jour"></param>
	/// <param name="Nbre_WE_Travaille_Personne"></param>
	/// <param name="duree_totale_shift_personne"></param>
	/// <param name="duree_Min_Shift_Consecutif_Personne"></param>
	/// <returns>vrai si la personne peut reprendre le travail ce jour là, faux sinon</returns>
	static bool is_Peut_Reprendre_Travail(Instance* instance, list<int>& l_Jour_OFF_Proche_WE_Personne, int personne, int jour, int Nbre_WE_Travaille_Personne, int duree_totale_shift_personne, int duree_Min_Shift_Consecutif_Personne);

	/// <summary>
	/// Indique s'il est judicieux pour une personne de travailler le weekend de la semaine actuelle.
	/// </summary>
	/// 
	/// <param name="l_Jour_OFF_Proche_WE_Personne"></param>
	/// <param name="nbre_WE_Travaille_Personne"></param>
	/// <param name="nbre_WE_Max_Personne"></param>
	/// <param name="semaine"></param>
	/// <returns>vrai s'il est intéressant pour la personne peut travailler le weekend cette semaine là, faux sinon</returns>
	static bool is_Peut_Travailler_WE_Semaine(list<int>& l_Jour_OFF_Proche_WE_Personne, int nbre_WE_Travaille_Personne, int nbre_WE_Max_Personne, int semaine);

	/// <summary>
	/// Indique pour un jour repos et en fonction des règles de l'emploi du temps donnés, si un weekend sera trop proche pour ne pas travailler un weekend à son issue.
	/// </summary>
	/// 
	/// <param name="instance"></param>
	/// <param name="jour"></param>
	/// <param name="i_Nbre_Jour_OFF_Consecutif_Min"></param>
	/// <param name="i_Nbre_Shift_Consecutif_Min"></param>
	/// <returns>vrai si le jour de repos regardé est proche d'un weekend, faux sinon</returns>
	static bool is_Jour_OFF_Proche_WE(Instance* instance, int jour, int i_Nbre_Jour_OFF_Consecutif_Min, int i_Nbre_Shift_Consecutif_Min);

	/// <summary>
	/// Calcule et renvoie la durée de travail minimum d'une personne sur sa séquence de jours de travail minimale.
	/// </summary>
	/// 
	/// <param name="instance"></param>
	/// <param name="personne"></param>
	/// <returns>la durée minimum d'une personne sur sa séquence de jours de travail minimale</returns>
	static int Duree_Minimum_Combinaison_Shift_Possible(Instance* instance, int personne);

};

