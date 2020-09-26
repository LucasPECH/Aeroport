#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
void user(avion* Avion, int x, Liste_Compagnie ListeC)
{
    int exist = 0;
    char Cnom[10];
    Compagnie* C;

    printf("Enter the airline of the plane\n");
    scanf("%s", Cnom);

    printf("Enter the name of a plane\n");
    scanf("%s", Avion->identifiant);

    if(x == 1)
    {
        printf("Enter the level of fuel of the plane\n");
        scanf("%d", &(Avion->carburant));
        printf("Enter its fuel consumption per minute\n");
        scanf("%d", &(Avion->consommation));
        Avion->heure_decollage[0] = 'B';
    }

    if(x == 2)
    {
        printf("Enter the hour of the take off\n");
        scanf("%s", Avion->heure_decollage);
        Avion->carburant = -9999;
    }

    while(ListeC != NULL && exist == 0)
    {
        if(strcmp(ListeC->compagnie->nom, Cnom)==0)
            exist = 1;
        else
            ListeC = ListeC->suivant;
    }
    if(exist == 0)
    {
        C = (Compagnie*) malloc(sizeof(Compagnie));
        strcpy(C->nom,Cnom);
        C->ListeAvion = NULL;
        Avion->compagnie = C;
    }
    else
        Avion->compagnie = ListeC->compagnie;
}

void AddAvion(avion* Avion, Compagnie* C, Liste_Avion* Liste_landing, Liste_Avion* Liste_Takeoff, Queue* Liste_Emergency, Queue* Queue_Takeoff, Liste_Compagnie* ListeC, Liste_blacklist* ListeB, int time, int X)
{
    int x;
    char name[7];
    char compagniename[10];
    Cellule_avion* save;
    if(X==0)
    {
        printf("Do you want to add a plane to landing at takeoff or in an emergency situation or remove a plane at take off?\n");
        printf("You can also decide to display the list of your choice\n");
        printf("Enter: \n1) For landing \n2) For takeoff \n3) For emergency \n4) For remove take off\n");
        printf("5) To display the list of landing \n6) To display list of take off\n7) To blacklist a company\n8) To see statue of company airplanes\n");
        printf("9) To display the historic\n");
        scanf("%d", &x);
    }

    if(x==1 || X==1)
    {
        user(Avion, 1, *ListeC);

        *Liste_landing = AddLandingList(*Liste_landing, *Avion, ListeC);
    }

    if(x==2 || X==2)
    {
        user(Avion, 2, *ListeC);
        *Liste_Takeoff = AddTakeoffList(*Liste_Takeoff, *Avion, ListeC);
    }

    if(x==3 || X==3)
    {
        printf("Give the name of the plane\n");
        scanf("%s", name);
        AddEmergency(Liste_landing, name, Liste_Emergency, ListeC);
    }

    if(x==4 || X==4)
    {
        printf("Give the name of the plane\n");
        scanf("%s", name);
        save = RemoveName(Liste_Takeoff, name, ListeC);
        if(save == NULL)
        {
            printf("This plane doesn't exist\n");
        }
        free(save);

    }

    if(x==5)
       {
        DisplayList(Liste_landing);
        DisplayList((&Liste_Emergency->first));
       }

    if(x==6)
    {
        DisplayList(Liste_Takeoff);
        DisplayList((&Queue_Takeoff->first));
    }

    if(x==7 || X==5)
    {
        printf("Give a company to blacklist\n");
        scanf("%s", compagniename);
        *ListeB = ListeBlacklisted(compagniename, ListeB);
    }
    if(x==8)
    {
        printf("Give the name of the company\n");
        scanf("%s", compagniename);
        LorT(compagniename, *ListeC, time);
    }
    if(x==9)
        printhistoric();
}

