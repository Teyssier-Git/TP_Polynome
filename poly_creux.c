#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "poly_creux.h"
#include <x86intrin.h>

p_polyf_creux_t creer_polynome (int degre)
{
  p_polyf_creux_t p ;

  p = (p_polyf_creux_t) malloc (sizeof (polyf_creux_t)) ;
  p->degre = degre;
  p->head = NULL;

  return p ;

}

void detruire_polynome (p_polyf_creux_t p)
{
  p_coeff_t act = p->head;
  while (act != NULL) {
      p_coeff_t suiv = act->suivant;
      free(act);
      act = suiv;
  }
  free(p);
}

void init_polynome (p_polyf_creux_t p, float x)
{
    p_coeff_t act = p->head;
    for (int i=0; i<=p->degre; i++) {
        if (act == NULL) {
            act = (p_coeff_t)malloc(sizeof(coeff_t));
            act->degre = i;
            act->coeff = x;
            act->suivant = NULL;
            act = act->suivant;
        } else {
            act->degre = i;
            act->coeff = x;
            act = act->suivant;
        }
    }
}

void ajout_coef_trie (p_polyf_creux_t p, float coeff, int degre) {
    if (p->head == NULL) {
        p->head = (p_coeff_t)malloc(sizeof(coeff_t));
        p->head->degre = degre;
        p->head->coeff = coeff;
        p->head->suivant = NULL;
    } else {
        if (p->head->degre > degre) {
            p_coeff_t nouv = (p_coeff_t)malloc(sizeof(coeff_t));
            nouv->suivant = p->head;
            p->head = nouv;
            nouv->degre = degre;
            nouv->coeff = coeff;
        } else {
            p_coeff_t act = p->head;
            while (act->suivant != NULL && act->suivant->degre < degre) {
                act = act->suivant;
            }
            if (act->suivant == NULL) {
                act->suivant = (p_coeff_t)malloc(sizeof(coeff_t));
                act->suivant->degre = degre;
                act->suivant->coeff = coeff;
                act->suivant->suivant = NULL;
            } else {
                p_coeff_t nouv = (p_coeff_t)malloc(sizeof(coeff_t));
                nouv->suivant = act->suivant;
                act->suivant = nouv;
                nouv->degre = degre;
                nouv->coeff = coeff;
            }
        }
    }
}


p_polyf_creux_t lire_polynome_float (char *nom_fichier) //marche pas !!!!!!!!
{
    FILE *f ;
    p_polyf_creux_t p ;
    int nb ;
    int i  ;
    int cr ;
    int degre = -1;

    f = fopen (nom_fichier, "r");
    if (f == NULL) {
        fprintf (stderr, "erreur ouverture %s \n", nom_fichier) ;
        exit (-1) ;
    }

    cr = fscanf (f, "%d\n", &nb) ;
    if (cr != 1) {
        fprintf (stderr, "erreur lecture du nombre de coeff\n") ;
        exit (-1) ;
    }
    p = creer_polynome (-1) ;

    for (i = 0 ; i <= nb; i++) {
        int deg;
        float coeff;
        cr = fscanf (f, "%d %f\n", &deg,&coeff) ;
        if (cr != 2) {
            fprintf (stderr, "erreur lecture coefficient %d\n", i) ;
            exit (-1) ;
        }
        if (coeff != 0.0) {
            if (deg > degre) {
                degre = deg;
            }
            ajout_coef_trie(p,coeff,deg);
        }
    }

    fclose (f) ;
    p->degre = degre;

    return p ;
}

void ecrire_polynome_float (p_polyf_creux_t p)
{
    p_coeff_t act = p->head;
    if (act != NULL) {
        if (act->degre == 0) {
            printf ("%.1f ", act->coeff) ;
        } else {
            printf ("%.1f X^%d ", act->coeff, act->degre);
        }
        act = act->suivant;
        while (act != NULL) {
            printf ("+ %.1f X^%d ", act->coeff, act->degre);
            act = act->suivant;
        }
    }

    printf ("\n") ;

    return ;
}

int egalite_polynome (p_polyf_creux_t p1, p_polyf_creux_t p2) {
    if (p1->degre != p2->degre){
        return 0;
    } else {
        p_coeff_t mon_cur_p1 = p1->head; //monome courant
        p_coeff_t mon_cur_p2 = p2->head;
        while(mon_cur_p1 != NULL && mon_cur_p2 != NULL){
            if(mon_cur_p1->degre != mon_cur_p2->degre){
                if (mon_cur_p1->coeff == 0){
                    mon_cur_p1 = mon_cur_p1->suivant;
                } else if(mon_cur_p2->coeff == 0) {
                    mon_cur_p2 = mon_cur_p2->suivant;
                } else {
                    return 0;
                }
            } else if(mon_cur_p1->coeff != mon_cur_p2->coeff) {
                return 0;
            } else {
                mon_cur_p1 = mon_cur_p1->suivant;
                mon_cur_p2 = mon_cur_p2->suivant;
            }
        }
    }
    return 1;
}

