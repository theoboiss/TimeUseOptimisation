#include "MetaHeuristique.hpp"

#define DEBUG false
#define DUREE_MAX 175.0
#define kMAX 4.0
#define DUREE_VOISINAGE_MAX DUREE_MAX/kMAX

// ################### ALGORITHMES GENERAUX ###################


Solution* MetaHeuristique::Descente_Voisinage_Variable(Solution solutionRealisable, Instance* instance, float coeff_Valeur_FO_Contrainte, chrono::time_point<chrono::system_clock> chrono_start)
{
    int k = 1;
    int compteur = 1;
    Solution* meilleureSolution = &solutionRealisable;
    
    while (k <= kMAX && Outils::get_Secondes_Ecoulees(chrono_start) < DUREE_MAX)
    {
        if (DEBUG) cout << compteur++ << " " << k << endl;

        Solution* meilleureSolutionVoisinage = Recherche_Meilleure_Solution_Voisinage(*meilleureSolution, instance, k, coeff_Valeur_FO_Contrainte, chrono_start);
        if (meilleureSolutionVoisinage == nullptr)
        {
            k = k + 1;
        }
        else if (meilleureSolutionVoisinage->i_valeur_fonction_objectif < meilleureSolution->i_valeur_fonction_objectif)
        {
            if (meilleureSolution != &solutionRealisable)
            {
                delete meilleureSolution;
            }
            meilleureSolution = meilleureSolutionVoisinage;
            k = 1;
        }
        else
        {
            delete meilleureSolutionVoisinage;
            k = k + 1;
        }

        if (DEBUG) cout << endl << endl;

    }

    if (meilleureSolution != &solutionRealisable)
    {
        return meilleureSolution;
    }
    return nullptr;
}


