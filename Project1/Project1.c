#include <stdio.h>
#include <math.h>
/* 
Group number: 3
Team members: 
Nick Benevento (Questions 4-6, 11, 14)
Zach Coveno (Questions 7-9, 12, 15)
Genchen Li (Questions 1-3, 10. 13)
Team collaborated, explained and helped each other with all th problems.

*/

/* PROJECT 1 */

/* In this assignment you will examine a program consisting of a number of functions - labelled ques1.c to ques15.c -- each of which tests your knowledge of bitwise C operations. Your goal is to determine what each function does and if there
is a simpler way to implement the function. There are two parts to the requirements.  */

/* Assume 32 bit integers */

/* PART A: You need to determine what each function is doing .
You have to determine what each function is doing WITHOUT running the code. You must provide an explanation for your answer - you will get no credit if you correctly identify the function but provide  an incorrect or insufficient/incomplete explanation.
 Once you determine the function, you should run the code to check your answer.
To run the code to test your function, you will need to insert appropriate C code to call the function and print out the result.
 Once you are done showing what each function does, you should then answer part b. */

/* PART B: For each function, can you write a simpler equivalent function in C (using any C operators) that produces the same result.
 By simpler, we are interested in fewer operations in C but also more readable code. */


/*
 As an example, consider the answers to function ques0 - which you worked out in class.

 HANDIN Instructions: You must write out your answers to Part A in a report to be submitted (as a tarfile or zipfile, along with your code) - in addition to the report, you can include comments in your code that may provide a secondary explanation (in case
 your report has errors). You MUST provide an explanation for what the function does and justify your answer. Simply providing an example is NOT an explanation - so don't expect any credit if
 your answers are simply a collection of examples or code comments. You are welcome to provide formal proofs where appropriate, but use your judgement on whether to pursue a formal proof or not. It is preferable to write your answers as comments in the code - but do not feel pressured to do so.
 For part B, write out your code in this file with the answer (a simpler code if it exists) after each of the assigned functions in a manner similar to the example shown in ques0.c (i.e., the answer to function ques7.c should be named ans7.c).

 IMPORTANT: The code you submit must compile correctly (using gcc). If the code you submit does not compile without errors, you will get a zero for Part B of this project.

 GRADING: Each question has a specific number of points (i.e., not all questions have the same level of difficulty). Your grade will be based
 on the correctness of your answer, expressing the answer as a logical function where possible, and the efficiency and readability of your rewritten code.

 Collaboration: You can discuss the project with your teammate. You CANNOT refer to online sources to get help; you can refer to the C syntax documentations. Your report must include names of both members of the team -- failure to do so would imply you are claiming you worked on your own, and may constitute a violation of academic integrity policies.*/

/* EXAMPLE
 this function is similar to CallMeLast in the inclass exercises */
/*  function ques0 returns 1 if x=y and returns 0 if x is not equal to y */
/* the function ans0 does the same in two C statements */

int ques0(int x, int y){
    int temp;
    temp = ~y;
    temp = temp +1;
    temp = temp + x;
    return !(temp);
}

/* Answer to PART B: */
int ans0(int x, int y){
    int z=0;
    if (x==y)  z= 0;
    else z= 1;

    return z;
}

/* QUESTIONS */

/* question 1 */
int ques1(int x)
{
    int z;
    int y  = ~x+1;//int y equals to the two's complement plus 1
    z= (~(y|x));//int z equals to the complement of (y OR x).
		//In OR operation, when x is 1 and y is 0, it outputs 1. When x is 0 and y is 1, it outputs 1. 
		//When x and y both be 0, it outputs 0. When x and y both be 1, it outputs 1.
    z = (z >> 31);//z equals to the first bit of z after right shift 31 times.
		  //z is the first bit of z after right shift 31 times.
		  //If the first bit is 1, which means that it is a negative number. If the first bit is 0, whcih means it is a positive number.

    return (z & 1);//returns z AND 1.
		   //In AND operation, only when x and y both be 1, it outputs 1. Else, it outputs 0.
		   //For this function, if x is 0, the function output is 1. Else, the function output is 0.


}

int ans1(int x)
{
    return (!x);// return NOT x.
}

