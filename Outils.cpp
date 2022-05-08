#include "Outils.hpp"


int Outils::CodageLineaire(double a, int x, int b, int modulo)
{
    if (x < 0)
    {
        int reste_modulo_dist_a_zero = - x % modulo;
        x += reste_modulo_dist_a_zero * modulo;
    }
    int y = a * x + b;
    return y % modulo;
}


bool Outils::Comparer(pair<int, int>& a, pair<int, int>& b)
{
    return a.second < b.second;
}


int Outils::i_Calcul_Valeur_Fonction_Objectif(Solution* uneSolution, Instance* instance)
{
    int i_fc_obj = 0;
    vector<vector<int>> v_i_nb_personne_par_Shift_et_jour(instance->get_Nombre_Shift(), vector<int>(instance->get_Nombre_Jour(), 0));

    for (int i = 0; i < uneSolution->v_v_IdShift_Par_Personne_et_Jour.size(); i++) {
        for (int j = 0; j < uneSolution->v_v_IdShift_Par_Personne_et_Jour[i].size(); j++)
        {
            if (uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j] != -1)
            {
                v_i_nb_personne_par_Shift_et_jour[uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j]][j] = v_i_nb_personne_par_Shift_et_jour[uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j]][j] + 1;
            }
        }
    }

    for (int i = 0; i < uneSolution->v_v_IdShift_Par_Personne_et_Jour.size(); i++)
    {
        for (int j = 0; j < uneSolution->v_v_IdShift_Par_Personne_et_Jour[i].size(); j++)
        {
            if (uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j] != -1)
            {
                i_fc_obj = i_fc_obj + instance->get_Poids_Refus_Pers_Jour_Shift(i, j, uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j]);
            }
            for (int k = 0; k < instance->get_Nombre_Shift(); k++)
            {
                if (uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j] != k)
                    i_fc_obj = i_fc_obj + instance->get_Poids_Affectation_Pers_Jour_Shift(i, j, k);
            }
        }
    }
    for (int k = 0; k < instance->get_Nombre_Shift(); k++)
    {
        for (int j = 0; j < instance->get_Nombre_Jour(); j++)
        {
            if (v_i_nb_personne_par_Shift_et_jour[k][j] < instance->get_Nbre_Personne_Requis_Jour_Shift(j, k))
                i_fc_obj = i_fc_obj + instance->get_Poids_Personne_En_Moins_Jour_Shift(j, k) * (instance->get_Nbre_Personne_Requis_Jour_Shift(j, k) - v_i_nb_personne_par_Shift_et_jour[k][j]);
            if (v_i_nb_personne_par_Shift_et_jour[k][j] > instance->get_Nbre_Personne_Requis_Jour_Shift(j, k))
                i_fc_obj = i_fc_obj + instance->get_Poids_Personne_En_Plus_Jour_Shift(j, k) * (v_i_nb_personne_par_Shift_et_jour[k][j] - instance->get_Nbre_Personne_Requis_Jour_Shift(j, k));
        }
    }

    return i_fc_obj;
}


