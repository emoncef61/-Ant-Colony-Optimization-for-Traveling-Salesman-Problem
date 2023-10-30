
#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include <string.h>
#include<time.h>


#define NUM_ANTS 10
#define ALPHA 1.0
#define BETA 2.0
#define EVAPORATION 0.5
#define Q 100.0
#define MAX_ITERATIONS 100
#define N 40

#define NUM_NODES 20



double pheromoneMatrix[N][N];  // Matrice des phéromones
double visibilityMatrix[N][N];  // Matrice de visibilité
double probabilityMatrix[N][N];  // Matrice de probabilité

int bestTour[N];  // Meilleur chemin trouvé
double bestTourLength = INFINITY;  // Longueur du meilleur chemin


typedef struct sommet
{
    int x;
    int y;
} Sommet ;



typedef struct {
    int x;
    int y;
} Vector2;

typedef struct {
    int path[NUM_NODES];
    double distance;
} PathResult;


/*-----------------------Partie Matrice-----------------------------------*/

double** creer_matrice(){
    double** matrice=(double**)malloc(N * sizeof(int * ));
    for (int i=0;i<N;i++){
        matrice [i]=(double*)malloc(N* sizeof(int));

    }
    if (matrice!=NULL){
        printf("matrice créer avec succés.\n");
        for (int i=0;i<N;i++){
            for (int j=0;j<N;j++){
                matrice[i][j]=0;
            }
        }

    }else{
        printf("la création de la matrice a échoué.\n");
    }

    return matrice;

}



void genere2 (double** matrice , double p ){
    for (int i=0;i<N;i++){
        for (int j=i+1;j<N;j++){
            double R=(double)rand()/RAND_MAX;
            if (R < p){

                matrice[i][j]=1;
                matrice[j][i]=1;}
        }
    }

}

//Calcul de la matrice de distance
double ** distance(Sommet * sommet , double ** matrice ){
    double ** matricedistance = (double **)malloc(N * sizeof(double*)); //allocation
    for (int i = 0; i < N; i++) {
        matricedistance[i] = (double*)malloc(N * sizeof(double));
    }
    //Initialisation
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matricedistance[i][j] = 0;
        }
    }
    //remplissage avec les distances
    for(int i =0 ; i<N ;i++){
        for(int j =0 ; j<N ;j++){
            if(matrice[i][j] == 1){
                matricedistance[i][j]=sqrt(((sommet[i].x - sommet[j].x)*(sommet[i].x - sommet[j].x)) + ((sommet[i].y - sommet[j].y)*(sommet[i].y - sommet[j].y)));
            }

        }
    }
    return matricedistance ;
}



void liberer(double** matrice){
    for (int i=0;i<N;i++){
        free(matrice [i]);
    }
    free(matrice);
}

/*----------------------Colonie de fourmis-----------------------------------*/

void initialize(double** distanceMatrix) {
    // Initialiser les matrices de phéromones, visibilité et probabilité
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j) {
                pheromoneMatrix[i][j] = 1.0;
                visibilityMatrix[i][j] = 1.0 / distanceMatrix[i][j];
            } else {
                pheromoneMatrix[i][j] = 0.0;
                visibilityMatrix[i][j] = 0.0;
            }
            probabilityMatrix[i][j] = 0.0;
        }
    }
}

int selectNextCity(int currentCity, bool *visited) {
    double total = 0.0;
    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            probabilityMatrix[currentCity][i] = pow(pheromoneMatrix[currentCity][i], ALPHA) *
                                                pow(visibilityMatrix[currentCity][i], BETA);
            total += probabilityMatrix[currentCity][i];
        } else {
            probabilityMatrix[currentCity][i] = 0.0;
        }
    }

    double r = (double) rand() / RAND_MAX;
    double sum = 0.0;
    for (int i = 0; i < N; i++) {
        probabilityMatrix[currentCity][i] /= total;
        sum += probabilityMatrix[currentCity][i];
        if (r <= sum) {
            return i;
        }
    }

    // Si aucune ville n'est sélectionnée, retourner la première ville non visitée
    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            return i;
        }
    }

    return -1;  // Erreur
}