/* question 2 */
int ques2(int x) {
    int mask = x>>31;// int mask is the first bit after int x right shift 31 times.
		     // If mask is 0, which means that x is a positive number. If mask is 1, which means that x is a negative number.
    int y= (x ^ mask);//int y equals to x XOR mask.
		      //In XOR operation, if x is 1 and y is 0, it outputs 1. If x is 0 and y is 1, it outputs 1. Else it outputs 0.
    int z = (~mask + 1);//int z equals to the two's complement if mask adds 1.

    return (y+z);
}

int ans2(int x)
{
   return abs(x);//returns the absolute value of x.
}

/* question 3 */
int ques3(int x)
{
    int y = !x;//int y equals to NOT x.
	       //in NOT operation, when x is 0, results 1. When x is 1, results 0.
    int z = x >> 31;//int z is the first bit of x after x right shift 31 times.
		    //If z is 0, x is a positive number. If z is 1, x is a negative number.
    z = z | y;//z equals to z OR y.
	      //For this function, if x is a positive number, the output is 1. If x is a negative number, the output is 0.

    return !z;//returns NOT z.
}

int ans3(int x)
{
  if(x<0)
    return 0;
  else
    return !!x;//returns NOT NOT x.
}


/* question 4 */
/* Assume 0 <= n <= 32 */
int ques4(int n) {
    /*  The ! operator is boolean not, so it switches between true and false (where false = 0, and true is any other value.
    Thus, if n is 0, then it will stay 0, and if it is any other number, n will be 1. So, x will be 0 if n is 0, otherwise
    x will become -2147483648*/
    int x = (!!n) << 31;
    /* If x is 0, it stays 0. Otherwise, x is -1 */
    x = x >> 31;
    /* y is initialized as the largest negative number, -2147483648 */
    int y = (1 << 31);
    /* y is shifted to the right by n-1, since the complement of 0 is -1 */
    y = y >> (n + (~0));

    /* if x is 0, this will always return 0. Otherwise, it will return -2147483648 divided by 2^(n-1) */
    return x & y;
}

int ans4(int n) {
    /* return 0 if n = 0 */
    if(n == 0) {
        return 0;
    }
    /* otherwise, return y divided by 2^(n-1) */
    else {
        int y = -2147483648;
        y = y/pow(2, (n-1));
        return y;
    }
}

/* question 5 */

int ques5(int x) {
    /* Shifts the number 31 bits to the left. If the number had a 1 in the right most bit, this turns the number
    into the greatest negative number, -2147483648. Otherwise, the number becomes 0 */
    int result = (x<<31);
    /* Shifting the number back to the right pads the left-most 31 bits with the sign bit, either 0 or 1.
    This means that if the number was originally even, it becomes 0. Otherwise, the number becomes -1. */
    result = (result >> 31);

    /* Essentially the same as saying, if the original number was even, return 0. Returns -1 if the number is odd */
    return result;
}

/* Returns 0 if x is even, and -1 if x is odd */
int ans5(int x) {
    if(x%2 == 0)
        return 0;
    else
        return -1;
}

/* question 6 */

int ques6(void) {
    /* the number 0101 0101 in binary, or 85 in decimal */
    int byte = 0x55;
    /* shifts the number one byte to the left, and ors it with the original. So,it basically
    doubles the binary sting from 0101 0101 to 0101 0101 0101 0101 */
    int word = byte | byte<<8;
    /* This return statement does something very similar to the line above it, but shifts the int over 2 bytes and
    then ors it. So, the final binary string that is returned is altenating 0101s. This is equivalent to 
    0x55555555 in hex, or 1,453,655,765 in decimal. It could be worth noting that the string 0101 0101 is equivalent 
    to the character U. */
    return word | word<<16;
}

/* returns a string of alternating 0's and 1's, which is equal to the number 1,453,655,765 */
int ans6(void) {
    return 0x55555555;
}


/* question 7 */
int ques7(int x) {
    /* Although this line looks simple, it is actually quite complicated. Basically, it 
    takes the right-most bit of the binary string x and returns what that number is. So, if the number is 
    8 (1000), the function returns 8. If the number is 6 (0110), the function returns 2. */
    return x & (~x+1);
}

