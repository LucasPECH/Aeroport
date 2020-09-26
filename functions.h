#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
typedef struct cellule_avion Cellule_avion;
typedef struct
{
    char nom[7];
    char acronyme[3];
    Cellule_avion* ListeAvion;
}Compagnie;

typedef struct
{
    char identifiant[7];
    int carburant;
    int consommation;
    char heure_decollage[3];
    Compagnie* compagnie;
}avion;

typedef struct cellule_avion
{
    avion Avion;
    struct cellule_avion* suivant_compagnie;
    struct cellule_avion* precedent_compagnie;
    struct cellule_avion* suivant_attente;
}Cellule_avion;

typedef Cellule_avion* Liste_Avion;

typedef struct Queue
{
    Cellule_avion* first;
    Cellule_avion* last;
}Queue;

typedef struct cellule_compagnie
{
    Compagnie* compagnie;
    struct cellule_compagnie* suivant;
}Cellule_compagnie;

typedef Cellule_compagnie* Liste_Compagnie;

typedef struct cellule_blacklist
{
    char name[10];
    struct cellule_blacklist* suivant;
}Cellule_blacklist;

typedef Cellule_blacklist* Liste_blacklist;


void DisplayList(Liste_Avion* ListeA);

Liste_Avion AddLandingList(Liste_Avion ListeA, avion A, Liste_Compagnie* ListeC);

Cellule_avion* AddTakeoffQueue(Liste_Avion ListeA, Queue* ListeA2, int time, Liste_Compagnie* ListeC);

Cellule_avion* RemoveEmergency(Liste_Avion ListeA, char emname[7]);

Cellule_avion* RemoveFirst(Liste_Avion* ListeA, Liste_Compagnie* ListeC, int x);

void AddLastqueue(avion* A, Queue* ListeA2, Liste_Compagnie* ListeC);

void AddFirstqueue(avion A, Queue* ListeA2, Liste_Compagnie* ListeC);

Liste_Avion AddTakeoffList(Liste_Avion ListeA, avion A, Liste_Compagnie* ListeC);

void user(avion* Avion, int x, Liste_Compagnie ListeC);

void AddAvion(avion* Avion, Compagnie* C, Liste_Avion* Liste_landing, Liste_Avion* Liste_Takeoff, Queue* Liste_Emergency, Queue* Queue_Takeoff, Liste_Compagnie* ListeC, Liste_blacklist* ListeB, int time, int X);

void CheckEmergencyFirst(Liste_Avion* ListeA, Queue* emergency, Liste_Compagnie* ListeC);

void AddEmergency(Liste_Avion* ListeA, char emname[7], Queue* emergency, Liste_Compagnie* ListeC);

void AvionToHistoric(Queue* Queue_Emergency, Queue* Queue_Takeoff, Liste_Avion* Liste_landing, Liste_Compagnie* ListeC, Liste_blacklist ListeB);

Cellule_avion* RemoveName(Liste_Avion* ListeA, char emname[7], Liste_Compagnie* ListeC);

void ChangeCarburant(Liste_Avion* Liste_landing);

Cellule_avion* RemoveQueue(Queue* Queue_Takeoff, Liste_Compagnie* ListeC);

void AvionCompagnieArray(Cellule_avion* New, Liste_Compagnie* ListeC);

void ReadTestFile(Liste_Avion *Liste_landing, Queue *Queue_Emergency, Liste_Avion *Liste_Takeoff, Liste_Compagnie* ListeC, avion* Avion);

Cellule_compagnie* AddCompagnie(Liste_Compagnie ListeC, Compagnie* C);

void Blacklist(Liste_Compagnie* ListeC, char Cname[10], Queue* Queue_Emergency, Liste_Avion* ListeL, Liste_Avion* ListeT);

Cellule_blacklist* ListeBlacklisted(char compagniename[10], Liste_blacklist* ListeB);

int length(Liste_Avion ListeA);

Cellule_avion* addLast(Liste_Avion ListeA, Cellule_avion* New);

void CheckBlacklist(Liste_Compagnie* ListeC, Liste_blacklist ListeB, Queue* Queue_Emergency, Liste_Avion* ListeL, Liste_Avion* ListeT);

void LorT(char Cname[10], Liste_Compagnie ListeC, int time);

void FillBlackList(Liste_blacklist* ListeB);

int random();

void printhistoric();
#endif // FUNCTIONS_H_INCLUDED
