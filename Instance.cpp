#include "Instance.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "math.h" 


#define LECTURE_INT_MAX 1000000

bool Instance::chargement_Instance(string s_chemin) throw(string)
{
    int i_section_en_cours;
    string s_lecture;
    int i_cpt_ligne,i_tmp,i,j,i_ajout,p_ajout,j_ajout,s_ajout;
    bool b_changement_section;
    map<string,int> m_NomShift_To_IdShift;
    map<string,int> m_NomPersonne_To_IdPersonne;
    istringstream is_tmp;
    vector<string> v_s_tmp,v_s_tmp2;
    int i_cpt_ID_Shift=0;
    int i_cpt_ID_Personne=0;
    vector<vector<string>> v_v_s_ens_succ_interdit;
    bool b_first_time_SECTION_SHIFT_ON_REQUESTS=true;
    bool b_first_time_SECTION_SHIFT_OFF_REQUESTS=true;
    bool b_first_time_SECTION_COVER=true;
    
    i_Nombre_Jour=0;
    i_Nombre_Shift=0;
    i_Nombre_Personne=0;
    v_Shift.clear();
    v_Personne.clear();
    v_v_v_Poids_Affectation_Pers_Jour_Shift.clear();
    v_v_v_Poids_Refus_Pers_Jour_Shift.clear();
    v_v_Nbre_Personne_Requis_Jour_Shift.clear();
    v_v_Poids_Personne_En_Plus_Jour_Shift.clear();
    v_v_Poids_Personne_En_Moins_Jour_Shift.clear();
    
    i_section_en_cours=-1;
    
    ifstream fichier(s_chemin.c_str(), std::ios::in);
    
    if(fichier)
    {
        i_cpt_ligne=0;
        while(getline(fichier, s_lecture))
        {
            i_cpt_ligne++;
            if((s_lecture!="")&&(s_lecture[0]!='#')&&(s_lecture!="\n")&&(s_lecture!="\r"))
            {
                b_changement_section=true;
                if((s_lecture.find("SECTION_HORIZON")!=string::npos))
                    i_section_en_cours=1;
                else
                    if((s_lecture.find("SECTION_SHIFTS")!=string::npos))
                         i_section_en_cours=2;
                    else
                        if((s_lecture.find("SECTION_STAFF")!=string::npos))
                             i_section_en_cours=3;
                        else
                            if((s_lecture.find("SECTION_DAYS_OFF")!=string::npos))
                                 i_section_en_cours=4;
                            else
                                if((s_lecture.find("SECTION_SHIFT_ON_REQUESTS")!=string::npos))
                                     i_section_en_cours=5;
                                else
                                    if((s_lecture.find("SECTION_SHIFT_OFF_REQUESTS")!=string::npos))
                                         i_section_en_cours=6;
                                    else
                                        if((s_lecture.find("SECTION_COVER")!=string::npos))
                                             i_section_en_cours=7;
                                        else
                                            b_changement_section=false;
                if(!b_changement_section)
                {
                    Shift unShift;
                    Personne unePersonne;
                    switch(i_section_en_cours)
                    {
                        case 1://SECTION_HORIZON
                            i_Nombre_Jour=atoi(s_lecture.c_str());
                            if((i_Nombre_Jour<0)||(i_Nombre_Jour>LECTURE_INT_MAX))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            break;
                        case 2://SECTION_SHIFTS
                            v_s_tmp.clear();
                            is_tmp.clear();
                            is_tmp.str(s_lecture);
                            while(getline(is_tmp,s_lecture,','))
                            {
                                if((s_lecture!="")&&(s_lecture!="\r"))
                                {
                                    v_s_tmp.push_back(s_lecture);
                                }
                            }
                            if((v_s_tmp.size()!=2)&&(v_s_tmp.size()!=3))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            m_NomShift_To_IdShift.insert(make_pair(v_s_tmp[0], i_cpt_ID_Shift));
                            i_cpt_ID_Shift++;
                            i_tmp=atoi(v_s_tmp[1].c_str());
                            if((i_tmp<0)||(i_tmp>LECTURE_INT_MAX))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            unShift.i_duree=i_tmp;
                            if(v_s_tmp.size()==3)
                            {
                                is_tmp.clear();
                                is_tmp.str(v_s_tmp[2]);
                                v_s_tmp.clear();
                                while(getline(is_tmp,s_lecture,'|'))
                                {
                                    if((s_lecture!="")&&(s_lecture!="\r"))
                                    {
                                        s_lecture.erase(remove(s_lecture.begin(), s_lecture.end(), '\r'), s_lecture.end());
                                        v_s_tmp.push_back(s_lecture);
                                    }
                                }
                                v_v_s_ens_succ_interdit.push_back(v_s_tmp);
                            }
                            else
                                v_v_s_ens_succ_interdit.push_back(vector<string> ());
                            v_Shift.push_back(unShift);
                            i_Nombre_Shift++;
                            break;
                        case 3://SECTION_STAFF
                            v_s_tmp.clear();
                            is_tmp.clear();
                            is_tmp.str(s_lecture);
                            while(getline(is_tmp,s_lecture,','))
                            {
                                if((s_lecture!="")&&(s_lecture!="\r"))
                                {
                                    v_s_tmp.push_back(s_lecture);
                                }
                            }
                            if(v_s_tmp.size()!=8)
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            m_NomPersonne_To_IdPersonne.insert(make_pair(v_s_tmp[0], i_cpt_ID_Personne));
                            i_cpt_ID_Personne++;
                            i_tmp=atoi(v_s_tmp[2].c_str());
                            if((i_tmp<0)||(i_tmp>LECTURE_INT_MAX))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            unePersonne.i_Duree_total_Max = i_tmp;
                            i_tmp=atoi(v_s_tmp[3].c_str());
                            if((i_tmp<0)||(i_tmp>LECTURE_INT_MAX))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            unePersonne.i_Duree_total_Min = i_tmp;
                            i_tmp=atoi(v_s_tmp[4].c_str());
                            if((i_tmp<0)||(i_tmp>i_Nombre_Jour))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            unePersonne.i_Nbre_Shift_Consecutif_Max = i_tmp;
                            i_tmp=atoi(v_s_tmp[5].c_str());
                            if((i_tmp<0)||(i_tmp>i_Nombre_Jour))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            unePersonne.i_Nbre_Shift_Consecutif_Min = i_tmp;
                            i_tmp=atoi(v_s_tmp[6].c_str());
                            if((i_tmp<0)||(i_tmp>i_Nombre_Jour))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            unePersonne.i_Nbre_Jour_OFF_Consecutif_Min = i_tmp;
                            i_tmp=atoi(v_s_tmp[7].c_str());
                            if((i_tmp<0)||(i_tmp>i_Nombre_Jour))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            unePersonne.i_Nbre_WE_Max = i_tmp;
                            is_tmp.clear();
                            is_tmp.str(v_s_tmp[1]);
                            v_s_tmp.clear();
                            while(getline(is_tmp,s_lecture,'|'))
                            {
                                if((s_lecture!="")&&(s_lecture!="\r"))
                                {
                                    s_lecture.erase(remove(s_lecture.begin(), s_lecture.end(), '\r'), s_lecture.end());
                                    v_s_tmp.push_back(s_lecture);
                                }
                            }
                            if(v_s_tmp.size()!=i_Nombre_Shift)
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            for(i=0;i<i_Nombre_Shift;i++)
                                unePersonne.v_Nbre_Max_Chaque_Shift.push_back(-1);
                            for(i=0;i<i_Nombre_Shift;i++)
                            {
                                is_tmp.clear();
                                is_tmp.str(v_s_tmp[i]);
                                v_s_tmp2.clear();
                                while(getline(is_tmp,s_lecture,'='))
                                {
                                    if((s_lecture!="")&&(s_lecture!="\r"))
                                    {
                                        s_lecture.erase(remove(s_lecture.begin(), s_lecture.end(), '\r'), s_lecture.end());
                                        v_s_tmp2.push_back(s_lecture);
                                    }
                                }
                                if(v_s_tmp2.size()!=2)
                                {
                                   cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                                   return false;
                                }
                                if(m_NomShift_To_IdShift.find(v_s_tmp2[0])!=m_NomShift_To_IdShift.end())
                                {
                                    i_tmp=atoi(v_s_tmp2[1].c_str());
                                    if((i_tmp<0)||(i_tmp>i_Nombre_Jour))
                                    {
                                       cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                                       return false;
                                    }
                                    unePersonne.v_Nbre_Max_Chaque_Shift[m_NomShift_To_IdShift[v_s_tmp2[0]]]=i_tmp;
                                }
                                else
                                {
                                    cout << " Erreur lecture des données : shift inconnu concernant le shift id="<<  i <<".\n";
                                    return false;
                                }
                            }
                            for(i=0;i<i_Nombre_Shift;i++)
                            {
                                if(unePersonne.v_Nbre_Max_Chaque_Shift[i]==-1)
                                {
                                   cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                                   return false;
                                }
                            }
                            v_Personne.push_back(unePersonne);
                            i_Nombre_Personne++;
                            break;
                        case 4://SECTION_DAYS_OFF
                            v_s_tmp.clear();
                            is_tmp.clear();
                            is_tmp.str(s_lecture);
                            while(getline(is_tmp,s_lecture,','))
                            {
                                if((s_lecture!="")&&(s_lecture!="\r"))
                                {
                                    s_lecture.erase(remove(s_lecture.begin(), s_lecture.end(), '\r'), s_lecture.end());
                                    v_s_tmp.push_back(s_lecture);
                                }
                            }
                            if(v_s_tmp.size()<=1)
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            if(m_NomPersonne_To_IdPersonne.find(v_s_tmp[0])!=m_NomPersonne_To_IdPersonne.end())
                            {
                                i_ajout=m_NomPersonne_To_IdPersonne[v_s_tmp[0]];
                                for(i=1;i<v_s_tmp.size();i++)
                                {
                                    i_tmp=atoi(v_s_tmp[i].c_str());
                                    if((i_tmp<0)||(i_tmp>i_Nombre_Jour))
                                    {
                                       cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                                       return false;
                                    }
                                    v_Personne[i_ajout].v_Id_Jour_Conges.push_back(i_tmp);
                                }
                            }
                            else
                            {
                                cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                                return false;
                            }
                            break;
                        case 5://SECTION_SHIFT_ON_REQUESTS
                            if(b_first_time_SECTION_SHIFT_ON_REQUESTS)
                            {
                                if((i_Nombre_Jour==0)||(i_Nombre_Personne==0)||(i_Nombre_Shift==0))
                                {
                                   cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                                   return false;
                                }
                                v_v_v_Poids_Affectation_Pers_Jour_Shift=vector<vector<vector<int> > >( i_Nombre_Personne, vector<vector<int> >(i_Nombre_Jour, vector<int>(i_Nombre_Shift, 0)));
                                b_first_time_SECTION_SHIFT_ON_REQUESTS=false;
                            }
                            v_s_tmp.clear();
                            is_tmp.clear();
                            is_tmp.str(s_lecture);
                            while(getline(is_tmp,s_lecture,','))
                            {
                                if((s_lecture!="")&&(s_lecture!="\r"))
                                {
                                    s_lecture.erase(remove(s_lecture.begin(), s_lecture.end(), '\r'), s_lecture.end());
                                    v_s_tmp.push_back(s_lecture);
                                }
                            }
                            if(v_s_tmp.size()!=4)
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            if(m_NomPersonne_To_IdPersonne.find(v_s_tmp[0])!=m_NomPersonne_To_IdPersonne.end())
                                p_ajout=m_NomPersonne_To_IdPersonne[v_s_tmp[0]];
                            else
                            {
                                cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                                return false;
                            }
                            j_ajout=atoi(v_s_tmp[1].c_str());
                            if((j_ajout<0)||(j_ajout>i_Nombre_Jour))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            if(m_NomShift_To_IdShift.find(v_s_tmp[2])!=m_NomShift_To_IdShift.end())
                                s_ajout=m_NomShift_To_IdShift[v_s_tmp[2]];
                            else
                            {
                                cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                                return false;
                            }
                            i_tmp=atoi(v_s_tmp[3].c_str());
                            if((i_tmp<0)||(i_tmp>LECTURE_INT_MAX))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            v_v_v_Poids_Affectation_Pers_Jour_Shift[p_ajout][j_ajout][s_ajout]=i_tmp;
                            break;
                        case 6://SECTION_SHIFT_OFF_REQUESTS
                            if(b_first_time_SECTION_SHIFT_OFF_REQUESTS)
                            {
                                if((i_Nombre_Jour==0)||(i_Nombre_Personne==0)||(i_Nombre_Shift==0))
                                {
                                   cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                                   return false;
                                }
                                v_v_v_Poids_Refus_Pers_Jour_Shift=vector<vector<vector<int> > >( i_Nombre_Personne, vector<vector<int> >(i_Nombre_Jour, vector<int>(i_Nombre_Shift, 0)));
                                b_first_time_SECTION_SHIFT_OFF_REQUESTS=false;
                            }
                            v_s_tmp.clear();
                            is_tmp.clear();
                            is_tmp.str(s_lecture);
                            while(getline(is_tmp,s_lecture,','))
                            {
                                if((s_lecture!="")&&(s_lecture!="\r"))
                                {
                                    s_lecture.erase(remove(s_lecture.begin(), s_lecture.end(), '\r'), s_lecture.end());
                                    v_s_tmp.push_back(s_lecture);
                                }
                            }
                            if(v_s_tmp.size()!=4)
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            if(m_NomPersonne_To_IdPersonne.find(v_s_tmp[0])!=m_NomPersonne_To_IdPersonne.end())
                                p_ajout=m_NomPersonne_To_IdPersonne[v_s_tmp[0]];
                            else
                            {
                                cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                                return false;
                            }
                            j_ajout=atoi(v_s_tmp[1].c_str());
                            if((j_ajout<0)||(j_ajout>i_Nombre_Jour))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            if(m_NomShift_To_IdShift.find(v_s_tmp[2])!=m_NomShift_To_IdShift.end())
                                s_ajout=m_NomShift_To_IdShift[v_s_tmp[2]];
                            else
                            {
                                cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                                return false;
                            }
                            i_tmp=atoi(v_s_tmp[3].c_str());
                            if((i_tmp<0)||(i_tmp>LECTURE_INT_MAX))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            v_v_v_Poids_Refus_Pers_Jour_Shift[p_ajout][j_ajout][s_ajout]=i_tmp;
                            break;
                        case 7://SECTION_COVER
                            if(b_first_time_SECTION_COVER)
                            {
                                if((i_Nombre_Jour==0)||(i_Nombre_Shift==0))
                                {
                                   cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                                   return false;
                                }
                                v_v_Nbre_Personne_Requis_Jour_Shift=vector<vector<int> >(i_Nombre_Jour, vector<int>(i_Nombre_Shift, 0));
                                v_v_Poids_Personne_En_Plus_Jour_Shift=vector<vector<int> >(i_Nombre_Jour, vector<int>(i_Nombre_Shift, 0));
                                v_v_Poids_Personne_En_Moins_Jour_Shift=vector<vector<int> >(i_Nombre_Jour, vector<int>(i_Nombre_Shift, 0));
                                b_first_time_SECTION_COVER=false;
                            }
                            v_s_tmp.clear();
                            is_tmp.clear();
                            is_tmp.str(s_lecture);
                            while(getline(is_tmp,s_lecture,','))
                            {
                                if((s_lecture!="")&&(s_lecture!="\r"))
                                {
                                    s_lecture.erase(remove(s_lecture.begin(), s_lecture.end(), '\r'), s_lecture.end());
                                    v_s_tmp.push_back(s_lecture);
                                }
                            }
                            if(v_s_tmp.size()!=5)
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            j_ajout=atoi(v_s_tmp[0].c_str());
                            if((j_ajout<0)||(j_ajout>i_Nombre_Jour))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            if(m_NomShift_To_IdShift.find(v_s_tmp[1])!=m_NomShift_To_IdShift.end())
                                s_ajout=m_NomShift_To_IdShift[v_s_tmp[1]];
                            else
                            {
                                cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                                return false;
                            }
                            i_tmp=atoi(v_s_tmp[2].c_str());
                            if((i_tmp<0)||(i_tmp>LECTURE_INT_MAX))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            v_v_Nbre_Personne_Requis_Jour_Shift[j_ajout][s_ajout]=i_tmp;
                            i_tmp=atoi(v_s_tmp[3].c_str());
                            if((i_tmp<0)||(i_tmp>LECTURE_INT_MAX))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            v_v_Poids_Personne_En_Moins_Jour_Shift[j_ajout][s_ajout]=i_tmp;
                            i_tmp=atoi(v_s_tmp[4].c_str());
                            if((i_tmp<0)||(i_tmp>LECTURE_INT_MAX))
                            {
                               cout<<" Erreur lecture des données : ligne "<<  i_cpt_ligne <<". \n";
                               return false;
                            }
                            v_v_Poids_Personne_En_Plus_Jour_Shift[j_ajout][s_ajout]=i_tmp;
                            break;
                      default :
                          cout << " Erreur lecture des données : ligne "<<  i_cpt_ligne <<" : identification de la ligne. \n";
                          return false;
                          break;
                    }
                }
            }
        }
        for(i=0;i<v_Shift.size();i++)
        {
            for(j=0;j<v_v_s_ens_succ_interdit[i].size();j++)
            {
                if(m_NomShift_To_IdShift.find(v_v_s_ens_succ_interdit[i][j])!=m_NomShift_To_IdShift.end())
                    v_Shift[i].v_Id_Shift_Suc_Interdit.push_back(m_NomShift_To_IdShift[v_v_s_ens_succ_interdit[i][j]]);
                else
                {
                    cout << " Erreur lecture des données : shift inconnu concernant le shift id="<<  i <<".\n";
                    return false;
                }
            }
        }
        
        fichier.close();
        
        return true;
    }
    else
    {
        cout<<" Erreur lecture des données : chemin non valide. \n";
        return false;
    }
}

