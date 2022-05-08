#include "MetaHeuristique.hpp"

#define DEBUG false
#define DUREE_MAX 179.0
#define kMAX 10
#define DUREE_VOISINAGE_MAX DUREE_MAX/kMAX

// ################### ALGORITHMES GENERAUX ###################


Solution* MetaHeuristique::Descente_Voisinage_Variable(Solution solutionInitiale, Instance* instance, float coeff_Valeur_FO_Contrainte, chrono::time_point<chrono::system_clock> chrono_start)
{
    //Initialisation
    int k = 1;
    int compteur = 1;
    Solution* meilleureSolution = &solutionInitiale;
    
    while (k <= kMAX && Outils::get_Secondes_Ecoulees(chrono_start) < DUREE_MAX) // On utilise les opérateurs classés par ordre d'efficacité
    {
        if (DEBUG) cout << compteur++ << " " << k << endl;

        // On recherche l'optimum local du voisinage délimité par l'opérateur k
        Solution* meilleureSolutionVoisinage = Recherche_Meilleure_Solution_Voisinage(*meilleureSolution, instance, k, coeff_Valeur_FO_Contrainte, chrono_start);

        if (meilleureSolutionVoisinage == nullptr) // Pas de meilleure solution trouvée dans le voisinage, on passe à l'itérateur suivant
        {
            k = k + 1;
        }
        else if (meilleureSolutionVoisinage->i_valeur_fonction_objectif < meilleureSolution->i_valeur_fonction_objectif) // La meilleure solution du voisinage est meilleure que la précédente
        {
            if (meilleureSolution != &solutionInitiale)
            {
                delete meilleureSolution;
            }
            meilleureSolution = meilleureSolutionVoisinage;
            k = 1;
        }
        else // La meilleure solution du voisinage n'est pas meilleure que la précédente
        {
            delete meilleureSolutionVoisinage;
            k = k + 1;
        }

        if (DEBUG) cout << endl << endl;

    }

    return (meilleureSolution == &solutionInitiale) ? nullptr : meilleureSolution; // On renvoie la meilleure solution trouvée si ce n'est pas la solution initiale
}


