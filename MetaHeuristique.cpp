#include "MetaHeuristique.hpp"

#define DEBUG true
#define DUREE_MAX 175.0
#define kMAX 4.0
#define DUREE_VOISINAGE_MAX DUREE_MAX/kMAX

// ################### ALGORITHMES GENERAUX ###################


Solution* MetaHeuristique::MeilleureSolution(Solution solutionRealisable, Instance* instance, float coeff_Valeur_FO_Contrainte, chrono::time_point<chrono::system_clock> chrono_start)
{
    int k = 1;
    int compteur = 1;
    Solution* meilleureSolution = &solutionRealisable;
    
    while (k <= kMAX && Outils::getSecondesEcoulees(chrono_start) < DUREE_MAX)
    {
        if (DEBUG) cout << compteur++ << " " << k << endl;

        Solution* meilleureSolutionVoisinage = RechercheVoisinageVariable(*meilleureSolution, instance, k, coeff_Valeur_FO_Contrainte, chrono_start);
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


Solution* MetaHeuristique::RechercheVoisinageVariable(Solution solutionRealisable, Instance* instance, int k, float coeff_Valeur_FO_Contrainte, chrono::time_point<chrono::system_clock> chrono_start)
{
    Solution* p_candidat = &solutionRealisable;
    Solution solutionVoisine = *p_candidat;

    int nombre_personne, nombre_jour, nombre_shift, nbIterMax, it;
    nombre_personne = instance->get_Nombre_Personne();
    nombre_jour = instance->get_Nombre_Jour();
    nombre_shift = instance->get_Nombre_Shift();
    //it = max(1, nombre_personne / 10);
    it = 1;

    list<int> personnes_problematiques = Outils::Personne_Contraintes_Non_Respectes(p_candidat, instance);

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
                    if (Outils::getSecondesEcoulees(chrono_start) >= DUREE_MAX
                        || Outils::getSecondesEcoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                    {
                        return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                    }

                    int shift_personne_jour = solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour];
                    if (shift_personne_jour != -1)
                    {
                        for (int incrShift = 1; incrShift < nombre_shift; incrShift++)
                        {
                            OperationOperateurModificationShift(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                                personne, jour, nombre_shift,
                                OperateurModificationShift);
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
                if (    Outils::getSecondesEcoulees(chrono_start) >= DUREE_MAX
                     || Outils::getSecondesEcoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                {
                    return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                }

                int shift_personne_jour = solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour];
                if (shift_personne_jour != -1)
                {
                    for (int incrShift = 1; incrShift < nombre_shift; incrShift++)
                    {
                        OperationOperateurModificationShift(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                            personne, jour, nombre_shift,
                            OperateurModificationShift);
                    }
                    solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] = shift_personne_jour;
                }
            }
        }
        break;

    case 2:
        /*
        * Opérateur de swap des jours
        * Domaine de définition de b : {0..nombre_jour}
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
                        if (Outils::getSecondesEcoulees(chrono_start) >= DUREE_MAX
                            || Outils::getSecondesEcoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                        {
                            return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                        }

                        if (solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] != -1)
                        {
                            OperationOperateurSwap(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                                personne, jour,
                                OperateurSwapJour, nombre_jour, b);
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
                    if (Outils::getSecondesEcoulees(chrono_start) >= DUREE_MAX
                        || Outils::getSecondesEcoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                    {
                        return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                    }

                    if (solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] != -1)
                    {
                        OperationOperateurSwap(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                            personne, jour,
                            OperateurSwapJour, nombre_jour, b);
                    }
                }
            }
        }
        break;

    case 3:
        /*
        * Opérateur de swap des personnes
        * Domaine de définition de b : {0..nombre_personne}
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
                        if (Outils::getSecondesEcoulees(chrono_start) >= DUREE_MAX
                            || Outils::getSecondesEcoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                        {
                            return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                        }

                        OperationOperateurSwap(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                            personne, jour,
                            OperateurSwapPersonne, nombre_personne, b);
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
                    if (Outils::getSecondesEcoulees(chrono_start) >= DUREE_MAX
                        || Outils::getSecondesEcoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                    {
                        return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                    }

                    OperationOperateurSwap(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                        personne, jour,
                        OperateurSwapPersonne, nombre_personne, b);
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
                    if (Outils::getSecondesEcoulees(chrono_start) >= DUREE_MAX
                        || Outils::getSecondesEcoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                    {
                        return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                    }

                    int shift_personne_jour = solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour];
                    OperationOperateurModificationShift(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                        personne, jour, nombre_shift,
                        OperateurModificationShiftAleatoire);
                    solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] = shift_personne_jour;
                }
            }
        }
        for (int personne = 0; personne < nombre_personne; personne++)
        {
            for (int jour = 0; jour < nombre_jour; jour++)
            {
                if (Outils::getSecondesEcoulees(chrono_start) >= DUREE_MAX
                    || Outils::getSecondesEcoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX)
                {
                    return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
                }

                int shift_personne_jour = solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour];
                OperationOperateurModificationShift(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                    personne, jour, nombre_shift,
                    OperateurModificationShiftAleatoire);
                solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] = shift_personne_jour;
            }
        }
        break;
    }

    return (p_candidat == &solutionRealisable) ? nullptr : p_candidat;
}



// ################### OPERATIONS MODIFICATION DE SHIFT ###################


void MetaHeuristique::OperationOperateurModificationShift(Solution* solutionRealisable, Solution** p_candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
    int personne, int jour, int nombre_shift,
    void OperateurModificationShift(Solution*, int, int, int))
{
    if (DEBUG) if ((personne + jour) % ((solutionVoisine->v_v_IdShift_Par_Personne_et_Jour.size() + solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[0].size()) / 10) == 0)
        cout << " . ";

    OperateurModificationShift(solutionVoisine, nombre_shift, personne, jour);
    Heuristique::InitValeurFonctionObjectif(solutionVoisine, instance, coeff_Valeur_FO_Contrainte);

    if (solutionVoisine->i_valeur_fonction_objectif >= 0 && solutionVoisine->i_valeur_fonction_objectif < (*p_candidat)->i_valeur_fonction_objectif)
    {
        if (*p_candidat != solutionRealisable)
        {
            delete* p_candidat;
        }
        *p_candidat = Outils::CopieSolution(solutionVoisine);

        if (DEBUG)
        {
            cout << " ! " << endl;
            solutionVoisine->Verification_Solution(instance);
            cout << "Valeur de la fonction objective sans pénalités : " << Outils::i_Calcul_Valeur_Fonction_Objectif(solutionVoisine, instance) << endl;
            cout << "Valeur de la fonction objectif : " << solutionVoisine->i_valeur_fonction_objectif << endl << endl;
        }
    }
}

// Opérateurs

void MetaHeuristique::OperateurModificationShift(Solution* uneSolution, int nombre_shift, int personne, int jour)
{
    int& shift_personne_jour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];

    // Modifie les shifts
    shift_personne_jour = (shift_personne_jour + 1) % nombre_shift;
}

void MetaHeuristique::OperateurModificationShiftAleatoire(Solution* uneSolution, int nombre_shift, int personne, int jour)
{
    int& shift_personne_jour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
    
    // Modifie les shifts y compris avec des jours de repos
    shift_personne_jour = rand() % (nombre_shift+1) - 1;
}



// ################### OPERATIONS SWAP ###################


void MetaHeuristique::OperationOperateurSwap(Solution* solutionRealisable, Solution** p_candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
    int personne, int jour,
    int* OperateurSwap(Solution*, int, int, int, int), int modulo, int b)
{
    if (DEBUG) if ((personne + jour) % ((solutionVoisine->v_v_IdShift_Par_Personne_et_Jour.size() + solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[0].size()) / 10) == 0
            && b % (solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[0].size() / 5) == 0)
        cout << " . ";

    OperateurSwap(solutionVoisine, modulo, personne, jour, b);
    Heuristique::InitValeurFonctionObjectif(solutionVoisine, instance, coeff_Valeur_FO_Contrainte);

    if (solutionVoisine->i_valeur_fonction_objectif >= 0 && solutionVoisine->i_valeur_fonction_objectif < (*p_candidat)->i_valeur_fonction_objectif)
    {
        if (*p_candidat != solutionRealisable)
        {
            delete *p_candidat;
        }
        *p_candidat = Outils::CopieSolution(solutionVoisine);

        if (DEBUG) {
            cout << " ! " << endl;
            solutionVoisine->Verification_Solution(instance);
            cout << "Valeur de la fonction objective sans pénalités : " << Outils::i_Calcul_Valeur_Fonction_Objectif(solutionVoisine, instance) << endl;
            cout << "Valeur de la fonction objectif : " << solutionVoisine->i_valeur_fonction_objectif << endl << endl;
        }
    }
    OperateurSwap(solutionVoisine, modulo, personne, jour, b);
}

// Opérateurs

int* MetaHeuristique::OperateurSwapJour(Solution* uneSolution, int nombre_jour, int personne, int jour, int b)
{
    int* p_to_swap_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
    int* p_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][(jour + b) % nombre_jour];
    int temp = *p_to_swap_shift_personne_jour;

    // Modifie les shifts
    *p_to_swap_shift_personne_jour = *p_shift_personne_jour;
    *p_shift_personne_jour = temp;

    return p_shift_personne_jour;
}

int* MetaHeuristique::OperateurSwapPersonne(Solution* uneSolution, int nombre_personne, int personne, int jour, int b)
{
    int* p_to_swap_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
    int* p_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[(personne + b) % nombre_personne][jour];
    int temp = *p_to_swap_shift_personne_jour;

    // Modifie les shifts
    *p_to_swap_shift_personne_jour = *p_shift_personne_jour;
    *p_shift_personne_jour = temp;

    return p_shift_personne_jour;
}
