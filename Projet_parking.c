//Voici toute les bibliothèques.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>
#include <math.h>


/*variable globale.
Il y a 27 parking.*/ 
#define NB_PARKING 27
//contient tous les caractères de la ligne / nb max de caractères/.
#define LIGNE 150 
#define nb_personne 100 //18180 trop élévée pour ma mémoire.

//Définition par une strucutre des catégorie du fichier parking.
typedef struct parking
{
    char identifiant[20];
    char nom[100];
    char adresse[100];
    char ville[50];
    char etat[15];
    char places_disponibles[20];
    char capacite_max[50];
    char mise_a_jour[50];
    char affichage_panneaux[50];
}parking;

typedef struct clients
{
    char plaque_immatriculation[50];
    struct tm date_enter;
    struct tm date_exit;
    struct tm heure_entrer;
    struct tm heure_sortie;
    int prix;
    char id_parking [50];
}clients;

//défition des tableaux des structures et du numéro client.
parking tab[NB_PARKING];
int num_clients[nb_personne];
clients tab_cl[nb_personne];

//Fonction pour l'ouverture du fichier.
//ligne représente toutes les cases d'une ligne.
void lesparkings() 
{
    int nb_parc = 0;
    char ligne[LIGNE]; 
    char *cases;
    FILE *fichier = fopen("C:/Users/DUBALLET/Desktop/parking-metropole.csv", "r");
    if (fichier == NULL)
    {
        printf("Impossible d'ouvrir le fichier parking metropole.\n");
        exit(1);
    }
    fgets(ligne, LIGNE, fichier);
    while (fgets(ligne, LIGNE,fichier) && nb_parc < NB_PARKING)
    {
        cases=strtok(ligne,";");
        strcpy(tab[nb_parc].identifiant, cases);
        cases = strtok(NULL, ";");
        strcpy(tab[nb_parc].nom, cases);
        cases = strtok(NULL, ";");
        strcpy(tab[nb_parc].adresse, cases);
        cases = strtok(NULL, ";");
        strcpy(tab[nb_parc].ville, cases);
        cases = strtok(NULL, ";");
        strcpy(tab[nb_parc].etat, cases);
        cases = strtok(NULL, ";");
        strcpy(tab[nb_parc].places_disponibles, cases);
        cases = strtok(NULL, ";");
        strcpy(tab[nb_parc].capacite_max, cases);
        cases = strtok(NULL, ";");
        strcpy(tab[nb_parc].mise_a_jour, cases);
        cases = strtok(NULL, ";\n");
        strcpy(tab[nb_parc].affichage_panneaux, cases);
        nb_parc++;
    }
    fclose(fichier);
}

//affiche tous les parkings présent dans le fichier CSV.
void afficheparking(parking *tab)
{
    int i = 0;
    char name[20];
    printf("Donnez l'identifiant du parking :\n");
    printf("Identifiant du parking :");
    scanf("%s", name);
    while (i < NB_PARKING && strcmp(tab[i].identifiant, name) != 0)
    {
        i++;
    }
    if (i < NB_PARKING)
    {
        printf("Voici le %s qui se trouve a %s, a l'adresse %s et il y a %s places.\n",tab[i].nom, tab[i].ville, tab[i].adresse, tab[i].places_disponibles);
    } 
    else 
    {
        printf("Aucun parking n'a ete trouve.\n");
    }
}

//Permet de hasher le mot de passe du mode administrateur.
void hash_password(const char *password, unsigned char *hash)
{
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, password, strlen(password));
    SHA256_Final(hash, &sha256_ctx);
}

//Permet à l'utilisateur de chercher un parking spécifique par son identifiants.
void afficheparkings(parking *tab)
{
    int i;
    for (int i = 0; i < NB_PARKING; i++) 
    {
        printf("Voici le %s qui se trouve a %s, a l'adresse %s et il y a %s places.\n",tab[i].nom, tab[i].ville, tab[i].adresse, tab[i].places_disponibles);  
    }      
}

