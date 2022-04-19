//#define CHEMIN_DOSSIER_DONNEES "/Users/kergosien/Desktop/Cours Optim Discret/Conception emploi du temps/Format Etudiant Public/"
#define CHEMIN_DOSSIER_DONNEES "/Users/boiss/workspaces/VisualStudio2019/DI4_OptiEmploiDuTemps/DI4_OptiEmploiDuTemps/Data/Format Etudiant Public/"
#define NOM_FICHIER_LISTE_FICHIER_DONNEES "data.txt"
#define NOM_FICHIER_LISTE_SORTIE "sortie.txt"

#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <map>
#include "Instance.hpp"
#include "Solution.hpp"

using namespace std;


int Resolution(Instance* instance);

Solution * GenerationSolutionRealisable(Instance * instance);

bool is_Peut_Reprendre_Travaille(Instance* instance, int personne, int jour, int Nbre_WE_Travaille_Personne, int duree_totale_shift_personne, int duree_shift_max);


int main(int argc, const char* argv[])
{
    try
    {
        string s_tmp;
        string s_chemin = CHEMIN_DOSSIER_DONNEES;
        s_chemin.append(NOM_FICHIER_LISTE_FICHIER_DONNEES);

        ifstream fichier(s_chemin.c_str(), std::ios::in); std::ofstream fichier_Sortie_Resume;
        s_chemin = CHEMIN_DOSSIER_DONNEES;
        s_chemin.append(NOM_FICHIER_LISTE_SORTIE);
        ofstream fichier_Sortie(s_chemin.c_str(), std::ios::out | std::ios::app);

        if (fichier)
        {
            if (fichier_Sortie)
            {
                fichier_Sortie << " Fichier données\t Tps de résolution \t Best solution" << endl;
                getline(fichier, s_tmp);
                while (s_tmp != "")
                {
                    Instance* instance = new Instance();
                    chrono::time_point<chrono::system_clock> chrono_start, chrono_end;
                    chrono::duration<double> elapsed;
                    int i_best_solution_score = 0;
                    s_chemin = CHEMIN_DOSSIER_DONNEES;
                    cout << " Résolution de " << s_tmp << endl;
                    s_chemin.append(s_tmp);
                    s_chemin.erase(remove(s_chemin.begin(), s_chemin.end(), '\r'), s_chemin.end());
                    s_chemin.erase(remove(s_chemin.begin(), s_chemin.end(), '\n'), s_chemin.end());

                    instance->chargement_Instance(s_chemin);
                    chrono_start = chrono::system_clock::now();
                    i_best_solution_score = Resolution(instance);
                    cout << " Fin de résolution de " << s_tmp << endl;
                    chrono_end = chrono::system_clock::now();
                    elapsed = chrono_end - chrono_start;
                    fichier_Sortie << s_chemin << "\t" << elapsed.count() << "\t" << i_best_solution_score << endl;
                    s_tmp = "";
                    getline(fichier, s_tmp);
                    delete instance;
                }
                fichier_Sortie.close();
            }
            else
            {
                cout << " Erreur lecture des données : chemin vers la sortie non valide. " << endl;
            }
            fichier.close();
        }
        else
        {
            cout << " Erreur lecture des données : chemin listant l'ensemble des données non valide. " << endl;
        }
    }

    catch (string err)
    {
        cout << "Erreur fatale : " << endl;
        cout << err << endl;
    }
    return 0;
}


