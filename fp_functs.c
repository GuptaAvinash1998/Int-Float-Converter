//NAME: Avinash Gupta
//CLASS: CS367 001
//RECITATION SECTION: 307

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fp.h"
#include<string.h>
#define MASK 0x3E00
#define MASK1 0x1FF
int computeFP(float val) {
// input: float value to be represented
// output: integer version in our representation
//
// Perform this the same way we did in class -
//    either dividing or multiplying the value by 2
//    until it is in the correct range (between 1 and 2).
//    Your exponent is the number of times this operation
//    was performed.   
// Deal with rounding by simply truncating the number.
// Check for overflow and underflow - 
//    with 5 exponent bits, we have overflow if the number to be 
//       stored is > 30
//    for overflow (exp > 30), return -1
//    For underflow (exp < 1), return 0
  
  int E = 0;//stores the E value
  
  if(val < 1){//if the value is less than 1

	while(!(val>=1 && val<2)){//if the value is not in the range [1,2), then we multiply it until it is in the range
		
		val*=2.0;
		E--;//every time we multiply, we subtract 1 to E
	}
  }else{//if the value is greater than 1

	while(!(val>=1 && val<2)){//if the value is not in the range [1,2) then we divide until it is in the range

		val/=2.0;
		E++;//everytime we divide by 2 we add 1 to E
	}
  }

  //at this point we have E and M
  
  float frac = val-1.0;//this stores the frac field

  int ifrac = 0;//integer representation of the frac
  
  ifrac = frac*(pow(2,9));//to convert it to the int form on paper, we are moving the decimal, which in the end means that we are multiplying the number until we get the precision that we want
  //int this case since the frac field is 9 bits long, we are multiplying it by 2^9 so that we get the right number, and when we cast it to an int, we do not loose any precision

  int exp = E+15;//the formula to calculate exp is exp = E+bias, where bias is 2^((#bits in exp)-1) -1. Since the exp field is 5 bits long, bias will be 2^(5-1) -1 = 15

  if(exp > 30){//checks for overflow and underflow for the exp field

	return -1;
  }

  if(exp < 1){

	return 0;
  }
  
  int ival = 0;//this is the number that we are going to return which is basically the integer representation of the floating point number
  ival |= (exp<<9); //the format of the normalized number is 1 bit for sign, 5 for exp and 9 for frac, since these are the forst 15 bits of the 32 bit number, we left shift exp till we get it in the right posotion and then ORing it
  ival |= ifrac;//we direct OR the frac value with the int
  return ival;
}
  
float getFP(int val) {
// Using the defined representation, compute the floating point
//    value
// For denormalized values (including 0), simply return 0.
// For special values, return -1;
//

 int exp = val&MASK;//the MASK is designed to extract the exp field from the int version fo the float
 exp >>= 9;//we then then shift the bits to the right end to get the original number 

 if(exp == MASK){//if the exp field has all ones like the mask, then the number is a special case number

	return -1;
  }

  if(exp == 0){//if the exp field has all 0's then the number is a denormalized number

	return 0;
  }

  int E = 0;
  E = exp-15;// formula to calculate exp is exp = E+bias => E = exp-bias

  int frac = val&MASK1;//MASK1 is designed to extract the last 9 bits of the number which is the frac field

  float M = 0.0;
  M = (frac/(pow(2,9))) + 1;//since we mutiplied the frac field by 2^9 last time, we are now dividing the frac field by 2^9 so that we get the small number that we had, and since M for normalized numbers are between [1,2), i.e. 1.something, we are adding 1 to the float
  
  float OGval = M*(pow(2,E));//to get back the old value, we apply the formula (-1)^s * M * 2^E  
  return OGval;
}

int multVals(int source1, int source2) {
// You must implement this by using the algorithm
//    described in class:
//    Add the exponents:  E = E1+E2 
//    multiply the fractional values: M = M1*M2
//    if M too large, divide it by 2 and increment E
//    save the result
// Be sure to check for overflow - return -1 in this case
// Be sure to check for underflow - return 0 in this case

 int exp1 = source1&MASK;//extarcts the exp field of the first number
 exp1 >>= 9;//same process as above
 
 if(exp1 > 30){//checking for overflow and underflow

	return -1;
 }

 if(exp1 < 1){

	return 0;
 }

 int exp2 = source2&MASK;//gets the exp field of the second number
 exp2 >>= 9;//same process as above

 if(exp2 > 30){//checks for overflow and underflow

	return -1;
 }

 if(exp2 < 1){

	return 0;
 }
 
 int E1 = exp1-15;//applying the formula to get E for the first number

 int E2 = exp2-15;//getting E for the second number using the same formula

 //when multiplying 2 floats, we take the E's, add them together and then we multiply the M's of each number, and generate a new number
 //basically doing, -1^s * (M1*M2) * 2^(E1+E2)
 int E = E1+E2;//adding exponents

 int frac1 = source1&MASK1;//extracting the frac field for the first number
 float M1 = (frac1/(pow(2,9)))+1;//calculating M using the same formula as before

 int frac2 = source2&MASK1;//same process for the second number
 float M2 = (frac2/(pow(2,9)))+1;

 float M = M1*M2;//multiplying Mantisa's
 
 if(M>1){
   if(!(M>=1 && M<2)){//after multiplying the number, if they are not in the range [1,2), then we divide it 

	while(!(M>=1 && M<2)){

		M /= 2.0;
		E++;
	}
   }  
 }
 
  if(M<1){

    if(!(M>=1 && M<2)){

	while(!(M>=1 && M<2)){

	  M *= 2.0;
	  E--;
	}
    }
  }
 float product =  M*(pow(2,E));//applying the formula -1*s * M * 2^E
 
 int ipro = computeFP(product);//using the first function to get the int representation of the number
 return ipro;
}

int addVals(int source1, int source2) {
// Do this function last - it is the most difficult!
// You must implement this as described in class:
//     If needed, adjust one of the two number so that 
//        they have the same exponent E
//     Add the two fractional parts:  F1' + F2 = F
//              (assumes F1' is the adjusted F1)
//     Adjust the sum F and E so that F is in the correct range
//     
// As described in the handout, you only need to implement this for
//    positive, normalized numbers
// Also, return -1 if the sum overflows
 
  int exp1 = source1&MASK;//same process as above, getting exp and E's for both the values
  exp1 >>= 9;

  if(exp1 > 30){

	return -1;
  }

  if(exp1 < 1){

	return 0;
  } 

  int exp2 = source2&MASK;
  exp2 >>= 9;

  if(exp1 > 30){

	return -1;
  }

  if(exp2 < 1){

	return 0;
  }

  int E1 = exp1-15;
  int E2 = exp2-15;

  int frac1 = source1&MASK1;//same process as above, calculating the fracs from both the numbers
  float M1 = (frac1/pow(2,9))+1;

  int frac2 = source2&MASK1;
  float M2 = (frac2/pow(2,9))+1;

  if(E1 > E2){//for addition, the exponent fields have to be equal, so if one is greater, we balance them out and in the process, we divide the mantisa

	while(!(E1==E2)){

		M2/=2.0;
		E2++;
	}
  }

  if(E1 < E2){

	while(!(E1==E2)){

		M1/=2.0;
		E1++;
	}
  }

  float M = M1+M2;//adding both the Mantisa's

  float sum = M*(pow(2,E1));//applying the formula

  int isum = computeFP(sum);//getting the int representation of the sum

  return isum;
}