void updatePheromones() {
    // Évaporation des phéromones
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pheromoneMatrix[i][j] *= (1.0 - EVAPORATION);
        }
    }

    // Ajout des nouvelles phéromones
    for (int k = 0; k < NUM_ANTS; k++) {
        double tourLength = 0.0;
        for (int i = 0; i < N - 1; i++) {
            int city1 = bestTour[i];
            int city2 = bestTour[i + 1];
            pheromoneMatrix[city1][city2] += Q / tourLength;
            pheromoneMatrix[city2][city1] += Q / tourLength;
        }
        int lastCity = bestTour[N - 1];
        int firstCity = bestTour[0];
        pheromoneMatrix[lastCity][firstCity] += Q / tourLength;
        pheromoneMatrix[firstCity][lastCity] += Q / tourLength;
    }
}

void antColonyOptimization(double ** distanceMatrix) {
    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        for (int ant = 0; ant < NUM_ANTS; ant++) {
            bool visited[N] = {false};
            int currentCity = rand() % N;    //choisir une ville au hasard
            visited[currentCity] = true;     // ville visité
            bestTour[0] = currentCity;      //initialise la le dapart par le ville au hasar
            double tourLength = 0.0;

            for (int i = 1; i < N; i++) {     //on itere sur le nb de so,,
                int nextCity = selectNextCity(currentCity, visited);
                visited[nextCity] = true;
                bestTour[i] = nextCity;
                tourLength += distanceMatrix[currentCity][nextCity];
                currentCity = nextCity;
            }

            tourLength += distanceMatrix[currentCity][bestTour[0]];
            if (tourLength < bestTourLength) {
                bestTourLength = tourLength;
            }
        }

        updatePheromones();
    }
}





/*-----------------------Partie affichage-----------------------------------*/
void end_sdl(char ok, // fin normale : ok = 0 ; anormale ok = 1
             char const* msg, // message à afficher
             SDL_Window* window, // fenêtre à fermer
             SDL_Renderer* renderer) { // renderer à fermer
    char msg_formated[255];
    int l;

    if (!ok) { // Affichage de ce qui ne va pas
        strncpy(msg_formated, msg, 250);
        l = strlen(msg_formated);
        strcpy(msg_formated + l, " : %s\n");

        SDL_Log(msg_formated, SDL_GetError());
    }

    if (renderer != NULL) { // Destruction si nécessaire du renderer
        SDL_DestroyRenderer(renderer); // Attention : on suppose que les NULL sont maintenus !!
        renderer = NULL;
    }
    if (window != NULL) { // Destruction si nécessaire de la fenêtre
        SDL_DestroyWindow(window); // Attention : on suppose que les NULL sont maintenus !!
        window= NULL;
    }

    SDL_Quit();

    if (!ok) { // On quitte si cela ne va pas
        exit(EXIT_FAILURE);
    }
}
int estCaseLibre(Sommet* sommets, int nbSommets,int w, int h,int x, int y) {
    for (int i = 0; i < nbSommets; i++) {
        if (abs(sommets[i].x + (1/2)*w - x) < 1.5*w && abs(sommets[i].y +(1/2)*h - y) <= 1.5*h) {
            return 0; // La case est adjacente à un sommet déjà placé
        }
    }
    return 1; // La case est libre
}

Sommet* placerSommets(int nbSommets,int LARGEUR_FENETRE , int HAUTEUR_FENETRE ) {
    Sommet* sommets = (Sommet*)malloc(nbSommets * sizeof(Sommet));

    int largeurCase = LARGEUR_FENETRE / (9 * nbSommets);
    int hauteurCase = HAUTEUR_FENETRE / (9 * nbSommets);

    for (int i = 0; i < nbSommets; i++) {
        sommets[i].x=0 ; sommets[i].y=0;
        int x, y;
        do {
            x = rand() % (9 * nbSommets) * largeurCase;
            y = rand() % (9 * nbSommets) * hauteurCase;
        } while (!estCaseLibre(sommets, i,largeurCase,hauteurCase, x, y));

        sommets[i].x = x + (rand()/RAND_MAX) * largeurCase;
        sommets[i].y = y + (rand()/RAND_MAX )* hauteurCase;
    }

    return sommets;
}


