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
#include "Heuristique.hpp"

using namespace std;

int Resolution(Instance* instance);


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

    uneSolution = Heuristique::GenerationSolutionRealisable(instance);
    uneSolution->i_valeur_fonction_objectif = Heuristique::i_Calcul_Valeur_Fonction_Objectif(uneSolution, instance);
    cout << uneSolution->i_valeur_fonction_objectif << " ";

    std::cout << uneSolution->Verification_Solution(instance);

    uneSolution->i_valeur_fonction_objectif += Heuristique::i_Calcul_Penalisation_Fonction_Objectif(uneSolution, instance);
    cout << uneSolution->i_valeur_fonction_objectif << endl;

    i_val_Retour_Fct_obj = uneSolution->i_valeur_fonction_objectif;
    delete uneSolution;
    return i_val_Retour_Fct_obj;
}
