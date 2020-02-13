#include <stdio.h>
#include <stdlib.h>

#include "poly_creux.h"

#include <x86intrin.h>

static const float duree_cycle = (float) 1 / (float) 2.6 ; // duree du cycle en nano seconde 10^-9

void calcul_flop (char *message, int nb_operations_flottantes, unsigned long long int cycles)
{
  printf ("%s %d operations %f GFLOP/s\n", message, nb_operations_flottantes, ((float) nb_operations_flottantes) / (((float) cycles) * duree_cycle)) ;
  return ;
}

int main (int argc, char **argv)
{
  p_polyf_creux_t pc1, pc2, pc3, pc4, pc5, pc6 ;
  unsigned long long start, end ;

  if (argc != 3)
    {
      fprintf (stderr, "deux paramètres (polynomes,fichiers) sont à passer \n") ;
      exit (-1) ;
    }

  pc1 = lire_polynome_float (argv [1]) ;
  pc2 = lire_polynome_float (argv [2]) ;

  printf ("pc1 = ") ;
  ecrire_polynome_float (pc1) ;

  printf ("pc2 = ") ;
  ecrire_polynome_float (pc2) ;

  /*
    ajouter du code pour tester les fonctions
    sur les polynomes
  */

  start = _rdtsc () ;

        pc3 = addition_polynome (pc1, pc2) ;

  end = _rdtsc () ;

  printf ("pc3 = ") ;
  ecrire_polynome_float (pc3) ;

  printf ("addition %Ld cycles\n", end-start) ;
  calcul_flop ("pc1+pc2", min(pc1->degre, pc2->degre)+1, end-start) ;
  detruire_polynome (pc3) ;

  pc4 = creer_polynome (200) ;
  pc5 = creer_polynome (200) ;

  init_polynome (pc4, 1.0) ;
  init_polynome (pc5, 2.0) ;

    start = _rdtsc () ;

        pc6 = composition_polynome (pc4, pc5) ;

  end = _rdtsc () ;

  printf ("composition %Ld cycles\n", end-start) ;
  calcul_flop ("pc4+pc5", min(pc4->degre, pc5->degre)+1, end-start) ;
  detruire_polynome (pc6) ;

}