Solution* MetaHeuristique::Recherche_Meilleure_Solution_Voisinage(Solution uneSolution, Instance* instance, int k, float coeff_Valeur_FO_Contrainte, chrono::time_point<chrono::system_clock> chrono_start)
{
    //Initialisation

    Solution* p_candidat = &uneSolution;
    Solution solutionVoisine = *p_candidat;

    int nombre_personne, nombre_jour, nombre_shift, min_nombre_personne_jour, it;
    nombre_personne = instance->get_Nombre_Personne();
    nombre_jour = instance->get_Nombre_Jour();
    nombre_shift = instance->get_Nombre_Shift();
    min_nombre_personne_jour = min(nombre_personne, nombre_jour);
    //it = max(1, nombre_personne / 10);
    it = 1;

    deque<int> personnes_problematiques = Outils::Personnes_Contraintes_Non_Respectes(p_candidat, instance);

    vector<int> jours(nombre_jour);
    vector<int> personnes(nombre_personne);
    for (int j = 0; j < nombre_jour; j++)
    {
        jours[j] = j;
    }
    for (int p = 0; p < nombre_personne; p++)
    {
        personnes[p] = p;
    }
    vector<int>::iterator it_personne = personnes.begin() + (rand() % personnes.size());
    vector<int>::iterator it_jour = jours.begin() + (rand() % jours.size());

    chrono::time_point<chrono::system_clock> chrono_voisinage_start = chrono::system_clock::now();

    
    switch (k) // On applique l'opérateur désiré
    {

    case 1:
        /*
        * Opérateur de modification des shifts aléatoire comprenant les jours de repos
        */
        if (personnes_problematiques.size()) // On applique l'opérateur aux personnes problématiques en priorité
        {
            for (int ite_personne = 0; ite_personne < personnes_problematiques.size(); ite_personne++)
            {
                int personne = personnes_problematiques.front();
                personnes_problematiques.pop_front();
                for (int jour = 0; jour < nombre_jour; jour++) // On applique l'opérateur sur tous les jours
                {

                    if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                        || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX) // On vérifie qu'il nous reste un temps raisonnable
                    {
                        return (p_candidat == &uneSolution) ? nullptr : p_candidat; // On renvoie la meilleure solution trouvée si ce n'est pas la solution de départ
                    }

                    int shift_personne_jour = solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour];
                    Operation_Operateur_Modification_Shift(&uneSolution, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte, // On applique l'opérateur lors de l'opération paramétrée
                        personne, jour, nombre_shift,
                        Operateur_Modification_Shift_Aleatoire);
                    solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] = shift_personne_jour; // On revient dans le voisinage
                }
            }
        }
        
        for (int personne = 0; personne < nombre_personne; personne++) // On applique l'opérateur sur toutes les personnes
        {
            for (int jour = 0; jour < nombre_jour; jour++) // On applique l'opérateur sur tous les jours
            {

                if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                    || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX) // On vérifie qu'il nous reste un temps raisonnable
                {
                    return (p_candidat == &uneSolution) ? nullptr : p_candidat; // On renvoie la meilleure solution trouvée si ce n'est pas la solution de départ
                }

                int shift_personne_jour = solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour];
                Operation_Operateur_Modification_Shift(&uneSolution, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte, // On applique l'opérateur lors de l'opération paramétrée
                    personne, jour, nombre_shift,
                    Operateur_Modification_Shift_Aleatoire);
                solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] = shift_personne_jour; // On revient dans le voisinage
            }
        }
        break;


    case 2:
        /*
        * Opérateur de swap des personnes et des jours
        * Domaine de définition de deplacement : {0..min(nombre_personne,nombre_jour)}
        */
        if (personnes_problematiques.size()) // On applique l'opérateur aux personnes problématiques en priorité
        {
            for (int deplacement = 1; deplacement < min_nombre_personne_jour; deplacement += it)
            {
                for (int ite_personne = 0; ite_personne < personnes_problematiques.size(); ite_personne++)
                {
                    int personne = personnes_problematiques.front();
                    personnes_problematiques.pop_front();
                    for (int jour = 0; jour < nombre_jour; jour++) // On applique l'opérateur sur tous les jours
                    {

                        if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                            || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX) // On vérifie qu'il nous reste un temps raisonnable
                        {
                            return (p_candidat == &uneSolution) ? nullptr : p_candidat; // On renvoie la meilleure solution trouvée si ce n'est pas la solution de départ
                        }

                        Operation_Operateur_Swap(&uneSolution, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte, // On applique l'opérateur lors de l'opération paramétrée
                            personne, jour,
                            Operateur_Swap_Personne_Jour, nombre_personne, nombre_jour, deplacement);
                    }
                }
            }
        }

        for (int deplacement = 1; deplacement < min_nombre_personne_jour; deplacement += it) // On applique l'opérateur sur toutes les personnes
        {
            for (int personne = 0; personne < nombre_personne; personne++)
            {
                for (int jour = 0; jour < nombre_jour; jour++) // On applique l'opérateur sur tous les jours
                {

                    if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                        || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX) // On vérifie qu'il nous reste un temps raisonnable
                    {
                        return (p_candidat == &uneSolution) ? nullptr : p_candidat; // On renvoie la meilleure solution trouvée si ce n'est pas la solution de départ
                    }

                    Operation_Operateur_Swap(&uneSolution, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte, // On applique l'opérateur lors de l'opération paramétrée
                        personne, jour,
                        Operateur_Swap_Personne_Jour, nombre_personne, nombre_jour, deplacement);
                }
            }
        }
        break;


    case 3:
        /*
        * Opérateur de swap des jours
        * Domaine de définition de deplacement : {0..nombre_jour}
        */
        if (personnes_problematiques.size()) // On applique l'opérateur aux personnes problématiques en priorité
        {
            for (int deplacement = 1; deplacement < nombre_jour; deplacement += it)
            {
                for (int ite_personne = 0; ite_personne < personnes_problematiques.size(); ite_personne++)
                {
                    int personne = personnes_problematiques.front();
                    personnes_problematiques.pop_front();
                    for (int jour = 0; jour < nombre_jour; jour++) // On applique l'opérateur sur tous les jours
                    {

                        if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                            || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX) // On vérifie qu'il nous reste un temps raisonnable
                        {
                            return (p_candidat == &uneSolution) ? nullptr : p_candidat; // On renvoie la meilleure solution trouvée si ce n'est pas la solution de départ
                        }

                        if (solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] != -1)
                        {
                            Operation_Operateur_Swap(&uneSolution, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte, // On applique l'opérateur lors de l'opération paramétrée
                                personne, jour,
                                Operateur_Swap_Jour, nombre_personne, nombre_jour, deplacement);
                        }
                    }
                }
            }
        }

        for (int deplacement = 1; deplacement < nombre_jour; deplacement += it)
        {
            for (int personne = 0; personne < nombre_personne; personne++) // On applique l'opérateur sur toutes les personnes
            {
                for (int jour = 0; jour < nombre_jour; jour++) // On applique l'opérateur sur tous les jours
                {

                    if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                        || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX) // On vérifie qu'il nous reste un temps raisonnable
                    {
                        return (p_candidat == &uneSolution) ? nullptr : p_candidat; // On renvoie la meilleure solution trouvée si ce n'est pas la solution de départ
                    }

                    if (solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] != -1)
                    {
                        Operation_Operateur_Swap(&uneSolution, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte, // On applique l'opérateur lors de l'opération paramétrée
                            personne, jour,
                            Operateur_Swap_Jour, nombre_personne, nombre_jour, deplacement);
                    }
                }
            }
        }
        break;

    
    case 4:
        /*
        * Opérateur de modification des shifts avec incrémentation
        * Domaine de définition de incrShift : {1..nombre_shift}
        */
        if (personnes_problematiques.size()) // On applique l'opérateur aux personnes problématiques en priorité
        {
            for (int ite_personne = 0; ite_personne < personnes_problematiques.size(); ite_personne++)
            {
                int personne = personnes_problematiques.front();
                personnes_problematiques.pop_front();

                for (int jour = 0; jour < nombre_jour; jour++) // On applique l'opérateur sur tous les jours
                {

                    if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                        || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX) // On vérifie qu'il nous reste un temps raisonnable
                    {
                        return (p_candidat == &uneSolution) ? nullptr : p_candidat; // On renvoie la meilleure solution trouvée si ce n'est pas la solution de départ
                    }

                    int shift_personne_jour = solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour];
                    if (shift_personne_jour != -1)
                    {
                        for (int incrShift = 1; incrShift < nombre_shift; incrShift++)
                        {
                            Operation_Operateur_Modification_Shift(&uneSolution, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte, // On applique l'opérateur lors de l'opération paramétrée
                                personne, jour, nombre_shift,
                                Operateur_Modification_Shift_Incrementation);
                        }
                        solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] = shift_personne_jour; // On revient dans le voisinage
                    }
                }
            }
        }

        for (int personne = 0; personne < nombre_personne; personne++) // On applique l'opérateur sur toutes les personnes
        {
            for (int jour = 0; jour < nombre_jour; jour++) // On applique l'opérateur sur tous les jours
            {

                if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                    || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX) // On vérifie qu'il nous reste un temps raisonnable
                {
                    return (p_candidat == &uneSolution) ? nullptr : p_candidat; // On renvoie la meilleure solution trouvée si ce n'est pas la solution de départ
                }

                int shift_personne_jour = solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour];
                if (shift_personne_jour != -1)
                {
                    for (int incrShift = 1; incrShift < nombre_shift; incrShift++)
                    {
                        Operation_Operateur_Modification_Shift(&uneSolution, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte, // On applique l'opérateur lors de l'opération paramétrée
                            personne, jour, nombre_shift,
                            Operateur_Modification_Shift_Incrementation);
                    }
                    solutionVoisine.v_v_IdShift_Par_Personne_et_Jour[personne][jour] = shift_personne_jour; // On revient dans le voisinage
                }
            }
        }
        break;


    case 5:
        /*
        * Opérateur de swap des personnes
        * Domaine de définition de deplacement : {0..nombre_personne}
        */
        if (personnes_problematiques.size()) // On applique l'opérateur aux personnes problématiques en priorité
        {
            for (int deplacement = 1; deplacement < nombre_personne; deplacement += it) // On fait varier le critère d'exploration du voisinage
            {
                for (int ite_personne = 0; ite_personne < personnes_problematiques.size(); ite_personne++)
                {
                    int personne = personnes_problematiques.front();
                    personnes_problematiques.pop_front();
                    for (int jour = 0; jour < nombre_jour; jour++) // On applique l'opérateur sur tous les jours
                    {

                        if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                            || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX) // On vérifie qu'il nous reste un temps raisonnable
                        {
                            return (p_candidat == &uneSolution) ? nullptr : p_candidat; // On renvoie la meilleure solution trouvée si ce n'est pas la solution de départ
                        }

                        Operation_Operateur_Swap(&uneSolution, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte, // On applique l'opérateur lors de l'opération paramétrée
                            personne, jour,
                            Operateur_Swap_Personne, nombre_personne, nombre_jour, deplacement);
                    }
                }
            }
        }

        for (int deplacement = 1; deplacement < nombre_personne; deplacement += it) // On fait varier le critère d'exploration du voisinage
        {
            for (int personne = 0; personne < nombre_personne; personne++) // On applique l'opérateur sur toutes les personnes
            {
                for (int jour = 0; jour < nombre_jour; jour++) // On applique l'opérateur sur tous les jours
                {

                    if (Outils::get_Secondes_Ecoulees(chrono_start) >= DUREE_MAX
                        || Outils::get_Secondes_Ecoulees(chrono_voisinage_start) >= DUREE_VOISINAGE_MAX) // On vérifie qu'il nous reste un temps raisonnable
                    {
                        return (p_candidat == &uneSolution) ? nullptr : p_candidat; // On renvoie la meilleure solution trouvée si ce n'est pas la solution de départ
                    }

                    Operation_Operateur_Swap(&uneSolution, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte, // On applique l'opérateur lors de l'opération paramétrée
                        personne, jour,
                        Operateur_Swap_Personne, nombre_personne, nombre_jour, deplacement);
                }
            }
        }
        break;
        


    }

    return (p_candidat == &uneSolution) ? nullptr : p_candidat; // On renvoie la meilleure solution trouvée si ce n'est pas la solution de départ
}



