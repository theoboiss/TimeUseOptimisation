#include "Solution.hpp"
#include <iostream>
#include <set>

Solution::Solution()
{ 
    i_valeur_fonction_objectif=0;
}
Solution::~Solution()
{
    
}

bool Solution::Verification_Solution(Instance *instance)
{
    bool b_solution_ok=true;
    bool b_test_si_premier_jour_off=false;
    int i_fc_obj,i,j,jj,k,i_duree_travail,i_shift_consecutif,i_nb_WE;
    vector<vector<int>> v_i_nb_personne_par_Shift_et_jour(instance->get_Nombre_Shift(),vector<int> (instance->get_Nombre_Jour(),0));
    
    //Vérification de la taille de v_v_IdShift_Par_Personne_et_Jour
    if(v_v_IdShift_Par_Personne_et_Jour.size()!=instance->get_Nombre_Personne())
    {
        cout<< "Erreur : v_v_IdShift_Par_Personne_et_Jour n'est pas de la bonne taille en nombre de personnes." <<endl;
        b_solution_ok=false;
    }
    for(i=0;i<v_v_IdShift_Par_Personne_et_Jour.size();i++)
    {
        vector<int> v_i_Nb_shift(instance->get_Nombre_Shift(),0);
        i_duree_travail=0;
        i_shift_consecutif=0;
        i_nb_WE=0;
        if(v_v_IdShift_Par_Personne_et_Jour[i].size()!=instance->get_Nombre_Jour())
        {
            cout<< "Erreur : v_v_IdShift_Par_Personne_et_Jour n'est pas de la bonne taille en nombre de jour pour la personne "<< i<<"." <<endl;
            b_solution_ok=false;
        }
        //Vérification que v_v_IdShift_Par_Personne_et_Jour contient que des Ids Shifts valident ou -1
        for(j=0;j<v_v_IdShift_Par_Personne_et_Jour[i].size();j++)
        {
        if((v_v_IdShift_Par_Personne_et_Jour[i][j]!=-1)&&((v_v_IdShift_Par_Personne_et_Jour[i][j]<0)||(v_v_IdShift_Par_Personne_et_Jour[i][j]>instance->get_Nombre_Shift())))
            {
                cout<< "Erreur : v_v_IdShift_Par_Personne_et_Jour["<< i<<"]["<<j <<"] ne contient pas un shift valide." <<endl;
                b_solution_ok=false;
            }
            else
            {
                if(v_v_IdShift_Par_Personne_et_Jour[i][j]!=-1)
                {
                    v_i_Nb_shift[v_v_IdShift_Par_Personne_et_Jour[i][j]]=v_i_Nb_shift[v_v_IdShift_Par_Personne_et_Jour[i][j]]+1;
                    v_i_nb_personne_par_Shift_et_jour[v_v_IdShift_Par_Personne_et_Jour[i][j]][j]=v_i_nb_personne_par_Shift_et_jour[v_v_IdShift_Par_Personne_et_Jour[i][j]][j]+1;
                    i_duree_travail=i_duree_travail+instance->get_Shift_Duree(v_v_IdShift_Par_Personne_et_Jour[i][j]);
                    i_shift_consecutif++;
                    if((j%7)==5)
                        i_nb_WE++;
                    if(((j%7)==6)&&(v_v_IdShift_Par_Personne_et_Jour[i][j-1]==-1))
                        i_nb_WE++;
                    //Vérification du nombre de shifts consécutifs maximum assignable à chaque personne
                    if(i_shift_consecutif>instance->get_Personne_Nbre_Shift_Consecutif_Max(i))
                    {
                        cout<< "Erreur : Le nombre max de shift consécutif de la personne "<< i<<" a été dépassé." <<endl;
                        b_solution_ok=false;
                    }

                    //Vérification des jours de congés de chaque personne
                    if(!instance->is_Available_Personne_Jour(i,j))
                    {
                        {
                            cout<< "Erreur : Jour de congé "<<j<<" de la personne "<< i<<" non respecté." <<endl;
                            b_solution_ok=false;
                        }
                    }
                }
                else
                {
                    //Vérification du nombre de shifts consécutifs minimum assignable à chaque personne
                    if((i_shift_consecutif<instance->get_Personne_Nbre_Shift_Consecutif_Min(i))&&(i_shift_consecutif!=0)&&((j-instance->get_Personne_Nbre_Shift_Consecutif_Min(i))>0))
                    {
                        cout<< "Erreur : Le nombre min de shift consécutif de la personne "<< i<<" n'a pas été dépassé." <<endl;
                        b_solution_ok=false;
                    }
                    i_shift_consecutif=0;
                    b_test_si_premier_jour_off=false;
                    if(j!=0) 
                    {
                        if(v_v_IdShift_Par_Personne_et_Jour[i][j-1]!=-1)
                            b_test_si_premier_jour_off=true;
                    }
                    if(b_test_si_premier_jour_off)
                    {
                        b_test_si_premier_jour_off=false;
                        for (jj=j; jj<(j+instance->get_Personne_Jour_OFF_Consecutif_Min(i)); jj++)
                        {
                            if(jj<v_v_IdShift_Par_Personne_et_Jour[i].size())
                                if(v_v_IdShift_Par_Personne_et_Jour[i][jj]!=-1)
                                    b_test_si_premier_jour_off=true;
                        }
                        if(b_test_si_premier_jour_off)
                        {
                            cout<< "Erreur : Le nombre de jours OFF consécutifs minimum de la personne "<< i<<" n'a pas été respecté." <<endl;
                            b_solution_ok=false;
                        }
                    }
                }
                //Vérification des successions des Shifts
                if(j!=(v_v_IdShift_Par_Personne_et_Jour[i].size()-1))
                {
                    if((v_v_IdShift_Par_Personne_et_Jour[i][j]!=-1)&&(v_v_IdShift_Par_Personne_et_Jour[i][j+1]!=-1))
                    {
                        if(!instance->is_possible_Shift_Succede(v_v_IdShift_Par_Personne_et_Jour[i][j], v_v_IdShift_Par_Personne_et_Jour[i][j+1]))
                        {
                            cout<< "Erreur : Deux shifts se succèdent alors qu'il ne devrait pas, pour la personne "<<i<<"le jour"<<j<<" et "<<j+1<<"."<<endl;
                            b_solution_ok=false;
                        }
                    }
                }
            }
        }
        //Vérification de la durée totale maximale et minimale de chaque personne
        if((i_duree_travail>instance->get_Personne_Duree_total_Max(i))||(i_duree_travail<instance->get_Personne_Duree_total_Min(i)))
        {
            cout<< "Erreur : La durée totale de travail de la personne "<< i<<" ne respect pas sa valeur max ou min." <<endl;
            b_solution_ok=false;
        }
        //Vérification du nombre de WE (samedi ou/et dimanche) de travail maximal pour chaque personne
        if(i_nb_WE>instance->get_Personne_Nbre_WE_Max(i))
        {
            cout<< "Erreur : Le nombre max de travail le WE de la personne "<< i<<" n'est pas respecté." <<endl;
            b_solution_ok=false;
        }
        //Vérification du nombre maximal de shifts de chaque personne
        for(j=0;j<instance->get_Nombre_Shift();j++)
        {
            if(v_i_Nb_shift[j]>instance->get_Personne_Shift_Nbre_Max(i, j))
            {
                cout<< "Erreur : Le nombre max de shift "<< j <<" de la personne "<< i<<" n'est pas respecté." <<endl;
                b_solution_ok=false;
            }
        }
    }
    //Vérification de la fonction objectif
    i_fc_obj=0;
    for(i=0;i<v_v_IdShift_Par_Personne_et_Jour.size();i++)
    {
        for(j=0;j<v_v_IdShift_Par_Personne_et_Jour[i].size();j++)
        {
            if(v_v_IdShift_Par_Personne_et_Jour[i][j]!=-1)
            {
                i_fc_obj=i_fc_obj+instance->get_Poids_Refus_Pers_Jour_Shift(i, j, v_v_IdShift_Par_Personne_et_Jour[i][j]);
            }
            for(k=0;k<instance->get_Nombre_Shift();k++)
            {
                if(v_v_IdShift_Par_Personne_et_Jour[i][j]!=k)
                    i_fc_obj=i_fc_obj+instance->get_Poids_Affectation_Pers_Jour_Shift(i, j, k);
            }
        }
    }
    for(k=0;k<instance->get_Nombre_Shift();k++)
    {
        for(j=0;j<instance->get_Nombre_Jour();j++)
        {
            if(v_i_nb_personne_par_Shift_et_jour[k][j]<instance->get_Nbre_Personne_Requis_Jour_Shift(j, k))
                i_fc_obj=i_fc_obj+instance->get_Poids_Personne_En_Moins_Jour_Shift(j, k)*(instance->get_Nbre_Personne_Requis_Jour_Shift(j, k)-v_i_nb_personne_par_Shift_et_jour[k][j]);
                if(v_i_nb_personne_par_Shift_et_jour[k][j]>instance->get_Nbre_Personne_Requis_Jour_Shift(j, k))
                    i_fc_obj=i_fc_obj+instance->get_Poids_Personne_En_Plus_Jour_Shift(j, k)*(v_i_nb_personne_par_Shift_et_jour[k][j]-instance->get_Nbre_Personne_Requis_Jour_Shift(j, k));
        }
    }
    if(i_fc_obj!=i_valeur_fonction_objectif)
    {
        cout<< "Erreur : la fonction objectif est mal calculée, cela devrait être : "<< i_fc_obj<<"." <<endl;
        b_solution_ok=false;
    }

    return b_solution_ok;
}
