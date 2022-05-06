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
#include "MetaHeuristique.hpp"

using namespace std;


int Resolution(Instance* instance);

void printSolution(Solution* uneSolution);


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
    Solution* solutionInitiale = new Solution();
    float coeff_Valeur_FO_Contrainte = 0.85;

    solutionInitiale = Heuristique::GenerationSolutionRealisable(instance);
    Heuristique::InitValeurFonctionObjectif(solutionInitiale, instance, coeff_Valeur_FO_Contrainte);

    //printSolution(solutionInitiale);
    i_val_Retour_Fct_obj = solutionInitiale->i_valeur_fonction_objectif;
    solutionInitiale->Verification_Solution(instance);
    cout << "Valeur de la fonction objective sans pénalités : " << Heuristique::i_Calcul_Valeur_Fonction_Objectif(solutionInitiale, instance) << endl;
    cout << "Valeur de la fonction objectif : " << solutionInitiale->i_valeur_fonction_objectif << endl << endl;

    
    Solution* meilleureSolutionTrouvee = MetaHeuristique::MeilleureSolution(*solutionInitiale, instance, coeff_Valeur_FO_Contrainte); // Valeur de la FO initialisé dedans
    if (meilleureSolutionTrouvee)
    {
        //printSolution(meilleureSolutionTrouvee);
        i_val_Retour_Fct_obj = meilleureSolutionTrouvee->i_valeur_fonction_objectif;
        meilleureSolutionTrouvee->Verification_Solution(instance);
        cout << "Valeur de la fonction objective sans pénalités : " << Heuristique::i_Calcul_Valeur_Fonction_Objectif(meilleureSolutionTrouvee, instance) << endl;
        cout << "Valeur de la fonction objectif : " << meilleureSolutionTrouvee->i_valeur_fonction_objectif << endl << endl;
        delete meilleureSolutionTrouvee;
    }
    else
    {
        cout << "Pas de meilleure solution trouvée" << endl;
    }
    delete solutionInitiale;
    return i_val_Retour_Fct_obj;
}

void printSolution(Solution* uneSolution)
{
    for (int personne = 0; personne < uneSolution->v_v_IdShift_Par_Personne_et_Jour.size(); personne++)
    {
        for (int jour = 1; jour < uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne].size(); jour++) // On parcourt les jours à partir du deuxième
        {
            cout << uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour] << '\t';
        }
        cout << endl;
    }
}
