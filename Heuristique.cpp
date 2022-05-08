#include "Heuristique.hpp"

#define DEBUG false

// ################### CALCUL DE LA VALEUR DE LA FONCTION OBJECTIVE ###################


void Heuristique::Init_Valeur_FO_Indicative(Solution* uneSolution, Instance* instance, float coeff_Valeur_FO_Contrainte)
{
    if (uneSolution->v_v_IdShift_Par_Personne_et_Jour.size() == 0 || uneSolution->v_v_IdShift_Par_Personne_et_Jour[0].size() == 0) return;

    uneSolution->i_valeur_fonction_objectif = Outils::Calcul_Valeur_FO(uneSolution, instance);

    uneSolution->i_valeur_fonction_objectif += Outils::Calcul_Penalisation_Valeur_FO(uneSolution, instance, coeff_Valeur_FO_Contrainte);

}



// ################### CREATION DE LA SOLUTION INITIALE ###################



Solution* Heuristique::Generation_Solution_Initiale(Instance* instance) {

    //-----------------------------------------Initialisation-------------------------------------------------------------

    Solution* S = new Solution();

    vector<list<int>> v_l_Jour_OFF_Proche_WE(instance->get_Nombre_Personne());
    /*
     * Pour chaque personne, on établit la liste des WE réservés (/jours de congés trop proches des WE (après le congé, reprendre le travail nous obligera à travailler le WE)),
     * et on vérifie avant de travailler un WE si un jour de congé problématique éxiste dans la semaine courante.
     * Si le nombre de WE réservés (/semaines contenant un jour problématique) est supérieur
     *Si oui, on favorise le travail ce WE.
     * Si non, on évite de travailler ce WE.
     */
    for (int personne = 0; personne < instance->get_Nombre_Personne(); personne++)
    {
        v_l_Jour_OFF_Proche_WE.at(personne) = list<int>();

        vector<int> personne_Id_Jour_Conges = instance->get_vector_Personne_Id_Jour_Conges(personne);

        for (int it = 0; it < personne_Id_Jour_Conges.size(); it++) // Pour chaque jour de congé
        {
            if (is_Jour_OFF_Proche_WE(instance, personne_Id_Jour_Conges.at(it), instance->get_Personne_Jour_OFF_Consecutif_Min(personne), instance->get_Personne_Nbre_Shift_Consecutif_Min(personne))) // S'il est proche du WE
            {
                int numero_Semaine_Jour_OFF_Proche_WE = personne_Id_Jour_Conges.at(it) / 7;

                v_l_Jour_OFF_Proche_WE.at(personne).push_back(numero_Semaine_Jour_OFF_Proche_WE); // On stocke son numéro de semaine pour la personne
            }
        }
        //Ce vecteur de liste contient pour chaque personne la liste contenant les WE où la personne va travailler en priorité
        v_l_Jour_OFF_Proche_WE.at(personne).unique();
    }
    //Initialisation des différents compteurs utilisés dans l'heuristique

    vector<int> v_Duree_Min_Shift_Consecutif_Personne(instance->get_Nombre_Personne());
    //Vecteur de taille instance->get_Nombre_Personne() qui contient une bonne estimation de la durée minimum des suites de shifts de taille instance->get_Personne_Nbre_Shift_Consecutif_Min(personne).

    vector<int> v_Nbre_Shift_Consecutif(instance->get_Nombre_Personne());
    //Vecteur de taille instance->get_Nombre_Personne() dont la case [personne] contient le compteur des shifts consécutifs lors de son appel dans l'heuristique.

    vector<int> v_Nbre_Jour_OFF_Consecutif(instance->get_Nombre_Personne());
    //Vecteur de taille instance->get_Nombre_Personne() dont la case [personne] contient le compteur des jours de repos consécutifs lors de son appel dans l'heuristique.


    vector<int> v_Nbre_WE_Travaille(instance->get_Nombre_Personne());
    //Vecteur de taille instance->get_Nombre_Personne() dont la case [personne] contient le compteur des WE où la personne à travaillé lors de son appel dans l'heuristique.


    vector<int> v_Duree_Totale_Shift_Personne(instance->get_Nombre_Personne());
    //Vecteur de taille instance->get_Nombre_Personne() dont la case [personne] contient le compteur du temps total de travaille lors de son appel dans l'heuristique.


    vector<vector<int>> v_v_Nbre_Chaque_Shift_Pers(instance->get_Nombre_Personne());
    //Vecteur de taille instance->get_Nombre_Personne() dont la case [personne][shift] contient le compteur de shift effectué par la personne lors de son appel dans l'heuristique.


    //--------------Tri des shifts par durée------------------------------------

    vector<pair<int, int>> v_v_Shift_trie;


    int duree_shift_min = instance->get_Shift_Duree(0);

    for (int shift = 0; shift < instance->get_Nombre_Shift(); shift++)
    {
        int duree_shift = instance->get_Shift_Duree(shift);

        v_v_Shift_trie.push_back({ shift, duree_shift });

        if (duree_shift_min > duree_shift)
        {
            duree_shift_min = duree_shift;
        }
    }
    sort(v_v_Shift_trie.begin(), v_v_Shift_trie.end(), Outils::Comparer);

    //-----------------Cas du premier jour------------------------


    S->v_v_IdShift_Par_Personne_et_Jour = vector<vector<int>>(instance->get_Nombre_Personne());

    //On parcourt les personnes
    for (int personne = 0; personne < instance->get_Nombre_Personne(); personne++)
    {
        //On initialise les le tableau de solutions avec des colonnes de -1
        S->v_v_IdShift_Par_Personne_et_Jour[personne] = vector<int>(instance->get_Nombre_Jour(), -1);

        //On initialise les différents compteurs
        v_v_Nbre_Chaque_Shift_Pers[personne] = vector<int>(instance->get_Nombre_Shift(), 0);

        v_Duree_Min_Shift_Consecutif_Personne.at(personne) = Duree_Minimum_Combinaison_Shift_Possible(instance, personne);

        int shift = -1;

        if (instance->is_Available_Personne_Jour(personne, 0) && is_Peut_Reprendre_Travail(instance, v_l_Jour_OFF_Proche_WE.at(personne), personne, 0, 0, v_Duree_Totale_Shift_Personne.at(0), v_Duree_Min_Shift_Consecutif_Personne.at(personne)))
            // On vérifie que la personne n'est pas en congé pour le 1er jour où pour les jours proches suivants pour qu'elle puisse commencer à travailler
        {
            int meilleur_candidat = -1;

            int plus_long_Duree_Shift = 0;

            //On parcourt les candidats
            for (int candidat = 0; candidat < instance->get_Nombre_Shift(); candidat++)
            {
                if (instance->get_Personne_Shift_Nbre_Max(personne, candidat) > 0) // La personne peut travailler sur ce shift
                {
                    int candidat_Shift_Duree = instance->get_Shift_Duree(candidat);

                    if (candidat_Shift_Duree > plus_long_Duree_Shift) //Pour le premier jour on se permet d'attribuer à la personne le shift de plus grande durée qu'elle peut réaliser.
                    {
                        meilleur_candidat = candidat;

                        plus_long_Duree_Shift = candidat_Shift_Duree;
                    }
                }
            }
            shift = meilleur_candidat;
        }

        S->v_v_IdShift_Par_Personne_et_Jour.at(personne).at(0) = shift;

        if (shift != -1)
        {
            v_Nbre_Shift_Consecutif.at(personne) = 1; //Initialisation des différents compteurs lorsque la personne ne travaille pas
            v_Nbre_Jour_OFF_Consecutif.at(personne) = 0;
            v_Duree_Totale_Shift_Personne.at(personne) = instance->get_Shift_Duree(shift);
            v_v_Nbre_Chaque_Shift_Pers.at(personne).at(shift) += 1;
        }
        else
        {
            v_Nbre_Shift_Consecutif.at(personne) = 0; //Initialisation des différents compteurs lorsque la personne travaille
            v_Nbre_Jour_OFF_Consecutif.at(personne) = 1;
            v_Duree_Totale_Shift_Personne.at(personne) = 0;
        }
    }


    //-----------------------Cas général----------------------------

    //On parcourt les personnes
    for (int personne = 0; personne < instance->get_Nombre_Personne(); personne++)
    {
        if (DEBUG) cout << "personne " << personne << " ";

        //On parcourt les jours à partir du deuxième
        for (int jour = 1; jour < instance->get_Nombre_Jour(); jour++)
        {
            if (DEBUG) cout << "jour " << jour << " ";

            int semaine = jour / 7;

            int shift = -1; //Initialisation du shift pour une personne et pour un jour

            // Si la durée totale des shifts de la personne avec le plus petit nouveau shift possible ne dépasse pas la durée maximale
            if (v_Duree_Totale_Shift_Personne.at(personne) + duree_shift_min <= instance->get_Personne_Duree_total_Max(personne))
            {
                // Si la personne ne travaille pas trop de fois de suite
                if (v_Nbre_Shift_Consecutif.at(personne) < instance->get_Personne_Nbre_Shift_Consecutif_Max(personne))
                {
                    // Si la personne peut travailler ce jour de weekend
                    if ((jour % 7 >= 5) && is_Peut_Travailler_WE_Semaine(v_l_Jour_OFF_Proche_WE.at(personne), v_Nbre_WE_Travaille.at(personne), instance->get_Personne_Nbre_WE_Max(personne), semaine)
                        || (jour % 7 < 5))
                    {
                        // Si la personne n'est pas en congé ce jour là
                        if (instance->is_Available_Personne_Jour(personne, jour))
                        {
                            if ((v_Nbre_Jour_OFF_Consecutif.at(personne) == 0) // Si la personne travaille depuis au moins le jour d'avant
                                || (v_Nbre_Jour_OFF_Consecutif.at(personne) >= instance->get_Personne_Jour_OFF_Consecutif_Min(personne) // Si la personne a eu assez de repos et peut reprendre son travail
                                    && is_Peut_Reprendre_Travail(instance, v_l_Jour_OFF_Proche_WE.at(personne), personne, jour, v_Nbre_WE_Travaille.at(personne), v_Duree_Totale_Shift_Personne.at(personne), v_Duree_Min_Shift_Consecutif_Personne.at(personne))))
                                // Et que la personne pourra travailler au moins le nombre consécutif minimum de quart d'heure de travail.
                            {
                                // Choix d'un shift
                                bool is_chosen_shift = false;

                                for (int candidat = 0; (candidat < instance->get_Nombre_Shift()) && (!is_chosen_shift); candidat++) //les id shift correspondent à candidat
                                {
                                    // Si la durée totale des shifts de la personne avec le candidat ne dépasse pas la durée maximale
                                    if (v_Duree_Totale_Shift_Personne.at(personne) + instance->get_Shift_Duree(candidat) <= instance->get_Personne_Duree_total_Max(personne))
                                    {
                                        if (S->v_v_IdShift_Par_Personne_et_Jour.at(personne).at(jour - 1) == -1) // Si le jour précédent était en jour repos
                                        {
                                            if (v_v_Nbre_Chaque_Shift_Pers.at(personne).at(candidat) < instance->get_Personne_Shift_Nbre_Max(personne, candidat))
                                            {
                                                //pour le choix des shifts dans le cas général, on ne s'embête pas à les optimiser, on prend le premier shift qui répond aux différentes contraintes
                                                shift = candidat;

                                                is_chosen_shift = true;
                                            }
                                        }
                                        else if (instance->is_possible_Shift_Succede(S->v_v_IdShift_Par_Personne_et_Jour.at(personne).at(jour - 1), candidat)) // Sinon, on vérifie que le candidat peut succéder au jour de travail précédent
                                        {
                                            if (v_v_Nbre_Chaque_Shift_Pers.at(personne).at(candidat) < instance->get_Personne_Shift_Nbre_Max(personne, candidat))
                                            {
                                                shift = candidat;

                                                is_chosen_shift = true;
                                            }
                                        }
                                    }

                                }
                            }
                        }
                    }
                }
            }
            //On effectue l'attibution du shift répondant aux différentes contraintes
            S->v_v_IdShift_Par_Personne_et_Jour.at(personne).at(jour) = shift;

            if (shift != -1)
            {
                //Mise à jour des différents compteurs quand la personne est de travaille

                v_Nbre_Shift_Consecutif.at(personne) += 1;

                v_Nbre_Jour_OFF_Consecutif.at(personne) = 0;

                v_Duree_Totale_Shift_Personne.at(personne) += instance->get_Shift_Duree(shift);

                v_v_Nbre_Chaque_Shift_Pers.at(personne).at(shift) += 1;

                if (jour % 7 == 6)	// Que ce jour là est dimanche (équivalent à : on a travaillé que le dimanche ou tout le WK)
                {
                    v_Nbre_WE_Travaille.at(personne) += 1;
                }
            }
            else
            {
                //Mise à jour des différents compteurs quand la personne est de repos

                v_Nbre_Shift_Consecutif.at(personne) = 0;

                v_Nbre_Jour_OFF_Consecutif.at(personne) += 1;

                if ((jour % 7 == 6) && ((S->v_v_IdShift_Par_Personne_et_Jour.at(personne).at(jour - 1) != -1)))	// Que ce jour là est dimanche, mais qu'on à travailler le samedi de la veille
                {
                    v_Nbre_WE_Travaille.at(personne) += 1;
                }
            }

            if (!v_l_Jour_OFF_Proche_WE.at(personne).empty() && jour % 7 == 6 && v_l_Jour_OFF_Proche_WE.at(personne).front() == semaine)
                //Si la personne a travaillé durant un WE qui se trouve sur la liste des WE prioritaire de travaille, on met à jour sa liste.
            {
                v_l_Jour_OFF_Proche_WE.at(personne).pop_front();
            }
        }
        if (DEBUG) cout << endl;
    }
    return S;
}



