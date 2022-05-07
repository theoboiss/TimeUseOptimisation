#include "MetaHeuristique.hpp"

using namespace std;


// ################### ALGORITHMES GENERAUX ###################


Solution* MetaHeuristique::MeilleureSolution(Solution solutionRealisable, Instance* instance, float coeff_Valeur_FO_Contrainte)
{
    int kmax = 3;
    int k = 1;
    int iterateur = 1;
    Solution* meilleureSolution = &solutionRealisable;

    //cout << endl << "Debug" << endl;
    while (k <= kmax)
    {
        cout << iterateur;
        cout << " " << k << endl;
        Solution* meilleureSolutionVoisinage = RechercheVoisinageVariable(*meilleureSolution, instance, k, coeff_Valeur_FO_Contrainte);
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
            //cout << "!";
            k = 1;
        }
        else
        {
            delete meilleureSolutionVoisinage;
            k = k + 1;
        }
        cout << endl << endl;
        iterateur++;
    }

    if (meilleureSolution != &solutionRealisable)
    {
        return meilleureSolution;
    }
    return nullptr;
}



Solution* MetaHeuristique::RechercheVoisinageVariable(Solution solutionRealisable, Instance* instance, int k, float coeff_Valeur_FO_Contrainte)
{
    Solution* p_candidat = &solutionRealisable;
    Solution solutionVoisine = *p_candidat;

    int nombre_personne, nombre_jour, nombre_shift, nbIterMax;
    nombre_shift = instance->get_Nombre_Shift();
    nombre_personne = instance->get_Nombre_Personne();
    nombre_jour = instance->get_Nombre_Jour();
    nbIterMax = nombre_personne + nombre_jour;

    list<int> personnes_problematiques = Outils::Personne_Contraintes_Non_Respectes(p_candidat, instance);

    switch (k)
    {
    case 1:
        /*
        * Opérateur de modification des shifts
        * Domaine de définition de proba : ]0..1]
        */
        if (personnes_problematiques.size() == 0)
        {
            for (int personne = 0; personne < nombre_personne; personne++)
            {
                for (int jour = 0; jour < nombre_jour; jour++)
                {
                    OperationOperateurModificationShift(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                        personne, jour, nombre_personne, nombre_jour, nombre_shift);
                }
            }
        }
        else
        {
            for (int ite_personne = 0; ite_personne < personnes_problematiques.size() ; ite_personne++)
            {
                int personne = personnes_problematiques.front();
                personnes_problematiques.pop_front();
                for (int jour = 0; jour < nombre_jour; jour++)
                {
                    OperationOperateurModificationShift(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                        personne, jour, nombre_personne, nombre_jour, nombre_shift);
                }
            }
        }
        break;

    case 2:
        /*
        * Opérateur de swap des personnes en codage lineaire
        * Domaine de définition de a : {0..nombre_personne}
        * Domaine de définition de b : {0..nombre_personne}
        */
        for (int a = 1; a < nombre_personne; a = a + nombre_personne / 10)
        {
            for (int b = 0; b < nombre_personne; b = b + nombre_personne / 10)
            {
                for (int personne = 0; personne < nombre_personne; personne++)
                {
                    for (int jour = 0; jour < nombre_jour; jour++)
                    {
                        OperationOperateurSwapCodageLineaire(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                            personne, jour, nombre_personne,
                            OperateurSwapPersonneCodageLineaire, a, b);
                    }
                }
            }
        }
        break;

    case 3:
        /*
        * Opérateur de swap des jours en codage lineaire
        * Domaine de définition de a : {0..nombre_jour}
        * Domaine de définition de b : {0..nombre_jour}
        */
        for (int a = 1; a < nombre_jour; a = a + nombre_jour / 20)
        {
            for (int b = 0; b < nombre_jour; b = b + nombre_jour / 20)
            {
                for (int personne = 0; personne < nombre_personne; personne++)
                {
                    for (int jour = 0; jour < nombre_jour; jour++)
                    {
                        OperationOperateurSwapCodageLineaire(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                            personne, jour, nombre_jour,
                            OperateurSwapJourCodageLineaire, a, b);
                    }
                }
            }
        }
        break;
    }

    if (p_candidat == &solutionRealisable)
    {
        return nullptr;
    }
    else
    {
        return p_candidat;
    }
}



// ################### OPERATIONS ###################


// Operation Swap avec Codage Lineaire

