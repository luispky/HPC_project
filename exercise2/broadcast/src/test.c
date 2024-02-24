#include <stdio.h>
#include <math.h>

int main ( )
{
  int two_to_generation;
  int rank, size;
  int to, from;

                                     /* scan over hypotetical virtual */
                                     /* machine of 16 nodes           */
  size = 16; 
  printf( "Binary tree set for %d  nodes \n", size );
  for ( rank =0 ; rank<size ; rank++ )
    {
      printf( " rank = %3d ", rank );
                                     /* two_to_generation reflects the */ 
                                     /* steps in the tree broadcast    */
      two_to_generation = 1;
      while( two_to_generation < size )
        {                            /* receive message */
          if ( rank >= two_to_generation && 
              rank < two_to_generation*2 )
            {
              from = rank - two_to_generation;
              if ( from < size )
                 printf ( " -- from %3d ", from );
            }
          if ( rank < two_to_generation )
            {                        /* send message */
              to = rank + two_to_generation;
              if ( to < size )
                 printf( " -- to %3d ", to );
            }
          two_to_generation = 2 * two_to_generation;
        }
                                     /* done for a given rank */
      printf("\n");
    }
}