// ################### INDICATEURS POUR PRENDRE DES DECISIONS LORS DE L'INITIALISATION ###################


bool Heuristique::is_Peut_Travailler_WE_Semaine(list<int>& l_Jour_OFF_Proche_WE_Personne, int nbre_WE_Travaille_Personne, int nbre_WE_Max_Personne, int semaine)
{
    return ((nbre_WE_Travaille_Personne + l_Jour_OFF_Proche_WE_Personne.size() < nbre_WE_Max_Personne) // S'il reste encore des WE sur lesquels on peut travailler
        || ((nbre_WE_Travaille_Personne + l_Jour_OFF_Proche_WE_Personne.size() == nbre_WE_Max_Personne) // Ou s'il ne reste que des WE réservés
            && (!l_Jour_OFF_Proche_WE_Personne.empty() && l_Jour_OFF_Proche_WE_Personne.front() == semaine))); // Et que le WE de cette semaine est réservé
}


bool Heuristique::is_Peut_Reprendre_Travail(Instance* instance, list<int>& l_Jour_OFF_Proche_WE_Personne, int personne, int jour, int Nbre_WE_Travaille_Personne, int duree_totale_shift_personne, int duree_Min_Shift_Consecutif_Personne)
{
    // Pour chaque jour suivants sur lesquels la personne devra travailler ensuite
    for (int i = jour; (i < jour + instance->get_Personne_Nbre_Shift_Consecutif_Min(personne)) && (i < instance->get_Nombre_Jour()); i++)  //On vérifie que la personne est disponible sur le nombre de jours consécutifs suivants minimum
    {
        if ((!instance->is_Available_Personne_Jour(personne, i)) // Si la personnne est en congé ce jour là
            || ((i % 7 >= 5) // Ou que c'est le weekend
                && (!is_Peut_Travailler_WE_Semaine(l_Jour_OFF_Proche_WE_Personne, Nbre_WE_Travaille_Personne, instance->get_Personne_Nbre_WE_Max(personne), i / 7))) // Et qu'elle s'apprête à travailler trop de weekends
            || (duree_totale_shift_personne + duree_Min_Shift_Consecutif_Personne > instance->get_Personne_Duree_total_Max(personne))) // Ou la personne ne dépassera pas sa durée maximale de travail
        {
            return false;
        }
    }
    return true;
}


