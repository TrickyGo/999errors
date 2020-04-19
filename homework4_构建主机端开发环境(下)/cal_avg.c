#include<stdio.h>
int main()

{

	int a=0,b=0,c=0,average=0;

	printf("Please Enter 3 numbers to calculate average:\n");
	scanf("%d %d %d",&a, &b, &c);

	average = (a+b+c)/3;

	printf("The average is: %d\n", average);

	return 0;

}
