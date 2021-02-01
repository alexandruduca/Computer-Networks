#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#define LINII 6
#define COLOANE 7
void Culoare(int tura)
{
  if (tura == 1)
    printf("In urma randomizarii, v-a fost asociata culoarea ROSU.\n");
  else
    printf("In urma randomizarii, v-a fost asociata culoarea GALBEN.\n");
  fflush(stdout);
}
void AfisareMatrice(int matrice[LINII][COLOANE])
{
  printf("A B C D X Y Z");
  printf("\n");
  printf("_____________");
  printf("\n");
  for (int i = 0; i < LINII; i++)
  {
    for (int j = 0; j < COLOANE; j++)
    {
      if (matrice[i][j] == 0)
        printf("_ ");
      else if (matrice[i][j] == 1)
        printf("R ");
      else if (matrice[i][j] == 2)
        printf("G ");
    }
    printf("\n");
  }
}
void MesajJucator(int tura, int retinetura)
{
  if (tura == retinetura)
    printf("Introduceti o coloana: ");
  else
    printf("Nu este randul dumneavoastra. ");
  fflush(stdout);
}
void MesajFinal(int Castigator, int retinetura)
{
  if (Castigator == retinetura)
    printf("Felicitari! Ati castigat! Daca doriti sa mai jucati, scrieti DA, altfel scrieti orice altceva: ");
  else
    printf("Ati pierdut! Daca doriti sa mai jucati, scrieti DA, altfel scrieti orice altceva: ");
  fflush(stdout);
}
void AfisareScor(int ScorPlayer1, int ScorPlayer2)
{
  printf("Scorul este: %d - %d\n", ScorPlayer1, ScorPlayer2);
  fflush(stdout);
}

extern int errno;
int port;
int main(int argc, char *argv[])
{
  int sd;
  struct sockaddr_in server;
  char msg[200];
  int tura = 0;
  int matrice[LINII][COLOANE] = {0};
  char mesaj[200] = "\0";
  int retinetura = 1;
  int caz = -1;
  int Castigator = -1;
  int valoare = 0;
  int ScorPlayer1 = 0;
  int ScorPlayer2 = 0;
  char InputGresit[100] = "Ati introdus o coloana invalida. Va rugam reincercati: ";
  char InputLung[100] = "Ati introdus un input gresit. Introduceti o coloana: ";
  char ColoanaPlina[100] = "Coloana in care incercati sa introduceti este plina. Va rugam reincercati: ";
  if (argc != 3)
  {
    printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
    return -1;
  }
  port = atoi(argv[2]);
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Eroare la socket().\n");
    return errno;
  }
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons(port);
  if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("[client]Eroare la connect().\n");
    return errno;
  }

  bzero(msg, 100);
  printf("Introduceti un username: ");
  fflush(stdout);
  read(0, msg, 100);
  msg[strcspn(msg, "\n")] = 0;
  write(sd, msg, 100);
rejoaca:
  for (int i = 0; i < LINII; i++)
    for (int j = 0; j < COLOANE; j++)
      matrice[i][j] = 0;
  Castigator = -1;
  read(sd, msg, 100);
  printf("Adversarul dumneavoastra este %s. ", msg);
  fflush(stdout);

  read(sd, &tura, sizeof(int));
  Culoare(tura);
  retinetura = tura;

  while (1)
  {
    caz = -1;
    printf("\n\n");
    fflush(stdout);
    read(sd, &tura, sizeof(int));
    AfisareScor(ScorPlayer1, ScorPlayer2);
    printf("\n");
    AfisareMatrice(matrice);
    fflush(stdout);
    if (Castigator != -1)
    {
      MesajFinal(Castigator, retinetura);
      read(0, msg, 100);
      msg[strcspn(msg, "\n")] = 0;
      write(sd, msg, 100);
      if (strcmp(msg, "DA") == 0)
        goto rejoaca;

      break;
    }
    MesajJucator(tura, retinetura);
    fflush(stdout);

    while (tura == retinetura)
    {
      bzero(msg, 100);
      read(0, msg, 100);
      msg[strcspn(msg, "\n")] = 0;
      write(sd, msg, 100);

      read(sd, &caz, sizeof(int));
      if (caz == 1)
      {
        printf("\n");
        fflush(stdout);
        AfisareMatrice(matrice);
        fflush(stdout);
        printf("%s", InputLung);
        fflush(stdout);
      }
      else if (caz == 2)
      {
        printf("\n");
        fflush(stdout);
        AfisareMatrice(matrice);
        fflush(stdout);
        printf("%s", InputGresit);
        fflush(stdout);
      }
      else if (caz == 3)
      {
        printf("\n");
        fflush(stdout);
        AfisareMatrice(matrice);
        fflush(stdout);
        printf("%s", ColoanaPlina);
        fflush(stdout);
      }
      else if (caz == 0)
        break;
    }
    for (int i = 0; i < LINII; i++)
      for (int j = 0; j < COLOANE; j++)
      {
        read(sd, &valoare, sizeof(int));
        matrice[i][j] = valoare;
      }

    read(sd, &Castigator, sizeof(int));
    read(sd, &ScorPlayer1, sizeof(int));
    read(sd, &ScorPlayer2, sizeof(int));
  }
  close(sd);
}