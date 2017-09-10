#include<stdio.h>
#include<math.h>

int main()
{
    int digsum;
    long long int n,sum=0,temp;
	scanf("%lld",&n);
	temp = n;
	
	while(temp>0)
	{
	    sum = sum + temp%10;
	    temp = temp/100;
	}
	
	temp = n/10;
	while(temp>0)
	{
	    digsum = (temp%10)*2;
	    digsum = digsum%10 + (digsum/10);
		sum = sum + digsum;
		temp = temp/100;
	}
	
	if(sum%10!=0)
		printf("INVALID\n");
	else
	{
		float a = log10(n);
		a = a - ((int) a)*1.0;
		a = pow(10,a)*10;
		a = (int) a;
		printf("%f\n",a);
		if(a==34.0 || a==37.0)
			printf("AMEX\n");
		else if(a==51 || a==52 || a==53 || a==54 || a==55)
			printf("MASTERCARD\n");
		else 
			printf("VISA\n");
	}
return 0;	
}