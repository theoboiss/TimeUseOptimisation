//#define CHEMIN_DOSSIER_DONNEES "/Users/kergosien/Desktop/Cours Optim Discret/Conception emploi du temps/Format Etudiant Public/"
#define CHEMIN_DOSSIER_DONNEES "/Users/boiss/workspaces/VisualStudio2019/DI4_OptiEmploiDuTemps/DI4_OptiEmploiDuTemps/Data/Format Etudiant Public/"
#define NOM_FICHIER_LISTE_FICHIER_DONNEES "data.txt"
#define NOM_FICHIER_LISTE_SORTIE "sortie.txt"

#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <list>
#include <algorithm>
#include "Instance.hpp"
#include "Solution.hpp"

using namespace std;

int i_Calcul_Valeur_Fonction_Objectif(Solution* uneSolution, Instance* instance);

bool Comparer(pair<int, int>& a, pair<int, int>& b);

int Resolution(Instance* instance);

bool is_Peut_Travailler_WE_Semaine(list<int>& l_Jour_OFF_Proche_WE_Personne, int nbre_WE_Travaille_Personne, int nbre_WE_Max_Personne, int semaine);

bool is_Jour_OFF_Proche_WE(Instance* instance, int jour, int i_Nbre_Jour_OFF_Consecutif_Min, int i_Nbre_Shift_Consecutif_Min);

Solution * GenerationSolutionRealisable(Instance * instance);

bool is_Peut_Reprendre_Travail(Instance* instance, list<int>& l_Jour_OFF_Proche_WE_Personne, int personne, int jour, int Nbre_WE_Travaille_Personne, int duree_totale_shift_personne, int duree_Min_Shift_Consecutif_Personne);

int Duree_Minimum_Combinaison_Shift_Possible(Instance* instance, int personne);


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
    uneSolution->i_valeur_fonction_objectif = i_Calcul_Valeur_Fonction_Objectif(uneSolution, instance);


    std::cout << uneSolution->Verification_Solution(instance);

    i_val_Retour_Fct_obj = uneSolution->i_valeur_fonction_objectif;
    delete uneSolution;
    return i_val_Retour_Fct_obj;
}


int i_Calcul_Valeur_Fonction_Objectif(Solution * uneSolution, Instance* instance)
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



// ################### HEURISTIQUE ###################





// ################### CREATION 1ERE SOLUTION "REALISABLE" ###################



bool is_Jour_OFF_Proche_WE(Instance* instance, int jour_OFF, int i_Nbre_Jour_OFF_Consecutif_Min, int i_Nbre_Shift_Consecutif_Min)
{
    for (int jour_Travail_Apres_Jour_OFF = jour_OFF + i_Nbre_Jour_OFF_Consecutif_Min; jour_Travail_Apres_Jour_OFF < jour_OFF + i_Nbre_Jour_OFF_Consecutif_Min + i_Nbre_Shift_Consecutif_Min; jour_Travail_Apres_Jour_OFF++)
    {
        if (jour_Travail_Apres_Jour_OFF % 7 >= 5)
        {
            return true;
        }
    }
    return false;
}

bool Comparer(pair<int, int>& a, pair<int, int>& b)
{
    return a.second < b.second;
}

