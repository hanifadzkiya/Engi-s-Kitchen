/*
	File	: ADTStack.c
	Tanggal	: 1 November 2018
	Body ADT Stack
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stack/stack.h"

int IsEmptySta(Stack S)
/*
	I.S	: Stack S sembarang
	F.S	: Mengembalikan nilai 1 bila stack kosong, 0 jika tidak
*/
{
	return (Top(S)==0);
}

void CreateEmpty(Stack *S)
/*
	I.S	: Stack S belum terdefinisi
	F.S	: Stack S terdefinisi sebagai stack kosong
*/
{
	Top(*S) = 0;
}

void Push(Stack *S, Infotype str)
/*
	I.S	: Stack S dan string str terdefinisi
	F.S	: Stack S terisi dengan str sebagai TOP
*/
{
	Top(*S)++;
	strcpy(InfoTop(*S),str);
}

void Pop(Stack *S, Infotype *str)
/*
	I.S	: Stack S terdefinisi, str kosong
	F.S	: Top Stack S berkurang 1 dan str berisi string Top sebelumnya
*/
{
	strcpy((*str), InfoTop(*S));
	Top(*S)--;
}

void Reverse(Stack *S)
/*
	I.S	: Stack terdefinisi dan tidak kosong
	F.S	: Stack memiliki isi kebalikan dari stack input
*/
{
	Stack SRev;
	Infotype temp;
	CreateEmpty(&SRev);

	while(!(IsEmptySta(*S)))
	{
		Pop(&(*S),&temp);
		Push(&SRev,temp);
	}
	(*S) = SRev;
}

void PrintStack(Stack S)
/*
	I.S	: Stack terdefinisi dan tidak kosong
	F.S	: Seluruh isi stack tercetak di layar
*/
{
	char str[105];
	while (!(IsEmptySta(S))){
		Pop(&S,&str);
		printf("%s\n", str);
	}
}

void TulisFileStack(Stack S,FILE *f){
	char str[105];
	while (!(IsEmptySta(S)))
	{
		Pop(&S,&str);
		fprintf(f,"%s\n", str);
	}
}

int NbElmtSt(Stack S){
	int cnt = 0;
	while (!IsEmptySta(S)){
		cnt++;
		Infotype X;
		Pop(&S, &X);
	}
	return cnt;
}