void MetaHeuristique::OperationOperateurSwapCodageLineaire(Solution* solutionRealisable, Solution** p_candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
    int personne, int jour, int modulo,
    int* OperateurSwapCodageLineaire(Solution*, int, int, int, int, int), int a, int b)
{
    if (solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[personne][jour] != -1)
    {
        //cout << " . ";
        OperateurSwapCodageLineaire(solutionVoisine, modulo, personne, jour, a, b);
        Heuristique::InitValeurFonctionObjectif(solutionVoisine, instance, coeff_Valeur_FO_Contrainte);

        if (solutionVoisine->i_valeur_fonction_objectif >= 0 && solutionVoisine->i_valeur_fonction_objectif < (*p_candidat)->i_valeur_fonction_objectif)
        {
            if (*p_candidat != solutionRealisable)
            {
                delete *p_candidat;
            }
            *p_candidat = Outils::CopieSolution(solutionVoisine);

            cout << " ! ";
            solutionVoisine->Verification_Solution(instance);
            cout << "Valeur de la fonction objective sans pénalités : " << Outils::i_Calcul_Valeur_Fonction_Objectif(solutionVoisine, instance) << endl;
            cout << "Valeur de la fonction objectif : " << solutionVoisine->i_valeur_fonction_objectif << endl << endl;
        }
        OperateurSwapCodageLineaire(solutionVoisine, modulo, personne, jour, a, b);
    }
}


int* MetaHeuristique::OperateurSwapJourCodageLineaire(Solution* uneSolution, int nombre_jour, int personne, int jour, int a, int b)
{
    int* p_to_swap_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
    int* p_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][Outils::CodageLineaire(a, jour, b, nombre_jour)];
    int temp = *p_to_swap_shift_personne_jour;

    // Modifie les shifts
    *p_to_swap_shift_personne_jour = *p_shift_personne_jour;
    *p_shift_personne_jour = temp;

    return p_shift_personne_jour;
}


int* MetaHeuristique::OperateurSwapPersonneCodageLineaire(Solution* uneSolution, int nombre_personne, int personne, int jour, int a, int b)
{
    int* p_to_swap_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
    int* p_shift_personne_jour = &uneSolution->v_v_IdShift_Par_Personne_et_Jour[Outils::CodageLineaire(a, personne, b, nombre_personne)][jour];
    int temp = *p_to_swap_shift_personne_jour;

    // Modifie les shifts
    *p_to_swap_shift_personne_jour = *p_shift_personne_jour;
    *p_shift_personne_jour = temp;

    return p_shift_personne_jour;
}


// Operation Modification de Shift avec Codage Lineaire

void MetaHeuristique::OperationOperateurModificationShift(Solution* solutionRealisable, Solution** p_candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
    int personne, int jour, int nombre_personne, int nombre_jour, int nombre_shift)
{
    if (solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[personne][jour] != -1) {
        for (int incrShift = 1; incrShift < nombre_shift; incrShift++)
        {
            cout << " . ";
            OperateurModificationShift(solutionVoisine, nombre_shift, personne, jour);
            Heuristique::InitValeurFonctionObjectif(solutionVoisine, instance, coeff_Valeur_FO_Contrainte);

            if (solutionVoisine->i_valeur_fonction_objectif >= 0 && solutionVoisine->i_valeur_fonction_objectif < (*p_candidat)->i_valeur_fonction_objectif)
            {
                if (*p_candidat != solutionRealisable)
                {
                    delete *p_candidat;
                }
                *p_candidat = Outils::CopieSolution(solutionVoisine);

                cout << " ! ";
                solutionVoisine->Verification_Solution(instance);
                cout << "Valeur de la fonction objective sans pénalités : " << Outils::i_Calcul_Valeur_Fonction_Objectif(solutionVoisine, instance) << endl;
                cout << "Valeur de la fonction objectif : " << solutionVoisine->i_valeur_fonction_objectif << endl << endl;
            }
            OperateurModificationShift(solutionVoisine, nombre_shift, personne, jour, true);
        }
    }
}


void MetaHeuristique::OperateurModificationShift(Solution* uneSolution, int nombre_shift, int personne, int jour, bool inverse)
{
    int& shift_personne_jour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];

    // Modifie les shifts
    shift_personne_jour = Outils::CodageLineaire(1, shift_personne_jour, (int)inverse*(-2) + 1, nombre_shift);
}