int Resolution(Instance* instance)
{
    int i_val_Retour_Fct_obj = 0;
    Solution* uneSolution = new Solution();

    /* EXEMPLE D UNE SOLUTION : */
    /*uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,1,3,-1,-1,-1,-1,-1,1,1,3,3,-1,-1,1,1,3,-1,-1,1,1,1,1,1,-1,-1,1,3 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 3,3,3,-1,-1,0,0,2,2,-1,-1,0,0,2,-1,-1,0,0,3,-1,-1,-1,0,2,3,3,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 3,3,-1,-1,3,3,3,3,3,-1,-1,-1,-1,-1,0,0,0,3,3,-1,-1,0,3,-1,-1,0,0,0 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,-1,-1,4,4,4,4,-1,-1,2,3,-1,-1,-1,1,2,1,1,3,-1,-1,1,3,-1,-1,1,1,2 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,-1,-1,0,0,-1,-1,3,3,3,3,3,-1,-1,3,3,-1,-1,-1,3,3,3,4,4,-1,-1,4,4 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 3,-1,-1,-1,4,4,4,4,4,-1,-1,-1,1,1,1,1,1,-1,-1,-1,-1,3,3,3,3,3,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 0,-1,-1,-1,2,2,2,2,4,-1,-1,-1,2,2,2,-1,-1,3,3,-1,-1,3,3,3,3,3,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,1,1,1,2,-1,-1,1,1,-1,-1,-1,2,1,2,-1,-1,1,1,-1,-1,2,1,2,-1,-1,2,1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,0,1,1,1,-1,-1,-1,0,0,0,4,-1,-1,-1,0,0,-1,-1,1,1,-1,-1,-1,0,1,1,1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 3,3,4,-1,-1,1,1,1,-1,-1,1,1,-1,-1,4,4,4,-1,-1,1,3,3,-1,-1,1,4,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,1,1,-1,-1,-1,-1,-1,1,1,1,1,-1,-1,-1,-1,4,4,4,4,4,-1,-1,1,1,1,1,1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 2,4,4,-1,-1,-1,-1,-1,-1,0,0,0,-1,-1,0,0,0,-1,-1,2,2,2,2,2,-1,-1,1,1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 0,3,3,-1,-1,-1,-1,0,0,3,-1,-1,3,3,3,3,3,-1,-1,0,0,0,0,1,-1,-1,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,-1,3,3,3,3,3,-1,-1,3,3,3,-1,-1,3,3,3,3,-1,-1,-1,-1,-1,-1,3,3,3,3 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 3,3,3,3,3,-1,-1,-1,-1,0,0,2,2,2,-1,-1,0,0,2,-1,-1,2,2,2,-1,-1,0,0 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 4,-1,-1,3,3,-1,-1,-1,2,3,4,-1,-1,-1,0,0,2,-1,-1,1,1,1,1,2,-1,-1,0,0 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 0,0,0,0,0,-1,-1,0,0,0,-1,-1,3,3,3,3,3,-1,-1,-1,-1,-1,0,3,-1,-1,0,0 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 1,2,2,2,-1,-1,-1,0,0,-1,-1,0,1,2,1,1,-1,-1,0,0,0,0,1,-1,-1,-1,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 2,2,2,-1,-1,3,3,3,3,-1,-1,-1,2,1,1,1,1,-1,-1,-1,-1,1,2,3,3,4,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,2,2,-1,-1,2,2,-1,-1,-1,2,2,3,3,3,-1,-1,2,2,-1,-1,3,3,3,3,3,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,-1,-1,3,3,-1,-1,3,3,3,3,3,-1,-1,-1,-1,2,2,2,2,2,-1,-1,-1,0,0,2,2 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 2,-1,-1,3,3,3,3,3,-1,-1,1,1,-1,-1,2,2,2,2,1,-1,-1,2,2,-1,-1,2,3,3 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,-1,0,4,4,4,4,-1,-1,-1,-1,-1,1,1,1,2,4,-1,-1,-1,-1,2,1,2,2,1,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 0,0,1,1,-1,-1,-1,0,0,0,-1,-1,0,0,-1,-1,0,0,0,0,1,-1,-1,-1,0,0,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,2,3,3,-1,-1,-1,1,2,1,1,-1,-1,-1,0,1,-1,-1,0,2,2,3,-1,-1,0,2,3,3 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 3,-1,-1,-1,-1,2,1,2,3,4,-1,-1,-1,-1,0,0,0,0,0,-1,-1,3,4,-1,-1,2,1,1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 1,1,-1,-1,1,1,1,-1,-1,1,1,1,1,3,-1,-1,1,1,1,-1,-1,-1,1,1,1,3,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,0,0,0,0,-1,-1,4,4,-1,-1,2,2,4,-1,-1,2,2,2,2,2,-1,-1,1,2,-1,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 1,4,-1,-1,-1,0,0,0,0,1,-1,-1,-1,-1,3,3,-1,-1,1,4,4,-1,-1,0,0,1,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,-1,2,2,2,-1,-1,-1,0,2,2,2,-1,-1,3,3,-1,-1,0,0,0,0,0,-1,-1,0,0,0 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,1,1,3,-1,-1,-1,1,1,2,3,-1,-1,-1,-1,-1,3,3,3,3,3,-1,-1,2,2,2,2,2 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,-1,0,0,0,-1,-1,-1,0,0,2,3,-1,-1,2,2,-1,-1,2,2,2,2,2,-1,-1,0,0,0 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 0,0,0,0,0,-1,-1,-1,-1,-1,0,0,0,0,0,-1,-1,0,0,0,0,-1,-1,0,0,0,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 0,-1,-1,2,2,3,3,3,-1,-1,2,2,-1,-1,2,2,-1,-1,0,0,0,0,3,-1,-1,-1,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 0,-1,-1,1,1,-1,-1,0,4,-1,-1,0,3,3,-1,-1,3,3,3,3,3,-1,-1,3,3,3,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 2,-1,-1,-1,-1,1,2,1,2,-1,-1,0,3,3,-1,-1,0,1,3,-1,-1,4,4,4,4,-1,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 3,3,-1,-1,1,1,1,2,2,-1,-1,-1,-1,-1,0,0,3,3,-1,-1,-1,-1,-1,-1,0,0,2,1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 2,2,-1,-1,1,1,1,2,1,-1,-1,1,1,1,4,-1,-1,1,1,-1,-1,1,2,-1,-1,-1,-1,-1 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { -1,-1,0,0,0,0,0,-1,-1,-1,0,0,-1,-1,0,0,0,0,4,-1,-1,-1,0,0,-1,-1,0,0 });
    uneSolution->v_v_IdShift_Par_Personne_et_Jour.push_back(vector<int>() = { 2,2,2,-1,-1,0,1,-1,-1,-1,2,3,3,3,-1,-1,2,2,1,-1,-1,0,0,0,0,2,-1,-1 });

    uneSolution->i_valeur_fonction_objectif = 4631;
     * * * * * * * * * * * */

    uneSolution = GenerationSolutionRealisable(instance);


    std::cout << uneSolution->Verification_Solution(instance);

    i_val_Retour_Fct_obj = uneSolution->i_valeur_fonction_objectif;
    delete uneSolution;
    return i_val_Retour_Fct_obj;
}