Solution* MetaHeuristique::Recherche_Meilleure_Solution_Voisinage(Solution solutionRealisable, Instance* instance, int k, float coeff_Valeur_FO_Contrainte, chrono::time_point<chrono::system_clock> chrono_start)
{
    Solution* p_candidat = &solutionRealisable;
    Solution solutionVoisine = *p_candidat;

    int nombre_personne, nombre_jour, nombre_shift, nbIterMax, it;
    nombre_personne = instance->get_Nombre_Personne();
    nombre_jour = instance->get_Nombre_Jour();
    nombre_shift = instance->get_Nombre_Shift();
    //it = max(1, nombre_personne / 10);
    it = 1;

    list<int> personnes_problematiques = Outils::Personnes_Contraintes_Non_Respectes(p_candidat, instance);

    chrono::time_point<chrono::system_clock> chrono_voisinage_start = chrono::system_clock::now();
    switch (k)
    {
    case 1:
        /*
        * Opérateur de modification des shifts
        * Domaine de définition de proba : ]0..1]
        */
        if (personnes_problematiques.size())
        {
            for (int ite_personne = 0; ite_personne < personnes_problematiques.size(); ite_personne++)
            {
                int personne = personnes_problematiques.front();
                personnes_problematiques.pop_front();
                for (int jour = 0; jour < nombre_jour; jour++)
                {
                    if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                        || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                    {
                        return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                    }

                    int shift_personne_jour = solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour];
                    if (shift_personne_jour != -1)
                    {
                        for (int incrShift = 1; incrShift < nombre_shift; incrShift++)
                        {
                            Operation_Operateur_Modification_Shift(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                                personne, jour, nombre_shift,
                                Operateur_Modification_Shift_Incrementation);
                        }
                        solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] = shift_personne_jour;
                    }
                }
            }
        }
        for (int personne = 0; personne < nombre_personne; personne++)
        {
            for (int jour = 0; jour < nombre_jour; jour++)
            {
                if (    Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                     || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                {
                    return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                }

                int shift_personne_jour = solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour];
                if (shift_personne_jour != -1)
                {
                    for (int incrShift = 1; incrShift < nombre_shift; incrShift++)
                    {
                        Operation_Operateur_Modification_Shift(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                            personne, jour, nombre_shift,
                            Operateur_Modification_Shift_Incrementation);
                    }
                    solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] = shift_personne_jour;
                }
            }
        }
        break;

    case 2:
        /*
        * Opérateur de swap des jours
        * Domaine de définition de deplacement : {0..nombre_jour}
        */
        if (personnes_problematiques.size())
        {
            for (int b = 1; b < nombre_jour; b += it)
            {
                for (int ite_personne = 0; ite_personne < personnes_problematiques.size(); ite_personne++)
                {
                    int personne = personnes_problematiques.front();
                    personnes_problematiques.pop_front();
                    for (int jour = 0; jour < nombre_jour; jour++)
                    {
                        if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                            || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                        {
                            return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                        }

                        if (solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] != -1)
                        {
                            Operation_Operateur_Swap(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                                personne, jour,
                                Operateur_Swap_Jour, nombre_jour, b);
                        }
                    }
                }
            }
        }
        for (int b = 1; b < nombre_jour; b += it)
        {
            for (int personne = 0; personne < nombre_personne; personne++)
            {
                for (int jour = 0; jour < nombre_jour; jour++)
                {
                    if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                        || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                    {
                        return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                    }

                    if (solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] != -1)
                    {
                        Operation_Operateur_Swap(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                            personne, jour,
                            Operateur_Swap_Jour, nombre_jour, b);
                    }
                }
            }
        }
        break;

    case 3:
        /*
        * Opérateur de swap des personnes
        * Domaine de définition de deplacement : {0..nombre_personne}
        */
        if (personnes_problematiques.size())
        {
            for (int b = 1; b < nombre_personne; b += it)
            {
                for (int ite_personne = 0; ite_personne < personnes_problematiques.size(); ite_personne++)
                {
                    int personne = personnes_problematiques.front();
                    personnes_problematiques.pop_front();
                    for (int jour = 0; jour < nombre_jour; jour++)
                    {
                        if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                            || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                        {
                            return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                        }

                        Operation_Operateur_Swap(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                            personne, jour,
                            Operateur_Swap_Personne, nombre_personne, b);
                    }
                }
            }
        }
        for (int b = 1; b < nombre_personne; b += it)
        {
            for (int personne = 0; personne < nombre_personne; personne++)
            {
                for (int jour = 0; jour < nombre_jour; jour++)
                {
                    if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                        || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                    {
                        return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                    }

                    Operation_Operateur_Swap(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                        personne, jour,
                        Operateur_Swap_Personne, nombre_personne, b);
                }
            }
        }
        break;

    case 4:
        /*
        * Opérateur de modification des shifts aléatoire comprenant les jours de repos
        */
        if (personnes_problematiques.size())
        {
            for (int ite_personne = 0; ite_personne < personnes_problematiques.size(); ite_personne++)
            {
                int personne = personnes_problematiques.front();
                personnes_problematiques.pop_front();
                for (int jour = 0; jour < nombre_jour; jour++)
                {
                    if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                        || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                    {
                        return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                    }

                    int shift_personne_jour = solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour];
                    Operation_Operateur_Modification_Shift(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                        personne, jour, nombre_shift,
                        Operateur_Modification_Shift_Aleatoire);
                    solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] = shift_personne_jour;
                }
            }
        }
        for (int personne = 0; personne < nombre_personne; personne++)
        {
            for (int jour = 0; jour < nombre_jour; jour++)
            {
                if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                    || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                {
                    return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                }

                int shift_personne_jour = solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour];
                Operation_Operateur_Modification_Shift(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                    personne, jour, nombre_shift,
                    Operateur_Modification_Shift_Aleatoire);
                solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] = shift_personne_jour;
            }
        }
        break;
    }

    return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
}



// ################### OPERATIONS MODIFICATION DE SHIFT ###################


void MetaHeuristique::Operation_Operateur_Modification_Shift(Solution* solutionRealisable, Solution** p_candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
    int personne, int jour, int nombre_shift,
    void OperateurModificationShift(Solution*, int, int, int))
{
    if (DEBUG) if ((personne + jour) % ((solutionVoisine->v_v_IdShift_Par_Personne_et_Jour.size() + solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[0].size()) / 10) == 0)
        cout << " . ";

    OperateurModificationShift(solutionVoisine, nombre_shift, personne, jour);
    Heuristique::Init_Valeur_FO_Indicative(solutionVoisine, instance, coeff_Valeur_FO_Contrainte);

    if (solutionVoisine->i_valeur_fonction_objectif >= 0 && solutionVoisine->i_valeur_fonction_objectif < (*p_candidat)->i_valeur_fonction_objectif)
    {
        if (*p_candidat != solutionRealisable)
        {
            delete* p_candidat;
        }
        *p_candidat = Outils::Copie_Solution(solutionVoisine);

        if (DEBUG)
        {
            cout << " ! " << endl;
            solutionVoisine->Verification_Solution(instance);
            cout << "Valeur de la fonction objective sans pénalités : " << Outils::Calcul_Valeur_FO(solutionVoisine, instance) << endl;
            cout << "Valeur de la fonction objectif indicative (avec pénalités) : " << solutionVoisine->i_valeur_fonction_objectif << endl << endl;
        }
    }
}