Liste_Avion AddLandingList(Liste_Avion ListeA, avion A, Liste_Compagnie* ListeC)
{
    Cellule_avion* New  = (Cellule_avion*) malloc(sizeof(Cellule_avion));

    Cellule_avion* pred = NULL;

    Cellule_avion* curr;

    int rides, rides2;

    curr = ListeA;

    New->Avion = A;


    AvionCompagnieArray(New, ListeC);

    if(ListeA == NULL)
    {
        New->suivant_attente = ListeA;
        return New;
    }
    rides = (New->Avion.carburant)/(New->Avion.consommation);
    rides2 = (curr->Avion.carburant)/(curr->Avion.consommation);

    while(curr->suivant_attente != NULL && rides>rides2)
    {
            pred = curr;
            curr = curr->suivant_attente;
            rides2 = (curr->Avion.carburant)/(curr->Avion.consommation);
    }

    if(rides2>=rides && pred == NULL)
    {
        New->suivant_attente = ListeA;
        return New;
    }
    else if(rides2<=rides && pred == NULL)
    {
        curr->suivant_attente = New;
        New->suivant_attente = NULL;
    }
    else if(curr->suivant_attente == NULL && rides<rides2)
    {
        pred->suivant_attente = New;
        New->suivant_attente = curr;
    }
    else
    {
        curr->suivant_attente = New;
        New->suivant_attente = NULL;
    }
    return ListeA;
}

Liste_Avion AddTakeoffList(Liste_Avion ListeA, avion A, Liste_Compagnie* ListeC)
{
    Cellule_avion* New  = (Cellule_avion*) malloc(sizeof(Cellule_avion));

    New->Avion = A;

    AvionCompagnieArray(New, ListeC);

    New->suivant_attente = ListeA;

    return New;
}

Cellule_avion* AddTakeoffQueue(Liste_Avion ListeA, Queue* ListeA2, int time, Liste_Compagnie* ListeC)
{
    Cellule_avion* curr = ListeA;
    Cellule_avion* pred = ListeA;
    Cellule_avion* save = ListeA;
    Cellule_compagnie* test = *ListeC;
    int decollage;

    while(curr!=NULL)
    {
        decollage = (curr->Avion.heure_decollage[0]-48)*600 + (curr->Avion.heure_decollage[1]-48)*60 + (curr->Avion.heure_decollage[2]-48)*10 + (curr->Avion.heure_decollage[3]-48);

        if(decollage < time+5)
        {

            if(curr == ListeA)
            {
                save = curr->suivant_attente;
            }
            else if(curr->suivant_attente == NULL)
            {
                pred->suivant_attente = NULL;
            }
            else
            {
                if(curr == save)
                    save = NULL;
                pred->suivant_attente = curr->suivant_attente;
            }

            if(curr->precedent_compagnie == NULL)
            {

                if(curr->suivant_compagnie != NULL)
                    curr->suivant_compagnie->precedent_compagnie = NULL;

                while(strcmp(curr->Avion.compagnie->nom, test->compagnie->nom)!=0)
                {
                    test = test->suivant;
                }

                test->compagnie->ListeAvion = curr->suivant_compagnie;
            }
            else if(curr->suivant_compagnie == NULL)
            {
                curr->precedent_compagnie->suivant_compagnie = NULL;
            }
            else
            {
                curr->precedent_compagnie->suivant_compagnie = curr->suivant_compagnie;
                curr->suivant_compagnie->precedent_compagnie = curr->precedent_compagnie;
            }
            AddFirstqueue(curr->Avion, ListeA2, ListeC);
        }

    pred = curr;
    curr = curr->suivant_attente;
    }
    return save;
}

void AddLastqueue(avion* A, Queue* ListeA2, Liste_Compagnie* ListeC)
{
    Cellule_avion* New  = (Cellule_avion*) malloc(sizeof(Cellule_avion));

    New->Avion = *A;
    AvionCompagnieArray(New, ListeC);

    if(ListeA2->first == NULL)
    {
        ListeA2->first = New;
        ListeA2->last = New;
    }
    else
    {
        ListeA2->last->suivant_attente = New;
        ListeA2->last = New;
        New->suivant_attente = NULL;
    }
}

void AddEmergency(Liste_Avion* ListeA, char emname[7], Queue* emergency, Liste_Compagnie* ListeC)
{
    Cellule_avion* save = NULL;

    save = RemoveName(ListeA, emname, ListeC);
    if(save != NULL)
    {
        AddLastqueue(&(save->Avion), emergency, ListeC);
    }
    else
        printf("This plane doesn't exist\n");
}