// ################### OPERATIONS MODIFICATION DE SHIFT ###################


void MetaHeuristique::Operation_Operateur_Modification_Shift(Solution* solutionRealisable, Solution** p_candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
    int personne, int jour, int nombre_shift,
    void OperateurModificationShift(Solution*, int, int, int))
{
    if (DEBUG) if ((personne + jour) % ((solutionVoisine->v_v_IdShift_Par_Personne_et_Jour.size() + solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[0].size()) / 10) == 0)
        cout << " . ";

    // On crée une solution voisine
    OperateurModificationShift(solutionVoisine, nombre_shift, personne, jour);
    Heuristique::Init_Valeur_FO_Indicative(solutionVoisine, instance, coeff_Valeur_FO_Contrainte);

    // Si elle est meilleure, on la désigne comme notre nouveau candidat
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
    //Initialisation
    int& shift_personne_jour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];

    // Modification des shifts
    shift_personne_jour = (shift_personne_jour + 1) % nombre_shift;
}

void MetaHeuristique::Operateur_Modification_Shift_Aleatoire(Solution* uneSolution, int nombre_shift, int personne, int jour)
{
    //Initialisation
    int& shift_personne_jour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
    int proba_alea = ((double)(rand() % 100)) / 100;
    
    // Modification des shifts, y compris avec des jours de repos
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
    void OperateurSwap(Solution*, int, int, int, int, int), int moduloX, int moduloY, int deplacement)
{
    if (DEBUG) if ((personne + jour) % ((solutionVoisine->v_v_IdShift_Par_Personne_et_Jour.size() + solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[0].size()) / 10) == 0
            && deplacement % (solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[0].size() / 5) == 0)
        cout << " . ";

    // On crée une solution voisine
    OperateurSwap(solutionVoisine, moduloX, moduloY, personne, jour, deplacement);
    Heuristique::Init_Valeur_FO_Indicative(solutionVoisine, instance, coeff_Valeur_FO_Contrainte);

    // Si elle est meilleure, on la désigne comme notre nouveau candidat
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
    OperateurSwap(solutionVoisine, moduloX, moduloY, personne, jour, deplacement); // On revient dans le voisinage
}

