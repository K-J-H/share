#include <stdio.h>
#include <time.h>
#include <string.h>
int getTimeNow(char *nowTime)
{
	char acYear[5] = {0};
	char acMonth[5] = {0};
	char acDay[5] = {0};
	char acHour[5] = {0};
	char acMin[5] = {0};
	char acSec[5] = {0};

	time_t now;
    struct tm* timenow;
    
    time(&now);
    timenow = localtime(&now);

    strftime(acYear,sizeof(acYear),"%Y",timenow);
    strftime(acMonth,sizeof(acMonth),"%m",timenow);
    strftime(acDay,sizeof(acDay),"%d",timenow);
    strftime(acHour,sizeof(acHour),"%H",timenow);
    strftime(acMin,sizeof(acMin),"%M",timenow);
    strftime(acSec,sizeof(acSec),"%S",timenow);

	strncat(nowTime, acYear, 4);
	strncat(nowTime, acMonth, 2);
	strncat(nowTime, acDay, 2);
	strncat(nowTime, acHour, 2);
	strncat(nowTime, acMin, 2);
	strncat(nowTime, acSec, 2);

	return 0;
}

int main(int argc, char *argv[])
{
    char nowTime[32] = {0};
    
    getTimeNow(nowTime);
    printf("nowTime is %s.\n", nowTime);
    
    return 0;
}