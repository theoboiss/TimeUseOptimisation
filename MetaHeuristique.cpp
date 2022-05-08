#include "MetaHeuristique.hpp"

using namespace std;

#define DEBUG true

// ################### ALGORITHMES GENERAUX ###################


Solution* MetaHeuristique::MeilleureSolution(Solution solutionRealisable, Instance* instance, float coeff_Valeur_FO_Contrainte)
{
    int kmax = 3;
    int k = 1;
    int iterateur = 1;
    Solution* meilleureSolution = &solutionRealisable;

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

    int nombre_personne, nombre_jour, nombre_shift, nbIterMax, it;
    nombre_personne = instance->get_Nombre_Personne();
    nombre_jour = instance->get_Nombre_Jour();
    //it = max(1, nombre_personne / 10);
    it = 1;
    //nbIterMax = nombre_personne + nombre_jour;

    list<int> personnes_problematiques = Outils::Personne_Contraintes_Non_Respectes(p_candidat, instance);

    switch (k)
    {
    case 1:
        /*
        * Opérateur de modification des shifts
        * Domaine de définition de proba : ]0..1]
        */
        nombre_shift = instance->get_Nombre_Shift();
        if (personnes_problematiques.size())
        {
            for (int ite_personne = 0; ite_personne < personnes_problematiques.size(); ite_personne++)
            {
                int personne = personnes_problematiques.front();
                personnes_problematiques.pop_front();
                for (int jour = 0; jour < nombre_jour; jour++)
                {
                    OperationOperateurModificationShift(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                        personne, jour, nombre_shift);
                }
            }
        }
        for (int personne = 0; personne < nombre_personne; personne++)
        {
            for (int jour = 0; jour < nombre_jour; jour++)
            {
                OperationOperateurModificationShift(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                    personne, jour, nombre_shift);
            }
        }
        break;

    case 2:
        /*
        * Opérateur de swap des jours en codage lineaire
        * Domaine de définition de a : {0..nombre_jour}
        * Domaine de définition de b : {0..nombre_jour}
        */
        if (personnes_problematiques.size())
        {
            //for (int a = 1; a < nombre_jour; a += it)
            //{
            for (int b = 1; b < nombre_jour; b += it)
            {
                for (int ite_personne = 0; ite_personne < personnes_problematiques.size(); ite_personne++)
                {
                    int personne = personnes_problematiques.front();
                    personnes_problematiques.pop_front();
                    for (int jour = 0; jour < nombre_jour; jour++)
                    {
                        OperationOperateurSwapCodageLineaire(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                            personne, jour,
                            OperateurSwapJourCodageLineaire, nombre_jour, 1, b);
                    }
                }
            }
            //}
        }
        //for (int a = 1; a < nombre_jour; a += it)
        //{
        for (int b = 1; b < nombre_jour; b += it)
        {
            for (int personne = 0; personne < nombre_personne; personne++)
            {
                for (int jour = 0; jour < nombre_jour; jour++)
                {
                    OperationOperateurSwapCodageLineaire(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                        personne, jour,
                        OperateurSwapJourCodageLineaire, nombre_jour, 1, b);
                }
            }
        }
        //}
        break;

    case 3:
        /*
        * Opérateur de swap des personnes en codage lineaire
        * Domaine de définition de a : {0..nombre_personne}
        * Domaine de définition de b : {0..nombre_personne}
        */
        if (personnes_problematiques.size())
        {
            //for (int a = 1; a < nombre_personne; a += it)
            //{
            for (int b = 1; b < nombre_personne; b += it)
            {
                for (int ite_personne = 0; ite_personne < personnes_problematiques.size(); ite_personne++)
                {
                    int personne = personnes_problematiques.front();
                    personnes_problematiques.pop_front();
                    for (int jour = 0; jour < nombre_jour; jour++)
                    {
                        OperationOperateurSwapCodageLineaire(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                            personne, jour,
                            OperateurSwapPersonneCodageLineaire, nombre_personne, 1, b);
                    }
                }
            }
            //}
        }
        //for (int a = 1; a < nombre_personne; a += it)
        //{
        for (int b = 1; b < nombre_personne; b += it)
        {
            for (int personne = 0; personne < nombre_personne; personne++)
            {
                for (int jour = 0; jour < nombre_jour; jour++)
                {
                    OperationOperateurSwapCodageLineaire(&solutionRealisable, &p_candidat, &solutionVoisine, instance, coeff_Valeur_FO_Contrainte,
                        personne, jour,
                        OperateurSwapPersonneCodageLineaire, nombre_personne, 1, b);
                }
            }
        }
        //}
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



// ################### OPERATION MODIFICATION DE SHIFT ###################


void MetaHeuristique::OperationOperateurModificationShift(Solution* solutionRealisable, Solution** p_candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
    int personne, int jour, int nombre_shift)
{
    if (solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[personne][jour] != -1) {
        for (int incrShift = 1; incrShift < nombre_shift; incrShift++)
        {
            if ((personne+jour) % ( (solutionVoisine->v_v_IdShift_Par_Personne_et_Jour.size()+solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[0].size())/10 ) == 0)
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

// Opérateur

void MetaHeuristique::OperateurModificationShift(Solution* uneSolution, int nombre_shift, int personne, int jour, bool inverse)
{
    int& shift_personne_jour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];

    // Modifie les shifts
    shift_personne_jour = (shift_personne_jour + 1 - 2*inverse) % nombre_shift;
}



// ################### OPERATIONS SWAP ###################


void MetaHeuristique::OperationOperateurSwapCodageLineaire(Solution* solutionRealisable, Solution** p_candidat, Solution* solutionVoisine, Instance* instance, float coeff_Valeur_FO_Contrainte,
    int personne, int jour,
    int* OperateurSwapCodageLineaire(Solution*, int, int, int, int, int), int modulo, int a, int b)
{
    if (solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[personne][jour] != -1)
    {
        if ((personne + jour) % ((solutionVoisine->v_v_IdShift_Par_Personne_et_Jour.size() + solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[0].size()) / 10) == 0
             && a % (solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[0].size() / 5) == 0
             && b % (solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[0].size() / 5) == 0)
            cout << " . ";
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

// Opérateurs

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
