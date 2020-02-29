#ifndef _LISTA_H_
#define _LISTA_H_

typedef struct celulag
{
	void *info;
	struct celulag *urm;
} Celula, *TLista, **ALista;

typedef struct stiva
{
	TLista S;//sus
	TLista F;//floor
}*TStiva;

typedef struct coada
{
	TLista S;
	TLista F;
}*TCoada;

typedef struct sistemhanoi
{
	char culoare[22];
	int nrdisc;
	int val;
	TStiva A, B, C;
	TCoada Q;
}*Han;

#endif