//Permet de mettre à jour lors de l'entrée ou la sortie d'un véhicule.
void mettreJourOccupation (parking *tab)
{
    char nom_parking[50];
    int i = 0;
    int place = 0;
    printf("Quel est l'identifiant du parking dont vous etes entre:\n");
    printf("Identifiant du parking :");
    scanf("%s", nom_parking);
    while (i < NB_PARKING && strcmp(tab[i].identifiant, nom_parking) != 0)
    {
        i++;
    }
    if (i < NB_PARKING)
    {
        place = atoi(tab[i].places_disponibles);
        if (place > 0) 
        {
            place--;
            sprintf(tab[i].places_disponibles, "%d", place); 
            printf("Il reste maintenant %d places dans le parking %s.\n", place, tab[i].nom);
            strcpy(tab_cl[*num_clients].id_parking, tab[i].identifiant);
        } 
        else 
        {
            printf("Le parking est complet pour le moment.\n");
        }
    } 
    else
    {
        printf ("Le parking est complet pour le moment reste informe sur le site pour si une place ce libère.\n");
    }   
}

//Pareil que la fonction précédente mais le sauvegarde dans le fichier CSV.
void sauvegarderEtatParking(parking *tab) 
{
    FILE *fichier = fopen("C:/Users/DUBALLET/Desktop/parking-metropole.csv", "w");
    if (fichier == NULL) 
    {
        printf("Impossible d'ouvrir le fichier.\n");
        exit(1);
    }
    fprintf(fichier, "Identifiant;Nom;Adresse;Ville;Etat;Places_disponibles;Capacite_max;Mise_a_jour;Affichage_panneaux\n");
    for (int i = 0; i < NB_PARKING; i++) {
        fprintf(fichier, "%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
                tab[i].identifiant,
                tab[i].nom,
                tab[i].adresse,
                tab[i].ville,
                tab[i].etat,
                tab[i].places_disponibles,
                tab[i].capacite_max,
                tab[i].mise_a_jour,
                tab[i].affichage_panneaux);
    }
    fclose(fichier);
    printf("L'etat du parking a ete sauvegarde avec succes.\n");
}

//Permet de sauvegarder son entrée dans le parking.
void entrerParking(clients* tab_cl, int* num_clients, parking *tab) 
{
    int i = 0;
    while (i < nb_personne)
    {
        if (num_clients[i] == 0)
        {
            printf("Entrer votre numero clients (different de 0) :\n");
            printf("Numero client :");
            scanf("%d", &num_clients[i]);
            printf("Entrer votre plaque d'immatriculation:\n");
            printf("Plaque d'immatriculation :");
            scanf("%s", tab_cl[i].plaque_immatriculation);
            printf("Entrer la date actuelle(de type JJ/MM/AAAA):\n");
            printf("Date acutelle :");
            scanf("%d/%d/%d", &tab_cl[i].date_enter.tm_mday, &tab_cl[i].date_enter.tm_mon, &tab_cl[i].date_enter.tm_year);
            printf("Entrer votre heure d'entrer(de type HH:MM):\n");
            printf("Heure entrer :");
            scanf("%d:%d", &tab_cl[i].heure_entrer.tm_hour, &tab_cl[i].heure_entrer.tm_min);
            break;
        }
        else
        {
            i++;
        }
    }
    mettreJourOccupation(tab);
    sauvegarderEtatParking(tab);
}

