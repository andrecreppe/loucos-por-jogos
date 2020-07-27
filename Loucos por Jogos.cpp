/*
	TRABLHO DE FUNDAMENTOS DE PROCESSAMENTOS DE DADOS (FPD) - Professora Ariane Scarelli
	3ºbim e 4ºbim/2017  
	
	LOJA:
		*LOUCOS POR GAMES ©*
	
	TURMA:
		71/81A
	
	DESENVOLVEDORES:
		ANDRÉ ZANARDI CREPPE (01)
		LUCAS SARTOR CHAUVIN (22)
		MIGUEL LAURIS TORRES (28)
*/


//BIBLIOTECAS
#include <conio.h>
#include <locale.h>
#include <tchar.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


//Setup dos arquivos
FILE *prod; //arquivo dos produtos
FILE *usr; //arquivo dos usuários


//Struct -> Produtos
struct loja {
	long int id;
	int qtd;
	float preco;
	char nome[150], plataforma[35], emp_marc[100];
	char exclusao;
	struct release {
		int mes, ano;
	}lancamento;
}produto;
//Struct -> Clientes/funcionários
struct cadastro {
	long int id, senha; //senha é de 6 dígitos
	char nome[70], email[120];
	int adm; //se sim adm==1
	struct nascimento {
		int dia, mes, ano;
	}aniversario;
	char exclusao;
}pessoa;


//Cabeçalho das funções -> Funções da Conio.c
void textcolor(int);
void gotoxy(int, int);
void HideCursor(int);


//Cabeçalho das funções -> Arquivo p/ produtos
int abrir_arqv_prod(const char[]);
int abrir_arqv_prod_casoaparte(const char[]);
//Escrita (pt1)
void campos_para_dados_prod(const char[], int);
void adicionar_prod();
int testar_id_prod();
void salvar_prod();
//Leitura (pt2)
void limpa_pesquisa_prod();
void pesquisa_simples_prod();
void pesquisa_avancada_id_prod();
int pesquisa_avancada_nome_prod();
//Alteração (pt3)
void altera_prod();
void mostra_dados_altera_prod(char[5000], int);
void digita_dados_prod(int,  int, int, float);
void excluir_prod();


//Cabeçalho das funções -> Arquivo p/ usuários
int abrir_arqv_usr(const char[]);
void criar_adm();
//Login (pt1)
int login();
//Escrita (pt2)
void campos_para_dados_usr(const char[], int);
void adicionar_usr();
int testar_id_usr();
void salvar_usr();
//Leitura (pt3)
void limpa_pesquisa_usr();
void pesquisa_simples_usr();
void pesquisa_avancada_id_usr();
int pesquisa_avancada_nome_usr();
//Alteração (pt4)
void altera_usr();
void mostra_dados_altera_usr(char[5000], int);
void digita_dados_usr(int, int, int, int, long int);
void excluir_usr();


//Cabeçalho das funções -> Estética
void loading();
void gamepad();
void limpatela();
void limpalinha();
void grade(int);
void linha(int);


//Cabeçalho das funções -> Menus
int menu_principal_adm();
int menu_principal_usr();
int menu_de_2(int, const char[], const char[], int, int);
int menu_de_2_usr(const char[], const char[], int, int, int, int);
int menu_de_3(int, const char[], const char[], const char[], int, int, int);


//Cabeçalho das funções -> Funções extras
int digita_num(int, int);
int consistencia_int(int, int, int, int, int, int);
int consistencia_int_dia(int, int, int, int);


//Variáveis Globais
int buffer; //Variáavel global usada como cache temporário de retorno de resultados das funções
char nome_usuario[70]; //Variável para aparecer o nome do usuário logado
int qtd_usr=0; //Variável global mostrando a quantidade de cadastros existentes no arquivo de USUÁRIOS
int qtd_prod=0; //Variável global mostrando a quantidade de cadastros existentes no arquivo de PRODUTOS