int Heuristique::Duree_Minimum_Combinaison_Shift_Possible(Instance* instance, int personne)
{
    int nb_shift_consecutif_min = instance->get_Personne_Nbre_Shift_Consecutif_Min(personne);

    vector<int> combinaison_shift(nb_shift_consecutif_min);

    bool est_initialise = false;

    int shift = 0;

    //On initialise le premier jour de la combinaison de shift de durée minimum on prenant un shift de durée minimum compatible avec la personne
    while (!est_initialise)
    {
        if (instance->get_Personne_Shift_Nbre_Max(personne, shift) > 0)
        {
            combinaison_shift.at(0) = shift;
            est_initialise = true;
        }
        shift++;
    }

    int duree_Totale_Combinaison_Shift = duree_Totale_Combinaison_Shift = instance->get_Shift_Duree(combinaison_shift[0]);

    //On finit d'initialiser les autres jours de la combinaison de shift minimu de la personne
    for (int i = 1; i < nb_shift_consecutif_min; i++)
    {
        bool est_Choisi_Shift_Combinaison = false;

        //On parcourt tout les shifts par ordre de durée en s'assurant d'en choisir un qui peut succéder au précédent 
        for (shift = 0; shift < instance->get_Nombre_Shift() && !est_Choisi_Shift_Combinaison; shift++)
        {
            if (instance->get_Personne_Shift_Nbre_Max(personne, shift) > 0
                && instance->is_possible_Shift_Succede(combinaison_shift[i - 1], shift))
            {
                combinaison_shift.at(i) = shift;

                est_Choisi_Shift_Combinaison = true;
            }
        }
        if (!est_Choisi_Shift_Combinaison)
        {
            //Le cas où la combinaison de shift minimum n'existerait pas, qui serait possible uniquement s'il existe un shift dont aucun autre ne pourrait succéder
            cout << "Erreur dans Duree_Minimum_Combinaison_Shift_Possible : Combinaison minimum impossible" << endl;

            break;
        }
        //On calcule la durée total de la combinaison de shift construite
        duree_Totale_Combinaison_Shift += instance->get_Shift_Duree(combinaison_shift[i]);
    }

    return duree_Totale_Combinaison_Shift;
}



bool Heuristique::is_Jour_OFF_Proche_WE(Instance* instance, int jour_OFF, int i_Nbre_Jour_OFF_Consecutif_Min, int i_Nbre_Shift_Consecutif_Min)
{
    for (int jour_Travail_Apres_Jour_OFF = jour_OFF + i_Nbre_Jour_OFF_Consecutif_Min; jour_Travail_Apres_Jour_OFF < jour_OFF + i_Nbre_Jour_OFF_Consecutif_Min + i_Nbre_Shift_Consecutif_Min; jour_Travail_Apres_Jour_OFF++)
    {
        if (jour_Travail_Apres_Jour_OFF % 7 >= 5)
        {
            return true;
        }
    }
    return false;
}

