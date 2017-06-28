#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    time_t curtime;
    struct tm *loctime;
    unsigned int year, week, wday, day,wkd1;
	if (argc == 5 && !strcmp("-d", argv[1])) {
		year = atoi(argv[2]) - 2000;
		day  = atoi(argv[3]);
		wday = atoi(argv[4]);
		printf("%d %d %d\n", year, day, wday);
	} else {
		curtime = time(NULL);
		loctime = localtime(&curtime);
		year = loctime->tm_year - 100;
		day  = loctime->tm_yday;
		wday = loctime->tm_wday;
	}
    wkd1 = 7-((wday+7-(day%7))%7);
	if (day > wkd1)
		week = ((day-wkd1)/7)+(wkd1?1:0)+((day%7)?1:0);
	else
		week = 1;
	//printf("%d %d %d %d %d %d\n", year, week, wday, day, wkd1, week);
    if(argc>1 && !strcmp("-w",argv[1])) printf("0%d\n",wday);
    else printf("%2d%s%d\n",year,(week<10?"0":""),week);
    return 0;
}