void dessinerGraphe(SDL_Renderer *renderer, double** matrice , Sommet* sommet ,int* state ) {
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); // Rouge pour les nœuds

    for (int i = 0; i < N; i++) {
        int x = sommet[i].x;
        int y = sommet[i].y;
        int width = 40;
        int height = 40;

        SDL_Rect rect = {x, y, width, height};
        SDL_RenderFillRect(renderer, &rect);
    }

    for (int i=0;i<N;i++){
        for (int j=i+1;j<N;j++){
            if(matrice[i][j]==1){
                SDL_SetRenderDrawColor(renderer, i*30 , 20 , 0, 255); // Rouge pour les nœuds
                SDL_RenderDrawLine(renderer,
                                   sommet[i].x+20, sommet[i].y+20, // x,y du point de la première extrémité
                                   sommet[j].x+20, sommet[j].y+20); // x,y seconde extrémité
            }}}


    int* st=(int*)malloc((N+1)*sizeof(int));
    for (int i = 0; i < N; i++)
    {
        st[i]=state[i];
    }
    st[N]=state[0];
    for (int i=0;i<N;i++){
        int a=st[i];
        int b=st[i+1];
        SDL_SetRenderDrawColor(renderer, 0 , 0, 255, 255); // vert pour le cycle
        SDL_RenderDrawLine(renderer,
                           sommet[a-1].x+20, sommet[a-1].y+20, // x,y du point de la première extrémité
                           sommet[b-1].x+20, sommet[b-1].y+20); // x,y seconde extrémité
    }

    SDL_RenderPresent(renderer);
    free(st);
}

void dessinercycle(SDL_Renderer *renderer, double** matrice , Sommet* sommet, int* state){

int* st=(int*)malloc((N+1)*sizeof(int));
for (int i = 0; i < N; i++)
{
st[i]=state[i];
}
st[N]=state[0];
for (int i=0;i<N;i++){
int a=st[i];
int b=st[i+1];
SDL_SetRenderDrawColor(renderer, 0 , 0 , 255, 255);  // vert pour le cycle
SDL_RenderDrawLine(renderer,
        sommet[a-1].x+20, sommet[a-1].y+20,                                          // x,y du point de la première extrémité
sommet[b-1].x+20, sommet[b-1].y+20);                                // x,y seconde extrémité
}

SDL_RenderPresent(renderer);
free(st);}



int main(int argc, char** argv) {

    srand(time(NULL));
    (void)argc;
    (void)argv;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL ;
    SDL_DisplayMode screen;









    /*********************************************************************************************************************/
    /* Initialisation de la SDL + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) end_sdl(0, "ERROR SDL INIT", window, renderer);

    SDL_GetCurrentDisplayMode(0, &screen);
    printf("Résolution écran\n\tw : %d\n\th : %d\n",
           screen.w, screen.h);

    /* Création de la fenêtre */
    window = SDL_CreateWindow("Premier dessin",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, screen.w * 0.66,
                              screen.h * 0.66,
                              SDL_WINDOW_OPENGL);
    if (window == NULL) end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

    /* Création du renderer */
    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

    /*creation de la matrice*/



    double** matrice = creer_matrice(N);


    for (int i=0;i<N;i++){
        for (int j=0;j<N;j++){
            printf("%d " ,matrice[i][j]);
        }
        printf("\n");}


    genere2( matrice ,1);
    printf("la 2eme\n");

    for (int i=0;i<N;i++){
        for (int j=0;j<N;j++){
            printf("%d " ,matrice[i][j]);
        }
        printf("\n");

    }






    Sommet* sommets = placerSommets(N, screen.w * 0.66 , screen.h * 0.66);


    for (int i = 0; i < N; i++) {
        printf("Sommet %d : (%d, %d)\n", i , sommets[i].x, sommets[i].y);}

    double** distanceMatrix = distance(sommets ,matrice);
    printf("la matrice de distance est : \n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%f ", distanceMatrix[i][j]);
        }
        printf("\n");
    }


    initialize(distanceMatrix);
    antColonyOptimization(distanceMatrix);

    printf("Best Tour: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", bestTour[i]);
    }
    printf("\nBest Tour Length: %.2f\n", bestTourLength);







    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);


    dessinerGraphe(renderer, matrice ,sommets,bestTour);
    dessinercycle(renderer,matrice,sommets ,bestTour);


    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    end_sdl(1, "Normal ending", window, renderer);
    return EXIT_SUCCESS;

















    free(sommets);


    liberer(matrice);
    liberer(distanceMatrix);
    return EXIT_SUCCESS;
}







