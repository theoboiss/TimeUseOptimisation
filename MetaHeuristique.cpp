#include "MetaHeuristique.hpp"


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
        cout << " " << k;
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
            cout << "!";
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
    Solution* candidat = &solutionRealisable;

    int nombre_personne, nombre_jour, nombre_shift, nbIterMax;
    switch (k)
    {
    case 1:
        /*
        * Opérateur de modification des shifts aléatoire
        * Domaine de définition de proba : ]0..1]
        */
        nombre_shift = instance->get_Nombre_Shift();
        nombre_personne = instance->get_Nombre_Personne();
        nombre_jour = instance->get_Nombre_Jour();
        nbIterMax = nombre_personne + nombre_jour;
        for (float proba = 0.5; proba > 0; proba = proba - 0.01)
        {
            for (int nbIter = 0; nbIter < nbIterMax; nbIter++)
            {
                cout << " .";
                Solution* solutionVoisine = OperateurModificationShiftAleatoire(&solutionRealisable, instance, instance->get_Nombre_Shift(), proba);
                Heuristique::InitValeurFonctionObjectif(solutionVoisine, instance, coeff_Valeur_FO_Contrainte);

                /*if (proba < 0.02 && nbIter == nbIterMax - 1) {
                    for (int personne = 0; personne < solutionVoisine->v_v_IdShift_Par_Personne_et_Jour.size(); personne++)
                    {
                        for (int jour = 1; jour < solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[personne].size(); jour++) // On parcourt les jours à partir du deuxième
                        {
                            cout << solutionVoisine->v_v_IdShift_Par_Personne_et_Jour[personne][jour] << '\t';
                        }
                        cout << endl;
                    }
                }*/

                if (solutionVoisine->i_valeur_fonction_objectif >= 0 && solutionVoisine->i_valeur_fonction_objectif < candidat->i_valeur_fonction_objectif)
                {
                    candidat = solutionVoisine;
                }
                else
                {
                    delete solutionVoisine;
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
        for (int a = 1; a < nombre_personne; a = a + nombre_personne / 10)
        {
            for (int b = 0; b < nombre_personne; b = b + nombre_personne / 10)
            {
                cout << " .";
                Solution* solutionVoisine = OperateurSwapPersonneCodageLineaire(&solutionRealisable, a, b);
                Heuristique::InitValeurFonctionObjectif(solutionVoisine, instance, coeff_Valeur_FO_Contrainte);

                if (solutionVoisine->i_valeur_fonction_objectif >= 0 && solutionVoisine->i_valeur_fonction_objectif < candidat->i_valeur_fonction_objectif)
                {
                    candidat = solutionVoisine;
                }
                else
                {
                    delete solutionVoisine;
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
        nombre_jour = instance->get_Nombre_Jour();
        for (int a = 1; a < nombre_jour; a = a + nombre_jour / 20)
        {
            for (int b = 0; b < nombre_jour; b = b + nombre_jour / 20)
            {
                cout << " .";
                Solution* solutionVoisine = OperateurSwapJourCodageLineaire(&solutionRealisable, a, b);
                Heuristique::InitValeurFonctionObjectif(solutionVoisine, instance, coeff_Valeur_FO_Contrainte);

                if (solutionVoisine->i_valeur_fonction_objectif >= 0 && solutionVoisine->i_valeur_fonction_objectif < candidat->i_valeur_fonction_objectif)
                {
                    candidat = solutionVoisine;
                }
                else
                {
                    delete solutionVoisine;
                }
            }
        }
        break;

    case 4:
        /*
        * Opérateur de modification des shifts en codage lineaire
        * Domaine de définition de a : {0..nombre_personne}
        * Domaine de définition de b : {0..nombre_personne}
        */
        nombre_shift = instance->get_Nombre_Shift();
        nombre_personne = instance->get_Nombre_Personne();
        nombre_jour = instance->get_Nombre_Jour();
        for (int a = 0; a < nombre_shift; a = a + nombre_shift/8)
        {
            for (int b = 0; b < nombre_shift; b = b + nombre_shift/8)
            {
                cout << " .";
                Solution* solutionVoisine = OperateurModificationShiftCodageLineaire(&solutionRealisable, a, b, 0, 1, 0, 1, instance->get_Nombre_Shift());
                Heuristique::InitValeurFonctionObjectif(solutionVoisine, instance, coeff_Valeur_FO_Contrainte);

                if (solutionVoisine->i_valeur_fonction_objectif >= 0 && solutionVoisine->i_valeur_fonction_objectif < candidat->i_valeur_fonction_objectif)
                {
                    candidat = solutionVoisine;
                }
                else
                {
                    delete solutionVoisine;
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
            int& shift = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
            if (shift != -1)
            {
                shift = CodageLineaire(a, shift, b, nombreShift);
            }
        }
    }

    return unVoisin;
}


Solution* MetaHeuristique::OperateurModificationShiftAleatoire(Solution* uneSolution, Instance* instance, int nombreShift, float proba)
{
    if (uneSolution->v_v_IdShift_Par_Personne_et_Jour.size() == 0) return nullptr;

    int nbPersonne = uneSolution->v_v_IdShift_Par_Personne_et_Jour.size();
    int nbJour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[0].size();

    Solution* unVoisin = new Solution();
    unVoisin->v_v_IdShift_Par_Personne_et_Jour = vector<vector<int>>(nbPersonne);

    for (int personne = 0; personne < nbPersonne; personne++)
    {
        unVoisin->v_v_IdShift_Par_Personne_et_Jour[personne] = vector<int>(nbJour);
        for (int jour = 1; jour < nbJour; jour++)
        {
            if ( ((float) (rand() % 100)) / 100 < proba)
            {
                int shift_jour_k = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
                // Modifie les shifts
                if (shift_jour_k != -1)
                {
                    int k = 0;
                    int shift_jour_kM1 = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour - 1];
                    while( jour+k < nbJour && shift_jour_kM1 != -1 && shift_jour_k != -1
                        && instance->is_possible_Shift_Succede(shift_jour_kM1, shift_jour_k))
                    {
                        int compteur_shift = 0;
                        shift_jour_kM1 = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour + k - 1];
                        shift_jour_k = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour + k];
                        while ( compteur_shift < nombreShift && jour+k < nbJour && shift_jour_kM1 != -1 && shift_jour_k != -1
                            && instance->is_possible_Shift_Succede(shift_jour_kM1, shift_jour_k))
                        {
                            shift_jour_k = (shift_jour_k + 1) % nombreShift;
                            compteur_shift++;
                        }
                        k++;
                    }
                }
                unVoisin->v_v_IdShift_Par_Personne_et_Jour[personne][jour] = shift_jour_k;
            }
        }
    }

    return unVoisin;
}
