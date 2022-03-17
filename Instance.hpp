#ifndef Instance_hpp
#define Instance_hpp

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class Instance
{
    private:
        string s_chemin;                        //Chemin du fichier de données
        int i_Nombre_Jour;                      //Nombre de jours numéroté (id) de 0 à i_Nombre_Jour-1
        int i_Nombre_Shift;                     //Nombre de shifts (=Quart de travail) numéroté (id) de 0 à i_Nombre_Shift-1
        int i_Nombre_Personne;                  //Nombre de personnes (=Quart de travail) numéroté (id) de 0 à i_Nombre_Personne-1
        
        class Shift
        {
        public:
            int i_duree;                        //Durée en minute du shift
            vector<int> v_Id_Shift_Suc_Interdit;//Ensemble des Ids de shifts qui ne peuvent pas succéder à ce shift
        };
        vector<Shift> v_Shift;                   //L'ensemble des shifts

        class Personne
        {
        public:
            int i_Duree_total_Max;              //Durée maximale de travail sur la période
            int i_Duree_total_Min;              //Durée minimale de travail sur la période
            int i_Nbre_Shift_Consecutif_Max;    //Nombre de shifts consécutifs maximum assignable
            int i_Nbre_Shift_Consecutif_Min;    //Nombre de shifts consécutifs minimum assignable
            int i_Nbre_Jour_OFF_Consecutif_Min; //Nombre de jours OFF consécutifs minimum
            int i_Nbre_WE_Max;                  //Nombre de WE (samedi ou/et dimanche) de travail maximal (Ids des samedis : 5, 12, 19, ... et Ids des dimanche : 6, 13, 20, ...)
            vector<int> v_Nbre_Max_Chaque_Shift;//Nombre maximal de shifts (par type). La taille du tableau est donc de i_Nombre_Shift.
            vector<int> v_Id_Jour_Conges;       //L'ensemble Ids jours où la personne est en congés.
        };
        vector<Personne> v_Personne;               //L'ensemble des personnes
    
    vector<vector<vector<int>>> v_v_v_Poids_Affectation_Pers_Jour_Shift;// v_v_v_Poids_Affectation_Pers_Jour_Shift[p][j]][s] : Poids pour chaque préférence d’une personne p à vouloir un shift s à un jour donné j
    vector<vector<vector<int>>> v_v_v_Poids_Refus_Pers_Jour_Shift;      // v_v_v_Poids_Refus_Pers_Jour_Shift[p][j]][s] : Poids pour chaque préférence d’une personne p à NE PAS vouloir un shift s à un jour donné j
    
    vector<vector<int>> v_v_Nbre_Personne_Requis_Jour_Shift;   //Nombre cible de personne requise pour  chaque jour j et chaque shift s : v_v_Nbre_Personne_Requis[j][s]
    
    vector<vector<int>> v_v_Poids_Personne_En_Plus_Jour_Shift; //Poids pour chaque personne travaillant en plus sur un jour j et un shift s donnés : v_v_Poids_Personne_En_Plus[j][s]
    vector<vector<int>> v_v_Poids_Personne_En_Moins_Jour_Shift;//Poids pour chaque personne manquante sur un jour j et un shift s donnés : v_v_Poids_Personne_En_Moins[j][s]
     
    public:
    /* Constructeurs et destructeur  */
        Instance();
        virtual ~Instance();
    
    /* Instancie les données (écrase les données précédentes), retourne VRAI si chargement correct, FAUX sinon */
        bool chargement_Instance(string s_chemin) throw(string);
        
    /* LES Getters */
        string get_chemin(void);
        int get_Nombre_Personne(void);
        int get_Nombre_Shift(void);
        int get_Nombre_Jour(void);
        int get_Shift_Duree(int i_Id_Shift) throw(string);
        vector<int> get_vector_Shift_Suc_Interdit(int i_Id_Shift) throw(string);
        int get_Personne_Duree_total_Max(int i_Id_Personne) throw(string);
        int get_Personne_Duree_total_Min(int i_Id_Personne) throw(string);
        int get_Personne_Nbre_Shift_Consecutif_Max(int i_Id_Personne) throw(string);
        int get_Personne_Nbre_Shift_Consecutif_Min(int i_Id_Personne) throw(string);
        int get_Personne_Jour_OFF_Consecutif_Min(int i_Id_Personne) throw(string);
        int get_Personne_Nbre_WE_Max(int i_Id_Personne) throw(string);
        int get_Personne_Shift_Nbre_Max(int i_Id_Personne, int i_Id_Shift) throw(string);
        vector<int> get_vector_Personne_Id_Jour_Conges(int i_Id_Personne) throw(string);
        int get_Poids_Affectation_Pers_Jour_Shift(int i_Id_Personne, int i_Id_Jour, int i_Id_Shift) throw(string);
        int get_Poids_Refus_Pers_Jour_Shift(int i_Id_Personne, int i_Id_Jour, int i_Id_Shift) throw(string);
        int get_Nbre_Personne_Requis_Jour_Shift(int i_Id_Jour, int i_Id_Shift) throw(string);
        int get_Poids_Personne_En_Plus_Jour_Shift(int i_Id_Jour, int i_Id_Shift) throw(string);
        int get_Poids_Personne_En_Moins_Jour_Shift(int i_Id_Jour, int i_Id_Shift) throw(string);
    
    /* Fonction peut-être utile */
    bool is_possible_Shift_Succede(int i_Id_Shift, int i_Id_Shift_Successeur) throw(string);
    bool is_Available_Personne_Jour(int i_Id_Personne, int i_Id_Jour) throw(string);
};


#endif
