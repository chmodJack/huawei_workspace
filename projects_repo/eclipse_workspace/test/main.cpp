#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
  /*char buffer[30];
  struct timeval tv;

  time_t curtime;



  gettimeofday(&tv, NULL);
  curtime=tv.tv_sec;

  strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
  printf("%s%ld\n",buffer,tv.tv_usec);

  return 0;*/

	clock_t begin, end;
	double time_spent;
	begin = clock();

	for (int i=0; i<100; i++) {
		printf("haha\r\n");
	}

	end = clock();
	time_spent = (double) (end-begin)/CLOCKS_PER_SEC;

	printf("time_spent:%.8f\r\n", time_spent);

	return 0;
}