//Permet de sauvegarder sa sortie d'un parking.
void sortieParking(clients* tab_cl, int* num_clients, parking * tab)
{
    char immat[50];
    int confirme;
    int i = 0;
    int j = 0;
    char park[20];
    int place = 0;
    int diff_date = 0;
    float diff_heure = 0;
    int diff_seconde = 0;
    struct tm sortie;
    struct tm entree;
    int heures_arrondi;
    int index_parking = - 1;
    printf("Veuillez entrer votre plaque d'immatriculation:\n");
    printf("Plaque d'immatriculation :");
    scanf("%s", immat);
    while(i < nb_personne)
    {
        if(strcmp(immat, tab_cl[i].plaque_immatriculation) == 0)
        {
            printf("Votre numero client est bien le %d (ecrit sur votre ticket d'entre).\n", num_clients[i]);
            printf("Si c'est bien vous appuyez sur 1 sinon appuyez sur 0 (sur votre ticket d'entre).\n");
            scanf("%d", &confirme);
            if(confirme == 1)
            {
                printf("Votre plaque d'immatriculation est %s.\n", tab_cl[i].plaque_immatriculation);
                printf("Votre date d'entree est %d/%d/%d.\n", tab_cl[i].date_enter.tm_mday, tab_cl[i].date_enter.tm_mon, tab_cl[i].date_enter.tm_year);
                if (tab_cl[i].heure_entrer.tm_min < 10 && tab_cl[i].heure_entrer.tm_hour >= 10)
                {
                    printf("Votre heure d'entree est le 0%d:%d.\n", tab_cl[i].heure_entrer.tm_hour, tab_cl[i].heure_entrer.tm_min);
                }
                if (tab_cl[i].heure_entrer.tm_hour < 10 && tab_cl[i].heure_entrer.tm_min >= 10)
                {
                    printf("Votre heure d'entree est le 0%d:%d.\n", tab_cl[i].heure_entrer.tm_hour, tab_cl[i].heure_entrer.tm_min);
                }
                if (tab_cl[i].heure_entrer.tm_min >= 10 && tab_cl[i].heure_entrer.tm_hour >= 10)
                {
                    printf("Votre heure d'entree est le %d:%d.\n", tab_cl[i].heure_entrer.tm_hour, tab_cl[i].heure_entrer.tm_min);
                }
                if (tab_cl[i].heure_entrer.tm_min < 10 && tab_cl[i].heure_entrer.tm_hour < 10)
                {
                    printf("Votre heure d'entree est le 0%d:0%d.\n", tab_cl[i].heure_entrer.tm_hour, tab_cl[i].heure_entrer.tm_min);
                }
                printf("Entrer la date actuelle (de type JJ/MM/AAAA):\n");
                printf("Date acutelle :");
                scanf("%d/%d/%d", &tab_cl[i].date_exit.tm_mday, &tab_cl[i].date_exit.tm_mon, &tab_cl[i].date_exit.tm_year);
                if (tab_cl[i].date_exit.tm_year < tab_cl[i].date_enter.tm_year || 
                    (tab_cl[i].date_exit.tm_year == tab_cl[i].date_enter.tm_year && 
                     tab_cl[i].date_exit.tm_mon < tab_cl[i].date_enter.tm_mon) ||
                    (tab_cl[i].date_exit.tm_year == tab_cl[i].date_enter.tm_year &&
                     tab_cl[i].date_exit.tm_mon == tab_cl[i].date_enter.tm_mon &&
                     tab_cl[i].date_exit.tm_mday < tab_cl[i].date_enter.tm_mday))
                {
                    printf("La date est incorrecte.\n");
                    break;
                }
                 
                printf("Saisissez l'heure de sortie (de type HH:MM):\n ");
                printf("Heure de sortie");
                scanf("%d:%d", &tab_cl[i].heure_sortie.tm_hour, &tab_cl[i].heure_sortie.tm_min);
                tab_cl[i].date_enter.tm_year -= 1900;
                tab_cl[i].date_enter.tm_mon -= 1;
                tab_cl[i].date_exit.tm_year -= 1900;
                tab_cl[i].date_exit.tm_mon -= 1;
                // mise à jour des valeur de champs heure et minutes ici les seconde ne sont pas pris en compte.
                struct tm h_entrer = tab_cl[i].date_enter;
                h_entrer.tm_hour = tab_cl[i].heure_entrer.tm_hour;
                h_entrer.tm_min = tab_cl[i].heure_entrer.tm_min;
                struct tm h_sortie = tab_cl[i].date_exit;
                h_sortie.tm_hour = tab_cl[i].heure_sortie.tm_hour;
                h_sortie.tm_min = tab_cl[i].heure_sortie.tm_min;
                time_t h_entrer_date = mktime(&h_entrer);
                time_t h_sortie_date = mktime(&h_sortie);
                diff_seconde = difftime(h_sortie_date, h_entrer_date);
                diff_heure = diff_seconde/ 3600.0;
                if (diff_heure > 0)
                {
                    heures_arrondi =  ceil(diff_heure);
                }
                else
                {
                    heures_arrondi = 1;
                }
                tab_cl[i].prix = heures_arrondi * 4;
                printf("Vous devez payer %d euros.\n", tab_cl[i].prix);
                printf("Entrez l'identifiant du parking :\n");
                printf("Identifiant du parking :");
                scanf("%s", park);
                while (j < NB_PARKING)
                {
                    if (strcmp(tab[j].identifiant, park) == 0) 
                    {
                        index_parking = j;
                        break;
                    }
                    j++;
                }
                if (index_parking != -1) 
                {
                    place = atoi(tab[index_parking].places_disponibles);
                    place++;  
                    sprintf(tab[index_parking].places_disponibles, "%d", place); 
                    printf("Une place a ete liberee. Il y a maintenant %d places disponibles dans le parking.\n", place);
                }
                else
                {
                    printf("Le parking est introuvable.\n");
                }
                sauvegarderEtatParking(tab); 
                break;
            }
            else
            {
                break;
            }
        }
        else
        {
            i++;
        }
    }
} 