//Programa principal (main)
main() {
	//Setup do .exe
	system("TITLE Trabalho de FPD - Loucos Por Jogos (71A)"); //Título da aba do .exe
	system("mode con: cols=160 lines=40"); //Tamanho customizado do .exe
	system("COLOR 70"); //Cor de fundo e texto do .exe
	setlocale(LC_ALL, "Portuguese");
	criar_adm(); //Cria um adm caso não exista um arquivo de USUÁRIOS
	HideCursor(0);
	
	//Declaração de variáveis locais de MAIN
	int op, id, end=0;
	
	//Início do programa (carregamento da base estética)
	while(true) {
		id=login(); //Login da pessoa como USUÁRIO (USR) ou ADMINISTRADOR (ADM)
		loading(); //Animação de carregamento
		int tam=strlen(nome_usuario);
	 	limpatela();
		end=0;
		
		//Menu principal dos ADMINISTRADORES (ADMs)
		if(id==1) {
			while(end!=1) {
				//Contagem de cadastros
				//Produtos
				if(abrir_arqv_prod_casoaparte("rb")==0) {
					qtd_prod=0; 
					while(!feof(prod))
						if(fread(&produto, sizeof(produto), 1, prod)==1 && produto.exclusao=='n') qtd_prod++;
					fclose(prod);
				}
				else qtd_prod=0;
				//Usuários
				qtd_usr=0; 
				abrir_arqv_usr("rb");
				while(!feof(usr))
					if(fread(&pessoa, sizeof(pessoa), 1, usr)==1 && pessoa.exclusao=='n') qtd_usr++;
				fclose(usr);
				
				//Setup do menu
				textcolor(0);	
				limpatela();
				gotoxy(71,11);printf("LOUCOS POR JOGOS©"); //Nome da nossa linda empresa
				int onome=69-(tam)/2;
				gotoxy(onome,12);printf("(Logado como");
				textcolor(3);gotoxy(onome+13,12);printf("*%s* [ADM]",nome_usuario);textcolor(0);
				textcolor(0);printf(")");
				op=menu_principal_adm();
				
				//Opções do menu
				switch(op) {
					case 16: //ADICIONAR PRODUTOS
						adicionar_prod();
						break;
					
					case 17: //ADICIONAR USUÁRIOS
						adicionar_usr();
						break;
						
					case 18: //PESQUISA DE PRODUTOS
						limpatela();
						textcolor(5);gotoxy(61,10);printf("(Quantidade de PRODUTOS cadastrados: %d)",qtd_prod);textcolor(0); //contador de usuários
						gotoxy(60,16);printf("Qual o tipo de pesquisa deseja realizar?");
						buffer=menu_de_3(68, "Pesquisa Simples", "Pesquisa Avançada", "(Voltar)", 11, 15, 10);
						if(buffer==18) 	{
							pesquisa_simples_prod(); //PESQUISA SIMPLES
							break;
						}
						else if(buffer==19){ //PESQUISA AVANÇADA
							limpatela();
							textcolor(5);gotoxy(61,10);printf("(Quantidade de PRODUTOS cadastrados: %d)",qtd_prod);textcolor(0); //contador de usuários
							gotoxy(62,16);printf("Qual método de pesquisa deseja fazer?");
							buffer=menu_de_3(67, "Pesquisa por Código", "Pesquisa por Nome", "(Voltar)", 6, 9, 10);
							if (buffer==18) {
								pesquisa_avancada_id_prod(); //PESQUISA AVAÇADA POR CÓDIGO
								break;
							}
							else if (buffer==19) {
								do { 
									buffer=pesquisa_avancada_nome_prod(); //PESQUISA AVANÇADA POR NOME
								} while(buffer!=1);
								break;
							}
							else break;
						}
						else break;
					
					case 19: //PESQUISA DE USUÁRIOS	
						limpatela();
						textcolor(5);gotoxy(61,10);printf("(Quantidade de USUÁRIOS cadastrados: %d)",qtd_usr);textcolor(0); //contador de usuários
						gotoxy(60,16);printf("Qual o tipo de pesquisa deseja realizar?");
						buffer=menu_de_3(68, "Pesquisa Simples", "Pesquisa Avançada", "(Voltar)", 11, 15, 10);
						if(buffer==18) 	{
							pesquisa_simples_usr(); //PESQUISA SIMPLES
							break;
						}
						else if(buffer==19){ //PESQUISA AVANÇADA
							limpatela();
							textcolor(5);gotoxy(61,10);printf("(Quantidade de USUÁRIOS cadastrados: %d)",qtd_usr);textcolor(0); //contador de usuários
							gotoxy(62,16);printf("Qual método de pesquisa deseja fazer?");
							buffer=menu_de_3(67, "Pesquisa por Código", "Pesquisa por Nome", "(Voltar)", 6, 9, 10);
							if (buffer==18) {
								pesquisa_avancada_id_usr(); //PESQUISA AVANÇADA POR CÓDIGO
								break;
							}
							else if (buffer==19) {
								do { 
									buffer=pesquisa_avancada_nome_usr(); //PESQUISA AVANÇADA POR NOME
								} while(buffer!=1);
								break;
							}
							else break;
						}
						else break;
							
					case 20: //ALTERAÇÃO DE PRODUTOS
						limpatela();
						gotoxy(60,14);printf("Qual o tipo de alteração deseja realizar?");
						buffer=menu_de_3(68, "Alterar Produtos", "Excluir Produtos", "(Voltar)", 2, 5, 10);
						if(buffer==18) {
							altera_prod(); //ALTERA OS DADOS DO PRODUTO
							break;
						}
						else if(buffer==19) {
							excluir_prod(); //EXCLUI O PRODUTO
							break;
						}
						else break;
						
					case 21: //ALTERAÇÃO DE USUÁRIOS
						limpatela();
						gotoxy(60,14);printf("Qual o tipo de alteração deseja realizar?");
						buffer=menu_de_3(67, "Alterar Cadastros", "Excluir Cadastros", "(Voltar)", 2, 5, 10);
						if(buffer==18) {
							altera_usr(); //ALTERA OS DADOS DO USUÁRIO
							break;
						}
						else if(buffer==19) {
							excluir_usr(); //EXCLUI O USUÁRIO
							break;
						}
						else break;
						
					case 22: //SOBRE O PROGRAMA
						limpatela();
						//Pixelart
						setlocale(LC_ALL, "C");
						textcolor(2);
						gotoxy(96,10);printf ("  %c       %c  ",219,219);
						gotoxy(96,11);printf ("   %c     %c  ",219,219);
						gotoxy(96,12);printf ("  %c%c%c%c%c%c%c%c%c  ",219,219,219,219,219,219,219,219,219,219);
						gotoxy(96,13);printf (" %c%c %c%c%c%c%c %c%c ",219,219,219,219,219,219,219,219,219,219);
						gotoxy(96,14);printf ("%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219);
						gotoxy(96,15);printf("%c  %c%c%c%c%c%c%c  %c",219,219,219,219,219,219,219,219,219,219);
						gotoxy(96,16);printf("%c  %c     %c  %c",219,219,219,219,219,219,219,219,219,219);
						gotoxy(96,17);printf("    %c%c %c%c    ",219,219,219,219,219,219,219,219);
						setlocale(LC_ALL, "Portuguese");
						textcolor(0);
						//Textos
						gotoxy(72,10);textcolor(11);printf("Sobre o programa:");
						gotoxy(53,12);textcolor(9);printf("Info:");textcolor(0);
						gotoxy(53,14);printf("Trabalho do 3° o do 4° bimestre da matéria");
						gotoxy(53,15);printf("de FPD. Trata-se de um programa que visa");
						gotoxy(53,16);printf("aplicar conhecimentos da área da informá-");
						gotoxy(53,17);printf("tica sobre linguagem C estudados durante o ano.");
						gotoxy(53,20);textcolor(9);printf("Developers (71/81A):");textcolor(0);
						gotoxy(53,22);printf("André Creppe (01), Lucas Sartor (22) e Miguel Torres (28)");
						gotoxy(53,25);textcolor(9);printf("Professora/Orientadora:");textcolor(0);
						gotoxy(53,27);printf("Ariane Scarelli");
						getch(); break;
						
					case 23: //SAÍDA
						limpatela();
						buffer=menu_de_3(67, "Trocar de Usuário", "Encerrar o programa", "(Cancelar)", 11, 12, 10);
						if(buffer==19) { //ENCERRA O PROGRAMA
							limpatela();
							gotoxy(62,17);printf("Obrigado por utilizar nosso software!");
							gotoxy(64,21);printf("Shutting down");
							for(int i=80; i<90; i+=2) {
								gotoxy(i,21);printf(".");
								Sleep(550);
							}
							return 0;
						}
						else if(buffer==18){ //TROCA DE USUÁRIO
							limpatela();
							end=1;
						}
						else break; //CANCELA A AÇÃO
				}
			}
		}
		//Menu principal dos USUÁRIOS (USRs)
		else {
			while(end!=1) {
				//Contagem de cadastros
				//Produtos
				if(abrir_arqv_prod_casoaparte("rb")==0) {
					qtd_prod=0; 
					while(!feof(prod))
						if(fread(&produto, sizeof(produto), 1, prod)==1 && produto.exclusao=='n') qtd_prod++;
					fclose(prod);
				}
				else qtd_prod=0;
				//Usuários
				qtd_usr=0; 
				abrir_arqv_usr("rb");
				while(!feof(usr))
					if(fread(&pessoa, sizeof(pessoa), 1, usr)==1 && pessoa.exclusao=='n') qtd_usr++;
				fclose(usr);
				
				//Setup do menu
				textcolor(0);	
				limpatela();
				gotoxy(71,11);printf("LOUCOS POR JOGOS©"); //Nome da nossa linda empresa
				int onome=69-(tam)/2;
				gotoxy(onome,12);printf("(Logado como");
				textcolor(3);gotoxy(onome+13,12);printf("*%s* [USR]",nome_usuario);textcolor(0);
				textcolor(0);printf(")");
				op=menu_principal_usr();
				
				//Opções do menu
				switch(op) {
					case 18: //PESQUISAR PRODUTOS
						limpatela();
						textcolor(5);gotoxy(61,10);printf("(Quantidade de PRODUTOS cadastrados: %d)",qtd_prod);textcolor(0); //contador de usuários
						gotoxy(60,16);printf("Qual o tipo de pesquisa deseja realizar?");
						buffer=menu_de_3(68, "Pesquisa Simples", "Pesquisa Avançada", "(Voltar)", 11, 15, 10);
						if(buffer==18) 	{
							pesquisa_simples_prod(); //PESQUISA SIMPLES
							break;
						}
						else if(buffer==19){ //PESQUISA AVANÇADA
							limpatela();
							textcolor(5);gotoxy(61,10);printf("(Quantidade de PRODUTOS cadastrados: %d)",qtd_prod);textcolor(0); //contador de usuários
							gotoxy(62,16);printf("Qual método de pesquisa deseja fazer?");
							buffer=menu_de_3(67, "Pesquisa por Código", "Pesquisa por Nome", "(Voltar)", 6, 9, 10);
							if (buffer==18) {
								pesquisa_avancada_id_prod(); //PESQUISA AVAÇADA POR CÓDIGO
								break;
							}
							else if (buffer==19) {
								do { 
									buffer=pesquisa_avancada_nome_prod(); //PESQUISA AVANÇADA POR NOME
								} while(buffer!=1);
								break;
							}
							else break;
						}
						else break;
						
					case 19: //PESQUISAR USUÁRIOS
						limpatela();
						textcolor(5);gotoxy(61,10);printf("(Quantidade de USUÁRIOS cadastrados: %d)",qtd_usr);textcolor(0); //contador de usuários
						gotoxy(60,16);printf("Qual o tipo de pesquisa deseja realizar?");
						buffer=menu_de_3(68, "Pesquisa Simples", "Pesquisa Avançada", "(Voltar)", 11, 15, 10);
						if(buffer==18) 	{
							pesquisa_simples_usr(); //PESQUISA SIMPLES
							break;
						}
						else if(buffer==19){ //PESQUISA AVANÇADA
							limpatela();
							textcolor(5);gotoxy(61,10);printf("(Quantidade de USUÁRIOS cadastrados: %d)",qtd_usr);textcolor(0); //contador de usuários
							gotoxy(62,16);printf("Qual método de pesquisa deseja fazer?");
							buffer=menu_de_3(67, "Pesquisa por Código", "Pesquisa por Nome", "(Voltar)", 6, 9, 10);
							if (buffer==18) {
								pesquisa_avancada_id_usr(); //PESQUISA AVANÇADA POR CÓDIGO
								break;
							}
							else if (buffer==19) {
								do { 
									buffer=pesquisa_avancada_nome_usr(); //PESQUISA AVANÇADA POR NOME
								} while(buffer!=1);
								break;
							}
							else break;
						}
						else break;
						
					case 20: //SOBRE
						limpatela();
						//Pixelart
						setlocale(LC_ALL, "C");
						textcolor(2);
						gotoxy(96,10);printf ("  %c       %c  ",219,219);
						gotoxy(96,11);printf ("   %c     %c  ",219,219);
						gotoxy(96,12);printf ("  %c%c%c%c%c%c%c%c%c  ",219,219,219,219,219,219,219,219,219,219);
						gotoxy(96,13);printf (" %c%c %c%c%c%c%c %c%c ",219,219,219,219,219,219,219,219,219,219);
						gotoxy(96,14);printf ("%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219);
						gotoxy(96,15);printf("%c  %c%c%c%c%c%c%c  %c",219,219,219,219,219,219,219,219,219,219);
						gotoxy(96,16);printf("%c  %c     %c  %c",219,219,219,219,219,219,219,219,219,219);
						gotoxy(96,17);printf("    %c%c %c%c    ",219,219,219,219,219,219,219,219);
						setlocale(LC_ALL, "Portuguese");
						textcolor(0);
						//Textos
						gotoxy(72,10);textcolor(11);printf("Sobre o programa:");
						gotoxy(53,12);textcolor(9);printf("Info:");textcolor(0);
						gotoxy(53,14);printf("Trabalho do 3° o do 4° bimestre da matéria");
						gotoxy(53,15);printf("de FPD. Trata-se de um programa que visa");
						gotoxy(53,16);printf("aplicar conhecimentos da área da informá-");
						gotoxy(53,17);printf("tica sobre linguagem C estudados durante o ano.");
						gotoxy(53,20);textcolor(9);printf("Developers (71/81A):");textcolor(0);
						gotoxy(53,22);printf("André Creppe (01), Lucas Sartor (22) e Miguel Torres (28)");
						gotoxy(53,25);textcolor(9);printf("Professora/Orientadora:");textcolor(0);
						gotoxy(53,27);printf("Ariane Scarelli");
						getch(); break;
						
					case 21: //SAÍDA
						limpatela();
						buffer=menu_de_3(67, "Trocar de Usuário", "Encerrar o programa", "(Cancelar)", 11, 12, 10);
						if(buffer==19) { //ENCERRA O PROGRAMA
							limpatela();
							gotoxy(62,17);printf("Obrigado por utilizar nosso software!");
							gotoxy(64,21);printf("Shutting down");
							for(int i=80; i<90; i+=2) {
								gotoxy(i,21);printf(".");
								Sleep(550);
							}
							return 0;
						}
						else if(buffer==18){ //TROCA DE USUÁRIO
							limpatela();
							end=1;
						}
						else break; //CANCELA A AÇÃO
				}
			}
		}
	}
}


//Funções da conio.c
void textcolor(int newcolor) { //muda a cor do texto
   CONSOLE_SCREEN_BUFFER_INFO csbi;
   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
      (csbi.wAttributes & 0xf0) | newcolor);
}
void gotoxy(int x, int y) { //move o cursor pela tela
	COORD c;
	c.X=x;
	c.Y=y;
	SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void HideCursor(int mostra){ //esconde ou mostra o cursor no programa
	CONSOLE_CURSOR_INFO cursor;
	if(mostra == 1)
		cursor = {20, TRUE};
	else
		cursor = {20, FALSE};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}


//Funções para estética
void loading() { //animação de carregando para o programa
	float num=0;
	gotoxy(75,19);printf("Carregando...");
	for(int i=60; i<=102; i++) {
		gotoxy(i,20);printf("-");
		gotoxy(i,22);printf("-");
	}
	setlocale(LC_ALL, "Portuguese");
	gotoxy(80,21);printf("%.0f%%", num);
	textcolor(12);
	for(int i=61; i<102; i++) {
		//Loading bar
		setlocale(LC_ALL, "C");
		gotoxy(i,21);printf("%c", 219);
		gotoxy(0,0);
		Sleep(75);
		//%
		textcolor(0);gotoxy(80,21);printf("%.0f%%", num);textcolor(12);
		num+=2.5;
	}
	textcolor(10);
	for(int i=61; i<102; i++) {
		gotoxy(i,21);printf("%c", 219);
	}
	textcolor(0);gotoxy(79,21);printf("100%%", num);
	gotoxy(75,19);printf(" Carregado!   ");
	Sleep(620);
	setlocale(LC_ALL, "Portuguese");
	limpatela();
}
void gamepad() { //carrega o formato característico do programa
	setlocale(LC_ALL, "C");
	textcolor(0);
	//Fundo + Formato
	for(int i=50; i<=110; i++) {
		gotoxy(i,5);printf("%c", 219);
	}
	for(int i=46; i<115; i++) {
		gotoxy(i,6);printf("%c", 219);

	}
	for(int i=34; i<127; i++) {
		gotoxy(i,7);printf("%c", 219);

	}
	for(int i=30; i<131; i++) {
		gotoxy(i,8);printf("%c", 219);

	}
	for(int i=9; i<=31; i++) {
		gotoxy(28,i);printf("%c", 219);
		gotoxy(29,i);printf("%c", 219);
		gotoxy(30+101,i);printf("%c", 219);
		gotoxy(31+101,i);printf("%c", 219);
	}
	for(int i=9; i<=31; i++) {
		
		gotoxy(28,i);printf("%c", 219);
		gotoxy(29,i);printf("%c", 219);
		gotoxy(30+101,i);printf("%c", 219);
		gotoxy(31+101,i);printf("%c", 219);
	}
	for(int i=30; i<131; i++) {
		gotoxy(i,32);printf("%c", 219);
	}
	for(int i=40; i<121; i++) {
		gotoxy(i,33);printf("%c", 219);
	}
	for(int i=50; i<83; i++) {
		gotoxy(i,34);printf("%c", 219);
		gotoxy(i+28,34);printf("%c", 219);
	}
	for(int i=30 ; i<131 ; i++) {
		for(int j=8 ; j<32 ; j++) {
			gotoxy(i,j); printf("%c", 219);
		}
	}
	//Botoes
	textcolor(2);
	for(int i=12; i<=13; i++) {
		gotoxy(121,i); printf("%c", 219);
		gotoxy(122,i); printf("%c", 219);
		gotoxy(123,i); printf("%c", 219);
	}
	textcolor(5);
	for(int i=15; i<=16; i++) {
		gotoxy(115,i); printf("%c", 219);
		gotoxy(116,i); printf("%c", 219);
		gotoxy(117,i); printf("%c", 219);
	}
	textcolor(1);
	for(int i=19; i<=20; i++) {
		gotoxy(121,i); printf("%c", 219);
		gotoxy(122,i); printf("%c", 219);
		gotoxy(123,i); printf("%c", 219);
	}
	textcolor(4);
	for(int i=15; i<=16; i++) {
		gotoxy(127,i); printf("%c", 219);
		gotoxy(128,i); printf("%c", 219);
		gotoxy(129,i); printf("%c", 219);
	}
	
	//Joystick
	textcolor(24);
	for(int i=12; i<=19; i++) {
		gotoxy(37,i); printf("%c", 219);
		gotoxy(38,i); printf("%c", 219);
		gotoxy(39,i); printf("%c", 219);
	}
	for(int i=15; i<=16; i++) {
		for(int j=32; j<45; j++) {
			gotoxy(j,i); printf("%c", 219);
			gotoxy(j,i); printf("%c", 219);
			gotoxy(j,i); printf("%c", 219);
		}
	}
	//Contorno
	linha(8);	grade(49);	grade(111);	linha(31);
	//Tela
	limpatela();
	setlocale(LC_ALL, "Portuguese");
}
void limpatela() { //limpa a "tela" do nosso gamepad
	setlocale(LC_ALL, "C");
	textcolor(7);
	for(int i=9; i<31; i++) {
		for(int j=50; j<=110; j++) {
			gotoxy(j,i);printf("%c", 219);
		}
	}
	setlocale(LC_ALL, "Portuguese");
	textcolor(0);
}
void limpalinha(int y) { //limpa uma linha desejada
	setlocale(LC_ALL, "C");	
	textcolor(7);
	for(int i=50; i<=110; i++) {
		gotoxy(i,30);printf("%c",219);
	} 
	setlocale(LC_ALL, "Portuguese");
	textcolor(0);
}
void grade(int x) { //faz uma gradezinha
	setlocale(LC_ALL, "C");
	textcolor(24);
	for(int i=9; i<=30; i++) {
		gotoxy(x,i);printf("%c", 220);
	}
	textcolor(0);
	setlocale(LC_ALL, "Portuguese");
}
void linha(int y) { //faz uma linha
	setlocale(LC_ALL, "C");
	textcolor(24);
	for(int i=49; i<=111; i++) {
		gotoxy(i,y);printf("%c", 219);
	}
	textcolor(0);
	setlocale(LC_ALL, "Portuguese");
}


//Fuções para o arquivo de produtos
int abrir_arqv_prod(const char modo[]) { //abrir o arquivo de produtos
	fclose(prod);
	if((prod=fopen("estoque.bin", modo))==NULL) {
		limpatela();
		textcolor(12);gotoxy(75,18);printf("ERROR 404!");
		gotoxy(73,20);printf("FILE NOT FOUND");textcolor(0);
		getch();
		return 1;
	}
	return 0;
}
int abrir_arqv_prod_casoaparte(const char modo[]) {
	fclose(prod);
	if((prod=fopen("estoque.bin", modo))==NULL) return 1;
	return 0;
}
//Escrita (pt1)
void campos_para_dados_prod(const char frase[], int x_frase) { //mostrar na tela os campos para digitar
	limpatela();
	gotoxy(x_frase,11);printf("%s", frase);
	gotoxy(52,15);printf("Id: ");
	gotoxy(52,17);printf("Nome: ");
	gotoxy(52,19);printf("Empresa/Desenvolvedora: ");
	gotoxy(52,21);printf("Plataforma: ");
	gotoxy(52,23);printf("Data de Lançamento: Mês ->  ");
	gotoxy(72,24);printf("Ano -> ");
	gotoxy(52,25);printf("Quantidade: ");
	gotoxy(52,27);printf("Preço(R$): ");
}
void adicionar_prod() { //adicionar os daddos digitados do usuário
	while(true){
		campos_para_dados_prod("CADASTRO DE PRODUTOS", 70); //Mostrar os campos na tela
		gotoxy(67,12);printf("(Digite 0 no ID para sair)");
		//Momento de preencher
		HideCursor(1);
		produto.id=testar_id_prod();//aceittar somente um id único
		system("cls");
		gamepad();
		campos_para_dados_prod("CADASTRO DE PRODUTOS", 70);
		if(produto.id==0) {
			fclose(prod);
			HideCursor(0);
			return;
		}
		textcolor(9);gotoxy(56,15);printf("%d",produto.id);
		fflush(stdin);textcolor(9);gotoxy(58,17);gets(produto.nome);
		fflush(stdin);gotoxy(76,19);gets(produto.emp_marc);
		fflush(stdin);gotoxy(64,21);gets(produto.plataforma);
		fflush(stdin);produto.lancamento.mes=digita_num(79,23);
			produto.lancamento.mes=consistencia_int(produto.lancamento.mes, 1, 12, 79, 23, 1); //consistencia do MES (1-12)
		fflush(stdin);produto.lancamento.ano=digita_num(79,24);
			produto.lancamento.ano=consistencia_int(produto.lancamento.ano, 1970, 2017, 79, 24, 1); //consistencia do ANO (1970-2017)
		fflush(stdin);produto.qtd=digita_num(64,25);
			produto.qtd=consistencia_int(produto.qtd, 1, 666, 64, 25, 2);
		fflush(stdin);gotoxy(63,27);scanf("%f", &produto.preco);
		fflush(stdin);produto.exclusao='n';
		salvar_prod(); //Salvar?
		
		//De novo?
		limpatela();
		gotoxy(66,14);printf("Deseja inserir novos dados?");
		HideCursor(0);
		if(menu_de_2(74, "Não", "Sim", 12, 10)==19)
			break;
	}
}
int testar_id_prod() { //filtrar para só haver um unico id
	int num;
	while(true) {
		textcolor(9);
		num=digita_num(56,15);
		consistencia_int(num, 0, 666, 56, 15, 2);//consistencia dos dados (tem que ser maior que 0)
		rewind(prod);
		while((fread(&produto, sizeof(produto), 1, prod)==1) && produto.id != num); //correr pela struct inteira ate acabar ou achar igual digitado
		if(produto.id==num && produto.exclusao=='n') {
			fflush(stdin);
			limpatela(); HideCursor(0);
			textcolor(12);
			gotoxy(65,18);printf("ERRO! CÓDIGO JÁ CADASTRADO!");
			gotoxy(63,20);printf("APERTE ENTER PARA DIGITAR OUTRO");
			getch();
			limpatela(); 
			campos_para_dados_prod("CADASTRO DE PRODUTOS", 70); HideCursor(1);
		}
		else break;
	}
	return num;
}
void salvar_prod() { //salvar no arquivo de produtos
	fclose(prod);abrir_arqv_prod("ab+");
	limpatela();
	HideCursor(0);
	gotoxy(64,14);printf("Deseja salvar os dados digitados?");
	if(menu_de_2(74, "Sim", "Não", 10, 12)==19) {
		if(fwrite(&produto, sizeof(produto), 1, prod)!=1) {
			textcolor(12);
			limpatela();
			gotoxy(12,12);printf("ERRO! ESCRITA NÃO EFETUADA!");
			textcolor(0);
			getch();
		}
		else {
			fflush(prod);
			limpatela();
			gotoxy(67,19);textcolor(5);printf("Dados Salvos com Sucesso!");
			fclose(prod);getch();textcolor(0);
		}
	}
	else
		fflush(stdin);
}
//Leitura (pt2)
void limpa_pesquisa_prod() { //rearranja os campos designados para a pesquisa (tabela)
	limpatela();
	grade(56);gotoxy(52,9);printf("ID");
	grade(71);gotoxy(62,9);printf("NOME");
	grade(86);gotoxy(74,9);printf("DESENVOLV.");
	grade(97);gotoxy(89,9);printf("QUANT.");
	gotoxy(100,9);printf("PREÇO(R$)");
	grade(56);	grade(71);	grade(86);	grade(97);
}
void pesquisa_simples_prod() { //mostra "todos" os dados
	int x=52, y=10, buff_y, letras, cont, exit=0;
	if(abrir_arqv_prod("rb")==1) return;
	while(!feof(prod)) {
		fflush(stdin);
		limpa_pesquisa_prod();
		cont=0;
		buff_y=y+1;
		while(cont<10) {
			if(feof(prod)) {
				textcolor(6);gotoxy(54,30);printf("FIM DE PESQUISA -> Pressione qualquer tecla para sair");
				getch(); gamepad();
				fclose(prod);
				return;
			}
			if(fread(&produto, sizeof(produto), 1, prod)==1 && produto.exclusao=='n') {
				textcolor(0);
				letras = strlen(produto.nome);				
				if(letras<12) {
					gotoxy(x+6, buff_y);printf("%s", produto.nome);	
				}	
				else {
					gotoxy(x+6, buff_y);printf("%s", produto.nome);
					textcolor(12);gotoxy(x+18, buff_y);printf(".");textcolor(0);
				}
				letras=strlen(produto.emp_marc);
				if(letras<12) {
					gotoxy(x+22, buff_y);printf("%s", produto.emp_marc);
				}
				else {
					gotoxy(x+22, buff_y);printf("%s", produto.emp_marc);
					textcolor(12);gotoxy(x+33, buff_y);printf(".");textcolor(0);
					gotoxy(x+35, buff_y);printf("     ");
				}
				gotoxy(x-1, buff_y);printf("%d", produto.id);
				gotoxy(x+38, buff_y);printf("%d", produto.qtd);
				gotoxy(x+50, buff_y);printf("%.2f", produto.preco);
				cont+=1; //limitador de 10
				buff_y+=2; //valor de y para printar as informacoes
			}
			grade(56);	grade(71);	grade(86);	grade(97); //arrumar a grade
			for(int i=9; i<31;i++) { //tira o excesso (arruma )
				textcolor(7);
				for(int x=72;x<74;x++) {
					gotoxy(x,i);printf("%c",219);
				}			
			} textcolor(0);
		}
		if(cont=10) { //caso de o limite maximo de produtos por
			textcolor(1);gotoxy(65,32);printf("Sair (TAB) // Prox. Pág. (Enter)");
			fflush(stdin);
			char carac=getch();
			if(carac==9) {
				gamepad();
				return;
			}
			else if(carac==13){
				limpa_pesquisa_prod();
			}
		}
	}
}
void pesquisa_avancada_id_prod() { //pesquisa pelo valor de ID
	int num, ok;
	while(true) {
		ok=0;
		if(abrir_arqv_prod("rb")==1) return;
		limpatela();campos_para_dados_prod("PESQUISA POR CÓGIGO", 70);
		gotoxy(69,12);printf("(Digite 0 para: SAIR)");
		HideCursor(1);gotoxy(56,15);scanf("%d", &num);
		HideCursor(0);
		if(num==0) break;
		while(!feof(prod)) {
			if(fread(&produto, sizeof(produto), 1, prod)==1 && produto.exclusao=='n' && produto.id==num) {
				ok=1;
				textcolor(9);gotoxy(56,15);printf("%d",num);
				gotoxy(58,17);printf("%s", produto.nome);
				gotoxy(76,19);printf("%s", produto.emp_marc);
				gotoxy(64,21);printf("%s", produto.plataforma);
				gotoxy(79,23);printf("%d", produto.lancamento.mes);
				gotoxy(79,24);printf("%d", produto.lancamento.ano);
				gotoxy(64,25);printf("%d", produto.qtd);
				gotoxy(63,27);printf("%.2f", produto.preco);
				getch();textcolor(0);
			}
		}
		if(ok==0) {
			limpatela();textcolor(12);
			gotoxy(69,18);printf("CADASTRO INEXISTENTE!");
			gotoxy(70,20);printf("DIGITE OUTRO VALOR");textcolor(0);getch();
		}
	fclose(prod);
	}
}
int pesquisa_avancada_nome_prod() { //pesquisa por um trecho de string (nome do produto)
	int ok, struc, x=52, y=10, buff_y, cont, letras, xx=0, num, yaho=0;
	char tecla, nome[250], aux[250], aux2[250], carac;
	while(true) {
		if(abrir_arqv_prod("rb")==1) return 1;
		rewind(prod); fflush(stdin);
		limpa_pesquisa_prod();
		gotoxy(72,8);printf("PESQUISA POR NOME");
		gotoxy(69,31);printf("(Digite 0 para: SAIR)");
		limpalinha(30); HideCursor(1);
		gotoxy(50,30);printf("Nome: "); //printa de novo o nome com string digitada
		gotoxy(55,30);	fflush(stdin);	gets(aux);	HideCursor(0);
		if(strcmpi(aux, "0")==0) {
			gamepad();
			return 1;
		}
		while(!feof(prod)) { //Caso acerte o nome de primeira
			if((fread(&produto, sizeof(produto), 1, prod)==1 && produto.exclusao=='n')) {
				if(strcmpi(aux,produto.nome)==0) {
					gamepad();
					campos_para_dados_prod("PESQUISA POR NOME",72);
					gotoxy(66,12);printf("(Pressione ENTER para voltar)");
					gotoxy(56,15);printf("%d", produto.id);
					gotoxy(58,17);printf("%s", produto.nome);
					gotoxy(76,19);printf("%s", produto.emp_marc);
					gotoxy(64,21);printf("%s", produto.plataforma);
					gotoxy(79,23);printf("%d", produto.lancamento.mes);
					gotoxy(79,24);printf("%d", produto.lancamento.ano);
					gotoxy(64,25);printf("%d", produto.qtd);
					gotoxy(63,27);printf("%.2f", produto.preco);
					getch(); xx=1;
					return 0;
				}		
			}
		}
		if(xx==0) { //Pedaço da palavra
			ok=0, buff_y=10, yaho=0, cont=0;
			char *SL;
			rewind(prod); fflush(stdin);
			while(!feof(prod)) {	
				if((fread(&produto, sizeof(produto), 1, prod)==1 && produto.exclusao=='n' && cont<10)) {
					strcpy(aux2,produto.nome);
					strupr(aux2); strupr(aux);
					SL=strstr(aux2,aux);
					if(SL!=NULL) { //printar os resultados positivos em formato de tabela
						ok=1;
						textcolor(0);
						gotoxy(x-1, buff_y);printf("%d", produto.id);
						letras = strlen(produto.nome); //se o nome fica ou passa dos espaços			
						if(letras<12){
							gotoxy(x+6, buff_y);printf("%s", produto.nome);	
						}	
						else {
							gotoxy(x+6, buff_y);printf("%s", produto.nome);
							textcolor(12);gotoxy(x+18, buff_y);printf(".");textcolor(0);
						}			
						letras=strlen(produto.emp_marc); //se a empresa fica ou passa dos espaços
						if(letras<11) {
							gotoxy(x+22, buff_y);printf("%s", produto.emp_marc);
						}
						else {
							gotoxy(x+22, buff_y);printf("%s", produto.emp_marc);
							textcolor(12);gotoxy(x+33, buff_y);printf(".");textcolor(0);
						}
						gotoxy(x+38, buff_y);printf("%d", produto.qtd);
						gotoxy(x+50, buff_y);printf("%.2f", produto.preco);
						cont+=1; //limitador de 10 por pagina
						buff_y+=2; //valor de y para printar as informacoes (alternável)
					}
					grade(56);	grade(71);	grade(86);	grade(97);
					for(int i=10; i<30; i++){
						textcolor(7);gotoxy(72,i);printf("%c",219);
						gotoxy(73,i);printf("%c",219);
					}
				}
				if(feof(prod)) { //acabou os dados
					textcolor(6);gotoxy(50,30);printf("     FIM DE PESQUISA -> TAB = Pesquisar // ENTER = Sair");
					yaho=0;
					while(((carac=getch()) != (9||13)) && yaho!=1) {
						if(carac==13) { //Sair (enter)
							gamepad();
							fclose(prod);
							return 1;
						}
						else if(carac==9)  {//Pesquisa (tab)
							gotoxy(50,30);printf("                                                             ");
							textcolor(0);gotoxy(50,30);printf("Pesquisa pelo ID: ");
							textcolor(3);num=digita_num(68,30);
							if(num==0) {
								gamepad();
								fclose(prod);
								return 1;
							}
							rewind(prod);
							xx=0;
							while(!feof(prod)) {
								if((fread(&produto, sizeof(produto), 1, prod)==1 && produto.exclusao=='n' && produto.id==num)) {
									gamepad();
									campos_para_dados_prod("PESQUISA POR NOME",72);
									gotoxy(66,12);printf("(Pressione ENTER para voltar)");
									gotoxy(56,15);printf("%d", produto.id);
									gotoxy(58,17);printf("%s", produto.nome);
									gotoxy(76,19);printf("%s", produto.emp_marc);
									gotoxy(64,21);printf("%s", produto.plataforma);
									gotoxy(79,23);printf("%d", produto.lancamento.mes);
									gotoxy(79,24);printf("%d", produto.lancamento.ano);
									gotoxy(64,25);printf("%d", produto.qtd);
									gotoxy(63,27);printf("%.2f", produto.preco);
									getch(); xx=1; yaho=1;
									return 0;		
								}
							}
							if(xx==1) 
								return 0;
								
							else if(xx==0){
								rewind(prod);
								limpatela();textcolor(12);
								gotoxy(67,18);printf("NENHUM CADASTRO ENCONTRADO");
								gotoxy(69,20);printf("DIGITE OUTRO NOME E ID");textcolor(0);
								getch(); return 0;
							}
						}
					}
				}
				else if (cont>=10) { //Próxima página (nao acabou os dados)
					yaho=0;
					textcolor(12);gotoxy(x+56,buff_y);printf("->");textcolor(0);
					fflush(stdin);
					while(((carac=getch()) != (9||13)) && yaho!=1) {
						if(carac==13) {
							//Avança (enter)
							limpa_pesquisa_prod();
							buff_y=10;
							while(true) {
								if((fread(&produto, sizeof(produto), 1, prod)==1 && produto.exclusao=='n')) {
									strcpy(aux2,produto.nome);
									strupr(aux2); strupr(aux);
									SL=strstr(aux2,aux); 
									if(SL!=NULL) {
										ok=1;
										textcolor(0);
										letras = strlen(produto.nome);
				
										if(letras<12) {
											gotoxy(x+6, buff_y);printf("%s", produto.nome);	
										}	
										
										else {
											gotoxy(x+6, buff_y);printf("%s", produto.nome);
											textcolor(12);gotoxy(x+18, buff_y);printf(".");textcolor(0);
										}			
										gotoxy(x-1, buff_y);printf("%d", produto.id);
										letras=strlen(produto.emp_marc); //se a empresa fica ou passa dos espaços
										if(letras<11) {
											gotoxy(x+22, buff_y);printf("%s", produto.emp_marc);
										}
										else {
											gotoxy(x+22, buff_y);printf("%s", produto.emp_marc);
											textcolor(12);gotoxy(x+33, buff_y);printf(".");textcolor(0);
										}
										gotoxy(x+38, buff_y);printf("%d", produto.qtd);
										gotoxy(x+50, buff_y);printf("%.2f", produto.preco);
										buff_y+=2;
										}
									grade(56);	grade(71);	grade(86);	grade(97);
								}
								if(feof(prod)) {
									textcolor(6);gotoxy(50,30);printf("     FIM DE PESQUISA -> TAB = Pesquisar // ENTER = Sair");
									while(((carac=getch()) != (9||13)) && yaho!=1) {
										if(carac==13) { //Sair (enter)
											gamepad();
											fclose(prod);
											return 1;
										}
										else if(carac==9)  {//Pesquisa (tab)
											gotoxy(50,30);printf("                                                             ");
											textcolor(0);gotoxy(50,30);printf("Pesquisa pelo ID: ");
											textcolor(3);num=digita_num(68,30);
											if(num==0) {
												gamepad();
												fclose(prod);
												return 1;
											}
											rewind(prod);
											xx=0;
											while(!feof(prod)) {
												if((fread(&produto, sizeof(produto), 1, prod)==1 && produto.exclusao=='n' && produto.id==num)) {
													gamepad();
													campos_para_dados_prod("PESQUISA POR NOME",72);
													gotoxy(66,12);printf("(Pressione ENTER para voltar)");
													gotoxy(56,15);printf("%d", produto.id);
													gotoxy(58,17);printf("%s", produto.nome);
													gotoxy(76,19);printf("%s", produto.emp_marc);
													gotoxy(64,21);printf("%s", produto.plataforma);
													gotoxy(79,23);printf("%d", produto.lancamento.mes);
													gotoxy(79,24);printf("%d", produto.lancamento.ano);
													gotoxy(64,25);printf("%d", produto.qtd);
													gotoxy(63,27);printf("%.2f", produto.preco);
													getch(); xx=1; yaho=1;
													return 0;		
												}
											}
											if(xx==1) 
												return 0;
												
											else if(xx==0){
												rewind(prod);
												limpatela();textcolor(12);
												gotoxy(67,18);printf("NENHUM CADASTRO ENCONTRADO");
												gotoxy(69,20);printf("DIGITE OUTRO NOME E ID");textcolor(0);
												getch(); return 0;
											}
										}
									}
								}
							}
							break;
						}
						else if(carac==9) {
							//Pesquisa por id específico (tab)
							gotoxy(50,30);printf("                                                    ");
							gotoxy(50,30);printf("Pesquisa pelo ID: ");
							textcolor(3);num=digita_num(68,30);
							if(num==0) {
								gamepad();
								return 1;
							}
							rewind(prod);
							if((fread(&produto, sizeof(produto), 1, prod)==1 && produto.exclusao=='n' && produto.id==num)) {
								gamepad();
								campos_para_dados_prod("PESQUISA POR NOME",72);
								gotoxy(66,12);printf("(Pressione ENTER para voltar)");
								gotoxy(56,15);printf("%d", produto.id);
								gotoxy(58,17);printf("%s", produto.nome);
								gotoxy(76,19);printf("%s", produto.emp_marc);
								gotoxy(64,21);printf("%s", produto.plataforma);
								gotoxy(79,23);printf("%d", produto.lancamento.mes);
								gotoxy(79,24);printf("%d", produto.lancamento.ano);
								gotoxy(64,25);printf("%d", produto.qtd);
								gotoxy(63,27);printf("%.2f", produto.preco);
								getch(); xx=1; yaho=1;
								return 0;			
							}
							else {
								limpatela();textcolor(12);
								gotoxy(67,18);printf("NENHUM CADASTRO ENCONTRADO");
								gotoxy(69,20);printf("DIGITE OUTRO NOME E ID");textcolor(0);
								getch(); return 0;
							}
						}
					}
				}
			}
			if(ok==0) {
				limpatela();textcolor(12);
				gotoxy(67,18);printf("NENHUM CADASTRO ENCONTRADO");
				gotoxy(71,20);printf("DIGITE OUTRO NOME");textcolor(0);
				getch(); return 0;
			}
		}
		fclose(prod);
		gamepad();
	}
}
//Alteração (pt3)
void altera_prod() { //altera os cadastros de produtos
	loja produto_aux; //struct auxiliar para buffer de alteração
	int aux_id,F;
	long fposicao;
	char conf;
	while(true) {
		if(abrir_arqv_prod("rb+")==1) return;
		limpatela();
		campos_para_dados_prod("ALTERÇÃO DOS PRODUTOS", 70);
		gotoxy(70,12);printf("(Digite 0 para: SAIR)");
	    HideCursor(1);	aux_id=digita_num(56,15);
	    if(aux_id==0) {
	    	HideCursor(0);
	    	return;
		}
	    
	    F = 0;
	    do {
			fread( &produto,sizeof(produto),1,prod);
			if ( produto.id == aux_id && produto.exclusao=='n')	{ //SOMENTE SE NÃO FOI EXCLUIDO
		   		F = 1;
		   		fposicao = ftell(prod);
		   		mostra_dados_altera_prod("ALTERAÇÃO DE PRODUTOS",70);
		   		produto_aux = produto;
		   		digita_dados_prod(produto.lancamento.mes, produto.lancamento.ano, produto.qtd, produto.preco); //dalteração dos dados
		   		//testa se os campos estao em branco ou não (volta o antigo/nao altera)
				if (produto.nome[0] == '\x0')
				    strcpy(produto.nome, produto_aux.nome);
				if (produto.emp_marc[0]== '\x0')
				   strcpy(produto.emp_marc, produto_aux.emp_marc);
				if (produto.plataforma[0] == '\x0')
				    strcpy(produto.plataforma, produto_aux.plataforma);
				if (produto.lancamento.mes == 0)
				    produto.lancamento.mes = produto_aux.lancamento.mes;
				if (produto.lancamento.ano == 0)
				    produto.lancamento.ano = produto_aux.lancamento.ano;
				if(produto.preco == 0.0)
					produto.preco=produto_aux.preco;
				if(produto.qtd == 0)
					produto.qtd = produto_aux.qtd;
			   	
				gotoxy(78,28); printf(" Confirma alteração? (S/N): ");
			   	do {
			    	gotoxy(54,22); conf = getche();
			   	} while( conf != 's' && conf != 'S' &&  conf != 'n' && conf != 'N' );
			   	if( conf == 's' || conf == 'S' ) {
			   		fseek (prod,fposicao-(sizeof(produto)),SEEK_SET);
					fwrite (&produto,sizeof(produto),1,prod);
					fflush (prod);
			   	}
			}
		} while((!F) && (!feof(prod))); 
		if (F==0) {
			HideCursor(0);limpatela();textcolor(12);
			gotoxy(69,18);printf("CADASTRO INEXISTENTE!");
			gotoxy(70,20);printf("DIGITE OUTRO VALOR");textcolor(0);getch();
		} 	
	}
}
void mostra_dados_altera_prod(char frase[5000], int x) { //põe na tela os "dados fantasmas"
	gotoxy(x,11);puts(frase);
	gotoxy(70,12);printf("                         ");
	textcolor(2);gotoxy(52,15);printf("Id: ");textcolor(0);printf("%d",produto.id);
	textcolor(2);gotoxy(52,17);printf("Nome: ");textcolor(0);puts(produto.nome);
	textcolor(2);gotoxy(52,19);printf("Empresa/Desenvolvedora: ");textcolor(0);puts(produto.emp_marc);
	textcolor(2);gotoxy(52,21);printf("Plataforma: ");textcolor(0);puts(produto.plataforma);
	textcolor(2);gotoxy(52,23);printf("Data de Lançamento: Mês -> ");textcolor(0);printf("%d",produto.lancamento.mes);
	textcolor(2);gotoxy(72,24);printf("Ano -> ");textcolor(0);printf("%d",produto.lancamento.ano);
	textcolor(2);gotoxy(52,25);printf("Quantidade: ");textcolor(0);printf("%d",produto.qtd);
	textcolor(2);gotoxy(52,27);printf("Preço(R$): ");textcolor(0);printf("%.2f",produto.preco);
}
void digita_dados_prod(int mes,  int ano, int qtd, float preco) { //função para digiar (ou não) as mudanças da "alteração"
	HideCursor(1);
	char aux_qtd[10000], aux_preco[10000], aux_mes[10000], aux_ano[10000];
	fflush(stdin);
	gotoxy(58,17);textcolor(9);gets(produto.nome);fflush(stdin);
	gotoxy(76,19);gets(produto.emp_marc); fflush(stdin);
	gotoxy(64,21);gets(produto.plataforma); fflush(stdin);
	
	gotoxy(79,23);gets(aux_mes);fflush(stdin);
	produto.lancamento.mes = atoi(aux_mes); //converte o texto em int
	while(produto.lancamento.mes>12 || produto.lancamento.mes<1) { //consistencia do mes
		if(produto.lancamento.mes==0) {
			produto.lancamento.mes=mes;
			break;
		}
		HideCursor(0);gotoxy(79,23);printf("                             ");
		gotoxy(79,23);textcolor(12);printf("DIGITE UM VALOR VÁLIDO!");getch();
		gotoxy(79,23);printf("                             ");
		gotoxy(79,23);textcolor(0);printf("%d", mes);
		HideCursor(1);gotoxy(79,23);textcolor(9);fflush(stdin);gets(aux_mes);
		produto.lancamento.mes = atoi(aux_mes); //converte o texto em int
	}	
	
	gotoxy(79,24);gets(aux_ano);fflush(stdin);
	produto.lancamento.ano = atoi(aux_ano); //converte o texto em int
	while(produto.lancamento.ano>2017 || produto.lancamento.ano<1970) { //consistencia do ano
		if(produto.lancamento.ano==0) {
			produto.lancamento.ano=ano;
			break;
		}
		HideCursor(0);gotoxy(79,24);printf("                             ");
		gotoxy(79,24);textcolor(12);printf("DIGITE UM VALOR VÁLIDO!");getch();
		gotoxy(79,24);printf("                             ");
		gotoxy(79,24);textcolor(0);printf("%d", ano);
		HideCursor(1);gotoxy(79,24);textcolor(9);fflush(stdin);gets(aux_ano);
		produto.lancamento.ano = atoi(aux_ano); //converte o texto em int
	}
		
	gotoxy(64,25);gets(aux_qtd);
	produto.qtd = atoi(aux_qtd); //converte o texto em int
	while(produto.qtd<1) { //consistencia da quantidade
		if(produto.qtd==0) {
			produto.qtd=qtd;
			break;
		}
		HideCursor(0);gotoxy(64,25);printf("                             ");
		gotoxy(64,25);textcolor(12);printf("DIGITE UM VALOR VÁLIDO!");getch();
		gotoxy(64,25);printf("                             ");
		gotoxy(64,25);textcolor(0);printf("%d", qtd);
		HideCursor(1);gotoxy(64,25);textcolor(9);fflush(stdin);gets(aux_qtd);
		produto.qtd = atoi(aux_qtd); //converte o texto em int
	}
		
	gotoxy(63,27);gets(aux_preco);
	produto.preco = atof(aux_preco); //converte o texto em float
	while(produto.preco<1) { //consistencia da quantidade
		if(produto.preco==0) {
			produto.preco=preco;
			break;
		}
		HideCursor(0);gotoxy(63,27);printf("                             ");
		gotoxy(63,27);textcolor(12);printf("DIGITE UM VALOR VÁLIDO!");getch();
		gotoxy(63,27);printf("                             ");
		gotoxy(63,27);textcolor(0);printf("%d", preco);
		HideCursor(1);gotoxy(63,27);textcolor(9);fflush(stdin);gets(aux_preco);
		produto.preco = atof(aux_preco); //converte o texto em int
	}
	
	fflush(stdin);
	HideCursor(0);
}
void excluir_prod() { //exclui os arquivoos
	loja produto_aux; //struct auxiliar para buffer de alteração
	int aux_id,F;
	long fposicao;
	char conf;
	
	while(true){
		if(abrir_arqv_prod("rb+")==1) return;
		limpatela();
		campos_para_dados_prod("EXCLUSÃO DOS PRODUTOS", 70);
		gotoxy(70,12);printf("(Digite 0 para: SAIR)");
	    HideCursor(1);aux_id=digita_num(56,15);
	    if(aux_id==0) {
	    	HideCursor(0);
	    	return;
		}
	    F = 0;
	    do {
			fread( &produto,sizeof(produto),1,prod);
			if (produto.id == aux_id && produto.exclusao=='n')	{ //SOMENTE SE NÃO FOI EXCLUIDO
		   		F = 1;
		   		fposicao = ftell(prod);
		   		mostra_dados_altera_prod("EXCLUSÃO DE PRODUTOS",69);
		   		produto_aux = produto;
				gotoxy(79,28); printf("Confirma exclusão? (S/N): ");
			   	while( conf != 's' && conf != 'S' &&  conf != 'n' && conf != 'N' ) {
			   		gotoxy(105,28); printf("   ");
			    	HideCursor(1);gotoxy(105,28); conf = getchar(); HideCursor(0);
			   	}
			   	if( conf == 's' || conf == 'S' ) {
			   		produto.exclusao='s';
			   		fseek (prod,fposicao-(sizeof(produto)),SEEK_SET);
					fwrite (&produto,sizeof(produto),1,prod);
					fflush(prod);
			   	}
			}
		} while((!F) && (!feof(prod))); 
		if (F==0) {
			HideCursor(0);limpatela();textcolor(12);
			gotoxy(69,18);printf("CADASTRO INEXISTENTE!");
			gotoxy(70,20);printf("DIGITE OUTRO VALOR");textcolor(0);getch();
		} 	
	}
}


//Fuções para o arquivo de usuários
int abrir_arqv_usr(const char modo[]) { //abrir o arquivo de usuários
	fclose(usr);
	if((usr=fopen("login.bin", modo))==NULL) return 1;
	return 0;
}
void criar_adm() { //cria um arquivo com o "adm mestre" caso ele já nao exista
	if(abrir_arqv_usr("rb")==1) {
		fclose(usr);
		abrir_arqv_usr("ab+");
		pessoa.id=0;
		strcpy(pessoa.nome, "THE MASTER");
		pessoa.senha=123321;
		strcpy(pessoa.email, "admestre.loucosporjogos@gmail.com.br\0");
		pessoa.aniversario.dia=11;
		pessoa.aniversario.mes=5;
		pessoa.aniversario.ano=2002;
		pessoa.adm=1;
		pessoa.exclusao='n';
		//salvar pela primeira vez
		if(fwrite(&pessoa, sizeof(pessoa), 1, usr)!=1) {
			limpatela(); textcolor(12);
			gotoxy(12,12);printf("ERRO! ADM MESTRE NÃO CADASTRADO");
			textcolor(0); getch();
		}
		else  {
			fflush(usr);
			fclose(usr);
		}
	}
}
//Login (pt1)
int login() { //autenticação do usuário e seperação entre ADM e USR
	int sair=0, num, retorno, carac=0, i=0, ok=0, 		x=73, y=23, x_use, exit;
	long int passwd, digitado;
	char senha[6];
	while(true) {
		system("cls");
		gamepad();
		while(true) {
			ok=0, sair=0;
			HideCursor(0);	limpatela();	HideCursor(1);
			gotoxy(65,10);printf("Bem vindo ao Loucos Por Jogos!");
			gotoxy(54,13);printf("Por favor indentifique-se:");textcolor(5);gotoxy(81,13);printf("Digite o ID do seu cadastro");
			textcolor(0);gotoxy(66,19);printf("Nome: ");
			gotoxy(66,21);printf("Tipo: ");
			gotoxy(66,23);printf("Senha: ");
			gotoxy(66,17);printf("Id: ");
			do {
				textcolor(0);num=digita_num(70, 17);
				if(num<0) {
					printf("                       ");
					textcolor(12);gotoxy(70,17);printf("DIGITE UM VALOR VÁLIDO!");
					getch();gotoxy(70,17);printf("                       ");
				}	
			} while(num<0);
			system("cls");
			gamepad();
			gotoxy(65,10);printf("Bem vindo ao Loucos Por Jogos!");
			gotoxy(54,13);printf("Por favor indentifique-se:");textcolor(5);gotoxy(81,13);printf("Digite a sua SENHA de login");
			gotoxy(80,14);printf("(Aperte ESC para trocar de ID)");
			textcolor(0);gotoxy(66,17);printf("Id:");textcolor(3);gotoxy(70,17);printf("%d",num);textcolor(0);
			abrir_arqv_usr("rb");
			
			while(!feof(usr)) {
				carac=0, i=0, exit=0;
				HideCursor(1);
				if(fread(&pessoa, sizeof(pessoa), 1, usr)==1 && pessoa.id==num) {
					ok=1;
					gotoxy(66,19);printf("Nome: ");textcolor(3);puts(pessoa.nome);textcolor(0); //mostra o nome
					if(pessoa.adm==1) {
						gotoxy(66,21);printf("Tipo:");textcolor(3);gotoxy(72,21);printf("ADM");textcolor(0); //mostra o tipo ADM
						retorno=1;
					}
					else {
						gotoxy(66,21);printf("Tipo:");textcolor(3);gotoxy(72,21);printf("USR");textcolor(0); //mostra o tipo USR
						retorno=0;
					}
					passwd=pessoa.senha;
					gotoxy(66,23);printf("Senha: "); //campo para se digitar a senha
					while(sair!=1) {
						HideCursor(1); gotoxy(73,23);	textcolor(3);
						i=0;
						carac=0;
						while((senha[i] = getch()) != 13) {
							if(senha[i]=='\033') {
								sair=1;
								break;
							}
							fflush(stdin);
							x_use=x;
							if(i>5) break;
							if(senha[i] == 8 && carac != 0) {
								i--;
								gotoxy(x_use+carac-1, y);	printf("  ");
								carac--;
							}
							else if(senha[i] == 8 && carac == 0) {
								senha[i] = '\0';
								gotoxy(x+carac, y);	printf("  ");
							}
							else {
								senha[i] = senha[i];
								printf("*");
								i++;
								carac++;
							} 
							gotoxy(x+carac, y);
						}
						HideCursor(0);
						fflush(stdin);
						digitado=atoi(senha);
						if(digitado==passwd && exit==0) { //senha deu match
							fclose(usr);
							limpatela();
							strcpy(nome_usuario, pessoa.nome);
							return retorno; //testa para ver se a senha bate com a registrada
						}
						else {
							if(i>5 && exit==0 && sair!=1) {
								textcolor(12);gotoxy(73,23);printf("SENHA MUITO LONGA! REDIGITE");textcolor(0);
								getch();	gotoxy(73,23);printf("                             ");
							}
							else if(exit==0 && sair!=1){
								textcolor(12);gotoxy(73,23);printf("SENHA INCORRETA! REDIGITE");textcolor(0);
								getch();	gotoxy(73,23);printf("                             ");
							}
						}
					}
				}
			}
			if(ok==0) {
				HideCursor(0);limpatela();
				textcolor(12);gotoxy(65,17);printf("USUÁRIO INCORRETO/INEXISTENTE");
				gotoxy(67,20);printf("DIGITE UM LOGIN EXISTENTE");textcolor(0);
				getch();limpatela();
			}
			fclose(usr);
		}
	}
}
//Escrita (pt2)
void campos_para_dados_usr(const char frase[], int x_frase) {
	limpatela();
	gotoxy(x_frase,11);printf("%s", frase);
	gotoxy(52,15);printf("Id: ");
	gotoxy(52,17);printf("Nome: ");
	gotoxy(52,19);printf("E-Mail: ");
	gotoxy(52,21);printf("Tipo: ");
	gotoxy(52,24);printf("Aniversário: Mês -> ");
	gotoxy(65,25);printf("Dia -> ");
	gotoxy(65,26);printf("Ano -> ");
	gotoxy(52,28);printf("Senha (numérica - 6 digitos): ");
}
void adicionar_usr() {
	int i=0, ok=0, carac=0, x_use, tam=0;
	char senha[6];
	while(true){
		campos_para_dados_usr("CADASTRO DE USUÁRIOS", 70); //Mostrar os campos na tela
		gotoxy(67,12);printf("(Digite 0 no ID para sair)");
		//Momento de preencher
		HideCursor(1);
		pessoa.id=testar_id_usr();//aceittar somente um id único
		system("cls");
		gamepad();
		campos_para_dados_usr("CADASTRO DE USUÁRIOS", 70);
		if(pessoa.id==0) {
			HideCursor(0);
			return;
		}
		textcolor(9);
		gotoxy(56,15);printf("%d",pessoa.id);
		fflush(stdin);gotoxy(58,17);gets(pessoa.nome); 
		fflush(stdin);gotoxy(60,19);gets(pessoa.email);
		//Menu para selecionar ADM ou USR
		HideCursor(0);
		int resp=menu_de_2_usr("ADM", "USR", 58, 21, 14, 14);
		gotoxy(58,22);printf("         ");
		gotoxy(58,21);printf("         ");
		textcolor(9);
		if(resp==21) {
			gotoxy(58,21);printf("ADM");
			pessoa.adm=1;
		}
		else {
			gotoxy(58,21);printf("USR");
			pessoa.adm=0;
		}
		HideCursor(1); textcolor(9);
		//Data de aniversario - o mes vem primeiro para poder ter a consistencia
		fflush(stdin);pessoa.aniversario.mes=digita_num(72,24);
			pessoa.aniversario.mes=consistencia_int(pessoa.aniversario.mes, 1, 12, 72, 24, 1); //consistencia do MES (1-12)
		fflush(stdin);pessoa.aniversario.dia=digita_num(72,25);
			pessoa.aniversario.dia=consistencia_int_dia(pessoa.aniversario.dia, pessoa.aniversario.mes, 72, 25); //consistencia do DIA, depende do mes (1-31)
		fflush(stdin);pessoa.aniversario.ano=digita_num(72,26);
			pessoa.aniversario.ano=consistencia_int(pessoa.aniversario.ano, 1895, 2017, 72, 26, 1); //consistencia do ANO (baseado no ano do trabalho e em quanto viveu o humano mais velho)
		//Adicionar uma senha numérica
		while(true) {
			textcolor(9);
			fflush(stdin);gotoxy(82,28);
			i=0, ok=0, carac=0, x_use=82, tam=0;
			memset(senha,0,strlen(senha)); //limpa a string
			memset(senha,0,sizeof(senha)); //limpa a string
			while((senha[i]=getche()) !=13 ) {
				if(senha[i] == '0'|| senha[i] == '1' || senha[i] == '2' || senha[i] == '3' || senha[i] == '4' || senha[i] == '5' || senha[i] == '6' || senha[i] == '7' || senha[i] == '8' || senha[i] == '9') {
					if(senha[i] == 8 && carac != 0) {
						i--;
						gotoxy(x_use+carac-1, 28);	printf("  ");
						carac--;
					}
					else if(senha[i] == 8 && carac == 0) {
						senha[i] = '\0';
						gotoxy(82+carac,28); printf("  ");
					}
					else {
						i++;
						carac++;
					}
					gotoxy(82+carac, 28);
					fflush(stdin);
				}
				else {
					gotoxy(82+carac,28); printf("  ");
					gotoxy(82+carac,28);
					fflush(stdin);
				}
			}
			tam=strlen(senha);
			if(tam<7) {
				gotoxy(82,28);printf("          ");
				textcolor(12);gotoxy(82,28);printf("SENHA MUITO CURTA! REDIGITE");getch();
				gotoxy(82,28);printf("                            ");textcolor(0);
			}
			else if(tam==7) {
				HideCursor(0);textcolor(0);
				gotoxy(93,27);printf("Salvar senha?");
				buffer=menu_de_2_usr("Sim", "Não", 95, 28, 10, 12);
				if(buffer==28) {
					pessoa.senha=atoi(senha);
					break;
				}
				else {
					gotoxy(93,27);printf("             ");
					gotoxy(95,28);printf("      ");
					gotoxy(95,29);printf("      ");
					gotoxy(82,28);printf("             ");
					HideCursor(1);
				}
			}
			else {
				gotoxy(82,28);printf("                             ");
				textcolor(12);gotoxy(82,28);printf("SENHA MUITO LONGA! REDIGITE");getch();
				gotoxy(82,28);printf("                            ");textcolor(0);
			}
		}
		fflush(stdin);pessoa.exclusao='n';
		salvar_usr(); //Salvar?
		
		//De novo?
		limpatela();
		gotoxy(66,14);printf("Deseja inserir novos dados?");
		HideCursor(0);
		if(menu_de_2(74, "Não", "Sim", 12, 10)==19)
			break;
	}
}
int testar_id_usr() {
	int num;
	abrir_arqv_usr("rb");
	while(true) {
		textcolor(9);
		num=digita_num(56,15);
		if(num==0) return 0;
		consistencia_int(num, 0, 666, 56, 15, 2); //consistencia dos dados (tem que ser maior que 0)
		while((fread(&pessoa, sizeof(pessoa), 1, usr)==1) && pessoa.id != num); //correr pela struct inteira ate acabar ou achar igual digitado
		if(pessoa.id==num && pessoa.exclusao=='n') {
			fflush(stdin);
			limpatela(); HideCursor(0);
			textcolor(12);
			gotoxy(65,18);printf("ERRO! CÓDIGO JÁ CADASTRADO!");
			gotoxy(63,20);printf("APERTE ENTER PARA DIGITAR OUTRO");
			getch();
			limpatela(); 
			campos_para_dados_usr("CADASTRO DE PRODUTOS", 70); HideCursor(1);
		}
		else {
			fclose(usr);
			break;
		}
	}
	fclose(usr);
	return num;
}
void salvar_usr() {
	fclose(usr);abrir_arqv_usr("ab+");
	limpatela();
	HideCursor(0);
	gotoxy(64,14);printf("Deseja salvar os dados digitados?");
	if(menu_de_2(74, "Sim", "Não", 10, 12)==19) {
		if(fwrite(&pessoa, sizeof(pessoa), 1, usr)!=1) {
			textcolor(12);
			limpatela();
			gotoxy(12,12);printf("ERRO! ESCRITA NÃO EFETUADA!");
			textcolor(0);
			getch();
		}
		else {
			fflush(usr);
			limpatela();
			gotoxy(67,19);textcolor(5);printf("Dados Salvos com Sucesso!");
			fclose(usr);getch();textcolor(0);
		}
	}
	else
		fflush(stdin);
}
//Leitura (pt3)
void limpa_pesquisa_usr() {
	limpatela();
	//gamepad();
	grade(56);gotoxy(52,9);printf("ID");
	grade(71);gotoxy(62,9);printf("NOME");
	grade(80);gotoxy(74,9);printf("TIPO");
	grade(111);gotoxy(93,9);printf("E-MAIL");
	grade(56);	grade(71);	grade(80);	grade(111);
}
void pesquisa_simples_usr() {
	int x=52, y=10, buff_y, letras, cont, exit=0;
	abrir_arqv_usr("rb");
	while(!feof(usr)) {
		fflush(stdin);
		limpa_pesquisa_usr();
		cont=0;
		buff_y=y+1;
		while(cont<10) {
			if(feof(usr)) {
				textcolor(6);gotoxy(54,30);printf("FIM DE PESQUISA -> Pressione qualquer tecla para sair");
				getch(); gamepad();
				fclose(usr);
				return;
			}
			if(fread(&pessoa, sizeof(pessoa), 1, usr)==1 && pessoa.exclusao=='n') {
				textcolor(0);
				letras = strlen(pessoa.nome);
				//tamanho do nome e limita-lo
				if(letras<12) {
					gotoxy(x+6, buff_y);printf("%s", pessoa.nome);
				}	
				else {
					gotoxy(x+6, buff_y);printf("%s", pessoa.nome);
					textcolor(12);gotoxy(x+18, buff_y);printf(".");textcolor(0);
					gotoxy(x+20,buff_y);printf("     ");
				}
				gotoxy(x-1, buff_y);printf("%d", pessoa.id); //printa o ID
				if(pessoa.adm==1) { //checa se ele é ADM ou USR
					gotoxy(x+22,buff_y);printf("       ");
					gotoxy(x+22,buff_y);printf("ADM");
				}
				else {
					gotoxy(x+22,buff_y);printf("USR");
				}
				letras=strlen(pessoa.email); //email
				if(letras<12) {
					gotoxy(x+30, buff_y);printf("%s", pessoa.email);
				}
				else {
					gotoxy(x+30, buff_y);printf("%s", pessoa.email);
					textcolor(12);gotoxy(x+58, buff_y);printf(".");textcolor(0);
				}
				setlocale(LC_ALL, "C");
				for(int i=x+60; i<x+81; i++) { //arruma o gamepad
					gotoxy(i,buff_y);printf("%c", 219);
				}
				textcolor(2);
				for(int i=12; i<=13; i++) { //arruma o botão
					gotoxy(121,i); printf("%c", 219);
					gotoxy(122,i); printf("%c", 219);
					gotoxy(123,i); printf("%c", 219);
				}
				textcolor(0);
				setlocale(LC_ALL, "Portuguese");
				cont+=1; //limitador de 10
				buff_y+=2; //valor de y para printar as informacoes
			}
			grade(56);	grade(71);	grade(80);	grade(111); //arrumar a grade
			for(int i=9; i<31;i++) { //tira o excesso (arruma )
				textcolor(7);
				for(int x=72;x<74;x++) {
					gotoxy(x,i);printf("%c",219);
				}			
			} textcolor(0);
		}
		if(cont=10) { //caso de o limite maximo de produtos por
			textcolor(1);gotoxy(65,32);printf("Sair (TAB) // Prox. Pág. (Enter)");
			fflush(stdin);
			char carac=getch();
			if(carac==9) {
				gamepad();
				return;
			}
			else if(carac==13){
				limpa_pesquisa_usr();
			}
		}
	}
}
void pesquisa_avancada_id_usr() {
	int num, ok;
	while(true) {
		ok=0;
		if(abrir_arqv_usr("rb")==1) return;
		limpatela();campos_para_dados_usr("PESQUISA POR CÓGIGO", 70);
		gotoxy(69,12);printf("(Digite 0 para: SAIR)");
		HideCursor(1);gotoxy(56,15);scanf("%d", &num);
		HideCursor(0);
		if(num==0) break;
		while(!feof(usr)) {
			if(fread(&pessoa, sizeof(pessoa), 1, usr)==1 && pessoa.exclusao=='n' && pessoa.id==num) {
				ok=1;
				textcolor(9);gotoxy(56,15);printf("%d",pessoa.id);
				gotoxy(58,17);printf("%s",pessoa.nome);
				gotoxy(60,19);printf("%s",pessoa.email);
				if(pessoa.adm==1) {
					gotoxy(58,21);printf("ADM");
				}
				else {
					gotoxy(58,21);printf("USR");
				}
				gotoxy(72,24);printf("%d",pessoa.aniversario.mes);
				gotoxy(72,25);printf("%d",pessoa.aniversario.dia);
				gotoxy(72,26);printf("%d",pessoa.aniversario.ano);
				gotoxy(82,28);printf("%d",pessoa.senha);
				getch();
			}
		}
		if(ok==0) {
			limpatela();textcolor(12);
			gotoxy(69,18);printf("CADASTRO INEXISTENTE!");
			gotoxy(70,20);printf("DIGITE OUTRO VALOR");textcolor(0);getch();
		}
	fclose(usr);
	}
}
int pesquisa_avancada_nome_usr() {
	int ok, struc, x=52, y=10, buff_y, cont, letras, xx=0, num, yaho=0;
	char tecla, nome[250], aux[250], aux2[250], carac;
	while(true) {
		abrir_arqv_usr("rb");
		limpa_pesquisa_usr();
		gotoxy(72,8);printf("PESQUISA POR NOME");
		gotoxy(69,31);printf("(Digite 0 para: SAIR)");
		limpalinha(30); HideCursor(1);
		gotoxy(50,30);printf("Nome: "); //printa de novo o nome com string digitada
		gotoxy(55,30);	fflush(stdin);	gets(aux);	HideCursor(0);
		if(strcmpi(aux, "0")==0) {
			gamepad();
			return 1;
		}
		while(!feof(usr)) { //Caso acerte o nome de primeira
			if((fread(&pessoa, sizeof(pessoa), 1,usr)==1 && pessoa.exclusao=='n')) {
				if(strcmpi(aux,pessoa.nome)==0) {
					gamepad();
					campos_para_dados_usr("PESQUISA POR NOME",72);
					gotoxy(66,12);printf("(Pressione ENTER para voltar)");
					gotoxy(56,15);printf("%d",pessoa.id);
					gotoxy(58,17);printf("%s",pessoa.nome);
					gotoxy(60,19);printf("%s",pessoa.email);
					if(pessoa.adm==1) {
						gotoxy(58,21);printf("ADM");
					}
					else {
						gotoxy(58,21);printf("USR");
					}
					gotoxy(72,24);printf("%d",pessoa.aniversario.mes);
					gotoxy(72,25);printf("%d",pessoa.aniversario.dia);
					gotoxy(72,26);printf("%d",pessoa.aniversario.ano);
					gotoxy(82,28);printf("%d",pessoa.senha);
					getch(); xx=1;
					return 0;
				}		
			}
		}
		if(xx==0) { //Pedaço da palavra
			ok=0, buff_y=10, yaho=0, cont=0;
			char *SL;
			rewind(usr); fflush(stdin);
			while(!feof(usr)) {	
				if((fread(&pessoa, sizeof(pessoa), 1, usr)==1 && pessoa.exclusao=='n' && cont<10)) {
					strcpy(aux2,pessoa.nome);
					strupr(aux2); strupr(aux);
					SL=strstr(aux2,aux);
					if(SL!=NULL) { //printar os resultados positivos em formato de tabela
						if(SL!=NULL) { //printar os resultados positivos em formato de tabela
							ok=1;
							textcolor(0);
							//tamanho do nome e limita-lo
							letras=strlen(pessoa.nome);
							if(letras<12) {
								gotoxy(x+6, buff_y);printf("%s", pessoa.nome);
							}	
							else {
								gotoxy(x+6, buff_y);printf("%s", pessoa.nome);
								textcolor(12);gotoxy(x+18, buff_y);printf(".");textcolor(0);
								gotoxy(x+20,buff_y);printf("                             ");
							}
							gotoxy(x-1, buff_y);printf("%d", pessoa.id); //printa o ID
							if(pessoa.adm==1) { //checa se ele é ADM ou USR
								gotoxy(x+22,buff_y);printf("ADM");
							}
							else {
								gotoxy(x+22,buff_y);printf("USR");
							}
							letras=strlen(pessoa.email); //email
							if(letras<28) {
								gotoxy(x+30, buff_y);printf("%s", pessoa.email);
							}
							else {
								gotoxy(x+30, buff_y);printf("%s", pessoa.email);
								textcolor(12);gotoxy(x+58, buff_y);printf(".");textcolor(0);
							}
							setlocale(LC_ALL, "C");
							for(int i=x+60; i<x+81; i++) { //arruma o gamepad
								gotoxy(i,buff_y);printf("%c", 219);
							}
							textcolor(2);
							for(int i=12; i<=13; i++) { //arruma o botão
								gotoxy(121,i); printf("%c", 219);
								gotoxy(122,i); printf("%c", 219);
								gotoxy(123,i); printf("%c", 219);
							}
							textcolor(0);
							setlocale(LC_ALL, "Portuguese");
							cont+=1; //limitador de 10 por pagina
							buff_y+=2; //valor de y para printar as informacoes (alternável)
						}
					}
					grade(56);	grade(71);	grade(80);	grade(111);
					for(int i=10; i<30; i++){
						textcolor(7);gotoxy(72,i);printf("%c",219);
						gotoxy(73,i);printf("%c",219);
					}
				}
				if(feof(usr)) { //acabou os dados
					textcolor(6);gotoxy(50,30);printf("     FIM DE PESQUISA -> TAB = Pesquisar // ENTER = Sair");
					yaho=0;
					while(((carac=getch()) != (9||13)) && yaho!=1) {
						if(carac==13) { //Sair (enter)
							gamepad();
							fclose(usr);
							return 1;
						}
						else if(carac==9)  {//Pesquisa (tab)
							gotoxy(50,30);printf("                                                             ");
							textcolor(0);gotoxy(50,30);printf("Pesquisa pelo ID: ");
							textcolor(3);num=digita_num(68,30);
							if(num==0) {
								gamepad();
								fclose(usr);
								return 1;
							}
							rewind(usr);
							xx=0;
							while(!feof(usr)) {
								if((fread(&pessoa, sizeof(pessoa), 1,usr)==1 && pessoa.exclusao=='n' && pessoa.id==num)) {
									gamepad();
									campos_para_dados_usr("PESQUISA POR NOME",72);
									gotoxy(66,12);printf("(Pressione ENTER para voltar)");
									gotoxy(56,15);printf("%d",pessoa.id);
									gotoxy(58,17);printf("%s",pessoa.nome);
									gotoxy(60,19);printf("%s",pessoa.email);
									if(pessoa.adm==1) {
										gotoxy(58,21);printf("ADM  ");
									}
									else {
										gotoxy(58,21);printf("USR  ");
									}
									gotoxy(72,24);printf("%d",pessoa.aniversario.mes);
									gotoxy(72,25);printf("%d",pessoa.aniversario.dia);
									gotoxy(72,26);printf("%d",pessoa.aniversario.ano);
									gotoxy(82,28);printf("%d",pessoa.senha);
									getch(); xx=1;
									return 0;	
								}
							}
							if(xx==1) 
								return 0;
								
							else if(xx==0){
								rewind(usr);
								limpatela();textcolor(12);
								gotoxy(67,18);printf("NENHUM CADASTRO ENCONTRADO");
								gotoxy(69,20);printf("DIGITE OUTRO NOME E ID");textcolor(0);
								getch(); return 0;
							}
						}
					}
				}
				else if (cont>=10) { //Próxima página (nao acabou os dados)
					yaho=0;
					textcolor(12);gotoxy(x+56,buff_y);printf("->");textcolor(0);
					fflush(stdin);
					while(((carac=getch()) != (9||13)) && yaho!=1) {
						if(carac==13) {
							//Avança (enter)
							limpa_pesquisa_usr();
							buff_y=10;
							while(true) {
								if((fread(&pessoa, sizeof(pessoa), 1, usr)==1 && pessoa.exclusao=='n')) {
									strcpy(aux2,produto.nome);
									strupr(aux2); strupr(aux);
									SL=strstr(aux2,aux); 
									if(SL!=NULL) {
										ok=1;
										textcolor(0);
										//tamanho do nome e limita-lo
										if(letras<12) {
											gotoxy(x+6, buff_y);printf("%s", pessoa.nome);
										}	
										else {
											gotoxy(x+6, buff_y);printf("%s", pessoa.nome);
											textcolor(12);gotoxy(x+18, buff_y);printf(".");textcolor(0);
											gotoxy(x+20,buff_y);printf("     ");
										}
										gotoxy(x-1, buff_y);printf("%d", pessoa.id); //printa o ID
										if(pessoa.adm==1) { //checa se ele é ADM ou USR
											gotoxy(x+22,buff_y);printf("ADM");
										}
										else {
											gotoxy(x+22,buff_y);printf("USR");
										}
										letras=strlen(pessoa.email); //email
										if(letras<28) {
											gotoxy(x+30, buff_y);printf("%s", pessoa.email);
										}
										else {
											gotoxy(x+30, buff_y);printf("%s", pessoa.email);
											textcolor(12);gotoxy(x+58, buff_y);printf(".");textcolor(0);
										}
										setlocale(LC_ALL, "C");
										for(int i=x+60; i<x+81; i++) { //arruma o gamepad
											gotoxy(i,buff_y);printf("%c", 219);
										}
										textcolor(2);
										for(int i=12; i<=13; i++) { //arruma o botão
											gotoxy(121,i); printf("%c", 219);
											gotoxy(122,i); printf("%c", 219);
											gotoxy(123,i); printf("%c", 219);
										}
										textcolor(0);
										setlocale(LC_ALL, "Portuguese");
										buff_y+=2;
										}
									grade(56);	grade(71);	grade(80);	grade(111);
								}
								if(feof(usr)) {
									textcolor(6);gotoxy(50,30);printf("     FIM DE PESQUISA -> TAB = Pesquisar // ENTER = Sair");
									while(((carac=getch()) != (9||13)) && yaho!=1) {
										if(carac==13) { //Sair (enter)
											gamepad();
											fclose(usr);
											return 1;
										}
										else if(carac==9)  {//Pesquisa (tab)
											gotoxy(50,30);printf("                                                             ");
											textcolor(0);gotoxy(50,30);printf("Pesquisa pelo ID: ");
											textcolor(3);num=digita_num(68,30);
											if(num==0) {
												gamepad();
												fclose(usr);
												return 1;
											}
											rewind(usr);
											xx=0;
											while(!feof(prod)) {
												if((fread(&pessoa, sizeof(pessoa), 1,usr)==1 && pessoa.exclusao=='n' && pessoa.id==num)) {
													gamepad();
													campos_para_dados_usr("PESQUISA POR NOME",72);
													gotoxy(66,12);printf("(Pressione ENTER para voltar)");
													gotoxy(56,15);printf("%d",pessoa.id);
													gotoxy(58,17);printf("%s",pessoa.nome);
													gotoxy(60,19);printf("%s",pessoa.email);
													if(pessoa.adm==1) {
														gotoxy(58,21);printf("ADM");
													}
													else {
														gotoxy(58,21);printf("USR");
													}
													gotoxy(72,24);printf("%d",pessoa.aniversario.mes);
													gotoxy(72,25);printf("%d",pessoa.aniversario.dia);
													gotoxy(72,26);printf("%d",pessoa.aniversario.ano);
													gotoxy(82,28);printf("%d",pessoa.senha);
													getch(); xx=1;
													return 0;	
												}
											}
											if(xx==1) 
												return 0;
												
											else if(xx==0){
												rewind(usr);
												limpatela();textcolor(12);
												gotoxy(67,18);printf("NENHUM CADASTRO ENCONTRADO");
												gotoxy(69,20);printf("DIGITE OUTRO NOME E ID");textcolor(0);
												getch(); return 0;
											}
										}
									}
								}
							}
							break;
						}
						else if(carac==9) {
							//Pesquisa por id específico (tab)
							gotoxy(50,30);printf("                                                    ");
							gotoxy(50,30);printf("Pesquisa pelo ID: ");
							textcolor(3);num=digita_num(68,30);
							if(num==0) {
								gamepad();
								return 1;
							}
							rewind(prod);
							if((fread(&pessoa, sizeof(pessoa), 1,usr)==1 && pessoa.exclusao=='n' && pessoa.id==num)) {
									gamepad();
									campos_para_dados_usr("PESQUISA POR NOME",72);
									gotoxy(66,12);printf("(Pressione ENTER para voltar)");
									gotoxy(56,15);printf("%d",pessoa.id);
									gotoxy(58,17);printf("%s",pessoa.nome);
									gotoxy(60,19);printf("%s",pessoa.email);
									if(pessoa.adm==1) {
										gotoxy(58,21);printf("ADM");
									}
									else {
										gotoxy(58,21);printf("USR");
									}
									gotoxy(72,24);printf("%d",pessoa.aniversario.mes);
									gotoxy(72,25);printf("%d",pessoa.aniversario.dia);
									gotoxy(72,26);printf("%d",pessoa.aniversario.ano);
									gotoxy(82,28);printf("%d",pessoa.senha);
									getch(); xx=1;
									return 0;	
								}
							else {
								limpatela();textcolor(12);
								gotoxy(67,18);printf("NENHUM CADASTRO ENCONTRADO");
								gotoxy(69,20);printf("DIGITE OUTRO NOME E ID");textcolor(0);
								getch(); return 0;
							}
						}
					}
				}
			}
			if(ok==0) {
				limpatela();textcolor(12);
				gotoxy(67,18);printf("NENHUM CADASTRO ENCONTRADO");
				gotoxy(71,20);printf("DIGITE OUTRO NOME");textcolor(0);
				getch(); return 0;
			}
		}
		fclose(usr);
		gamepad();
	}
}
//Alteração (pt4)
void altera_usr() {
	cadastro pessoa_aux; //struct auxiliar para buffer de alteração
	int aux_id,F;
	long fposicao;
	char conf;
	while(true) {
		if(abrir_arqv_usr("rb+")==1) return;
		limpatela();
		campos_para_dados_usr("ALTERÇÃO DOS PRODUTOS", 70);
		gotoxy(70,12);printf("(Digite 0 para: SAIR)");
	    HideCursor(1);	aux_id=digita_num(56,15);
	    if(aux_id==0) {
	    	HideCursor(0);
	    	return;
		}
	    
	    F = 0;
	    do {
			fread(&pessoa,sizeof(pessoa),1,usr);
			if (pessoa.id == aux_id && pessoa.exclusao=='n')	{ //SOMENTE SE NÃO FOI EXCLUIDO
		   		F = 1;
		   		fposicao = ftell(usr);
		   		mostra_dados_altera_usr("ALTERAÇÃO DE USUÁRIOS ",69);
		   		pessoa_aux = pessoa;
		   		digita_dados_usr(pessoa.adm, pessoa.aniversario.mes, pessoa.aniversario.dia, pessoa.aniversario.ano, pessoa.senha); //dalteração dos dados
				
				//testa se os campos estao em branco ou não (volta o antigo/nao altera)
				if (pessoa.nome[0] == '\x0')
				    strcpy(pessoa.nome, pessoa_aux.nome);
				if (pessoa.email[0]== '\x0')
				   strcpy(pessoa.email, pessoa_aux.email);
				//O ESTADO DE ADM JA FOI TESTADO NA ALTERAÇÃO DOS DADOS
				if (pessoa.aniversario.mes == 0)
				    pessoa.aniversario.mes = pessoa_aux.aniversario.mes;
				if(pessoa.aniversario.dia == 0)
					pessoa.aniversario.dia = pessoa_aux.aniversario.dia;
				if (pessoa.aniversario.ano== 0)
				    pessoa.aniversario.ano = pessoa_aux.aniversario.ano;
				if(pessoa.senha == 0)
					pessoa.senha = pessoa_aux.senha;
				
				textcolor(0); gotoxy(81,29); printf("Confirma alteração? (S/N): ");
			   	do {
			    	gotoxy(108,29); conf = getchar();
			   	} while( conf != 's' && conf != 'S' &&  conf != 'n' && conf != 'N' );
			   	if( conf == 's' || conf == 'S' ) {
			   		fseek (usr,fposicao-(sizeof(pessoa)),SEEK_SET);
					fwrite (&pessoa,sizeof(pessoa),1,usr);
					fflush (usr);
			   	}
			}
		} while((!F) && (!feof(usr))); 
		if (F==0) {
			HideCursor(0);limpatela();textcolor(12);
			gotoxy(69,18);printf("CADASTRO INEXISTENTE!");
			gotoxy(70,20);printf("DIGITE OUTRO VALOR");textcolor(0);getch();
		} 	
	}
}
void mostra_dados_altera_usr(char frase[5000], int x) {
	gotoxy(x,11);puts(frase);
	gotoxy(70,12);printf("                         ");
	textcolor(2);gotoxy(52,15);printf("Id: ");textcolor(0);printf("%d",pessoa.id);
	textcolor(2);gotoxy(52,17);printf("Nome: ");textcolor(0);puts(pessoa.nome);
	textcolor(2);gotoxy(52,19);printf("E-Mail: ");textcolor(0);puts(pessoa.email);
	textcolor(2);gotoxy(52,21);printf("Tipo: ");textcolor(0);
	if(pessoa.adm==1) {
		gotoxy(58,21);printf("ADM");
	}
	else {
		gotoxy(58,21);printf("USR");
	}
	textcolor(2);gotoxy(52,24);printf("Aniversário: Mês -> ");textcolor(0);printf("%d",pessoa.aniversario.mes);
	textcolor(2);gotoxy(65,25);printf("Dia -> ");textcolor(0);printf("%d",pessoa.aniversario.dia);
	textcolor(2);gotoxy(65,26);printf("Ano -> ");textcolor(0);printf("%d",pessoa.aniversario.ano);
	textcolor(2);gotoxy(52,28);printf("Senha (numérica - 6 digitos): ");textcolor(0);printf("%d",pessoa.senha);
}
void digita_dados_usr(int adm, int mes, int dia, int ano, long int senha) {
	HideCursor(1);
	int aux_adm;
	char aux_mes[10000], aux_dia[10000], aux_ano[10000], aux_senha[10000];
	fflush(stdin);
	gotoxy(58,17);textcolor(9);gets(pessoa.nome);fflush(stdin);
	gotoxy(60,19);gets(pessoa.email); fflush(stdin);
	//Alteração do tipo de usuário por menu
	HideCursor(0);textcolor(0);gotoxy(58,21);printf("Deseja alterar o tipo do Usuário?");
	buffer=menu_de_2_usr("Não", "Sim", 58, 22, 12, 10);
	if(buffer==23) {
		for(int i=21; i<=23; i++) {
			gotoxy(58,i);printf("                                     ");
		}
		aux_adm=menu_de_2_usr("ADM", "USR", 58, 21, 14, 14);
		textcolor(9);
		if(aux_adm==21) {
			pessoa.adm=1;
			gotoxy(58,21);printf("       ");
			gotoxy(58,22);printf("       ");
			gotoxy(58,21);printf("ADM");
		}
		else {
			pessoa.adm=0;
			gotoxy(58,21);printf("       ");
			gotoxy(58,22);printf("       ");
			gotoxy(58,21);printf("USR");
		}
	}
	else {
		if(adm==1) {
			gotoxy(58,21);printf("                                   ");
			gotoxy(58,22);printf("       ");
			gotoxy(58,23);printf("       ");
			gotoxy(58,21);printf("ADM");
		}
		else {
			gotoxy(58,21);printf("                                   ");
			gotoxy(58,22);printf("       ");
			gotoxy(58,23);printf("       ");
			gotoxy(58,21);printf("USR");
		}
	}
	HideCursor(1);fflush(stdin);
	//Alteração da data de nascimento com base na consistencia
	gotoxy(72,24);gets(aux_mes);fflush(stdin); //mes
	produto.lancamento.mes = atoi(aux_mes); //converte o texto em int
	while(pessoa.aniversario.mes>12 || pessoa.aniversario.mes<1) { //consistencia do mes
		if(pessoa.aniversario.mes==0) {
			pessoa.aniversario.mes=mes;
			break;
		}
		HideCursor(0);gotoxy(72,24);printf("                             ");
		gotoxy(72,24);textcolor(12);printf("DIGITE UM VALOR VÁLIDO!");getch();
		gotoxy(72,24);printf("                             ");
		gotoxy(72,24);textcolor(0);printf("%d", mes);
		HideCursor(1);gotoxy(72,24);textcolor(9);fflush(stdin);gets(aux_mes);
		pessoa.aniversario.mes = atoi(aux_mes); //converte o texto em int
	}	
	gotoxy(72,25);gets(aux_dia);fflush(stdin); //dia
	pessoa.aniversario.dia = atoi(aux_dia); //converte o texto em int
	while(pessoa.aniversario.dia>31 || pessoa.aniversario.dia<1) { //consistencia do dia
		if(pessoa.aniversario.dia==0) {
			pessoa.aniversario.dia=dia;
			break;
		}
		HideCursor(0);gotoxy(72,25);printf("                             ");
		gotoxy(72,25);textcolor(12);printf("DIGITE UM VALOR VÁLIDO!");getch();
		gotoxy(72,25);printf("                             ");
		gotoxy(72,25);textcolor(0);printf("%d", ano);
		HideCursor(1);gotoxy(72,25);textcolor(9);fflush(stdin);gets(aux_dia);
		pessoa.aniversario.dia = atoi(aux_dia); //converte o texto em int
	}
	gotoxy(72,26);gets(aux_ano);fflush(stdin); //ano
	pessoa.aniversario.ano = atoi(aux_ano); //converte o texto em int
	while(pessoa.aniversario.ano>2017 || pessoa.aniversario.ano<1895) { //consistencia do ano
		if(pessoa.aniversario.ano==0) {
			pessoa.aniversario.ano=ano;
			break;
		}
		HideCursor(0);gotoxy(72,26);printf("                             ");
		gotoxy(72,26);textcolor(12);printf("DIGITE UM VALOR VÁLIDO!");getch();
		gotoxy(72,26);printf("                             ");
		gotoxy(72,26);textcolor(0);printf("%d", ano);
		HideCursor(1);gotoxy(72,26);textcolor(9);fflush(stdin);gets(aux_ano);
		pessoa.aniversario.ano= atoi(aux_ano); //converte o texto em int
	}
	
	textcolor(9);gotoxy(82,28);gets(aux_senha);fflush(stdin);
	pessoa.senha = atoi(aux_senha);
	buffer=strlen(aux_senha);
	while(buffer>6 || buffer<6) {
		if(buffer==0) {
			pessoa.senha=senha;
			break;
		}
		HideCursor(0);gotoxy(82,28);printf("                             ");
		if(buffer>6) {
			gotoxy(82,28);textcolor(12);printf("SENHA MUITO GRANDE!");getch();
		}
		else {
			gotoxy(82,28);textcolor(12);printf("SENHA MUITO PEQUENA!");getch();
		}
		gotoxy(82,28);printf("                             ");
		gotoxy(82,28);textcolor(0);printf("%d", senha);
		HideCursor(1);gotoxy(82,28);textcolor(9);fflush(stdin);gets(aux_senha);
		pessoa.aniversario.ano = atoi(aux_senha); //converte o texto em int
		buffer=strlen(aux_senha);
	}	
	fflush(stdin);
	HideCursor(0);
}
void excluir_usr() {
	cadastro pessoa_aux; //struct auxiliar para buffer de alteração
	int aux_id,F;
	long fposicao;
	char conf;
	
	while(true){
		if(abrir_arqv_usr("rb+")==1) return;
		limpatela();
		campos_para_dados_usr("EXCLUSÃO DE USUÁRIOS", 70);
		gotoxy(70,12);printf("(Digite 0 para: SAIR)");
	    HideCursor(1);aux_id=digita_num(56,15);
	    if(aux_id==0) {
	    	HideCursor(0);
	    	return;
		}
	    F = 0;
	    do {
			fread( &pessoa,sizeof(pessoa),1,usr);
			if (pessoa.id == aux_id && pessoa.exclusao=='n')	{ //SOMENTE SE NÃO FOI EXCLUIDO
		   		F = 1;
		   		fposicao = ftell(usr);
		   		mostra_dados_altera_usr("EXCLUSÃO DE USUÁRIOS",69);
		   		pessoa_aux = pessoa;
				gotoxy(81,29); printf("Confirma exclusão? (S/N): ");
			   	while( conf != 's' && conf != 'S' &&  conf != 'n' && conf != 'N' ) {
			   		gotoxy(107,29); printf("   ");
			    	HideCursor(1);gotoxy(107,29); conf = getchar(); HideCursor(0);
			   	}
			   	if( conf == 's' || conf == 'S' ) {
			   		pessoa.exclusao='s';
			   		fseek (usr,fposicao-(sizeof(pessoa)),SEEK_SET);
					fwrite (&pessoa,sizeof(pessoa),1,usr);
					fflush(usr);
			   	}
			}
		} while((!F) && (!feof(usr))); 
		if (F==0) {
			HideCursor(0);limpatela();textcolor(12);
			gotoxy(69,18);printf("CADASTRO INEXISTENTE!");
			gotoxy(70,20);printf("DIGITE OUTRO VALOR");textcolor(0);getch();
		} 	
	}
}


//Funções para menus de selecionar
int menu_principal_adm() { //menu principal para os usuários administradores (completo)
	const char op1[]="Cadastrar (Produtos)", op2[]="Cadastrar (Usuários)", op3[]="Pesquisar (Produtos)", op4[]="Pesquisar (Usuários)", op5[]="Alterar   (Produtos)", op6[]="Alterar   (Usuários)", op7[]="Sobre", op8[]="Sair"; 
	int x=67, y=16, ay, old_y=y;
	char tecla;
	gotoxy(x+3,y);printf(op1);
	gotoxy(x+3,y+1);printf(op2);
	gotoxy(x+3,y+2);printf(op3);
	gotoxy(x+3,y+3);printf(op4);
	gotoxy(x+3,y+4);printf(op5);
	gotoxy(x+3,y+5);printf(op6);
	gotoxy(x+3,y+6);printf(op7);
	gotoxy(x+3,y+7);printf(op8);
	textcolor(14);gotoxy(x,y);printf("-> ");
	do {
		//Data e hora
		textcolor(0);
		SYSTEMTIME time;
		GetLocalTime(&time);
		gotoxy(50,29);printf("Data: %2d/%2d/%2d",time.wDay, time.wMonth, time.wYear);
		gotoxy(50,30);;printf("Horário: %2dh:%2dmin", time.wHour, time.wMinute);
		textcolor(0);
		
		//Mover a setinha
		if(tecla==80 && y<23) //Menor que o mais em baixo possível
			y+=1;
		else if(tecla==80 && y==23) //O mais em baixo possível
			y=16;
		else if(tecla==72 && y>16) //Maior que o mais em baixo possível
			y-=1;
		else if(tecla==72 && y==16) //O mais em cima possível
			y=23;		
		ay=y;
		if(tecla==80 || tecla==72)
		{
			textcolor(14);
			for(int i=16; i<24; i++) {
				gotoxy(x,i);printf("   ");
				gotoxy(x,ay);printf("-> ");
			}
		}
		
		//Pintar a linha da setinha
		textcolor(14);
		if(y==16) { 
			gotoxy(x+3,old_y);printf(op1);
			textcolor(0);
			gotoxy(x+3,old_y+1);printf(op2);
			gotoxy(x+3,old_y+7);printf(op8);
		}
		else if(y==17) {
			gotoxy(x+3,old_y+1);printf(op2);
			textcolor(0);
			gotoxy(x+3,old_y);printf(op1);
			gotoxy(x+3,old_y+2);printf(op3);
		}
		else if(y==18) {
			gotoxy(x+3,old_y+2);printf(op3);
			textcolor(0);
			gotoxy(x+3,old_y+1);printf(op2);
			gotoxy(x+3,old_y+3);printf(op4);
		}
		else if(y==19) {
			gotoxy(x+3,old_y+3);printf(op4);
			textcolor(0);
			gotoxy(x+3,old_y+2);printf(op3);
			gotoxy(x+3,old_y+4);printf(op5);
		}
		else if(y==20) {
			gotoxy(x+3,old_y+4);printf(op5);
			textcolor(0);
			gotoxy(x+3,old_y+3);printf(op4);
			gotoxy(x+3,old_y+5);printf(op6);
		}
		else if(y==21) {
			gotoxy(x+3,old_y+5);printf(op6);
			textcolor(0);
			gotoxy(x+3,old_y+4);printf(op5);
			gotoxy(x+3,old_y+6);printf(op7);
		}
		else if(y==22) {
			gotoxy(x+3,old_y+6);printf(op7);
			textcolor(0);
			gotoxy(x+3,old_y+5);printf(op6);
			gotoxy(x+3,old_y+7);printf(op8);
		}
		else if(y==23) {
			textcolor(12);gotoxy(x+3,old_y+7);printf(op8);
			gotoxy(x,ay);printf("-> ");
			textcolor(0);
			gotoxy(x+3,old_y);printf(op1);
			gotoxy(x+3,old_y+6);printf(op7);
		}
	}while((tecla = getch()) != 13);
	return y;
}
int menu_principal_usr() { //menu principal para os usuários comuns (menos opções)
	const char op1[]="Pesquisar (Produtos)", op2[]="Pesquisar (Usuários)", op3[]="Sobre", op4[]="Sair"; 
	int x=67, y=18, ay, old_y=y;
	char tecla;
	gotoxy(x+3,y);printf(op1);
	gotoxy(x+3,y+1);printf(op2);
	gotoxy(x+3,y+2);printf(op3);
	gotoxy(x+3,y+3);printf(op4);
	textcolor(14);gotoxy(x,y);printf("-> ");
	do {
		//Data e hora
		textcolor(0);
		SYSTEMTIME time;
		GetLocalTime(&time);
		gotoxy(50,29);printf("Data: %2d/%2d/%2d",time.wDay, time.wMonth, time.wYear);
		gotoxy(50,30);;printf("Horário: %2dh:%2dmin", time.wHour, time.wMinute);
		textcolor(0);
		
		//Mover a setinha
		if(tecla==80 && y<21) //Menor que o mais em baixo possível
			y+=1;
		else if(tecla==80 && y==21) //O mais em baixo possível
			y=18;
		else if(tecla==72 && y>18) //Maior que o mais em baixo possível
			y-=1;
		else if(tecla==72 && y==18) //O mais em cima possível
			y=21;		
		ay=y;
		if(tecla==80 || tecla==72)
		{
			textcolor(14);
			for(int i=18; i<22; i++) {
				gotoxy(x,i);printf("   ");
				gotoxy(x,ay);printf("-> ");
			}
		}
		
		//Pintar a linha da setinha
		textcolor(14);
		if(y==18) { 
			gotoxy(x+3,old_y);printf(op1);
			textcolor(0);
			gotoxy(x+3,old_y+1);printf(op2);
			gotoxy(x+3,old_y+3);printf(op4);
		}
		else if(y==19) {
			gotoxy(x+3,old_y+1);printf(op2);
			textcolor(0);
			gotoxy(x+3,old_y);printf(op1);
			gotoxy(x+3,old_y+2);printf(op3);
		}
		else if(y==20) {
			gotoxy(x+3,old_y+2);printf(op3);
			textcolor(0);
			gotoxy(x+3,old_y+1);printf(op2);
			gotoxy(x+3,old_y+3);printf(op4);
		}
		else if(y==21) {
			textcolor(12);gotoxy(x+3,old_y+3);printf(op4);
			gotoxy(x,ay);printf("-> ");
			textcolor(0);
			gotoxy(x+3,old_y);printf(op1);
			gotoxy(x+3,old_y+2);printf(op3);
		}
	}while((tecla = getch()) != 13);
	return y;
}
int menu_de_2(int x, const char op1[], const char op2[], int c1, int c2) { //menu para 2 opções
	int y=19, old_y=y;
	char tecla;
	gotoxy(x+3,y);printf(op1);
	gotoxy(x+3,y+1);printf(op2);
	gotoxy(x,y);printf("-> ");
	do
	{	
		if(tecla==80 && y<20) //Menor que o mais em baixo possível
			y+=1;
		else if(tecla==80 && y==20) //O mais em baixo possível
			y=19;
		else if(tecla==72 && y>19) //Maior que o mais em baixo possível
			y-=1;
		else if(tecla==72 && y==19) //O mais em cima possível
			y=20;		
		
		//Pintar a linha da setinha
		if(y==19) {
			textcolor(c1);gotoxy(x,19);printf("-> ");
			gotoxy(x+3,old_y);printf(op1);
			gotoxy(x,20);printf("   ");
			textcolor(0);gotoxy(x+3,old_y+1);printf(op2);
		}
		else {
			textcolor(c2);gotoxy(x,20);printf("-> ");
			gotoxy(x+3,old_y+1);printf(op2);
			gotoxy(x,19);printf("   ");
			textcolor(0);gotoxy(x+3,old_y);printf(op1);
		}
	}while((tecla = getch()) != 13);
	return y;
}
int menu_de_2_usr(const char op1[], const char op2[], int x, int y, int c1, int c2) { //menu para 2 opções (ADM ou COMUM -> adicionar dados de usuários)
	int old_y=y, stat=y;
	char tecla;
	gotoxy(x+3,y);printf(op1);
	gotoxy(x+3,y+1);printf(op2);
	gotoxy(x,y);printf("-> ");
	do
	{	
		if(tecla==80 && y<stat+1) //Menor que o mais em baixo possível
			y+=1;
		else if(tecla==80 && y==stat+1) //O mais em baixo possível
			y=stat;
		else if(tecla==72 && y>stat) //Maior que o mais em baixo possível
			y-=1;
		else if(tecla==72 && y==stat) //O mais em cima possível
			y=stat+1;		
		
		//Pintar a linha da setinha
		if(y==stat) {
			textcolor(c1);gotoxy(x,stat);printf("-> ");
			gotoxy(x+3,old_y);printf(op1);
			gotoxy(x,stat+1);printf("   ");
			textcolor(0);gotoxy(x+3,old_y+1);printf(op2);
		}
		else {
			textcolor(c2);gotoxy(x,stat+1);printf("-> ");
			gotoxy(x+3,old_y+1);printf(op2);
			gotoxy(x,stat);printf("   ");
			textcolor(0);gotoxy(x+3,old_y);printf(op1);
		}
	}while((tecla = getch()) != 13);
	return y;
}
int menu_de_3(int x, const char op1[], const char op2[], const char op3[], int c1, int c2, int c3) { //menu para 3 opções
	int y=18, old_y=y;
	char tecla;
	gotoxy(x+3,y);printf(op1);
	gotoxy(x+3,y+1);printf(op2);
	gotoxy(x+3,y+2);printf(op3);
	gotoxy(x,y);printf("-> ");
	do
	{	
		if(tecla==80 && y<20) //Menor que o mais em baixo possível
			y+=1;
		else if(tecla==80 && y==20) //O mais em baixo possível
			y=18;
		else if(tecla==72 && y>18) //Maior que o mais em baixo possível
			y-=1;
		else if(tecla==72 && y==18) //O mais em cima possível
			y=20;		
		
		//Pintar a linha da setinha
		//arrumar
		if(y==18) {
			textcolor(c1);gotoxy(x,18);printf("-> ");
			gotoxy(x+3,old_y);printf(op1);
			gotoxy(x,19);printf("   ");gotoxy(x,20);printf("   ");
			textcolor(0);
			gotoxy(x+3,old_y+1);printf(op2);
			gotoxy(x+3,old_y+2);printf(op3);
		}
		else if(y==19){
			textcolor(c2);gotoxy(x,19);printf("-> ");
			gotoxy(x+3,old_y+1);printf(op2);
			gotoxy(x,18);printf("   ");gotoxy(x,20);printf("   ");
			textcolor(0);
			gotoxy(x+3,old_y);printf(op1);
			gotoxy(x+3,old_y+2);printf(op3);
		}
		else if(y==20){
			textcolor(c3);gotoxy(x,20);printf("-> ");
			gotoxy(x+3,old_y+2);printf(op3);
			gotoxy(x,18);printf("   ");gotoxy(x,19);printf("   ");
			textcolor(0);
			gotoxy(x+3,old_y);printf(op1);
			gotoxy(x+3,old_y+1);printf(op2);
		}
	}while((tecla = getch()) != 13);
	return y;
}


//Funções Extras
int digita_num(int x, int y) { //digitar e o scanf não bugar (caso seja inserido um caractere) para numeros tipo INT
	int i, num;
	do {
		gotoxy(x,y);
		num=scanf("%d", &i);
		if(num!=1) 	{
			gotoxy(x,y);printf("                           ");
			textcolor(12);gotoxy(x,y);printf("DIGITE UM NÚMERO!");textcolor(9);
			getch();
			gotoxy(x,y);printf("                           ");
			scanf("%*s");
		}
	} while (num!=1);
	return i;
} 
int consistencia_int(int num, int min, int max, int x, int y, int caso) { //consistencia dos dados digitados pelo usuário
	if(caso==1) { //caso com um valor minimo e maximo
		while(num<min || num>max) {
			textcolor(12);HideCursor(0);
			gotoxy(x,y);printf("VALOR INVALIDO!");textcolor(9);
			getch();HideCursor(1);
			for(int a=x; a<110; a++) {
				gotoxy(a,y);printf(" ");
			}
			num=digita_num(x,y);
		}
	}
	else if(caso==2) { //caso com apenas um valor mínimo
		while(num<min) {
			textcolor(12);HideCursor(0);
			gotoxy(x,y);printf("VALOR INVALIDO!");textcolor(9);
			getch();HideCursor(1);
			for(int a=x; a<110; a++) {
				gotoxy(a,y);printf(" ");
			}
			num=digita_num(x,y);
		}
	}
	else if(caso==3) {
		
	}
	return num;
}
int consistencia_int_dia(int num, int mes, int x, int y) { //consistencia dos dados digitados pelo usuário
	if(mes==2) { //fevereiro 
		while(num>28 || num<1) {
			textcolor(12);HideCursor(0);
			gotoxy(x,y);printf("VALOR INVALIDO!");textcolor(9);
			getch();HideCursor(1);
			for(int a=x; a<110; a++) {
				gotoxy(a,y);printf(" ");
			}
			num=digita_num(x,y);
		}
	}
	else if(mes==1 || mes==3 || mes==5 || mes==7 || mes==8 || mes==10 || mes==12) {
		while(num>31 || num<1) {
			textcolor(12);HideCursor(0);
			gotoxy(x,y);printf("VALOR INVALIDO!");textcolor(9);
			getch();HideCursor(1);
			for(int a=x; a<110; a++) {
				gotoxy(a,y);printf(" ");
			}
			num=digita_num(x,y);
		}
	}
	else {
		while(num>30 || num<1) {
			textcolor(12);HideCursor(0);
			gotoxy(x,y);printf("VALOR INVALIDO!");textcolor(9);
			getch();HideCursor(1);
			for(int a=x; a<110; a++) {
				gotoxy(a,y);printf(" ");
			}
			num=digita_num(x,y);
		}
	}
	return num;
}

