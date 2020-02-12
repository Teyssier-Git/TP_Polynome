#include <stdio.h>
#include <stdlib.h>

#include "poly_creux.h"


int main (int argc, char **argv)
{
  p_polyf_creux_t p1, p2/*, p3*/;

  if (argc != 3)
    {
      fprintf (stderr, "deux paramètres (polynomes,fichiers) sont à passer \n") ;
      exit (-1) ;
    }

  p1 = lire_polynome_float (argv [1]) ;
  p2 = lire_polynome_float (argv [2]) ;

  printf("== %s ==\n",argv[1]);
  ecrire_polynome_float (p1) ;
  printf("\n== %s ==\n",argv[2]);
  ecrire_polynome_float (p2) ;

/*
  printf("\n================== Test eval_polynome ===================\n");
  printf("%s : %f\n",argv[1],eval_polynome(p1,1.0));
  printf("%s : %f\n",argv[2],eval_polynome(p2,1.0));

  printf("\n================= Test egalite_polynome =================\n");
  printf("%s %s %s\n",argv[1],(egalite_polynome(p1,p2) ? "==" : "!="),argv[2]);
  printf("%s %s %s\n",argv[2],(egalite_polynome(p2,p1) ? "==" : "!="),argv[1]);
  printf("%s %s %s\n",argv[1],(egalite_polynome(p1,p1) ? "==" : "!="),argv[1]);
  printf("%s %s %s\n",argv[2],(egalite_polynome(p2,p2) ? "==" : "!="),argv[2]);

  printf("\n============= Test multiplication_polynomes =============\n");
  p3 = multiplication_polynomes(p1,p2);
  printf("%s * %s : ",argv[1],argv[2]);
  ecrire_polynome_float(p3);

  printf("\n================ Test puissance_polynome ================\n");
  p3 = puissance_polynome(p1,2);
  printf("%s^2 : ",argv[1]);
  ecrire_polynome_float(p3);

  printf("\n=============== Test composition_polynome ===============\n");
  p3 = composition_polynome(p1,p2);
  printf("%s o %s : ",argv[1],argv[2]);
  ecrire_polynome_float(p3);

  detruire_polynome(p1);
  detruire_polynome(p2);
  detruire_polynome(p3);
  */
}