//Permet de comparer le mot de passe hasher et le mot de passe donné par l'utilisateur.
int compare_hashes(unsigned char *hash1, unsigned char *hash2)
{
    return memcmp(hash1, hash2, SHA256_DIGEST_LENGTH);
}

//Permet d'acceder au mode administrateur en entrant le mot de passe.
int modeAdministrateur(parking *tab)
{
    char mdp_2[50];
    char park[50];
    char champs[50];
    int i = 0;
    int trouve = 0; 
    const char *password = "Bonjour";
    unsigned char mdp_hash[SHA256_DIGEST_LENGTH];
    hash_password(password, mdp_hash); 
    printf("Si vous etes l administrateur, entrez le mot de passe :\n");
    printf("Mot de passe :");
    scanf("%s", mdp_2);
    unsigned char mdp_2_hash[SHA256_DIGEST_LENGTH];
    hash_password(mdp_2, mdp_2_hash);
    if (compare_hashes(mdp_hash, mdp_2_hash) == 0)
    {
        printf("Bienvenue dans le mode administrateur.\n"); 
        printf("Entrer l'identifiant du parking dont vous voulez modifier les informations :\n");
        printf ("Entrer l'identifiant parking :");
        scanf("%s", park);

        for (i = 0; i < NB_PARKING; i++)
        {
            if (strcmp(tab[i].identifiant, park) == 0)
            {
                trouve = 1;
                printf("Entrer le champ que vous souhaitez modifier (ex: Nom, Adresse, Ville, etc.) :\n");
                printf("champs de la modification :");
                scanf(" %[^\n]", champs); 
                if (strcmp(champs, "Identifiant") == 0)
                {
                    printf("Entrer le nouvel identifiant : ");
                    scanf(" %[^\n]", tab[i].identifiant);
                }
                else if (strcmp(champs, "Nom") == 0)
                {
                    printf("Entrer le nouveau nom : ");
                    scanf(" %[^\n]", tab[i].nom);
                }
                else if (strcmp(champs, "Adresse") == 0)
                {
                    printf("Entrer la nouvelle adresse : ");
                    scanf(" %[^\n]", tab[i].adresse);
                }
                else if (strcmp(champs, "Ville") == 0)
                {
                    printf("Entrer la nouvelle ville : ");
                    scanf(" %[^\n]", tab[i].ville);
                }
                else if (strcmp(champs, "État") == 0 || strcmp(champs, "Etat") == 0)
                {
                    printf("Entrer le nouvel état : ");
                    scanf(" %[^\n]", tab[i].etat);
                }
                else if (strcmp(champs, "Places disponibles") == 0)
                {
                    printf("Entrer la nouvelle disponibilité : ");
                    scanf(" %[^\n]", tab[i].places_disponibles);
                }
                else if (strcmp(champs, "Capacité max") == 0 || strcmp(champs, "Capacite max") == 0)
                {
                    printf("Entrer la nouvelle capacité max : ");
                    scanf(" %[^\n]", tab[i].capacite_max);
                }
                else if (strcmp(champs, "Date de mise à jour") == 0)
                {
                    printf("Entrer la nouvelle date de mise à jour : ");
                    scanf(" %[^\n]", tab[i].mise_a_jour);
                }
                else if (strcmp(champs, "Affichage panneaux") == 0)
                {
                    printf("Entrer le nouvel affichage panneau : ");
                    scanf(" %[^\n]", tab[i].affichage_panneaux);
                }
                else
                {
                    printf("L'entrée du champ est invalide.\n");
                    return 0;
                }
                FILE *fichier = fopen("C:/Users/DUBALLET/Desktop/parking-metropole.csv", "w");
                if (fichier == NULL)
                {
                    printf("Erreur d'ouverture du fichier.\n");
                    exit(1);
                }
                fprintf(fichier, "Identifiant;Nom;Adresse;Ville;Etat;Places_disponibles;Capacite_max;Mise_a_jour;Affichage_panneaux\n");
                for (int j = 0; j < NB_PARKING; j++)
                {
                    fprintf(fichier, "%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
                            tab[j].identifiant,
                            tab[j].nom,
                            tab[j].adresse,
                            tab[j].ville,
                            tab[j].etat,
                            tab[j].places_disponibles,
                            tab[j].capacite_max,
                            tab[j].mise_a_jour,
                            tab[j].affichage_panneaux);
                }
                fclose(fichier);
                printf("La modification a ete enregistree avec succes.\n");
                break;
            }
        }
        if (trouve == 0)
        {
            printf("L'identifiant n a pas ete trouve.\n");
        }
    }
    else
    {
        printf("Mot de passe incorrect.\n");
    }
    return 0;
}

