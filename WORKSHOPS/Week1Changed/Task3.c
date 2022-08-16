#include <stdio.h>
void main(int argc, char *argv[])
{
	for(int n =1; n <100; n++)
	{
		if(n % 2 == 0)
		{
			if(n % 3 == 0 )
			{
			  if(n % 5 == 0 )
			  {				
	             printf("BishBashBosh\n");
			   }else
			   {
			   	      printf("BishBash\n");      
			   }	
			}
			else if(n % 5 == 0 ){
				
             printf("BishBosh\n");
             
			}
			else{
			 printf("Bish\n");
			}
			
		}
		else if(n % 3 == 0 )
		{
			if(n % 5 == 0 )
			{
				
             printf("BashBosh\n");
             
			}else{
             printf("Bash\n");
			}
             
		}
		else if(n % 5 == 0 )
		{
				
             printf("Bosh\n");
             
		}
		else{
			printf("%d\n", n);
		}
	}
}