Solution * GenerationSolutionRealisable(Instance * instance) {
    // Initialisation

    Solution* S = new Solution();

    vector<int> v_Nbre_Shift_Consecutif(instance->get_Nombre_Personne());
    vector<int> v_Nbre_Jour_OFF_Consecutif(instance->get_Nombre_Personne());
    vector<int> v_Nbre_WE_Travaille(instance->get_Nombre_Personne());
    vector<int> v_Duree_Totale_Shift_Personne(instance->get_Nombre_Personne());
    vector<vector<int>> v_v_Nbre_Chaque_Shift_Pers(instance->get_Nombre_Personne());

    //map<int, int> m_v_Shift_trie;

    int duree_shift_min = instance->get_Shift_Duree(1);
    int duree_shift_max = instance->get_Shift_Duree(1);
    for (int shift = 0; shift < instance->get_Nombre_Shift(); shift++)
    {
        int duree_shift = instance->get_Shift_Duree(shift);
        if (duree_shift_min > duree_shift)
        {
            duree_shift_min = duree_shift;
        }
        if (duree_shift_max < duree_shift)
        {
            duree_shift_max = duree_shift;
        }
    }


    //------Cas du premier jour------

    S->v_v_IdShift_Par_Personne_et_Jour = vector<vector<int>>(instance->get_Nombre_Personne());
    for (int personne = 0; personne < instance->get_Nombre_Personne(); personne++)
    {
        S->v_v_IdShift_Par_Personne_et_Jour[personne] = vector<int>(instance->get_Nombre_Jour(), -1);
        v_v_Nbre_Chaque_Shift_Pers[personne] = vector<int>(instance->get_Nombre_Shift(), 0);
        int shift = -1;

        if ( instance->is_Available_Personne_Jour(personne, 0) && is_Peut_Reprendre_Travaille(instance, personne, 0, 0, v_Duree_Totale_Shift_Personne.at(0), duree_shift_max) )
        {
            int meilleur_candidat = -1;
            int plus_long_Duree_Shift = 0;
            for (int candidat = 0; candidat < instance->get_Nombre_Shift(); candidat++)
            {
                if (instance->get_Personne_Shift_Nbre_Max(personne, candidat) > 0) // La personne peut travailler sur ce shift
                {
                    int candidat_Shift_Duree = instance->get_Shift_Duree(candidat);
                    if (candidat_Shift_Duree > plus_long_Duree_Shift) // A REVOIR
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
            v_Nbre_Shift_Consecutif.at(personne) = 1; //Initialisation du compteur des shifts consecutifs et des jours OFF consécutifs
            v_Nbre_Jour_OFF_Consecutif.at(personne) = 0;
            v_Duree_Totale_Shift_Personne.at(personne) = instance->get_Shift_Duree(shift);
            v_v_Nbre_Chaque_Shift_Pers.at(personne).at(shift) += 1;
        }
        else
        {
            v_Nbre_Shift_Consecutif.at(personne) = 0; //Initialisation du compteur des shifts consecutifs et des jours OFF consécutifs
            v_Nbre_Jour_OFF_Consecutif.at(personne) = 1;
            v_Duree_Totale_Shift_Personne.at(personne) = 0;
        }
    }


    //------Cas général------

    for (int personne = 0; personne < instance->get_Nombre_Personne(); personne++)
    {
        for (int jour = 1; jour < instance->get_Nombre_Jour(); jour++) //On parcourt les jours à partir du deuxième
        {
            int shift = -1; //Initialisation du shift pour une personne et pour un jour
            
            // Si la durée totale des shifts de la personne avec le plus petit nouveau shift possible ne dépasse pas la durée maximale
            if (v_Duree_Totale_Shift_Personne.at(personne) + duree_shift_min <= instance->get_Personne_Duree_total_Max(personne))
            {
                // Si la personne ne travaille pas trop de fois de suite
                if (v_Nbre_Shift_Consecutif.at(personne) < instance->get_Personne_Nbre_Shift_Consecutif_Max(personne))
                {
                    // Si la personne peut travailler ce jour de weekend
                    if (((v_Nbre_WE_Travaille.at(personne) < instance->get_Personne_Nbre_WE_Max(personne)) && (jour % 7 >= 5)) || (jour % 7 < 5))
                    {
                        // Si la personne n'est pas en congé ce jour là
                        if (instance->is_Available_Personne_Jour(personne, jour))
                        {
                            if (    ( v_Nbre_Jour_OFF_Consecutif.at(personne) == 0 ) // Si la personne travaille depuis au moins le jour d'avant
                                 || (    v_Nbre_Jour_OFF_Consecutif.at(personne) >= instance->get_Personne_Jour_OFF_Consecutif_Min(personne) // Si la personne a eu assez de repos et peut reprendre son travail
                                      && is_Peut_Reprendre_Travaille(instance, personne, jour, v_Nbre_WE_Travaille.at(personne), v_Duree_Totale_Shift_Personne.at(personne), duree_shift_max) ) ) // Et que la personne pourra travailler au moins le nombre consécutif minimum de quart d'heure de travail.
                            {
                                // Choix d'un shift
                                bool is_chosen_shift = false;
                                for (int candidat = 0; (candidat < instance->get_Nombre_Shift()) && (!is_chosen_shift); candidat++) //les id shift correspondent à candidat
                                {
                                    // Si la durée totale des shifts de la personne avec le candidat ne dépasse pas la durée maximale
                                    if (v_Duree_Totale_Shift_Personne.at(personne) + instance->get_Shift_Duree(candidat) <= instance->get_Personne_Duree_total_Max(personne))
                                    {
                                        if (S->v_v_IdShift_Par_Personne_et_Jour.at(personne).at(jour-1) == -1) // Si le jour précédent était un jour repos
                                        {
                                            if (v_v_Nbre_Chaque_Shift_Pers.at(personne).at(candidat) < instance->get_Personne_Shift_Nbre_Max(personne, candidat))
                                            {
                                                shift = candidat;
                                                is_chosen_shift = true;
                                            }
                                        }
                                        else if ( instance->is_possible_Shift_Succede(S->v_v_IdShift_Par_Personne_et_Jour.at(personne).at(jour-1), candidat) ) // Sinon, on vérifie que le candidat peut succéder au jour de travail précédent
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

            S->v_v_IdShift_Par_Personne_et_Jour.at(personne).at(jour) = shift; // A VERIFIER
            if (shift != -1)
            {
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
                v_Nbre_Shift_Consecutif.at(personne) = 0;
                v_Nbre_Jour_OFF_Consecutif.at(personne) += 1;
                if ( (jour % 7 == 6) && ((S->v_v_IdShift_Par_Personne_et_Jour.at(personne).at(jour-1) != -1)) )	// Que ce jour là est dimanche (équivalent à : on a travaillé que le dimanche ou tout le WK)
                {
                    v_Nbre_WE_Travaille.at(personne) += 1;
                }
            }
        }
    }
    return S;
}


bool is_Peut_Reprendre_Travaille(Instance * instance, int personne, int jour, int Nbre_WE_Travaille_Personne, int duree_totale_shift_personne, int duree_shift_max)
{
    // Pour chaque jour suivants sur lesquels la personne devra travailler ensuite
    for (int i = jour; (i < jour + instance->get_Personne_Nbre_Shift_Consecutif_Min(personne)) && (i < instance->get_Nombre_Jour()); i++)  //On vérifie que la personne est disponible sur le nombre de jours consécutifs suivants minimum
    {
        if (    (!instance->is_Available_Personne_Jour(personne, i)) // Si la personnne est en congé ce jour là
             || (    Nbre_WE_Travaille_Personne >= instance->get_Personne_Nbre_WE_Max(personne) // Ou qu'elle s'apprête à travailler trop de weekends
                  && i % 7 >= 5 ) // Et que c'est le weekend
             || (duree_totale_shift_personne + duree_shift_max * instance->get_Personne_Nbre_Shift_Consecutif_Min(personne) > instance->get_Personne_Duree_total_Max(personne)) ) // Ou la personne ne dépassera pas sa durée maximale de travail
        {
            return false;
        }
    }
    return true;
}


int Duree_Minimum_Combinaison_Shift_Possible(...) {
    
}

/*
meilleur_combinaison_shift-consecutif_duree_minimum(tableau_Id-Shift_trié, personne):
    int combinaison_shift [nb_shift_consecutif_min]
    bool iniisalise = false
    int shift =0
    Tant que (! initialise)
        si get_Personne_Shift_Nbre_Max(personne,shift) > 0
            comobinaison_shift[0]= shift
            initilise = true
        fin Si
        shift++
    fin Tant QUe
    int dureeTotal =0
    for (i = 1; i <nb_shift_consecutif_min; i++)
        bool is_chosen = false
        shift = 0
        tant que ( !is_chosen && shift < instance->get_Nombre_Shift())
            si get_Personne_Shift_Nbre_Max(personne,shift) > 0 &&  is_possible_Shift_Succede(combinaison_shift[i-1],shift)
                combinsaison_shift[i] = shift
                is_chosen = true
            FinSi
            shift++
        fin Tantque
        si(shift >= instance->get_nombre_Shift())
            break pour
            Afficher("Erreur combinaison minimum impossible")
        fin si
        dureeeTotale += instance->get_Shift_Duree(combinaison_shift[i])
    fin pour
    return dureeTotale

*/