Solution * GenerationSolutionRealisable(Instance * instance) {
    // Initialisation

    Solution* S = new Solution();

    /*
    * Pour chaque personne, on établit la liste des WE réservés (/jours de congés trop proches des WE (après le congé, reprendre le travail nous obligera à travailler le WE)),
    * et on vérifie avant de travailler un WE si un jour de congé problématique éxiste dans la semaine courante.
    * Si le nombre de WE réservés (/semaines contenant un jour problématique) est supérieur 
    * Si oui, on favorise le travail ce WE.
    * Si non, on évite de travailler ce WE.
    * 
    * Personne 29, semaine 4 (avec i_Nbre_Shift_Consecutif_Min = 2 et i_Nbre_Jour_OFF_Consecutif_Min = 2) :
    * Lundi, Mardi, Mercredi CONGE, Jeudi, Vendredi, Samedi, Dimanche
    *   T      T            X         X        X        X       ...
    *                       |---------> (i_Nbre_Jour_OFF_Consecutif_Min)
    *                                 ~~~~~~~~~|--------> (i_Nbre_Shift_Consecutif_Min)
    *                                                   WE      donc mercredi est un jour OFF proche du WE (problématique)
    *          |------------>
    *                       ~~~~~~~~~~|-------->
    *                                          PAS WE donc mardi (si c'était un jour de congé) n'est pas problématique
    * 
    *                                          |-------->
    *                                                   ~~~~~~~~~|---------->
    *                                                            WE     donc vendredi (si c'était un jour de congé) est problématique
    */
    vector<list<int>> v_l_Jour_OFF_Proche_WE(instance->get_Nombre_Personne());
    for (int personne = 0; personne < instance->get_Nombre_Personne(); personne++)
    {
        v_l_Jour_OFF_Proche_WE.at(personne) = list<int>();
        vector<int> personne_Id_Jour_Conges = instance->get_vector_Personne_Id_Jour_Conges(personne);

        for (int it = 0; it < personne_Id_Jour_Conges.size(); it++) // Pour chaque jour de congé
        {
            if (is_Jour_OFF_Proche_WE(instance, personne_Id_Jour_Conges.at(it), instance->get_Personne_Jour_OFF_Consecutif_Min(personne), instance->get_Personne_Nbre_Shift_Consecutif_Min(personne))) // S'il est proche du WE
            {
                int numero_Semaine_Jour_OFF_Proche_WE = personne_Id_Jour_Conges.at(it) / 7;
                v_l_Jour_OFF_Proche_WE.at(personne).push_back(numero_Semaine_Jour_OFF_Proche_WE); // On stocke son numéro de semaine pour la personne
            }
        }
        v_l_Jour_OFF_Proche_WE.at(personne).unique();
    }

    vector<int> v_Duree_Min_Shift_Consecutif_Personne(instance->get_Nombre_Personne());
    vector<int> v_Nbre_Shift_Consecutif(instance->get_Nombre_Personne());
    vector<int> v_Nbre_Jour_OFF_Consecutif(instance->get_Nombre_Personne());
    vector<int> v_Nbre_WE_Travaille(instance->get_Nombre_Personne());
    vector<int> v_Duree_Totale_Shift_Personne(instance->get_Nombre_Personne());
    vector<vector<int>> v_v_Nbre_Chaque_Shift_Pers(instance->get_Nombre_Personne());

    vector<pair<int, int>> v_v_Shift_trie;

    int duree_shift_min = instance->get_Shift_Duree(0);
    for (int shift = 0; shift < instance->get_Nombre_Shift(); shift++)
    {
        int duree_shift = instance->get_Shift_Duree(shift);

        v_v_Shift_trie.push_back({ shift, duree_shift });

        if (duree_shift_min > duree_shift)
        {
            duree_shift_min = duree_shift;
        }
    }
    sort(v_v_Shift_trie.begin(), v_v_Shift_trie.end(), Comparer); // TODO Test

    //------Cas du premier jour------

    S->v_v_IdShift_Par_Personne_et_Jour = vector<vector<int>>(instance->get_Nombre_Personne());
    for (int personne = 0; personne < instance->get_Nombre_Personne(); personne++)
    {
        S->v_v_IdShift_Par_Personne_et_Jour[personne] = vector<int>(instance->get_Nombre_Jour(), -1);
        v_v_Nbre_Chaque_Shift_Pers[personne] = vector<int>(instance->get_Nombre_Shift(), 0);
        v_Duree_Min_Shift_Consecutif_Personne.at(personne) = Duree_Minimum_Combinaison_Shift_Possible(instance, personne);
        int shift = -1;

        if ( instance->is_Available_Personne_Jour(personne, 0) && is_Peut_Reprendre_Travail(instance, v_l_Jour_OFF_Proche_WE.at(personne), personne, 0, 0, v_Duree_Totale_Shift_Personne.at(0), v_Duree_Min_Shift_Consecutif_Personne.at(personne)) )
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
            int semaine = jour / 7;
            int shift = -1; //Initialisation du shift pour une personne et pour un jour
            
            // Si la durée totale des shifts de la personne avec le plus petit nouveau shift possible ne dépasse pas la durée maximale
            if (v_Duree_Totale_Shift_Personne.at(personne) + duree_shift_min <= instance->get_Personne_Duree_total_Max(personne))
            {
                // Si la personne ne travaille pas trop de fois de suite
                if (v_Nbre_Shift_Consecutif.at(personne) < instance->get_Personne_Nbre_Shift_Consecutif_Max(personne))
                {
                    // Si la personne peut travailler ce jour de weekend
                    if (    (jour % 7 >= 5) && is_Peut_Travailler_WE_Semaine(v_l_Jour_OFF_Proche_WE.at(personne), v_Nbre_WE_Travaille.at(personne), instance->get_Personne_Nbre_WE_Max(personne), semaine)
                         || (jour % 7 < 5) )
                    {
                        // Si la personne n'est pas en congé ce jour là
                        if (instance->is_Available_Personne_Jour(personne, jour))
                        {
                            if (    ( v_Nbre_Jour_OFF_Consecutif.at(personne) == 0 ) // Si la personne travaille depuis au moins le jour d'avant
                                 || (    v_Nbre_Jour_OFF_Consecutif.at(personne) >= instance->get_Personne_Jour_OFF_Consecutif_Min(personne) // Si la personne a eu assez de repos et peut reprendre son travail
                                      //&& v_Duree_Totale_Shift_Personne.at(personne) < instance->get_Personne_Duree_total_Min(personne)
                                      && is_Peut_Reprendre_Travail(instance, v_l_Jour_OFF_Proche_WE.at(personne), personne, jour, v_Nbre_WE_Travaille.at(personne), v_Duree_Totale_Shift_Personne.at(personne), v_Duree_Min_Shift_Consecutif_Personne.at(personne)) ) ) // Et que la personne pourra travailler au moins le nombre consécutif minimum de quart d'heure de travail.
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

            if (!v_l_Jour_OFF_Proche_WE.at(personne).empty() && jour % 7 == 6 && v_l_Jour_OFF_Proche_WE.at(personne).front() == semaine)
            {
                v_l_Jour_OFF_Proche_WE.at(personne).pop_front();
            }
        }
    }
    return S;
}


bool is_Peut_Travailler_WE_Semaine(list<int>& l_Jour_OFF_Proche_WE_Personne, int nbre_WE_Travaille_Personne, int nbre_WE_Max_Personne, int semaine)
{
    return (    (nbre_WE_Travaille_Personne + l_Jour_OFF_Proche_WE_Personne.size() < nbre_WE_Max_Personne) // S'il reste encore des WE sur lesquels on peut travailler
             || (    (nbre_WE_Travaille_Personne + l_Jour_OFF_Proche_WE_Personne.size() == nbre_WE_Max_Personne) // Ou s'il ne reste que des WE réservés
                  && (!l_Jour_OFF_Proche_WE_Personne.empty() && l_Jour_OFF_Proche_WE_Personne.front() == semaine) ) ); // Et que le WE de cette semaine est réservé
}


bool is_Peut_Reprendre_Travail(Instance* instance, list<int>& l_Jour_OFF_Proche_WE_Personne, int personne, int jour, int Nbre_WE_Travaille_Personne, int duree_totale_shift_personne, int duree_Min_Shift_Consecutif_Personne)
{
    // Pour chaque jour suivants sur lesquels la personne devra travailler ensuite
    for (int i = jour; (i < jour + instance->get_Personne_Nbre_Shift_Consecutif_Min(personne)) && (i < instance->get_Nombre_Jour()); i++)  //On vérifie que la personne est disponible sur le nombre de jours consécutifs suivants minimum
    {
        if (    (!instance->is_Available_Personne_Jour(personne, i)) // Si la personnne est en congé ce jour là
             || (    (i % 7 >= 5) // Ou que c'est le weekend
                  && (!is_Peut_Travailler_WE_Semaine(l_Jour_OFF_Proche_WE_Personne, Nbre_WE_Travaille_Personne, instance->get_Personne_Nbre_WE_Max(personne), i/7)) ) // Et qu'elle s'apprête à travailler trop de weekends
                    //Nbre_WE_Travaille_Personne >= instance->get_Personne_Nbre_WE_Max(personne) ) // Et qu'elle s'apprête à travailler trop de weekends
             || (duree_totale_shift_personne + duree_Min_Shift_Consecutif_Personne > instance->get_Personne_Duree_total_Max(personne))) // Ou la personne ne dépassera pas sa durée maximale de travail
        {
            return false;
        }
    }
    return true;
}


int Duree_Minimum_Combinaison_Shift_Possible(Instance * instance, int personne) {
    int nb_shift_consecutif_min = instance->get_Personne_Nbre_Shift_Consecutif_Min(personne);
    vector<int> combinaison_shift(nb_shift_consecutif_min);
    bool est_initialise = false;
    int shift = 0;
    
    while (!est_initialise)
    {
        if (instance->get_Personne_Shift_Nbre_Max(personne, shift) > 0)
        {
            combinaison_shift.at(0) = shift;
            est_initialise = true;
        }
        shift++;
    }

    int duree_Totale_Combinaison_Shift = duree_Totale_Combinaison_Shift = instance->get_Shift_Duree(combinaison_shift[0]);
    for (int i = 1; i < nb_shift_consecutif_min; i++)
    {
        bool est_Choisi_Shift_Combinaison = false;
        for (shift = 0; shift < instance->get_Nombre_Shift() && !est_Choisi_Shift_Combinaison; shift++)
        {
            if (    instance->get_Personne_Shift_Nbre_Max(personne, shift) > 0
                 && instance->is_possible_Shift_Succede(combinaison_shift[i-1], shift))
            {
                combinaison_shift.at(i) = shift;
                est_Choisi_Shift_Combinaison = true;
            }
        }
        if (!est_Choisi_Shift_Combinaison)
        {
            cout << "Erreur dans Duree_Minimum_Combinaison_Shift_Possible : Combinaison minimum impossible" << endl;
            break;
        }
        duree_Totale_Combinaison_Shift += instance->get_Shift_Duree(combinaison_shift[i]);
    }

    return duree_Totale_Combinaison_Shift;
}
