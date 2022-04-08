//#define CHEMIN_DOSSIER_DONNEES "/Users/kergosien/Desktop/Cours Optim Discret/Conception emploi du temps/Format Etudiant Public/"
#define CHEMIN_DOSSIER_DONNEES "/Users/boiss/workspaces/VisualStudio2019/DI4_OptiEmploiDuTemps/DI4_OptiEmploiDuTemps/Data/Format Etudiant Public/"
#define NOM_FICHIER_LISTE_FICHIER_DONNEES "data.txt"
#define NOM_FICHIER_LISTE_SORTIE "sortie.txt"

#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include "Instance.hpp"
#include "Solution.hpp"

using namespace std;


int Resolution(Instance* instance);

Solution * GenerationSolutionRealisable(Instance * instance);

bool is_Peut_Reprendre_Travaille(Instance* instance, int personne, int jour, int Nbre_WE_Travaille_Personne);


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

    //i_val_Retour_Fct_obj = uneSolution->i_valeur_fonction_objectif;
    //delete uneSolution;
    //return i_val_Retour_Fct_obj;
    return 0;
}


Solution * GenerationSolutionRealisable(Instance * instance) {
    Solution* S = new Solution();

    vector<int> v_Nbre_Shift_Consecutif(instance->get_Nombre_Personne());
    vector<int> v_Nbre_Jour_OFF_Consecutif(instance->get_Nombre_Personne());
    vector<int> v_Nbre_WE_Travaille(instance->get_Nombre_Personne());
    vector<int> v_Duree_Totale_Shift_Personne(instance->get_Nombre_Personne());
    vector<vector<int>> v_v_Nbre_Chaque_Shift_Pers(instance->get_Nombre_Personne());

    //------Cas du premier jour------
    S->v_v_IdShift_Par_Personne_et_Jour = vector<vector<int>>(instance->get_Nombre_Personne());
    for (int personne = 0; personne < instance->get_Nombre_Personne(); personne++)
    {
        cout << "Boucle personne : " << personne << endl;
        S->v_v_IdShift_Par_Personne_et_Jour[personne] = vector<int>(instance->get_Nombre_Jour(), -1);
        v_v_Nbre_Chaque_Shift_Pers[personne] = vector<int>(instance->get_Nombre_Shift(), 0);
        int shift = -1;

        if ((instance->is_Available_Personne_Jour(personne, 0)) && (is_Peut_Reprendre_Travaille(instance, personne, 0, 0)))
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
            
            if (v_Duree_Totale_Shift_Personne.at(personne) < instance->get_Personne_Duree_total_Max(personne))
            {
                if (v_Nbre_Shift_Consecutif.at(personne) < instance->get_Personne_Nbre_Shift_Consecutif_Max(personne))
                {
                    if (((v_Nbre_WE_Travaille.at(personne) < instance->get_Personne_Nbre_WE_Max(personne)) && (jour % 7 >= 5)) || (jour % 7 < 5))
                    {
                        if (instance->is_Available_Personne_Jour(personne, jour))
                        {
                            if ( ( (v_Nbre_Jour_OFF_Consecutif.at(personne) == 0) ||v_Nbre_Jour_OFF_Consecutif.at(personne) >= instance->get_Personne_Jour_OFF_Consecutif_Min(personne) )
                                &&
                                 (is_Peut_Reprendre_Travaille(instance, personne, jour, v_Nbre_WE_Travaille.at(personne))) )
                                //v_Nbre_Jour_OFF_Consecutif[personne] == 0 :situation dans laquelle la personne travaille depuis au moins le jour d'avant
                                    // 0 < v_Nbre_Jour_OFF_Consecutif[personne] < get_Personne_Nbre_Jour_OFF_Consecutif_Min(personne)  : situation dans laquelle la personne accumule ses repos consécutifs
                                    // v_Nbre_Jour_OFF_Consecutif[personne] >= get_Personne_Nbre_Jour_OFF_Consecutif_Min(personne) : situation dans laquelle la personne peut reprendre son travail
                                // is_Peut_Reprendre_Travaille(personne, jour, v_Nbre_WE_Travaille[personne]) : on vérifie que la personne pourra travailler au moins le nombre consécutif minimum de quart d'heure de travail.
                            { //choix d'un shift
                                bool is_chosen_shift = false;
                                for (int candidat = 0; (candidat < instance->get_Nombre_Shift()) && (!is_chosen_shift); candidat++) //les id shift correspondent à candidat
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


bool is_Peut_Reprendre_Travaille(Instance * instance, int personne, int jour, int Nbre_WE_Travaille_Personne)
{
    for (int i = jour; (i < jour + instance->get_Personne_Nbre_Shift_Consecutif_Min(personne)) && (i < instance->get_Nombre_Jour()); i++)  //On vérfiie que la personne est disponible sur la nombre de jour minimum consécutif suivant (contraintre shift consécutif minimum) )
    {
        if ((!instance->is_Available_Personne_Jour(personne, i)) || (Nbre_WE_Travaille_Personne >= instance->get_Personne_Nbre_WE_Max(personne) && i % 7 >= 5))
        {
            return false;
        }
    }
    return true;
}
