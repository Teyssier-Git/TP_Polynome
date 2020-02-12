#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "poly_creux.h"
#include <x86intrin.h>

p_polyf_creux_t creer_polynome ()
{
  p_polyf_creux_t p ;

  p = (p_polyf_creux_t) malloc (sizeof (polyf_creux_t)) ;
  p->degre = 0 ;
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
    p = creer_polynome () ;

    for (i = 0 ; i <= nb; i++) {
        int deg;
        float coeff;
        cr = fscanf (f, "%d %f\n", &deg,&coeff) ;
        if (cr != 2) {
            fprintf (stderr, "erreur lecture coefficient %d\n", i) ;
            exit (-1) ;
        }
        if (coeff != 0.0) {
            ajout_coef_trie(p,coeff,deg);
        }
    }

    fclose (f) ;

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
/*
int egalite_polynome (p_polyf_t p1, p_polyf_t p2)
{
  // if (p1->degre == p2->degre) {
  //     for (int i=0; i<=p1->degre; i++) {
  //           if (p1->coeff[i] != p2->coeff[i]) {
  //               return 0;
  //           }
  //     }
  //     return 1;
  // }
  // return 0 ;
  int min = min(p1->degre,p2->degre);
  int max = max(p1->degre,p2->degre);
  for (int i=0; i<=min; i++) {
      if (p1->coeff[i] != p2->coeff[i]) {
          return 0;
      }
  }
  p_polyf_t p3 = (max(p1->degre,p2->degre) == p1->degre ? p1 : p2);
  for (int i=min+1; i<=max; i++) {
      if (p3->coeff[i] != 0) {
          return 0;
      }
  }
  return 1;
}

p_polyf_t addition_polynome (p_polyf_t p1, p_polyf_t p2)
{
  p_polyf_t p3 ;
  register unsigned int i ;

  p3 = creer_polynome (max (p1->degre, p2->degre));

  for (i = 0 ; i <= min (p1->degre, p2->degre); ++i)
    {
      p3->coeff [i] = p1->coeff [i] + p2->coeff [i] ;
    }

  if (p1->degre > p2->degre)
    {
      for (i = (p2->degre + 1) ; i <= p1->degre; ++i)
	p3->coeff [i] = p1->coeff [i] ;
    }
  else if (p2->degre > p1->degre)
    {
      for (i = (p1->degre + 1) ; i <= p2->degre; ++i)
	p3->coeff [i] = p2->coeff [i] ;
    }

  return p3 ;
}

p_polyf_t multiplication_polynome_scalaire (p_polyf_t p, float alpha)
{
  p_polyf_t pN = creer_polynome(p->degre);
  for (int i=0; i<=p->degre; i++) {
      pN->coeff[i] = p->coeff[i] * alpha;
  }
  return pN;
}

float eval_polynome (p_polyf_t p, float x)
{
    float resultat = 0.0;
    for (int i=0; i<=p->degre; i++) {
        resultat = resultat + p->coeff[i] * powf(x,(float)i);
    }
    return resultat;
}

p_polyf_t multiplication_polynomes (p_polyf_t p1, p_polyf_t p2)
{
  // p_polyf_t p3 = creer_polynome(p1->degre + p2->degre);
  // init_polynome(p3,0.0);
  // for (int i=0; i<p1->degre; i++) {
  //     for (int j=0; j<p2->degre; j++) {
  //         p3->coeff[i+j] = p3->coeff[i+j] + p1->coeff[i] * p2->coeff[j];
  //     }
  // }
  // return p3;
  p_polyf_t p = creer_polynome(p1->degre + p2->degre);
  init_polynome(p,0.0);
  for (int i = 0; i<=p1->degre; i++){
      for (int j = 0; j<=p2->degre; j++){
          if (p1->coeff[i]!=0 && p2->coeff[j]!=0) {
              p->coeff[i+j] += (p1->coeff[i] * p2->coeff[j]);
          }
      }
  }
  return p;
}

p_polyf_t puissance_polynome (p_polyf_t p, int n)
{
  p_polyf_t pR = creer_polynome(p->degre * n);
  init_polynome(pR,0.0);
  pR->coeff[0] = 1.0;

  while (n>0) {
      pR = multiplication_polynomes(pR,p);
      n--;
  }

  return pR;
}

p_polyf_t composition_polynome (p_polyf_t p, p_polyf_t q)
{
    // p_polyf_t pR = creer_polynome(p->degre + q->degre));
    // init_polynome(pR,0.0);
    //
    // for (int i=1; i<=p->degre; i++) {
    //     //if (p->coeff[i]!=0) {
    //         printf("%f\n",pR->coeff[0]);
    //         pR = addition_polynome(pR,multiplication_polynome_scalaire(puissance_polynome(q,i),p->coeff[i]));
    //     //}
    // }
    //
    // return pR ;

    p_polyf_t res = creer_polynome(p->degre+q->degre);
    init_polynome(res, 0.0);
    for(int i=0; i<=p->degre; i++){
        res = addition_polynome(multiplication_polynome_scalaire(puissance_polynome(q,i),p->coeff[i]),res);
    }
    return res ;
}
*/
