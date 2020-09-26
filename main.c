#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.c"

int main()
{
    FILE *fichier;
    fichier=fopen("Historic.txt","w");

    int time=0, x;

    Liste_blacklist ListeB = NULL;

    avion Avion;
    Compagnie C;

    Liste_Compagnie ListeC = NULL;

    Liste_Avion Liste_landing = NULL;

    Liste_Avion Liste_Takeoff = NULL;
    Queue Liste_Emergency;

    Liste_Emergency.first = NULL;
    Liste_Emergency.last = NULL;

    Queue Queue_Takeoff;

    Queue_Takeoff.first = NULL;
    Queue_Takeoff.last = NULL;

    ReadTestFile(&Liste_landing, &Liste_Emergency, &Liste_Takeoff, &ListeC, &Avion);

    DisplayList(&(Liste_Emergency.first));

    while(time < 1440)
    {
        printf("TIME : %d\n\n", time);

        if(time%5 == 0)
        {
            x = 0;
            AddAvion(&Avion, &C, &Liste_landing, &Liste_Takeoff, &Liste_Emergency, &Queue_Takeoff, &ListeC, &ListeB, time, x);
            x = random();
            //printf("RANDOM OPERATION :\n");
           // AddAvion(&Avion, &C, &Liste_landing, &Liste_Takeoff, &Liste_Emergency, &Queue_Takeoff, &ListeC, &ListeB, time, x);

        }

        CheckBlacklist(&ListeC, ListeB, &Liste_Emergency, &Liste_landing, &Liste_Takeoff);

        Liste_Takeoff = AddTakeoffQueue(Liste_Takeoff, &Queue_Takeoff, time, &ListeC);

        if(Liste_landing != NULL)
            CheckEmergencyFirst(&Liste_landing, &Liste_Emergency, &ListeC);

        AvionToHistoric(&Liste_Emergency, &Queue_Takeoff, &Liste_landing, &ListeC, ListeB);

        ChangeCarburant(&Liste_landing);
        ChangeCarburant(&(Liste_Emergency.first));

        time++;
    }
    fclose(fichier);
    return 0;
}