int Outils::i_Calcul_Penalisation_Fonction_Objectif(Solution* uneSolution, Instance* instance, float coeff_Valeur_FO_Contrainte)
{
    int poids = 0;
    int nombre_contraintes_non_respectees = 0;

    bool b_test_si_premier_jour_off = false;
    int i, j, jj, k, i_duree_travail, i_shift_consecutif, i_nb_WE;
    vector<vector<int>> v_i_nb_personne_par_Shift_et_jour(instance->get_Nombre_Shift(), vector<int>(instance->get_Nombre_Jour(), 0));

    //Vérification de la taille de v_v_IdShift_Par_Personne_et_Jour
    if (uneSolution->v_v_IdShift_Par_Personne_et_Jour.size() != instance->get_Nombre_Personne())
    {
        nombre_contraintes_non_respectees++;
    }
    for (i = 0; i < uneSolution->v_v_IdShift_Par_Personne_et_Jour.size(); i++)
    {
        vector<int> v_i_Nb_shift(instance->get_Nombre_Shift(), 0);
        i_duree_travail = 0;
        i_shift_consecutif = 0;
        i_nb_WE = 0;
        if (uneSolution->v_v_IdShift_Par_Personne_et_Jour[i].size() != instance->get_Nombre_Jour())
        {
            nombre_contraintes_non_respectees++;
        }
        //Vérification que uneSolution->v_v_IdShift_Par_Personne_et_Jour contient que des Ids Shifts valident ou -1
        for (j = 0; j < uneSolution->v_v_IdShift_Par_Personne_et_Jour[i].size(); j++)
        {
            if ((uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j] != -1) && ((uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j] < 0) || (uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j] > instance->get_Nombre_Shift())))
            {
                nombre_contraintes_non_respectees++;
            }
            else
            {
                if (uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j] != -1)
                {
                    v_i_Nb_shift[uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j]] = v_i_Nb_shift[uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j]] + 1;
                    v_i_nb_personne_par_Shift_et_jour[uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j]][j] = v_i_nb_personne_par_Shift_et_jour[uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j]][j] + 1;
                    i_duree_travail = i_duree_travail + instance->get_Shift_Duree(uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j]);
                    i_shift_consecutif++;
                    if ((j % 7) == 5)
                        i_nb_WE++;
                    if (((j % 7) == 6) && (uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j - 1] == -1))
                        i_nb_WE++;
                    //Vérification du nombre de shifts consécutifs maximum assignable à chaque personne
                    if (i_shift_consecutif > instance->get_Personne_Nbre_Shift_Consecutif_Max(i))
                    {
                        nombre_contraintes_non_respectees++;
                    }

                    //Vérification des jours de congés de chaque personne
                    if (!instance->is_Available_Personne_Jour(i, j))
                    {
                        {
                            nombre_contraintes_non_respectees++;
                        }
                    }
                }
                else
                {
                    //Vérification du nombre de shifts consécutifs minimum assignable à chaque personne
                    if ((i_shift_consecutif < instance->get_Personne_Nbre_Shift_Consecutif_Min(i)) && (i_shift_consecutif != 0) && ((j - instance->get_Personne_Nbre_Shift_Consecutif_Min(i)) > 0))
                    {
                        nombre_contraintes_non_respectees++;
                    }
                    i_shift_consecutif = 0;
                    b_test_si_premier_jour_off = false;
                    if (j != 0)
                    {
                        if (uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j - 1] != -1)
                            b_test_si_premier_jour_off = true;
                    }
                    if (b_test_si_premier_jour_off)
                    {
                        b_test_si_premier_jour_off = false;
                        for (jj = j; jj < (j + instance->get_Personne_Jour_OFF_Consecutif_Min(i)); jj++)
                        {
                            if (jj < uneSolution->v_v_IdShift_Par_Personne_et_Jour[i].size())
                                if (uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][jj] != -1)
                                    b_test_si_premier_jour_off = true;
                        }
                        if (b_test_si_premier_jour_off)
                        {
                            nombre_contraintes_non_respectees++;
                        }
                    }
                }
                //Vérification des successions des Shifts
                if (j != (uneSolution->v_v_IdShift_Par_Personne_et_Jour[i].size() - 1))
                {
                    if ((uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j] != -1) && (uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j + 1] != -1))
                    {
                        if (!instance->is_possible_Shift_Succede(uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j], uneSolution->v_v_IdShift_Par_Personne_et_Jour[i][j + 1]))
                        {
                            nombre_contraintes_non_respectees++;
                        }
                    }
                }
            }
        }
        //Vérification de la durée totale maximale et minimale de chaque personne
        if ((i_duree_travail > instance->get_Personne_Duree_total_Max(i)) || (i_duree_travail < instance->get_Personne_Duree_total_Min(i)))
        {
            nombre_contraintes_non_respectees++;
            int valeur = instance->get_Personne_Duree_total_Min(i) - i_duree_travail;
            if (valeur > 0)
            {
                poids += valeur;
            }
        }
        //Vérification du nombre de WE (samedi ou/et dimanche) de travail maximal pour chaque personne
        if (i_nb_WE > instance->get_Personne_Nbre_WE_Max(i))
        {
            nombre_contraintes_non_respectees++;
        }
        //Vérification du nombre maximal de shifts de chaque personne
        for (j = 0; j < instance->get_Nombre_Shift(); j++)
        {
            if (v_i_Nb_shift[j] > instance->get_Personne_Shift_Nbre_Max(i, j))
            {
                nombre_contraintes_non_respectees++;
            }
        }
    }

    //int poids = uneSolution->i_valeur_fonction_objectif * coeff_Valeur_FO_Contrainte;
    //int poids = log10(10+uneSolution->i_valeur_fonction_objectif * coeff_Valeur_FO_Contrainte);

    int A = uneSolution->i_valeur_fonction_objectif + (nombre_contraintes_non_respectees * coeff_Valeur_FO_Contrainte);
    int B = nombre_contraintes_non_respectees * poids;
    //int C = A + B;

    int petiteValFO = uneSolution->i_valeur_fonction_objectif * coeff_Valeur_FO_Contrainte;
    int C = nombre_contraintes_non_respectees * (petiteValFO + poids);
    return C;
}


Solution* Outils::CopieSolution(Solution* uneSolution)
{
    if (uneSolution->v_v_IdShift_Par_Personne_et_Jour.size() == 0) return nullptr;

    int nbPersonne = uneSolution->v_v_IdShift_Par_Personne_et_Jour.size();
    int nbJour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[0].size();

    Solution* Copie = new Solution();
    Copie->i_valeur_fonction_objectif = uneSolution->i_valeur_fonction_objectif;

    Copie->v_v_IdShift_Par_Personne_et_Jour = vector<vector<int>>(nbPersonne);
    for (int personne = 0; personne < nbPersonne; personne++)
    {
        Copie->v_v_IdShift_Par_Personne_et_Jour[personne] = vector<int>(nbJour);
        for (int jour = 0; jour < nbJour; jour++)
        {
            Copie->v_v_IdShift_Par_Personne_et_Jour[personne][jour] = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
        }
    }
    return Copie;
}


list<int> Outils::Personne_Contraintes_Non_Respectes(Solution* uneSolution, Instance* instance)
{
    list<int> personnes_problematiques;

    for (int personne = 0; personne < uneSolution->v_v_IdShift_Par_Personne_et_Jour.size(); personne++)
    {
        int i_duree_travail = 0;
        for (int jour = 0; jour < uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne].size(); jour++)
        {
            if (uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour] != -1)
            {
                i_duree_travail = i_duree_travail + instance->get_Shift_Duree(uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour]);
            }
        }

        //Vérification de la durée totale maximale et minimale de chaque personne
        if ((i_duree_travail > instance->get_Personne_Duree_total_Max(personne)) || (i_duree_travail < instance->get_Personne_Duree_total_Min(personne)))
        {
            personnes_problematiques.push_back(personne);
        }
    }
    return personnes_problematiques;
}