/* Although this code might not have less operations, it is much more readable and easier to 
see what the function does. */
int ans7(int x) // x & -x
{
  int i=0;
  /* loops through the bits of x */
  while(i<32)
  {
    int temp =1;
    temp= temp & x; /* temp will only be 1 if there is a 1 in the right-most bit of x */
    /* if temp is 1, then the function returns 2 to the power of i, which is how many times the 
    binary string was shifted right. This gets the original value of what the 1 in the string was.
    For example, if x was 0110, the function would return 2^1, since it would take 1 shift to get the
    1 in the right-most bit */
    if(temp==1)
      return pow(2,i);
    x= x>>1; /* shifts the bits of x over to the right by 1 */
    i++;
  }
}

/* question 8 */
int ques8(int x) { //SIGN CHECK
    int y = x >> 31; //if x is negative y is all 1's(-1)... if x is positive y is all 0's (0)
    int z = !!x; //If x is zero z is zero... if x is anything else z is 1

    return y | z; //If x is negative this returns -1... if x is 0 this returns 0.... if x is positive this returns 1
}

/* This function is essentially a sign-check, where it returns 0 if the number is positive,
-1 if it is negative, and 0 if it is 0 */
int ans8(int x)
{
  if(x>0)
    return 1;
  else if(x==0)
    return 0;
  else
    return -1;
}

/* question 9 */

int ques9(int x, int n, int c) {
    int n8 = n << 3; //multiplies n by 8 
    int mask = 0xff << n8; //multiplies 255 (decimal for 0xff) by 2 raised to n8
    int cshift = c << n8; //multiplies c by 2 raised to n8
    int z= (x & ~mask); 

    return (z  | cshift);
}

int ans9(int x, int n, int c)
  {
    if(n>3 || n<0)
    {
      int nope =NULL;
      return nope; //you cant shift more than 31 bits and you can't shift negative ints. These are undefined operations in c code
    }
    else
    {
     int n8= n*8; //easier to read than <<3, same result
     int mask = 255* pow(2, n8); // 2 raised to the power n8 is the same as shifting n8 bits
     int cshift = c* pow(2, n8);
     int z= (x & ~mask); //Due to the variablility of mask (depending on n8) this stays the same
    
    return (z| cshift); //Due to the variability on both of these ints this line also remains the same
    }
  }



/* question 10 */

int ques10(int x) {
	int y = !!x; /* if x was 0, it stays 0. Otherwise, x becomes 1 */

    /* using the ! operator with anything except 0 results in 0. So, if x is anything
    except 0, z becomes 0 */
	int z = (!(x+x)); 
    /* if x was 0, then y wil become 0, so this wil return 0. If x wasn't 0, then z becomes 0, 
    so this will still return 0. It always returns 0 regardless of the value of x */
    return y & z; 
}

/* the function always returns 0 */
int ans10(int x)
{
  return 0;
}

/* question 11 */

int ques11(int x, int y) {
    /* If x was negative, a becomes -1. Otherwise a becomes 0 */
	int a = x >> 31;
    /* If y was negative, b becomes -1. Otherwise b becomes 0 */
	int b = y >> 31;

    /* This can be broken up into two expressions separated by the | sign. The first part,
    (!a & b), switches a to 0 if it was previously -1, or switches it to 1 if it was 0. This 
    is then anded with b. The second part of the equation is a little more complicated, but starts with 
    !(a ^ b). This uses the exclusive or operator between a and b, which will only become -1 if a and b
    are different, and 0 otherwise. This answer is then flipped by the ! operator. next, this
    is anded with the other expression, which adds y to the complement of x, and shifts that answer
    31 spaces to the right. So, this will become either 0 or -1, depending on the values of x and y.
    The first and second part of the expression is or'd together, and then the final answer is flipped 
    due to the ! operator in front of the whole expression.
    A much easier way to write the same line is to say: return (x && !y) , which was 
    found using a truth table, since there can only be 2 values for a and b */
    return !((!a & b) | (!(a ^ b) & (y+~x)>>31));
}

/* there can only be 4 different combinations for x and y after they are passed in,
so  this function is a much simpler way to output the same answers as question 11 */
int ans11(int x, int y) {
    /* if x was greater than or equal to 0, it becomes 0. */
    if(x >= 0)
        x = 0;
    else 
        x = -1; /* otherwise, x is -1 */
    /* if y was greater than or equal to 0, it becomes 0 */
    if(y >= 0)
        y = 0;
    else
        y = -1; /* otherwise, it becomes -1 */

    /* returns either true or false depending on the values of x and y, according
    to the truth table that was constructed for question 11 (truth table is in the 
    explanation document) */
    return (x && !y);
}


