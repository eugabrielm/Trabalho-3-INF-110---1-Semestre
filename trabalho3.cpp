// INF110 - Trabalho pratico 3
//
// programa para ler, modificar e gravar uma imagem no formato PNM
//
// Autores: Andre Gustavo dos Santos			(criado em 16/06/14)
//          Andre Gustavo dos Santos			(modificado em 22/05/18)
//					Andre Gustavo dos Santos			(modificado em 13/09/21)
//					Andre Gustavo dos Santos			(modificado em 25/06/22)

// Aluno: Gabriel Moreira Marques      				Matricula : 108207

#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

const int MAXALTURA = 700; // tamanho maximo aceito (pode ser alterado)
const int MAXLARGURA = 700;

using namespace std;

int main()
{
	unsigned char imagem[MAXALTURA + 1][MAXLARGURA + 1]; // a imagem propriamente dita (soma 1 para poder começar de i=1)
	int largura, altura;								 // dimensoes da imagem
	char tipo[4];										 // tipo da imagem
	ifstream arqentrada;								 // arquivo que contem a imagem original
	ofstream arqsaida;									 // arquivo que contera a imagem modificada
	char comentario[200], c;							 // auxiliares
	int i, j, n, m, k, valor;							 // auxiliares
	char nomearq[100];
	unsigned char imagemc[MAXALTURA + 1][MAXLARGURA + 1][3]; // imagem colorida
	int soma, gx, gy;										 // auxiliares
	char nomearq2[100];										 // variavel que armazena o nome da nova imagem

	//*** LEITURA DA IMAGEM ***//
	// inicialmente nao sera necessario entender nem mudar nada nesta parte

	//*** Abertura do arquivo ***//
	cout << "Nome do arquivo com a imagem PNM: ";
	cin >> nomearq;
	arqentrada.open(nomearq, ios::in); // Abre arquivo para leitura
	if (!arqentrada)
	{
		cout << "Nao consegui abrir arquivo " << nomearq << endl;
		return 0;
	}
	//***************************//
	cout << "Digite um valor para escolher o filtro: <<" << endl;
	cout << "1 - Escurecer\n 2 - Clarear \n 3 - Negativo\n 4 - Espelhar\n 5 - Filtro de Sobel\n 6 - Realçar\n 7 - Tons de Cinza\n 8 - Filtro Extra\n";
	cin >> n;

	//*** Leitura do cabecalho ***//
	arqentrada >> tipo; // Le o tipo de arquivo
	arqentrada.get();	// Le e descarta o \n do final da 1a. linha

	if (strcmp(tipo, "P2") == 0)
	{
		cout << "Imagem em tons de cinza\n";
	}
	else if (strcmp(tipo, "P3") == 0)
	{
		cout << "Imagem colorida\n"; // Reconhece a imagem P3
	}
	else if (strcmp(tipo, "P1") == 0)
	{
		cout << "Imagem preto e branco\n";
		cout << "Desculpe, nao trabalho com esse tipo de imagem.\n";
		arqentrada.close();
		return 0;
	}
	else if (strcmp(tipo, "P4") == 0 || strcmp(tipo, "P5") == 0 || strcmp(tipo, "P6") == 0)
	{
		cout << "Imagem no formato RAW\n";
		cout << "Desculpe, nao trabalho com esse tipo de imagem.\n";
		arqentrada.close();
		return 0;
	}
	if (strcmp(tipo, "P2") == 0)
	{
		while ((c = arqentrada.get()) == '#')	 // Enquanto for comentario
			arqentrada.getline(comentario, 200); // Le e descarta a linha "inteira"

		arqentrada.putback(c); // Devolve o caractere lido para a entrada, pois como
							   // nao era comentario, era o primeiro digito da largura

		arqentrada >> largura >> altura; // Le as dimensoes da imagem, numero de pixels da horizontal e da vertical
		cout << "Tamanho: " << largura << " x " << altura << endl;
		if (largura > MAXLARGURA)
		{
			cout << "Desculpe, ainda nao trabalho com imagens com mais de " << MAXLARGURA << " pixels de largura.\n";
			arqentrada.close();
			return 0;
		}
		if (altura > MAXALTURA)
		{
			cout << "Desculpe, ainda nao trabalho com imagens com mais de " << MAXALTURA << " pixels de altura.\n";
			arqentrada.close();
			return 0;
		}

		arqentrada >> valor; // Valor maximo do pixel (temos que ler, mas nao sera usado)
		//****************************//

		//*** Leitura dos pixels da imagem ***//
		for (i = 1; i <= altura; i++)
			for (j = 1; j <= largura; j++)
			{
				arqentrada >> valor;
				imagem[i][j] = (unsigned char)valor;
			}
		//************************************//

		arqentrada.close(); // Fecha arquivo apos a leitura
	}
	//*** FIM DA LEITURA DA IMAGEM DO TIPO P2***//

	if (strcmp(tipo, "P3") == 0) // Lê as dimensões da imagem colorida
	{
		while ((c = arqentrada.get()) == '#')	 // Enquanto for comentario
			arqentrada.getline(comentario, 200); // Le e descarta a linha "inteira"

		arqentrada.putback(c);			 // Devolve o caractere lido para a entrada, pois como
										 // nao era comentario, era o primeiro digito da largura
		arqentrada >> largura >> altura; // Le as dimensoes da imagem, numero de pixels da horizontal e da vertical
		cout << "Tamanho: " << largura << " x " << altura << endl;
		if (largura > MAXLARGURA)
		{
			cout << "Desculpe, ainda nao trabalho com imagens com mais de " << MAXLARGURA << " pixels de largura.\n";
			arqentrada.close();
			return 0;
		}
		if (altura > MAXALTURA)
		{
			cout << "Desculpe, ainda nao trabalho com imagens com mais de " << MAXALTURA << " pixels de altura.\n";
			arqentrada.close();
			return 0;
		}
		arqentrada >> valor; // Valor maximo do pixel (temos que ler, mas nao sera usado)
		//****************************//
		//*** Leitura dos pixels da imagem ***//
		for (i = 1; i <= altura; i++)
			for (j = 1; j <= largura; j++)
				for (k = 0; k < 3; k++)
				{
					arqentrada >> valor;
					imagemc[i][j][k] = (unsigned char)valor;
				}
		//************************************//
		arqentrada.close(); // Fecha arquivo apos a leitura
	}
	//***   FIM DA LEITURA DA IMAGEM DO TIPO P3     ***//

	//*** TRATAMENTO DA IMAGEM ***//

	// Filtros

	if (n == 1)
	{ // Escolhe o filtro
		//*** Escurece a imagem ***//
		int fator;
		cout << "Qual o fator de escurecimento (1-100)? ";
		cin >> fator;

		if (strcmp(tipo, "P2") == 0)
		{
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
				{
					valor = (int)imagem[i][j];			 // pega o valor do pixel
					valor -= fator;						 // escurece o pixel
					if (valor < 0)						 // se der negativo
						valor = 0;						 //  deixa preto
					imagem[i][j] = (unsigned char)valor; // modifica o pixel
				}
			//*************************//
		}
		if (strcmp(tipo, "P3") == 0) // Escurece fotos coloridas;
		{
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
					for (k = 0; k < 3; k++)
					{
						valor = (int)imagemc[i][j][k];			 // pega o valor do pixel
						valor -= fator;							 // escurece o pixel
						if (valor < 0)							 // se der negativo
							valor = 0;							 //  deixa preto
						imagemc[i][j][k] = (unsigned char)valor; // modifica o pixel
					}
		}
		//*** FIM DO TRATAMENTO DA IMAGEM PARA ESCURECER A IMAGEM***//
	}
	if (n == 2)
	{
		//*** CLAREAR A IMAGEM  ***/
		// CLAREAR TIPO P2
		int fator;
		cout << "Qual o fator de clareamento (1-100)? ";
		cin >> fator;

		if (strcmp(tipo, "P2") == 0)
		{
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
				{
					valor = (int)imagem[i][j];			 // pega o valor do pixel
					valor += fator;						 // clareia o pixel
					if (valor > 255)					 // se der valor maior que 255
						valor = 255;					 //  deixa branco
					imagem[i][j] = (unsigned char)valor; // modifica o pixel
				}
			//*************************//
		}
		if (strcmp(tipo, "P3") == 0) // Clareia fotos coloridas
		{
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
					for (k = 0; k < 3; k++)
					{
						valor = (int)imagemc[i][j][k];			 // pega o valor do pixel
						valor += fator;							 // clareia o pixel
						if (valor > 255)						 // se der maior que 255
							valor = 255;						 //  deixa branco
						imagemc[i][j][k] = (unsigned char)valor; // modifica o pixel
					}
		}
	}

	// NEGATIVO
	if (n == 3)
	{

		if (strcmp(tipo, "P2") == 0)
		{
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
				{
					valor = 255 - (int)imagem[i][j]; // pega o valor do pixel

					imagem[i][j] = (unsigned char)valor; // modifica o pixel
				}
			//*************************//
		}

		if (strcmp(tipo, "P3") == 0) // negativa fotos coloridas
		{
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
					for (k = 0; k < 3; k++)
					{
						valor = 255 - (int)imagemc[i][j][k];

						imagemc[i][j][k] = (unsigned char)valor; // modifica o pixel
					}
		}
	}

	// ESPELHAR
	if (n == 4)
	{
		if (strcmp(tipo, "P2") == 0) // Para imagens do tipo P2
		{
			int v[altura + 1][largura + 1];
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
					v[i][j] = (int)imagem[i][j]; // o vetor v recebe os pixels
			for (i = 1; i <= altura; i++)
				for (j = 1, k = largura; j <= largura && k >= 1; j++, k--) // k = largura
					imagem[i][j] = v[i][k];
		}

		if (strcmp(tipo, "P3") == 0) //
		{
			int v[altura + 1][largura + 1][3];
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
					for (k = 0; k < 3; k++)
						v[i][j][k] = (int)imagemc[i][j][k]; // o vetor v recebe os pixels

			for (i = 1; i <= altura; i++)
				for (j = 1, m = largura; j <= largura && m >= 1; j++, m--)
					for (k = 0; k < 3; k++)
						imagemc[i][j][k] = v[i][m][k];
		}
	}

	// FILTRO DE SOBEL
	if (n == 5)
	{
		cout << "Qual variação do filtro de sobel você deseja?:\n 1 - Média Aritmética\n 2 - Magnitude de Gradiente\n";
		int s;
		cin >> s;
		while (s != 1 && s != 2)
		{
			cout << "Digite 1 ou 2 , por favor" << endl;
			cin >> s;
		}

		if (strcmp(tipo, "P2") == 0)
		{ //
			int v[altura + 1][largura + 1];

			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)

					v[i][j] = (int)imagem[i][j];
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)

				{
					gy = (v[i][j] * 0) + (v[i - 1][j] * 2) + (v[i - 1][j - 1] * 1) + (v[i - 1][j + 1] * 1) + (v[i + 1][j] * -2) + (v[i + 1][j - 1] * -1) + (v[i + 1][j + 1] * -1);
					gx = (v[i][j] * 0) + (v[i][j - 1] * 2) + (v[i][j + 1] * -2) + (v[i - 1][j - 1] * 1) + (v[i - 1][j + 1] * -1) + (v[i + 1][j - 1] * 1) + (v[i + 1][j + 1] * -1);
					if (s == 1)
						soma = (gx + gy) / 2;
					if (s == 2)
						soma = (sqrt(gx * gx + gy * gy));
					if (soma < 0)
						soma = 0;
					if (soma > 255)
						soma = 255;
					imagem[i][j] = (unsigned char)soma;
				}
		}

		if (strcmp(tipo, "P3") == 0)
		{ //
			int v[altura + 1][largura + 1][3];

			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
					for (k = 0; k < 3; k++)
						v[i][j][k] = (int)imagemc[i][j][k];
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
					for (k = 0; k < 3; k++)
					{
						gy = (v[i][j][k] * 0) + (v[i - 1][j][k] * 2) + (v[i - 1][j - 1][k] * 1) + (v[i - 1][j + 1][k] * 1) + (v[i + 1][j][k] * -2) + (v[i + 1][j - 1][k] * -1) + (v[i + 1][j + 1][k] * -1);
						gx = (v[i][j][k] * 0) + (v[i][j - 1][k] * 2) + (v[i][j + 1][k] * -2) + (v[i - 1][j - 1][k] * 1) + (v[i - 1][j + 1][k] * -1) + (v[i + 1][j - 1][k] * 1) + (v[i + 1][j + 1][k] * -1);
						if (s == 1)
							soma = (gx + gy) / 2;
						if (s == 2)
							soma = (sqrt(gx * gx + gy * gy));
						if (soma < 0)
							soma = 0;
						if (soma > 255)
							soma = 255;
						imagemc[i][j][k] = (unsigned char)soma;
					}
		}
	}

	// Filtro de Realce
	if (n == 6)
	{
		if (strcmp(tipo, "P2") == 0)
		{ //
			int v[altura + 1][largura + 1];

			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)

					v[i][j] = (int)imagem[i][j];
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)

				{
					gy = (v[i][j] * 5) + (v[i - 1][j] * -1) + (v[i][j - 1] * -1) + (v[i][j + 1] * -1) + (v[i + 1][j] * -1);

					if (gy < 0)
						gy = 0;
					if (gy > 255)
						gy = 255;
					imagem[i][j] = (unsigned char)gy;
				}
		}

		if (strcmp(tipo, "P3") == 0)
		{ //
			int v[altura + 1][largura + 1][3];

			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
					for (k = 0; k < 3; k++)
						v[i][j][k] = (int)imagemc[i][j][k];
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
					for (k = 0; k < 3; k++)
					{
						gy = (v[i][j][k] * 5) + (v[i - 1][j][k] * -1) + (v[i][j - 1][k] * -1) + (v[i][j + 1][k] * -1) + (v[i + 1][j][k] * -1);

						if (gy < 0)
							gy = 0;
						if (gy > 255)
							gy = 255;
						imagemc[i][j][k] = (unsigned char)gy;
					}
		}
	}

	// TONS DE CINZA
	if (n == 7)
	{

		if (strcmp(tipo, "P3") == 0)
		{ //
			int v[altura + 1][largura + 1][3];

			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
					for (k = 0; k < 3; k++)
					{
						v[i][j][k] = ((int)imagemc[i][j][0] + (int)imagemc[i][j][1] + (int)imagemc[i][j][2]) / 3;
						if (valor > 255)
							valor = 255;
						if (valor < 0)
							valor = 0;
					}
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
					for (k = 0; k < 3; k++)
						imagemc[i][j][k] = (unsigned char)v[i][j][k];
		}
	}
	// Operação Extra
	if (n == 8)
	{
		if (strcmp(tipo, "P2") == 0)
		{ //
			int v[altura + 1][largura + 1];

			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)

					v[i][j] = (int)imagem[i][j];
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)

				{
					gy = (v[i][j] * -5) + (v[i][j] * 2) + (v[i - 1][j] * 5) + (v[i + 1][j] * -5) + (v[i][j - 1] * 2) + (v[i][j + 1] * 5);
					if (gy < 0)
						gy = 0;
					if (gy > 255)
						gy = 255;
					imagem[i][j] = (unsigned char)gy;
				}
		}

		if (strcmp(tipo, "P3") == 0)
		{ //
			int v[altura + 1][largura + 1][3];

			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
					for (k = 0; k < 3; k++)
						v[i][j][k] = (int)imagemc[i][j][k];
			for (i = 1; i <= altura; i++)
				for (j = 1; j <= largura; j++)
					for (k = 0; k < 3; k++)
					{
						gy = (v[i][j][0] * -5) + (v[i][j][1] * 2) + (v[i - 1][j][2] * 5) + (v[i + 1][j][0] * -5) + (v[i][j - 1][1] * 2) + (v[i][j + 1][2] * 5);
						if (gy < 0)
							gy = 0;
						if (gy > 255)
							gy = 255;
						imagemc[i][j][k] = (unsigned char)gy;
					}
		}
	}

	//*** GRAVACAO DA IMAGEM ***//
	// inicialmente nao sera necessario entender nem mudar nada nesta parte

	//*** Grava a nova imagem ***//
	cout << "Insira o nome da imagem com .pnm no final\n";
	cin >> nomearq2;
	arqsaida.open(nomearq2, ios::out); // Abre arquivo para escrita
	if (!arqsaida)
	{
		cout << "Nao consegui criar " << nomearq2 << endl;
		return 0;
	}

	arqsaida << tipo << endl;			  // tipo
	arqsaida << "# TP3-INF110, by AGS\n"; // comentario
	arqsaida << largura << " " << altura; // dimensoes
	arqsaida << " " << 255 << endl;

	if (strcmp(tipo, "P2") == 0) // maior valor
		for (i = 1; i <= altura; i++)
			for (j = 1; j <= largura; j++)
				arqsaida << (int)imagem[i][j] << endl; // pixels

	if (strcmp(tipo, "P3") == 0)
	{

		for (i = 1; i <= altura; i++)
			for (j = 1; j <= largura; j++)
				for (k = 0; k < 3; k++)
					arqsaida << (int)imagemc[i][j][k] << endl; // pixels
	}
	cout << "#############################\nImagem criada com sucesso!!!!\n#############################";
	arqsaida.close(); // fecha o arquivo
	//***************************//

	//*** FIM DA GRAVACAO DA IMAGEM ***//

	return 0;
}