p_polyf_creux_t addition_polynome (p_polyf_creux_t p1, p_polyf_creux_t p2){
    p_polyf_creux_t p3 ;

    p3 = creer_polynome (max (p1->degre, p2->degre));
    int deg = 0;
    p_coeff_t mon_cur_p1=p1->head;
    p_coeff_t mon_cur_p2=p2->head;
    while (mon_cur_p1!=NULL && mon_cur_p2!=NULL) {
        if (mon_cur_p1->degre!=mon_cur_p2->degre) {
            if (mon_cur_p1->degre<mon_cur_p2->degre) {
                ajout_coef_trie(p3,mon_cur_p1->coeff, mon_cur_p1->degre);
                mon_cur_p1=mon_cur_p1->suivant;
            } else {
                ajout_coef_trie(p3,mon_cur_p2->coeff, mon_cur_p2->degre);
                mon_cur_p2=mon_cur_p2->suivant;
            }
        } else if (mon_cur_p1->coeff+mon_cur_p2->coeff!=0) {
            ajout_coef_trie(p3,mon_cur_p1->coeff+mon_cur_p2->coeff,mon_cur_p2->degre);
            deg = mon_cur_p1->degre;
            mon_cur_p1 = mon_cur_p1->suivant;
            mon_cur_p2 = mon_cur_p2->suivant;
        }
     }
     while (mon_cur_p1!=NULL) {
         if (mon_cur_p1->coeff!=0) {
             ajout_coef_trie(p3,mon_cur_p1->coeff,mon_cur_p1->degre);
             deg=mon_cur_p1->degre;
         }
         mon_cur_p1=mon_cur_p1->suivant;
     }
     while(mon_cur_p2!=NULL){
         if(mon_cur_p2->coeff!=0){
             ajout_coef_trie(p3,mon_cur_p2->coeff,mon_cur_p2->degre);
             deg=mon_cur_p2->degre;
         }
         mon_cur_p2=mon_cur_p2->suivant;
     }
     p3->degre=deg;
     return p3 ;
}

p_polyf_creux_t multiplication_polynome_scalaire (p_polyf_creux_t p, float alpha)
{
    if (alpha != 0) {
        p_polyf_creux_t pN = creer_polynome(p->degre);
        p_coeff_t act = p->head;
        while (act != NULL) {
            ajout_coef_trie(pN, act->coeff * alpha, act->degre);
            act = act->suivant;
        }
        return pN;
    }
    return creer_polynome(0);
}

float eval_polynome (p_polyf_creux_t p, float x)
{
    float resultat = 0.0;
    p_coeff_t act = p->head;
    while (act != NULL) {
        resultat = resultat + act->coeff * powf(x,(float)act->degre);
        act = act->suivant;
    }
    return resultat;
}

p_polyf_creux_t multiplication_polynomes (p_polyf_creux_t p1, p_polyf_creux_t p2){
    p_polyf_creux_t p = creer_polynome(p1->degre + p2->degre);
    p_coeff_t mon_cur_p1=p1->head;
    while (mon_cur_p1 != NULL) {
        p_polyf_creux_t p4 = creer_polynome(p2->degre + mon_cur_p1->degre);
        p_coeff_t mon_cur_p2=p2->head;
        while (mon_cur_p2 != NULL) {
            float coeff = mon_cur_p1->coeff * mon_cur_p2->coeff;
            int deg = mon_cur_p1->degre + mon_cur_p2->degre;
            ajout_coef_trie(p4,coeff,deg);
            mon_cur_p2 = mon_cur_p2->suivant;
        }
        mon_cur_p1 = mon_cur_p1->suivant;
        p = addition_polynome(p,p4);
    }
    return p;
}

p_polyf_creux_t puissance_polynome (p_polyf_creux_t p, int n)
{
  p_polyf_creux_t pR = creer_polynome(p->degre * n);
  ajout_coef_trie(pR,1.0,0);

  while (n>0) {
      pR = multiplication_polynomes(pR,p);
      n--;
  }

  return pR;
}

p_polyf_creux_t composition_polynome (p_polyf_creux_t p, p_polyf_creux_t q)
{
    p_polyf_creux_t res = creer_polynome(p->degre+q->degre);
    p_coeff_t act = p->head;
    while (act != NULL) {
        res = addition_polynome(multiplication_polynome_scalaire(puissance_polynome(q,act->degre),act->coeff),res);
        act = act->suivant;
    }
    return res ;
}