// Opérateurs

void MetaHeuristique::Operateur_Swap_Jour(Solution* uneSolution, int nombre_personne, int nombre_jour, int personne, int jour, int deplacement)
{
    // Initialisation
    int* p_to_swap_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
    int* p_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][(jour + deplacement) % nombre_jour];
    int temp = *p_to_swap_shift_personne_jour;

    // Swap
    *p_to_swap_shift_personne_jour = *p_shift_personne_jour;
    *p_shift_personne_jour = temp;
}

void MetaHeuristique::Operateur_Swap_Personne(Solution* uneSolution, int nombre_personne, int nombre_jour, int personne, int jour, int deplacement)
{
    // Initialisation
    int* p_to_swap_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
    int* p_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[(personne + deplacement) % nombre_personne][jour];
    int temp = *p_to_swap_shift_personne_jour;

    // Swap
    *p_to_swap_shift_personne_jour = *p_shift_personne_jour;
    *p_shift_personne_jour = temp;
}

void MetaHeuristique::Operateur_Swap_Personne_Jour(Solution* uneSolution, int nombre_personne, int nombre_jour, int personne, int jour, int deplacement)
{
    // Initialisation
    int* p_to_swap_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
    int* p_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[(personne + deplacement) % nombre_personne][(jour + deplacement) % nombre_jour];
    int temp = *p_to_swap_shift_personne_jour;

    // Swap
    *p_to_swap_shift_personne_jour = *p_shift_personne_jour;
    *p_shift_personne_jour = temp;
}