// Opérateurs

void MetaHeuristique::Operateur_Modification_Shift_Incrementation(Solution* uneSolution, int nombre_shift, int personne, int jour)
{
    int& shift_personne_jour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];

    // Modifie les shifts
    shift_personne_jour = (shift_personne_jour + 1) % nombre_shift;
}

void MetaHeuristique::Operateur_Modification_Shift_Aleatoire(Solution* uneSolution, int nombre_shift, int personne, int jour)
{
    int& shift_personne_jour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
    int proba_alea = ((double)(rand() % 100)) / 100;
    
    // Modifie les shifts y compris avec des jours de repos
    if (shift_personne_jour != -1) // on est sur un jour de travail
    {
        if (proba_alea < 0.30) // 0.29 ~ 2/7 : proba d'un weekend dans une semaine
        {
            shift_personne_jour = -1;
        }
        else // jour de travail aléatoire
        {
            shift_personne_jour = rand() % nombre_shift;
        }
    }
    else // on est sur un jour de repos
    {
        if (proba_alea < 0.30) // 0.29 ~ 2/7 : proba d'un weekend dans une semaine
        {
            shift_personne_jour = rand() % nombre_shift;
        }
    }
}



// ################### OPERATIONS SWAP ###################


void MetaHeuristique::Operation_Operateur_Swap(Solution* solutionRealisable, Solution** p_candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
    int personne, int jour,
    void OperateurSwap(Solution*, int, int, int, int), int modulo, int deplacement)
{
    if (DEBUG) if ((personne + jour) % ((solutionVoisine->v_v_IdShift_Par_Personne_et_Jour.size() + solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[0].size()) / 10) == 0
            && deplacement % (solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[0].size() / 5) == 0)
        cout << " . ";

    OperateurSwap(solutionVoisine, modulo, personne, jour, deplacement);
    Heuristique::Init_Valeur_FO_Indicative(solutionVoisine, instance, coeff_Valeur_FO_Contrainte);

    if (solutionVoisine->i_valeur_fonction_objectif >= 0 && solutionVoisine->i_valeur_fonction_objectif < (*p_candidat)->i_valeur_fonction_objectif)
    {
        if (*p_candidat != solutionRealisable)
        {
            delete *p_candidat;
        }
        *p_candidat = Outils::Copie_Solution(solutionVoisine);

        if (DEBUG) {
            cout << " ! " << endl;
            solutionVoisine->Verification_Solution(instance);
            cout << "Valeur de la fonction objective sans pénalités : " << Outils::Calcul_Valeur_FO(solutionVoisine, instance) << endl;
            cout << "Valeur de la fonction objectif indicative (avec pénalités) : " << solutionVoisine->i_valeur_fonction_objectif << endl << endl;
        }
    }
    OperateurSwap(solutionVoisine, modulo, personne, jour, deplacement);
}

// Opérateurs

void MetaHeuristique::Operateur_Swap_Jour(Solution* uneSolution, int nombre_jour, int personne, int jour, int deplacement)
{
    // Initialisation
    int* p_to_swap_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
    int* p_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][(jour + deplacement) % nombre_jour];
    int temp = *p_to_swap_shift_personne_jour;

    // Swap
    *p_to_swap_shift_personne_jour = *p_shift_personne_jour;
    *p_shift_personne_jour = temp;
}

void MetaHeuristique::Operateur_Swap_Personne(Solution* uneSolution, int nombre_personne, int personne, int jour, int deplacement)
{
    // Initialisation
    int* p_to_swap_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
    int* p_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[(personne + deplacement) % nombre_personne][jour];
    int temp = *p_to_swap_shift_personne_jour;

    // Swap
    *p_to_swap_shift_personne_jour = *p_shift_personne_jour;
    *p_shift_personne_jour = temp;
}
