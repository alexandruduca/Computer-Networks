#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define LINII 6
#define COLOANE 7
int ConversieInput(char input[20])
{
	input[0] = tolower(input[0]);
	if (strcmp(input, "a") == 0)
		return 0;
	if (strcmp(input, "b") == 0)
		return 1;
	if (strcmp(input, "c") == 0)
		return 2;
	if (strcmp(input, "d") == 0)
		return 3;
	if (strcmp(input, "x") == 0)
		return 4;
	if (strcmp(input, "y") == 0)
		return 5;
	if (strcmp(input, "z") == 0)
		return 6;

	return -1;
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
void AdaugareMutare(int matrice[LINII][COLOANE], int col, int tura)
{
	for (int i = LINII - 1; i >= 0; i--)
	{
		if (matrice[i][col] == 0)
		{
			matrice[i][col] = tura;
			break;
		}
	}
}
int SchimbaTura(int tura)
{
	if (tura == 1)
		return 2;
	return 1;
}
int Verificare(int matrice[LINII][COLOANE])
{
	for (int i = 0; i < LINII; i++)
		for (int j = 0; j < COLOANE; j++)
		{
			if (matrice[i][j] != 0)
			{
				if (matrice[i][j] == matrice[i][j + 1] && matrice[i][j + 1] == matrice[i][j + 2] && matrice[i][j + 2] == matrice[i][j + 3] && j + 3 < COLOANE)
					return matrice[i][j]; // verificare linie

				if (matrice[i][j] == matrice[i + 1][j] && matrice[i + 1][j] == matrice[i + 2][j] && matrice[i + 2][j] == matrice[i + 3][j] && i + 3 < LINII)
					return matrice[i][j]; // verificare linie

				if (matrice[i][j] == matrice[i + 1][j + 1] && matrice[i + 1][j + 1] == matrice[i + 2][j + 2] && matrice[i + 2][j + 2] == matrice[i + 3][j + 3] && i + 3 < LINII && j + 3 < COLOANE)
					return matrice[i][j]; // verificare diagonala principala

				if (matrice[i][j] == matrice[i + 1][j - 1] && matrice[i + 1][j - 1] == matrice[i + 2][j - 2] && matrice[i + 2][j - 2] == matrice[i + 3][j - 3] && i + 3 < LINII && j - 3 >= 0)
					return matrice[i][j]; // verificare diagonala secundara
			}
		}
	return -1;
}
int VerificareColoana(int matrice[LINII][COLOANE], int col)
{
	if (matrice[0][col] != 0)
		return -1; // coloana plina
	return 0;
}
int RandomCuloare()
{
	srand(time(NULL));
	int r = rand();
	if (r % 2 == 0)
		return 1; // 1 pentru Rosu
	else
		return 2; // 2 pentru Galben
}
#define PORT 3333
extern int errno;
int main()
{
	struct sockaddr_in server;
	struct sockaddr_in from;
	int sd;
	int optval = 1;
	int matrice[LINII][COLOANE] = {0};
	char player1[100] = "\0";
	char player2[100] = "\0";
	char player11[100] = "\0";
	char player22[100] = "\0";
	char input[100] = "\0";
	int tura = 0;
	int turaClient1 = 0;
	int turaClient2 = 0;
	int ok = 0;
	int coloana = 0;
	int caz = 0;
	int Castigator = -1;
	int ScorPlayer1 = 0;
	int ScorPlayer2 = 0;

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("[server]Eroare la socket().\n");
		return errno;
	}
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	bzero(&server, sizeof(server));
	bzero(&from, sizeof(from));

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);
	if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		perror("[server]Eroare la bind().\n");
		return errno;
	}
	if (listen(sd, 1) == -1)
	{
		perror("[server]Eroare la listen().\n");
		return errno;
	}
	while (1)
	{
		int client1, client2;
		int length = sizeof(from);

		printf("[server]Asteptam la portul %d...\n", PORT);
		fflush(stdout);

		client1 = accept(sd, (struct sockaddr *)&from, &length);
		client2 = accept(sd, (struct sockaddr *)&from, &length);

		if (client1 < 0)
		{
			perror("[server]Eroare la accept().\n");
			continue;
		}
		if (client2 < 0)
		{
			perror("[server]Eroare la accept().\n");
			continue;
		}

		int pid;
		if ((pid = fork()) == -1)
		{
			close(client1);
			close(client2);
			continue;
		}
		else if (pid > 0)
		{
			// parinte
			close(client1);
			close(client2);
			continue;
		}
		else if (pid == 0)
		{
			// copil
			close(sd);
			bzero(player1, 100);
			bzero(player2, 100);
			read(client1, player1, 100);
			read(client2, player2, 100);
		rejoaca:
			for (int i = 0; i < LINII; i++)
				for (int j = 0; j < COLOANE; j++)
					matrice[i][j] = 0;
			Castigator = -1;
			tura = RandomCuloare();
			write(client1, player2, 100);
			write(client2, player1, 100);

			write(client1, &tura, sizeof(int));
			turaClient1 = tura;
			tura = SchimbaTura(tura);
			write(client2, &tura, sizeof(int));
			turaClient2 = tura;

			tura = 1;
			while (1)
			{
				bzero(input, 100);
				fflush(stdout);
				caz = -1;
				AfisareMatrice(matrice);
				printf("\n");
				fflush(stdout);
				write(client1, &tura, sizeof(int));
				write(client2, &tura, sizeof(int));
				if (Castigator != -1)
				{
					read(client1, player11, 100);
					read(client2, player22, 100);
					if (strcmp(player11, "DA") == 0 && strcmp(player22, "DA") == 0)
						goto rejoaca;
				}

				while (tura == turaClient1 && caz != 0)
				{
					if (caz != 0)
						read(client1, input, 100);
					coloana = ConversieInput(input);
					ok = VerificareColoana(matrice, coloana);

					if (strlen(input) > 1)
						caz = 1; //cazul cand inputul este mai mare decat trebuie
					else if (coloana == -1)
						caz = 2; //cazul cand inputul nu este corect ( a, b, c, d, x, y, z)
					else if (ok == -1)
						caz = 3; //cazul cand coloana este plina
					else
						caz = 0;

					printf("caz: %d\n", caz);
					printf("input: %s\n", input);
					printf("coloana: %d\n", coloana);
					printf("plina: %d\n", ok);
					write(client1, &caz, sizeof(int));
				}
				while (tura == turaClient2 && caz != 0)
				{
					if (caz != 0)
						read(client2, input, 100);
					coloana = ConversieInput(input);
					ok = VerificareColoana(matrice, coloana);

					if (strlen(input) > 1)
						caz = 1; //cazul cand inputul este mai mare decat trebuie
					else if (coloana == -1)
						caz = 2; //cazul cand inputul nu este corect ( a, b, c, d, x, y, z)
					else if (ok == -1)
						caz = 3; //cazul cand coloana este plina
					else
						caz = 0;

					printf("caz: %d\n", caz);
					printf("input: %s\n", input);
					printf("coloana: %d\n", coloana);
					printf("plina: %d\n", ok);
					write(client2, &caz, sizeof(int));
				}

				AdaugareMutare(matrice, coloana, tura);
				for (int i = 0; i < LINII; i++)
					for (int j = 0; j < COLOANE; j++)
					{
						write(client1, &matrice[i][j], sizeof(int));
						write(client2, &matrice[i][j], sizeof(int));
					}
				Castigator = Verificare(matrice);
				write(client1, &Castigator, sizeof(int));
				write(client2, &Castigator, sizeof(int));
				if (Castigator != -1)
					if (turaClient1 == Castigator)
						ScorPlayer1++;
					else
						ScorPlayer2++;
				write(client1, &ScorPlayer1, sizeof(int));
				write(client2, &ScorPlayer1, sizeof(int));
				write(client1, &ScorPlayer2, sizeof(int));
				write(client2, &ScorPlayer2, sizeof(int));
				tura = SchimbaTura(tura);
			}
			close(client1);
			close(client2);
			exit(0);
		}

	} /* while */
} /* main */