Cellule_avion* RemoveFirst(Liste_Avion* ListeA, Liste_Compagnie* ListeC, int x)
{
    Cellule_avion* save = *ListeA;

    Cellule_avion* curr = *ListeA;

    Cellule_compagnie* test = *ListeC;

    if(curr->precedent_compagnie == NULL)
    {
        if(curr->suivant_compagnie != NULL)
            curr->suivant_compagnie->precedent_compagnie = NULL;


        while(strcmp(curr->Avion.compagnie->nom, test->compagnie->nom)!=0)
            test = test->suivant;

        test->compagnie->ListeAvion = curr->suivant_compagnie;
    }
    else if(curr->suivant_compagnie == NULL)
    {
        curr->precedent_compagnie->suivant_compagnie = NULL;
    }
    else
    {
        curr->precedent_compagnie->suivant_compagnie = curr->suivant_compagnie;
        curr->suivant_compagnie->precedent_compagnie = curr->precedent_compagnie;
    }
    if(x != 1)
        *ListeA = (*ListeA)->suivant_attente;

    return save;
}

void AddFirstqueue(avion A, Queue* ListeA2, Liste_Compagnie* ListeC)
{
    Cellule_avion* New  = (Cellule_avion*) malloc(sizeof(Cellule_avion));

    New->Avion = A;

    AvionCompagnieArray(New, ListeC);

    if(ListeA2->first == NULL)
    {
        ListeA2->first = New;
        ListeA2->last = New;
    }
    else
    {
        New->suivant_attente = ListeA2->first;
        ListeA2->first = New;
    }
}

void CheckEmergencyFirst(Liste_Avion* ListeA, Queue* emergency, Liste_Compagnie* ListeC)
{
    Cellule_avion* save;

    if((*ListeA)->Avion.carburant/(*ListeA)->Avion.consommation <= 2)
    {
        save =  RemoveFirst(ListeA, ListeC, 0);
        AddFirstqueue(save->Avion, emergency, ListeC);
    }
}

Cellule_avion* RemoveQueue(Queue* Queue_Takeoff, Liste_Compagnie* ListeC)
{
    Cellule_avion* save;

    Cellule_avion* curr = (*Queue_Takeoff).first;

    Cellule_compagnie* test = *ListeC;

    if(curr->precedent_compagnie == NULL)
    {
        if(curr->suivant_compagnie != NULL)
            curr->suivant_compagnie->precedent_compagnie = NULL;

        while(strcmp(curr->Avion.compagnie->nom, test->compagnie->nom)!=0)
            test = test->suivant;

        test->compagnie->ListeAvion = curr->suivant_compagnie;
    }
    else if(curr->suivant_compagnie == NULL)
    {
        curr->precedent_compagnie->suivant_compagnie = NULL;
    }
    else
    {
        curr->precedent_compagnie->suivant_compagnie = curr->suivant_compagnie;
        curr->suivant_compagnie->precedent_compagnie = curr->precedent_compagnie;
    }

    if((*Queue_Takeoff).first == (*Queue_Takeoff).last)
    {
        save = (*Queue_Takeoff).first;
        (*Queue_Takeoff).first = NULL;
        (*Queue_Takeoff).last = NULL;
        return save;
    }
    else
    {
        save = (*Queue_Takeoff).first;
        (*Queue_Takeoff).first = (*Queue_Takeoff).first->suivant_attente;
        return save;
    }
}

void AvionToHistoric(Queue* Queue_Emergency, Queue* Queue_Takeoff, Liste_Avion* Liste_landing, Liste_Compagnie* ListeC, Liste_blacklist ListeB)
{
    Cellule_avion* A;
    int exist = 0;
    FILE* fichier = NULL;

    fichier = fopen("HISTORIC.txt", "a");
    if (fichier != NULL)
    {
        if((*Queue_Emergency).first == NULL && (*Queue_Takeoff).first == NULL && *Liste_landing != NULL)
        {
            A = RemoveFirst(Liste_landing, ListeC, 0);

            fprintf(fichier, "%s- A ---- %d-%d\n", A->Avion.identifiant, A->Avion.carburant, A->Avion.consommation);

            free(A);
        }
        if((*Queue_Emergency).first == NULL && (*Queue_Takeoff).first != NULL)
        {
            A = RemoveQueue(Queue_Takeoff, ListeC);

            fprintf(fichier, "%s- D %s ----\n", A->Avion.identifiant, A->Avion.heure_decollage);

            free(A);
        }

        if((*Queue_Emergency).first != NULL)
        {
            A = RemoveQueue(Queue_Emergency, ListeC);

            while(ListeB != NULL && exist == 0)
            {
                if(strcmp(A->Avion.compagnie->nom, ListeB->name) == 0)
                    exist = 1;
                ListeB = ListeB->suivant;
            }

            if(A->Avion.carburant <= 0)
                fprintf(fichier, "%s- C ---- %d-%d\n", A->Avion.identifiant, A->Avion.carburant, A->Avion.consommation);
            else if(exist == 1)
            {
                fprintf(fichier, "%s- N ---- %d-%d\n", A->Avion.identifiant, A->Avion.carburant, A->Avion.consommation);
            }
            else
                fprintf(fichier, "%s- U ---- %d-%d\n", A->Avion.identifiant, A->Avion.carburant, A->Avion.consommation);
            free(A);
        }
        fclose(fichier);
    }
}

