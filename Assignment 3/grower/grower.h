/* File: grower.h */

//                                                                                                #  
//                                                                                              # #  
//                                                                                                ## 
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                          ###      
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                  #
//                                                                                                 ##
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                   
//                                                                                                   
//     # ##                                                                                          
//    #   #                                                                                          
//   #                                                                                               
//   #                                                                                               
//  # #                                                                                              

#include "stdint.h"

#define GROWER_HEIGHT (3)
#define GROWER_WIDTH (3)

uint8_t grower[GROWER_HEIGHT][GROWER_WIDTH] = {
     {0, 1, 0},
    {0, 0, 1},
    {1, 1, 1}
};