/* question 12 */
int ques12(int x, int m, int n) {
    int a = ~m+1;  //2's compliment so -a
    int b = ~x +1; //2's compliment so -x
    a = x + a;  //-m+x
    b = b + n;  //-x+n

    return !((a|b) >> 31); //If either are negative this is all 1's (-1) which becomes 0 after boolean not... if both are 0 this is positive or 0 this is all 0's (0) which becomes 1 after boolean not
}


int ans12(int x, int m, int n)
{
  if(m>x || x>n) //This is equivalent to saying if a or b are negative
    return 0;
  else
    return 1;
}

/* question 13 */

int ques13(int x) {

    int mask1, mask2, mask4, mask8, mask16;//The function uses 5 masks to check for number of 1 in the binary string.

    mask2 = 0x33 + (0x33 << 8);//mask2 repeats "0011" eight times
    mask2 += mask2 << 16;
    mask1 = mask2 ^ (mask2 << 1);//mask1 repeats "01" sixteen times.
    mask4 = 0x0F + (0x0F << 8);//mask 4 repeats "0000 1111" four times.
    mask4 += mask4 << 16;
    mask8 = 0xFF + (0xFF << 16);//mask 8 repeats "0000 0000 1111 1111" two times.
    mask16 = 0xFF + (0xFF << 8);//mask 16 repeats "0000 0000 0000 0000 1111 1111 1111 1111" one time.

    x = (x & mask1) + ((x >> 1) & mask1);
    x = (x & mask2) + ((x >> 2) & mask2);
    x = (x & mask4) + ((x >> 4) & mask4);
    x = (x & mask8) + ((x >> 8) & mask8);
    x = (x & mask16) + ((x >> 16) & mask16);

    return x;//For this function, it returns the number of 1 in the binary string. 
}

/* uses a for loop to count the number of 1's in the binary string of x and returns that number */
int ans13(int x)
{
  int count=0; /* counter variable to count the number of 1's. */
  int i=0;
  /* loops through all the bits of x */
  while(i<32)
  {
    int temp =1; /* temp variable with value of 1 */
    temp= temp & x; /* bitwise and operator with x and temp.Temp will only become 1 if x has a 1 in the right-most bit */
    /* if temp was 1, then add 1 to the counter since there was a 1 in x */
    if(temp==1)
      count++;
    x= x>>1; /* shift x over by 1 to get the next bit value */
    i++;
  }
  /* returns how many 1's are in the binary string of x */
  return count;
}

/* question 14 
If the number in binary has an even number of 1's, the function returns 0. If the number in binary
has an odd amount of 1s, the function returns 1 */
int ques14(int x) {
    int result = 0;
    int i;

    /* loop through 32 times */
    for (i = 0; i < 32; i++)
        /* uses the exclusive or function to compare result with the value of x after it has been shifted
        over i times. This just compares the right most bit of x, and if it is different than the right-most
        bit of result, then result maintains the value of 1. This process is then repeated from the for loop, 
        but will only change the value of return whenever there is a 1 in x. So, this loop will only
        put out a different answer for return depending on how many 1's there are in x (when there is a 1,
        it flips the value of return). */
        result ^=  (x >> i) & 0x1;
    /* if there are an even amount of 1s in the binary string of x, then this returns 0. If the number of 
    1's in x is odd, this returns 1 */
    return result;
}

/* uses a for loop to count the amount of 1's in the binary string of i. Returns
1 if there are an odd amount, and 0 if there are an even amount */
int ans14(int x) {
    int counter = 0; /* counter to keep track of how many 1's are in the binary string x */
    /* loops through all the bits of i */
    for(int i = 0; i < 32; i++) {
        int temp = 1; /* creates a temp variable with a value of 1 */
        /* uses bitwise and operator with temp and x. So, temp will only become 1 if the right-most bit
        of x is also a 1 */
        temp = temp & x;  
        /* If temp is 1, meaning there was a 1 in x, increase the counter */
        if(temp == 1) {
            counter++;
        }
        x = x >> 1; /* shifts over x by 1, so whatever was in the right-most bit slot is replaced by the value to the left of it */
    }
    /* if the counter was an even number, returns 0. If counter was odd, return 1 */
    return (counter %2 != 0);
}