void ChangeCarburant(Liste_Avion* Liste_landing)
{
    Cellule_avion* curr;

    curr = *Liste_landing;

    while(curr != NULL)
    {
        curr->Avion.carburant -= curr->Avion.consommation;
        curr = curr->suivant_attente;
    }
}

void DisplayList(Liste_Avion* ListeA)
{
    Cellule_avion* curr;

    curr = *ListeA;

    while(curr != NULL)
    {
        printf("%s\n", curr->Avion.identifiant);
        curr = curr->suivant_attente;
    }
}

Cellule_avion* RemoveName(Liste_Avion* ListeA, char emname[7], Liste_Compagnie* ListeC)
{
    Cellule_avion* curr = *ListeA;
    Cellule_avion* pred = NULL;
    Cellule_avion* save = NULL;
    Cellule_compagnie* test = *ListeC;

    while(curr != NULL)
    {
        if(strcmp(emname,curr->Avion.identifiant) == 0)
        {
            if(curr == *ListeA)
            {
                save = *ListeA;
                *ListeA = (*ListeA)->suivant_attente;
            }
            else
            {
                save = pred->suivant_attente;
                pred->suivant_attente = curr->suivant_attente;
            }
            if(curr->precedent_compagnie == NULL)
            {
                if(curr->suivant_compagnie != NULL)
                    curr->suivant_compagnie->precedent_compagnie = NULL;

                while(strcmp(curr->Avion.compagnie->nom, test->compagnie->nom)!=0)
                    test = test->suivant;

                test->compagnie->ListeAvion = curr->suivant_compagnie;
            }
            else if(curr->suivant_compagnie == NULL)
            {
                curr->precedent_compagnie->suivant_compagnie = NULL;
            }
            else
            {
                curr->precedent_compagnie->suivant_compagnie = curr->suivant_compagnie;
                curr->suivant_compagnie->precedent_compagnie = curr->precedent_compagnie;
            }
        }
        pred = curr;
        curr = curr->suivant_attente;
    }
    return save;
}


