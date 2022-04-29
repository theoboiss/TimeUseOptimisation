#include "MetaHeuristique.h"

int MetaHeuristique::CodageLineaire(int a, int x, int b, int modulo)
{
    return a*x + b % modulo;
}

Solution* MetaHeuristique::OperateurSwapCodageLineaire(Solution* uneSolution, int a, int b)
{
    Solution* unVoisin = new Solution();
    int nbPersonne, nbJour;

    nbPersonne = uneSolution->v_v_IdShift_Par_Personne_et_Jour.size();
    for (int personne = 0; personne < nbPersonne; personne++)
    {
        nbJour = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne].size();
        for (int jour = 0; jour < nbJour; jour++)
        {
            unVoisin->v_v_IdShift_Par_Personne_et_Jour[CodageLineaire(a, personne, b, nbPersonne)][CodageLineaire(a, jour, b, nbJour)] = uneSolution->v_v_IdShift_Par_Personne_et_Jour[personne][jour];
        }
    }

    return unVoisin;
}