Instance::Instance()
{
    s_chemin="";
    i_Nombre_Shift=0;
    i_Nombre_Personne=0;
    i_Nombre_Jour=0; 
}

Instance::~Instance()
{
    //RAS
}
string Instance::get_chemin(void)
{
    return s_chemin;
} 
int Instance::get_Nombre_Jour(void)
{
    return i_Nombre_Jour;
}

int Instance::get_Nombre_Personne(void)
{
    return i_Nombre_Personne;
}
int Instance::get_Nombre_Shift(void)
{
    return i_Nombre_Shift;
}
int Instance::get_Shift_Duree(int i_Id_Shift) throw(string)
{
    if(v_Shift.size()==0)
        return 0;
    if((i_Id_Shift>=0)&&(i_Id_Shift<i_Nombre_Shift))
        return v_Shift[i_Id_Shift].i_duree;
    else
    {
        string err = "Instance.cpp : get_Shift_Duree : l'Id Shift doit être compris entre 0 et " +to_string(i_Nombre_Shift)+ ".";
        throw string(err);
    }
}
vector<int> Instance::get_vector_Shift_Suc_Interdit(int i_Id_Shift) throw(string)
{
    if(v_Shift.size()==0)
        return vector<int> ();
    if((i_Id_Shift>=0)&&(i_Id_Shift<i_Nombre_Shift))
        return v_Shift[i_Id_Shift].v_Id_Shift_Suc_Interdit;
    else
    {
        string err = "Instance.cpp : get_vector_Shift_Suc_Interdit : l'Id Shift doit être compris entre 0 et " +to_string(i_Nombre_Shift)+ ".";
        throw string(err);
    }
}
int Instance::get_Personne_Duree_total_Max(int i_Id_Personne) throw(string)
{
    if(v_Personne.size()==0)
        return 0;
    if((i_Id_Personne>=0)&&(i_Id_Personne<i_Nombre_Personne))
        return v_Personne[i_Id_Personne].i_Duree_total_Max;
    else
    {
        string err = "Instance.cpp : get_Personne_Duree_total_Max : l'Id Personne doit être compris entre 0 et " +to_string(i_Nombre_Personne)+ ".";
        throw string(err);
    }
}
int Instance::get_Personne_Duree_total_Min(int i_Id_Personne) throw(string)
{
    if(v_Personne.size()==0)
        return 0;
    if((i_Id_Personne>=0)&&(i_Id_Personne<i_Nombre_Personne))
        return v_Personne[i_Id_Personne].i_Duree_total_Min;
    else
    {
        string err = "Instance.cpp : get_Personne_Duree_total_Min : l'Id Personne doit être compris entre 0 et " +to_string(i_Nombre_Personne)+ ".";
        throw string(err);
    }
}
int Instance::get_Personne_Nbre_Shift_Consecutif_Max(int i_Id_Personne) throw(string)
{
    if(v_Personne.size()==0)
        return 0;
    if((i_Id_Personne>=0)&&(i_Id_Personne<i_Nombre_Personne))
        return v_Personne[i_Id_Personne].i_Nbre_Shift_Consecutif_Max;
    else
    {
        string err = "Instance.cpp : get_Personne_Nbre_Shift_Consecutif_Max : l'Id Personne doit être compris entre 0 et " +to_string(i_Nombre_Personne)+ ".";
        throw string(err);
    }
}
int Instance::get_Personne_Nbre_Shift_Consecutif_Min(int i_Id_Personne) throw(string)
{
    if(v_Personne.size()==0)
        return 0;
    if((i_Id_Personne>=0)&&(i_Id_Personne<i_Nombre_Personne))
        return v_Personne[i_Id_Personne].i_Nbre_Shift_Consecutif_Min;
    else
    {
        string err = "Instance.cpp : get_Personne_Nbre_Shift_Consecutif_Min : l'Id Personne doit être compris entre 0 et " +to_string(i_Nombre_Personne)+ ".";
        throw string(err);
    }
}
int Instance::get_Personne_Jour_OFF_Consecutif_Min(int i_Id_Personne) throw(string)
{
    if(v_Personne.size()==0)
        return 0;
    if((i_Id_Personne>=0)&&(i_Id_Personne<i_Nombre_Personne))
        return v_Personne[i_Id_Personne].i_Nbre_Jour_OFF_Consecutif_Min;
    else
    {
        string err = "Instance.cpp : get_Personne_Jour_OFF_Consecutif_Min : l'Id Personne doit être compris entre 0 et " +to_string(i_Nombre_Personne)+ ".";
        throw string(err);
    }
}
int Instance::get_Personne_Nbre_WE_Max(int i_Id_Personne) throw(string)
{
    if(v_Personne.size()==0)
        return 0;
    if((i_Id_Personne>=0)&&(i_Id_Personne<i_Nombre_Personne))
        return v_Personne[i_Id_Personne].i_Nbre_WE_Max;
    else
    {
        string err = "Instance.cpp : get_Personne_Nbre_WE_Max : l'Id Personne doit être compris entre 0 et " +to_string(i_Nombre_Personne)+ ".";
        throw string(err);
    }
}
int Instance::get_Personne_Shift_Nbre_Max(int i_Id_Personne, int i_Id_Shift) throw(string)
{
    if(v_Personne.size()==0)
        return 0;
    if((i_Id_Personne>=0)&&(i_Id_Personne<i_Nombre_Personne))
    {
        if((i_Id_Shift>=0)&&(i_Id_Shift<i_Nombre_Shift))
            return v_Personne[i_Id_Personne].v_Nbre_Max_Chaque_Shift[i_Id_Shift];
        else
        {
            string err = "Instance.cpp : get_Personne_Shift_Nbre_Max : l'Id Personne doit être compris entre 0 et " +to_string(i_Nombre_Shift)+ ".";
            throw string(err);
        }
    }
    else
    {
        string err = "Instance.cpp : get_Personne_Shift_Nbre_Max : l'Id Personne doit être compris entre 0 et " +to_string(i_Nombre_Personne)+ ".";
        throw string(err);
    }
}
vector<int> Instance::get_vector_Personne_Id_Jour_Conges(int i_Id_Personne) throw(string)
{
    if(v_Personne.size()==0)
        return vector<int> ();
    if((i_Id_Personne>=0)&&(i_Id_Personne<i_Nombre_Personne))
        return v_Personne[i_Id_Personne].v_Id_Jour_Conges;
    else
    {
        string err = "Instance.cpp : get_vector_Personne_Id_Jour_Conges : l'Id Personne doit être compris entre 0 et " +to_string(i_Nombre_Personne)+ ".";
        throw string(err);
    }
}
int Instance::get_Poids_Affectation_Pers_Jour_Shift(int i_Id_Personne, int i_Id_Jour, int i_Id_Shift) throw(string)
{
    if(v_v_v_Poids_Affectation_Pers_Jour_Shift.size()==0)
        return 0;
    if((i_Id_Personne>=0)&&(i_Id_Personne<i_Nombre_Personne))
    {
        if((i_Id_Jour>=0)&&(i_Id_Jour<i_Nombre_Jour))
        {
            if((i_Id_Shift>=0)&&(i_Id_Shift<i_Nombre_Shift))
                return v_v_v_Poids_Affectation_Pers_Jour_Shift[i_Id_Personne][i_Id_Jour][i_Id_Shift];
            else
            {
                string err = "Instance.cpp : get_Poids_Affectation_Pers_Jour_Shift : l'Id Shift doit être compris entre 0 et " +to_string(i_Nombre_Shift)+ ".";
                throw string(err);
            }
        }
        else
        {
            string err = "Instance.cpp : get_Poids_Affectation_Pers_Jour_Shift : l'Id Jour doit être compris entre 0 et " +to_string(i_Nombre_Jour)+ ".";
            throw string(err);
        }
    }
    else
    {
        string err = "Instance.cpp : get_Poids_Affectation_Pers_Jour_Shift : l'Id Personne doit être compris entre 0 et " +to_string(i_Nombre_Personne)+ ".";
        throw string(err);
    }
}
int Instance::get_Poids_Refus_Pers_Jour_Shift(int i_Id_Personne, int i_Id_Jour, int i_Id_Shift) throw(string)
{
    if(v_v_v_Poids_Refus_Pers_Jour_Shift.size()==0)
        return 0;
    if((i_Id_Personne>=0)&&(i_Id_Personne<i_Nombre_Personne))
    {
        if((i_Id_Jour>=0)&&(i_Id_Jour<i_Nombre_Jour))
        {
            if((i_Id_Shift>=0)&&(i_Id_Shift<i_Nombre_Shift))
                return v_v_v_Poids_Refus_Pers_Jour_Shift[i_Id_Personne][i_Id_Jour][i_Id_Shift];
            else
            {
                string err = "Instance.cpp : get_Poids_Refus_Pers_Jour_Shift : l'Id Shift doit être compris entre 0 et " +to_string(i_Nombre_Shift)+ ".";
                throw string(err);
            }
        }
        else
        {
            string err = "Instance.cpp : get_Poids_Refus_Pers_Jour_Shift : l'Id Jour doit être compris entre 0 et " +to_string(i_Nombre_Jour)+ ".";
            throw string(err);
        }
    }
    else
    {
        string err = "Instance.cpp : get_Poids_Refus_Pers_Jour_Shift : l'Id Personne doit être compris entre 0 et " +to_string(i_Nombre_Personne)+ ".";
        throw string(err);
    }
}
int Instance::get_Nbre_Personne_Requis_Jour_Shift(int i_Id_Jour, int i_Id_Shift) throw(string)
{
    if(v_v_Nbre_Personne_Requis_Jour_Shift.size()==0)
        return 0;
    if((i_Id_Jour>=0)&&(i_Id_Jour<i_Nombre_Jour))
    {
        if((i_Id_Shift>=0)&&(i_Id_Shift<i_Nombre_Shift))
            return v_v_Nbre_Personne_Requis_Jour_Shift[i_Id_Jour][i_Id_Shift];
        else
        {
            string err = "Instance.cpp : get_Nbre_Personne_Requis_Jour_Shift : l'Id Shift doit être compris entre 0 et " +to_string(i_Nombre_Shift)+ ".";
            throw string(err);
        }
    }
    else
    {
        string err = "Instance.cpp : get_Nbre_Personne_Requis_Jour_Shift : l'Id Jour doit être compris entre 0 et " +to_string(i_Nombre_Jour)+ ".";
        throw string(err);
    }
}
int Instance::get_Poids_Personne_En_Plus_Jour_Shift(int i_Id_Jour, int i_Id_Shift) throw(string)
{
    if(v_v_Poids_Personne_En_Plus_Jour_Shift.size()==0)
        return 0;
    if((i_Id_Jour>=0)&&(i_Id_Jour<i_Nombre_Jour))
    {
        if((i_Id_Shift>=0)&&(i_Id_Shift<i_Nombre_Shift))
            return v_v_Poids_Personne_En_Plus_Jour_Shift[i_Id_Jour][i_Id_Shift];
        else
        {
            string err = "Instance.cpp : get_Poids_Personne_En_Plus_Jour_Shift : l'Id Shift doit être compris entre 0 et " +to_string(i_Nombre_Shift)+ ".";
            throw string(err);
        }
    }
    else
    {
        string err = "Instance.cpp : get_Poids_Personne_En_Plus_Jour_Shift : l'Id Jour doit être compris entre 0 et " +to_string(i_Nombre_Jour)+ ".";
        throw string(err);
    }
}
int Instance::get_Poids_Personne_En_Moins_Jour_Shift(int i_Id_Jour, int i_Id_Shift) throw(string)
{
    if(v_v_Poids_Personne_En_Moins_Jour_Shift.size()==0)
        return 0;
    if((i_Id_Jour>=0)&&(i_Id_Jour<i_Nombre_Jour))
    {
        if((i_Id_Shift>=0)&&(i_Id_Shift<i_Nombre_Shift))
            return v_v_Poids_Personne_En_Moins_Jour_Shift[i_Id_Jour][i_Id_Shift];
        else
        {
            string err = "Instance.cpp : get_Poids_Personne_En_Moins_Jour_Shift : l'Id Shift doit être compris entre 0 et " +to_string(i_Nombre_Shift)+ ".";
            throw string(err);
        }
    }
    else
    {
        string err = "Instance.cpp : get_Poids_Personne_En_Moins_Jour_Shift : l'Id Jour doit être compris entre 0 et " +to_string(i_Nombre_Jour)+ ".";
        throw string(err);
    }
}
bool Instance::is_possible_Shift_Succede(int i_Id_Shift, int i_Id_Shift_Successeur) throw(string)
{
    if(v_Shift.size()==0)
        return false;
    if((i_Id_Shift>=0)&&(i_Id_Shift<i_Nombre_Shift))
    {
        if((i_Id_Shift_Successeur>=0)&&(i_Id_Shift_Successeur<i_Nombre_Shift))
        {
            for(int i=0;i<v_Shift[i_Id_Shift].v_Id_Shift_Suc_Interdit.size();i++)
                if(v_Shift[i_Id_Shift].v_Id_Shift_Suc_Interdit[i]==i_Id_Shift_Successeur)
                    return false;
            return true;
        }
        else
        {
            string err = "Instance.cpp : is_possible_Shift_Succede : l'Id Shift doit être compris entre 0 et " +to_string(i_Nombre_Shift)+ ".";
            throw string(err);
        }
    }
    else
    {
        string err = "Instance.cpp : is_possible_Shift_Succede : l'Id Shift doit être compris entre 0 et " +to_string(i_Nombre_Shift)+ ".";
        throw string(err);
    }
}
bool Instance::is_Available_Personne_Jour(int i_Id_Personne, int i_Id_Jour) throw(string)
{
    if(v_Shift.size()==0)
        return false;
    if((i_Id_Personne>=0)&&(i_Id_Personne<i_Nombre_Personne))
    {
        if((i_Id_Jour>=0)&&(i_Id_Jour<i_Nombre_Jour))
        {
            for(int i=0;i<v_Personne[i_Id_Personne].v_Id_Jour_Conges.size();i++)
                if(v_Personne[i_Id_Personne].v_Id_Jour_Conges[i]==i_Id_Jour)
                    return false;
            return true;
        }
        else
        {
            string err = "Instance.cpp : is_Available_Personne_Jour : l'Id Jour doit être compris entre 0 et " +to_string(i_Nombre_Jour)+ ".";
            throw string(err);
        }
    }
    else
    {
        string err = "Instance.cpp : is_Available_Personne_Jour : l'Id Personne doit être compris entre 0 et " +to_string(i_Nombre_Personne)+ ".";
        throw string(err);
    }
}