void ReadTestFile(Liste_Avion *Liste_landing, Queue *Queue_Emergency, Liste_Avion *Liste_Takeoff, Liste_Compagnie* ListeC, avion* Avion)
{
    FILE* fichier = NULL;
    FILE* historic = NULL;
    Cellule_compagnie* curr = *ListeC;

    fichier = fopen("test.txt", "r");
    historic = fopen("HISTORIC.txt", "a");


    char t[19];
    char Cnom[10];

    int exist = 0, code = 0;
    Compagnie* C;



    if (fichier != NULL)
    {
        while(fscanf(fichier, "%s",t) != EOF)
        {
            if(strlen(t) == 6)
            {
                strcpy(Avion->identifiant, t);
            }

            if(strlen(t) == 1)
            {
                if(strcmp(t,"R")==0)
                {
                    code = 1;
                }
                if(strcmp(t,"U") == 0 || strcmp(t,"N") == 0)
                {
                    code = 2;
                }
                if(strcmp(t,"D") == 0)
                {
                    code = 3;
                }
                if(strcmp(t,"C") == 0)
                {
                    code = 4;
                }
            }
            if(strlen(t)!=1)
            {
                if(code == 1)
                {
                    sscanf(t,"%d-%d", &(Avion->carburant), &(Avion->consommation));
                    printf("Enter the airline of %s\n", Avion->identifiant);
                    scanf("%s", Cnom);
                    Avion->heure_decollage[0] = 'B';


                    while(curr != NULL && exist == 0)
                    {
                        if(strcmp(curr->compagnie->nom, Cnom)==0)
                            exist = 1;
                        else
                            curr = curr->suivant;
                    }
                    if(exist == 0)
                    {
                        C = (Compagnie*) malloc(sizeof(Compagnie));
                        strcpy(C->nom,Cnom);
                        C->ListeAvion = NULL;
                        Avion->compagnie = C;
                    }
                    else
                        Avion->compagnie = curr->compagnie;

                    *Liste_landing = AddLandingList(*Liste_landing, *Avion, ListeC);
                    code=0;
                }
                curr = *ListeC;
                exist = 0;
                if(code == 2)
                {
                    sscanf(t,"%d-%d", &(Avion->carburant), &(Avion->consommation));

                    printf("Enter the airline of %s\n", Avion->identifiant);
                    scanf("%s", Cnom);
                    Avion->heure_decollage[0] = 'B';

                    while(curr != NULL && exist == 0)
                    {
                        if(strcmp(curr->compagnie->nom, Cnom)==0)
                            exist = 1;
                        else
                            curr = curr->suivant;
                    }
                    if(exist == 0)
                    {
                        C = (Compagnie*) malloc(sizeof(Compagnie));
                        strcpy(C->nom,Cnom);
                        C->ListeAvion = NULL;
                        Avion->compagnie = C;
                    }
                    else
                        Avion->compagnie = curr->compagnie;

                    AddLastqueue(Avion, Queue_Emergency, ListeC);
                    code=0;
                }
                curr = *ListeC;
                exist = 0;
                if(code == 3)
                {
                    printf("Enter the airline of %s\n", Avion->identifiant);
                    scanf("%s", Cnom);
                    sscanf(t,"%s", Avion->heure_decollage);
                    Avion->carburant = -9999;

                    while(curr != NULL && exist == 0)
                    {
                        if(strcmp(curr->compagnie->nom, Cnom)==0)
                            exist = 1;
                        else
                            curr = curr->suivant;
                    }
                    if(exist == 0)
                    {
                        C = (Compagnie*) malloc(sizeof(Compagnie));
                        strcpy(C->nom,Cnom);
                        C->ListeAvion = NULL;
                        Avion->compagnie = C;
                    }
                    else
                        Avion->compagnie = curr->compagnie;

                    *Liste_Takeoff = AddTakeoffList(*Liste_Takeoff, *Avion, ListeC);

                    code=0;
                }
                curr = *ListeC;
                exist = 0;
                if(code == 4)
                {
                    sscanf(t,"%s", Avion->heure_decollage);
                    fprintf(historic, "%s- C %s ----\n", Avion->identifiant, Avion->heure_decollage);
                    code = 0;
                }
            }
        }
        fclose(fichier);
        fclose(historic);
    }
}


void AvionCompagnieArray(Cellule_avion* New, Liste_Compagnie* ListeC)
{
    int exist = 0;
    Cellule_compagnie* curr;

    *ListeC = AddCompagnie(*ListeC, New->Avion.compagnie);
    curr = *ListeC;

    while(*ListeC != NULL && exist == 0)
    {
        if(strcmp(curr->compagnie->nom, New->Avion.compagnie->nom)==0)
            exist = 1;
        else
            curr = curr->suivant;
    }
    curr->compagnie->ListeAvion = addLast(curr->compagnie->ListeAvion, New);
}

Cellule_compagnie* AddCompagnie(Liste_Compagnie ListeC, Compagnie* C)
{
    int exist = 0;
    Cellule_compagnie* save = ListeC;
    Cellule_compagnie* New = (Cellule_compagnie*) malloc(sizeof(Cellule_compagnie));

    while(save != NULL)
    {
        if(strcmp(save->compagnie->nom, C->nom)==0)
            exist = 1;

        save = save->suivant;
    }
    if(exist == 0)
    {
        New->compagnie = C;

        New->suivant = ListeC;

        return New;
    }
    else
        return ListeC;
}