//Vérifie si une voiture peut rentrer dans le parking en fonction des places.
int verifierPlacesDisponibles (parking *tab)
{
    int i = 1;
    char name[50];
    printf("Donnez l'identifiant du parking : ");
    scanf("%s", name);
    while (i < NB_PARKING && strcmp(tab[i].identifiant, name) != 0)
    {
        if ((atoi(tab[i].capacite_max) - atoi(tab[i].places_disponibles)) == 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
        i++;
    }
}

//Permet d'enregistrer les informations clients dans un fichier CSV.
//Une seul entrer parce que sinon mon pc ne peut pas ouvrir le fichier.
void suiviClient(clients* tab_cl)
{
    int i = 0;
    FILE *file = fopen("C:/Users/DUBALLET/Desktop/clients.csv", "w");
    if (file == NULL)
    {
        printf("Impossible d'ouvrir le fichier clients.\n");
        exit (1);
    }
    fprintf(file, "Plaque d'immatriculation; Montant du prix; Heure d'entrée; Heure de sortie\n");
    while(i < nb_personne)
    {
        if(num_clients[i] != 0)
        {
            fprintf(file,"%s;", tab_cl[i].plaque_immatriculation);
            fprintf(file,"%d;", tab_cl[i].prix);
            if (tab_cl[i].heure_entrer.tm_min < 10 || tab_cl[i].heure_entrer.tm_hour >= 10)
            {
                fprintf(file,"%s;", tab_cl[i].plaque_immatriculation);
                fprintf(file,"%d;", tab_cl[i].prix);
                fprintf(file,"%d:%d;", tab_cl[i].heure_entrer.tm_hour, tab_cl[i].heure_entrer.tm_min);
                fprintf(file,"%d:%d\n", tab_cl[i].heure_sortie.tm_hour, tab_cl[i].heure_sortie.tm_min);
            break;
            }
        }
        fclose(file);
    }
}

//Programme principal pour appeler les fonctions avec un menu.
int main()
{
    float taux_occupation;
    int choix = -1;
    int i;
    lesparkings();
    printf("Bonjour, bienvenue sur l'application.\n");
    printf("Aujourdhui le nombre total de parking est de %d.\n",NB_PARKING);
    while (choix != 0)
    {
        printf("Voici les choix possible sur cette application: choisissez celle que vous voulez utiliser\n"
               "- 0 vous permet de quitter le menu.\n" 
               "- 1 afficher un parking a l'aide son identifiant.\n"
               "- 2 afficher tous les parkings.\n"
               "- 3 permet de vous enregistrer dans un parking parmis les choix disponibles dans l'option 2.\n"
               "- 4 permet de vous faire sortir du parking et de connaitre le prix a payer.\n"
               "- 5 seul l'administrateur peut y acceder avec le mode de passe.\n"
               "- 6 vous permet de connaitre en temps reel la disponibilite d'un parking.\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        if(choix == 1)
        {
            afficheparking(tab);
        }
        else if(choix == 2)
        {
            afficheparkings(tab);
        }
        else if(choix == 3)
        {
            entrerParking(tab_cl, num_clients, tab);
            suiviClient(tab_cl);
        }
        else if(choix == 4)
        {
            sortieParking(tab_cl, num_clients, tab);
            suiviClient(tab_cl);
        }
        else if(choix == 5)
        {
        
            modeAdministrateur(tab);
        }
        else if(choix == 6)
        {
            verifierPlacesDisponibles(tab);
            if (verifierPlacesDisponibles (tab) == 0)
            {
                printf("Il y a encore de la places dans ce parking.\n");
            }
            else
            {
                printf("Il n'y a plus de places disponibles.\n");
            }
        }
        else if (choix == 0)
        {
            printf("Bonne journee, a bientot.");
        }
        else
        {
            printf("Le choix n'est pas dans les options recommande.\n");
        }
    }
    return 0;
}