/* question 15 */

int ques15(int x, int n) {
    /* for ques15 only, assume n is not a negative number  */

    int temp = (1 << n); //2 raised to the n
    int z = temp + ~0; //temp-1 which is all 0's with n 1's at the end

    return (z & x);  // trucates x after the nth bit
}

/* returns the first n bits of x */
int ans15(int x, int n)
{
  int count=0;
  int i=0;
  while(i<n)  //counts the first n bits of x 
  {
    int temp =1;
    temp= temp & x;
    /* if there was a 1 in the bit of x, add that value to count */
    if(temp==1)
    {
      count =count + pow(2, i); /* have to add 2 raised to the power of i since each bit increases value by 2x */
    }
   
    x= x>>1; /* shift the bits of x over by 1 */
    i++;
  }
  return count; 
  
}



int main(){
    int a, b, c;

    int t1;

 	printf("Enter first number, an integer stored into variable A preferably between 1 and 20: ");
 	scanf ("%d",&a);
	printf("\n");
 	printf("Enter second number, an integer stored into variable B preferably between 1 and 20: ");
 	scanf ("%d", &b);
	printf("\n");
    printf("Enter third number, an integer stored into variable C preferably between 1 and 20: ");
 	scanf ("%d", &c);
	printf("\n");


	printf("you entered a = %d b = %d c = %d \n", a,b,c);


	//t1=ques0(a,b);
	//printf("output of ques0 is t1 = %d  \n", t1);
    /* To test/run the functions, you will need to input numbers and then call each of the functions, and print the return value */
    
    t1 = ques1(a);
    printf("\nOutput of ques1 is = %d\n", t1);
    t1 = ans1(a);
    printf("Output of ans1 is = %d\n\n", t1);

    t1 = ques2(a);
    printf("Output of ques2 is = %d\n", t1);
    t1 = ans2(a);
    printf("Output of ans2 is = %d\n\n", t1);

    t1 = ques3(a);
    printf("Output of ques3 is = %d\n", t1);
    t1 = ans3(a);
    printf("Output of ans3 is = %d\n\n", t1);

    t1 = ques4(a);
    printf("Output of ques4 is = %d\n", t1);
    t1 = ans4(a);
    printf("Output of ans4 is = %d\n\n", t1);

    t1 = ques5(a);
    printf("Output of ques5 is = %d\n", t1);
    t1 = ans5(a);
    printf("Output of ans5 is = %d\n\n", t1);

    t1 = ques6();
    printf("Output of ques6 is = %d\n", t1);
    t1 = ans6();
    printf("Output of ans6 is = %d\n\n", t1);

    t1 = ques7(a);
    printf("Output of ques7 is = %d\n", t1);
    t1 = ans7(a);
    printf("Output of ans7 is = %d\n\n", t1);

    t1 = ques8(a);
    printf("Output of ques8 is = %d\n", t1);
    t1 = ans8(a);
    printf("Output of ans8 is = %d\n\n", t1);

    t1 = ques9(a, b, c);
    printf("Output of ques9 is = %d\n\n", t1);
    //t1 = ans9(a, b, c);
    //printf("Output of ans9 is = %d\n\n", t1);

    t1= ques10(a);
    printf("Output of ques10 is = %d\n", t1);
    t1 = ans10(a);
    printf("Output of ans10 is = %d\n\n", t1);

    t1 = ques11(a, b);
    printf("Output of ques11 is = %d\n", t1);
    t1 = ans11(a, b);
    printf("Output of ans11 is = %d\n\n", t1);

    t1 = ques12(a, b, c);
    printf("Output of ques12 is = %d\n", t1);
    t1 = ans12(a, b, c);
    printf("Output of ans12 is = %d\n\n", t1);

    t1 = ques13(a);
    printf("Output of ans13 is = %d\n", t1);
    t1 = ans13(a);
    printf("Output of ans13 is = %d\n\n", t1);

    t1 = ques14(a);
    printf("Output of ques14 is = %d\n", t1);
    t1 = ans14(a);
    printf("Output of ans14 is = %d\n\n", t1);

    t1 = ques15(a, b);
    printf("Output of ans15 is = %d\n", t1);
    t1 = ans15(a, b);
    printf("Output of ans15 is = %d\n", t1);

	return 0;
}

