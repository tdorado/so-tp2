#include <constantPrints.h>

static const char weekDays[7][10] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};
static const char months[12][11] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

void printTime()
{
    int times[7];
    getAllTimes(times);
    printf("Hora: %d:%d:%d.\n", times[2], times[1], times[0]);
}

void printDate()
{
    int times[7];
    getAllTimes(times);
    printf("Fecha: %s %d de %s de %d.\n", weekDays[times[3] - 1], times[4], months[times[5] - 1], times[6]);
}

void printTimeUTC()
{
    printf("Zona horaria: UTC %d.\n", getTimeUTC());
}

void help()
{
    printf("\n");
    printf("Welcome to Titanium OS!\n");
    printf("\n");
    printf("- For more information, write info\n");
    printf("- To clean the console, write clear\n");
    printf("- If you want to display time on console, write displayTime -console\n");
    printf("- If you want to display the digital time, write displayTime -digital\n");
    printf("- If you want to change the timezone, write setTimeZone NUMBER\n");
    printf("- If you want to change the backgroundColor, write changeBackGroundColor COLOR\n");
    printf("- If you want to change the textColor, write changeTextColor COLOR\n");
    printf("Where COLOR could be default, red, orange, yellow, green or blue\n");
    printf("- If you want the terminal to reply a message, write echo MESSAGE\n");
    printf("- If you want to play Blobwars (you should), write blobWars\n");
    printf("- Write exceptionZero for trying our divZero exception catch\n");
    printf("- Write exceptionOpCode for trying our opCode exception catch\n");
    printf("- If you want to exit, write exit\n");
    printf("\n");
}

void info()
{
    printf("\n");
    printf("This is a rudimentary OS, created by 2 ITBA students, passionate for what they do and what they study.\n");
    printf("In this OS, you will be able to:\n");
    printf("- See what time is it.\n");
    printf("- See what time is it on a digital watch.\n");
    printf("- Play a game call BlobWars -> It is a strategy game where you can play against a IA or against a friend, where you got a squared matrix and the mission is to \"convert\" as much pieces of the enemy you can.\n");
    printf("- You can test the exceptions\n");
    printf("- Eventually, you can inform us of any bug you'll find and we'll be happy to solve them (hope they're few). You can send an email to : tdorado@itba.edu.ar or tdallas@itba.edu.ar\n");
    printf("\nTHANKS FOR USING OUR OS! :)\n");
    printf("\n");
}

void displayTime()
{
    printf("Epoch Time: %d.\n", time(NULL));
    printTime();
    printDate();
    printTimeUTC();
}