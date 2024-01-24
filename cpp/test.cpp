#include <SDL2/SDL.h>
#include "seq_reader.h"


int main (int argc, char *argv[]) 
{
  std::vector<std::vector<double>> I ;
  
  std::vector<std::vector<std::vector<double>>> Iall ;
  
  seq_reader::seq F("../Sample.seq") ; 
  F.read_all(Iall) ; 
    
  for (int i=0 ; i<150 ; i++)
  {
    F.show(Iall[i],1,{0},{20000},10) ; 
  }
  
}