void Blacklist(Liste_Compagnie* ListeC, char Cname[10], Queue* Queue_Emergency, Liste_Avion* ListeL, Liste_Avion* ListeT)
{
    Cellule_compagnie* curr;

    Cellule_avion* A;

    curr = *ListeC;

    int cpt = 0, i = 0, exist=0;

    while(curr != NULL && exist == 0)
    {
        if(strcmp(curr->compagnie->nom, Cname)== 0)
            exist = 1;
        else
            curr = curr->suivant;
    }
    if(curr != NULL)
        cpt = length(curr->compagnie->ListeAvion);

    while(i < cpt)
    {
        A = RemoveFirst(&(curr->compagnie->ListeAvion), ListeC, 1);

        if(A->Avion.heure_decollage[0] == 'B')
        {
            RemoveName(ListeL, A->Avion.identifiant, ListeC);
            AddLastqueue(&A->Avion, Queue_Emergency, ListeC);
        }

        else if(A->Avion.carburant == -9999)
        {
            RemoveName(ListeT, A->Avion.identifiant, ListeC);
            free(A);
        }
        i++;
    }
}

int length(Liste_Avion ListeA)
{
    int cpt = 0;

    while(ListeA != NULL)
    {
        cpt++;
        ListeA = ListeA->suivant_compagnie;
    }

    return cpt;
}


Cellule_blacklist* ListeBlacklisted(char compagniename[10], Liste_blacklist* ListeB)
{
    Cellule_blacklist* New = (Cellule_blacklist*) malloc(sizeof(Cellule_blacklist));

    strcpy(New->name, compagniename);

    New->suivant = *ListeB;

    return New;
}

Cellule_avion* addLast(Liste_Avion ListeA, Cellule_avion* New)
{
    Cellule_avion* curr = ListeA;

    if(ListeA == NULL)
    {
        New->precedent_compagnie = NULL;
        New->suivant_compagnie = NULL;
        return New;
    }
    else
    {
        while(curr->suivant_compagnie != NULL)
        {
            curr = curr->suivant_compagnie;
        }
        curr->suivant_compagnie = New;
        New->suivant_compagnie = NULL;
        New->precedent_compagnie = curr;
    }
    return ListeA;

}

void CheckBlacklist(Liste_Compagnie* ListeC, Liste_blacklist ListeB, Queue* Queue_Emergency, Liste_Avion* ListeL, Liste_Avion* ListeT)
{
    while(ListeB != NULL)
    {
        Blacklist(ListeC, ListeB->name, Queue_Emergency, ListeL, ListeT);
        ListeB = ListeB->suivant;
    }
}

void FillBlackList(Liste_blacklist* ListeB)
{
    FILE* fichier = NULL;

    char t[10];

    fichier = fopen("blacklist.txt", "r+");

    if(fichier != NULL)
    {
        while(fscanf(fichier, "%s",t) != EOF)
        {
            *ListeB = ListeBlacklisted(t, ListeB);
        }
        fclose(fichier);
    }
}

void LorT(char Cname[10], Liste_Compagnie ListeC, int time)
{
    int exist=0, Late=0, Total=0, decollage;

    Cellule_compagnie* curr;

    curr = ListeC;

    Cellule_avion* A;

    while(curr != NULL && exist == 0)
    {
        if(strcmp(curr->compagnie->nom, Cname)== 0)
            exist = 1;
        else
            curr = curr->suivant;
    }

    A = curr->compagnie->ListeAvion;

    if(exist==1)
    {
        while(A != NULL)
        {
            if(A->Avion.heure_decollage[0] == 'B')
            {
                printf("WAITING FOR LANDING : %s\n", A->Avion.identifiant);
            }
            else
            {
                Total++;
                printf("WAITING TO TAKE OFF : %s\n", A->Avion.identifiant);
                decollage = (A->Avion.heure_decollage[0]-48)*600 + (A->Avion.heure_decollage[1]-48)*60 + (A->Avion.heure_decollage[2]-48)*10 + (A->Avion.heure_decollage[3]-48);
                if(decollage < time)
                {
                    printf("THIS PLANE IS LATE\n");
                    Late++;
                }

            }
            A = A->suivant_compagnie;
        }
        printf("LATE : %d\nON TIME : %d\n", Late, Total-Late);
    }
    else
        printf("No such company");

}

int random()
{
    int x;
    srand(time(NULL));
    x=rand()%5+1;
    return x;
}

void printhistoric()
{
    char ligne[80];

    FILE* fichier = NULL;

    fichier = fopen("HISTORIC.txt", "r");

    if (fichier != NULL)
    {
        fgets(ligne, 80, fichier);
        return ligne;

    }
    printf("\n");
    fclose(fichier);
}










