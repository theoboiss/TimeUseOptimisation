#include "MetaHeuristique.hpp"


Solution* MetaHeuristique::MeilleureSolution(Solution solutionRealisable, Instance* instance)
{
    int kmax = 3;
    int k = 1;
    int iterateur = 1;
    Solution* meilleureSolution = &solutionRealisable;

    cout << endl << "Debug" << endl;
    while (k <= kmax)
    {
        cout << iterateur;
        cout << " " << k;
        Solution* meilleureSolutionVoisinage = RechercheVoisinageVariable(*meilleureSolution, instance, k);
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


Solution* MetaHeuristique::RechercheVoisinageVariable(Solution solutionRealisable, Instance* instance, int k)
{
    Solution* candidat = &solutionRealisable;

    int nombre_personne, nombre_jour, nombre_shift;
    switch (k)
    {
    case 1:
        /*
        * Opérateur de swap des jours en codage lineaire
        * Domaine de définition de a : {0..nombre_jour}
        * Domaine de définition de b : {0..nombre_jour}
        */
        nombre_jour = instance->get_Nombre_Jour();
        for (int a = 1; a < nombre_jour; a++)
        {
            for (int b = 0; b < nombre_jour; b++)
            {
                cout << " .";
                Solution* solutionVoisine = OperateurSwapJourCodageLineaire(&solutionRealisable, a, b);
                Heuristique::InitValeurFonctionObjectif(solutionVoisine, instance);

                if (solutionVoisine->i_valeur_fonction_objectif < candidat->i_valeur_fonction_objectif)
                {
                    candidat = solutionVoisine;
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
        nombre_personne = instance->get_Nombre_Personne();
        for (int a = 1; a < nombre_personne; a++)
        {
            for (int b = 0; b < nombre_personne; b++)
            {
                cout << " .";
                Solution* solutionVoisine = OperateurSwapPersonneCodageLineaire(&solutionRealisable, a, b);
                Heuristique::InitValeurFonctionObjectif(solutionVoisine, instance);

                if (solutionVoisine->i_valeur_fonction_objectif < candidat->i_valeur_fonction_objectif)
                {
                    candidat = solutionVoisine;
                }
            }
        }
        break;

    case 3:
        /*
        * Opérateur de modification des shifts en codage lineaire
        * Domaine de définition de a : {0..personne}
        * Domaine de définition de b : {0..personne}
        */
        nombre_shift = instance->get_Nombre_Shift();
        nombre_personne = instance->get_Nombre_Personne();
        nombre_jour = instance->get_Nombre_Jour();
        for (int a = 0; a < nombre_shift; a++)
        {
            for (int b = 0; b < nombre_shift; b++)
            {
                cout << " .";
                Solution* solutionVoisine = OperateurModificationShiftCodageLineaire(&solutionRealisable, a, b, 0, 1, 0, 1, instance->get_Nombre_Shift());
                Heuristique::InitValeurFonctionObjectif(solutionVoisine, instance);

                if (solutionVoisine->i_valeur_fonction_objectif < candidat->i_valeur_fonction_objectif)
                {
                    candidat = solutionVoisine;
                }
            }
        }
        break;
    }

    if (candidat == &solutionRealisable)
    {
        return nullptr;
    }
    else
    {
        return candidat;
    }
}


int MetaHeuristique::CodageLineaire(int a, int x, int b, int modulo)
{
    return (a*x + b) % modulo; // BUG : Si x == -1, return peut être < -1
}


Solution* MetaHeuristique::OperateurSwapJourCodageLineaire(Solution* uneSolution, int a, int b)
{
    if (uneSolution->v_v_IdShift_Par_Personne_et_Jour.size() == 0) return nullptr;
    
    int nbPersonne = uneSolution->v_v_IdShift_Par_Personne_et_Jour.size();
    int nbJour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[0].size();

    Solution* unVoisin = new Solution();
    unVoisin->v_v_IdShift_Par_Personne_et_Jour = vector<vector<int>>(nbPersonne);

    for (int personne = 0; personne < nbPersonne; personne++)
    {
        unVoisin->v_v_IdShift_Par_Personne_et_Jour[personne] = vector<int>(nbJour);
        for (int jour = 0; jour < nbJour; jour++)
        {
            // Swap entre les jours
            unVoisin->v_v_IdShift_Par_Personne_et_Jour[personne][CodageLineaire(a, jour, b, nbJour)] = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
        }
    }

    return unVoisin;
}


Solution* MetaHeuristique::OperateurSwapPersonneCodageLineaire(Solution* uneSolution, int a, int b)
{
    if (uneSolution->v_v_IdShift_Par_Personne_et_Jour.size() == 0) return nullptr;

    int nbPersonne = uneSolution->v_v_IdShift_Par_Personne_et_Jour.size();
    int nbJour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[0].size();

    Solution* unVoisin = new Solution();
    unVoisin->v_v_IdShift_Par_Personne_et_Jour = vector<vector<int>>(nbPersonne);
    for (int personne = 0; personne < nbPersonne; personne++)
    {
        unVoisin->v_v_IdShift_Par_Personne_et_Jour[personne] = vector<int>(nbJour);
    }

    for (int personne = 0; personne < nbPersonne; personne++)
    {
        unVoisin->v_v_IdShift_Par_Personne_et_Jour[personne] = vector<int>(nbJour);
        for (int jour = 0; jour < nbJour; jour++)
        {
            // Swap entre les personnes
            unVoisin->v_v_IdShift_Par_Personne_et_Jour[CodageLineaire(a, personne, b, nbPersonne)][jour] = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
        }
    }

    return unVoisin;
}


Solution* MetaHeuristique::OperateurModificationShiftCodageLineaire(Solution* uneSolution, int a, int b, int c, int d, int e, int f, int nombreShift)
{
    if (uneSolution->v_v_IdShift_Par_Personne_et_Jour.size() == 0) return nullptr;

    int nbPersonne = uneSolution->v_v_IdShift_Par_Personne_et_Jour.size();
    int nbJour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[0].size();

    Solution* unVoisin = new Solution();
    unVoisin->v_v_IdShift_Par_Personne_et_Jour = vector<vector<int>>(nbPersonne);

    for (int personne = c; personne < nbPersonne; personne = personne + d)
    {
        unVoisin->v_v_IdShift_Par_Personne_et_Jour[personne] = vector<int>(nbJour);
        for (int jour = e; jour < nbJour; jour = jour + f)
        {
            // Modifie les shifts
            unVoisin->v_v_IdShift_Par_Personne_et_Jour[personne][jour] = CodageLineaire(a, uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour], b, nombreShift);
        }
    }

    return unVoisin;
}


Solution* MetaHeuristique::OperateurModificationShiftAleatoire(Solution* uneSolution)
{
    if (uneSolution->v_v_IdShift_Par_Personne_et_Jour.size() == 0) return nullptr;


    return nullptr;
}
