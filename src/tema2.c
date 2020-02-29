#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structuri.h"
//BAJENARU CATALIN
//GRUPA 311CB

//lista de sisteme hanoi, identificatorul fiind culoarea
//un sistem hanoi are 3 stive 1 coada o culoare
//si o valoare care indica daca a fost calculata coada

#define cul(p)  (((Han)(p)->info)->culoare)
#define sis(p)  (((Han)(p)->info))

void littlefree(TLista it)
{
	TLista aux;
	while(it)
		{
			aux = it->urm;
			free(it->info);
			free(it);
			it = aux;
		}
}

void eliberare(TLista L)
{//dealoca tot ce a fost alocat si nu a fost dealocat inca
	TLista p = L, it, aux;
	while(p)
	{
		it = sis(p)->A->S;
		littlefree(it);
		free(sis(p)->A);
		it = sis(p)->B->S;
		littlefree(it);
		free(sis(p)->B);
		it = sis(p)->C->S;
		littlefree(it);
		free(sis(p)->C);
		it = sis(p)->Q->S;
		littlefree(it);
		free(sis(p)->Q);	
		aux = p->urm;
		free(p->info);
		free(p);
		p = aux;	
	}
}

void initsist(TLista p, char str[22])
{//initializeaza un sistem Hanoi
	(p->info) = (Han)malloc(sizeof(struct sistemhanoi));
	sis(p)->A = (TStiva)malloc(sizeof(struct stiva));
	sis(p)->B = (TStiva)malloc(sizeof(struct stiva));
	sis(p)->C = (TStiva)malloc(sizeof(struct stiva));
	sis(p)->Q = (TCoada)malloc(sizeof(struct coada));
	if(!p->info || !sis(p)->A || !sis(p)->B || !sis(p)->C || !sis(p)->Q)
	{
		eliberare(p);
		return;
	}
	sis(p)->Q->F = sis(p)->Q->S = NULL;
	sis(p)->A->S = sis(p)->A->F = NULL;
	sis(p)->B->S = sis(p)->B->F = NULL;
	sis(p)->C->S = sis(p)->C->F = NULL;
	strcpy(cul(p), str);
	sis(p)->val = sis(p)->nrdisc = 0;
	p->urm = NULL;
}

TLista pop(TStiva A)
{//scoatere elem din stiva si-l returneaza
	if(!A->S)
		return NULL;
	TLista aux = A->S;
	A->S = A->S->urm;
	return aux;
}

void push(TStiva A, TLista p)
{//adugare elem in stiva
//inceputul listei este discul cel mai mic
	TLista aux = A->S;
	A->S = p;
	A->S->urm = aux;
}

void reverse(TStiva dest, TStiva sursa)
{//rastoarna stiva
	while(sursa->S)
	{
		push(dest, pop(sursa));
	}
}

void addisc(TStiva A, int x)
{
	TLista cel = (TLista)malloc(sizeof(struct celulag));
	if(!cel)
		return;
	cel->info = (int*)malloc(sizeof(int));
	if(!cel->info)
	{
		free(cel);
		return;
	}
	*(int*)cel->info = x;
	if(!A->S)
	{
		A->S = A->F = cel;
		A->S->urm = NULL;
		return;
	}
	TStiva aux = (TStiva)malloc(sizeof(struct stiva));
	if(!aux)
	{
		free(cel->info);
		free(cel);
		return;
	}
	int ok = 0;
	aux->F = aux->S = NULL;
	while(A->S)
	{//pune elementele din stiva actuala in alta + noul element
		if(ok || x > *((int*)A->S->info))
		{//adauga elem din fosta stiva
			push(aux, pop(A));
		}
		else
			{//adauga noul disc
				push(aux, cel);
				ok=1;
			}
	}
	if(!ok)push(aux,cel);
	reverse(A, aux);
	free(aux);
}

void add(ALista L, char str[22], int x)
{//adauga un nou disc si daca e cazul un nou sistem
	TLista p = *L, ant;
	int ok = 0;
	while(p)
	{
		if(strcmp(cul(p), str) == 0)
		{
			ok = 1;
			break;
		}
		ant = p;
		p = p->urm;
	}
	if(!ok)
	{//nu a fost gasita culoarea in lista 
		if(!(*L))
		{//nu a fost alocata inca nicio celula
			(*L) = (TLista)malloc(sizeof(struct celulag));
			if(!(*L))
				return;
			initsist(*L, str);
			addisc(sis(*L)->A, x);
			sis(*L)->nrdisc++;
			return;
		}
		else
		{//adauga o noua celula la finalul listei
			ant->urm = (TLista)malloc(sizeof(struct celulag));
			if(!ant->urm)
			{
				eliberare(*L);
				return;
			}
				initsist(ant->urm, str);
			addisc(sis(ant->urm)->A, x);
			sis(ant->urm)->nrdisc++;
		}
	}
	else
	{//exista culoarea implicit sistemul Hanoi
		addisc(sis(p)->A, x);
		sis(p)->nrdisc++;
	}
}

void show(FILE *out, TLista L, char str[22])
{
	TLista p = L, el;
	int ok = 0;
	while(p)
	{
		if(strcmp(cul(p), str) == 0)
		{
			ok = 1;
			break;
		}
		p = p->urm;
	}
	if(!ok)
	{//culoare nu apare in lista
		fprintf(out, "A_%s:\nB_%s:\nC_%s:\n", str, str, str);
		return;
	}
	TStiva aux = (TStiva)malloc(sizeof(struct stiva));
	if(!aux)
		return;
	aux->S = NULL;
	fprintf(out, "A_%s:", cul(p));	
	reverse(aux, sis(p)->A);
	while(aux->S)
	{
		el = pop(aux);
		fprintf(out, " %d", *(int *)el->info);
		push(sis(p)->A, el);
	}
	fprintf(out, "\n");
	fprintf(out, "B_%s:", cul(p));
	reverse(aux, sis(p)->B);
	while(aux->S)
	{
		el = pop(aux);
		fprintf(out, " %d", *(int *)el->info);
		push(sis(p)->B, el);
	}
	fprintf(out, "\n");
	fprintf(out, "C_%s:", cul(p));
	reverse(aux, sis(p)->C);
	while(aux->S)
	{
		el = pop(aux);
		fprintf(out, " %d", *(int *)el->info);
		push(sis(p)->C, el);
	}
	fprintf(out,"\n");
	free(aux);
}

TLista aloc(char a, char b)
{//face un element TLista cu info string
	char str[4];
	str[0] = a;
	str[1] = '-';
	str[2] = '>';
	str[3] = b;
	str[4] = 0;
	TLista cel = (TLista)malloc(sizeof(struct celulag));
	if(!cel)
		return NULL;
	cel->info = (char*)malloc(sizeof(char) * 5);
	if(!cel->info)
	{
		free(cel);
		return NULL;
	}
	cel->urm = NULL;
	memcpy((char *)cel->info, str, 5);
	return cel;
}

void qpush(TCoada queue, TLista cel)
{//adauga in coada
	if(!queue->S)
	{
		queue->F = queue->S = cel;
		queue->F->urm = queue->S->urm = NULL;
		return;
	}
	queue->F->urm = cel;
	queue->F = queue->F->urm;
	queue->F->urm = NULL;
}

TLista qpop(TCoada queue)
{//scoate din coada
	TLista aux = queue->S;
	queue->S = queue->S->urm;
	return aux;
}

void mutarihan(char init, char aux, char dest, int n, TCoada queue)
{//alg Hanoi recursiv
	if(n == 1)
	{
		qpush(queue, aloc(init, dest));
		return;
	}
	mutarihan(init, dest, aux, n - 1, queue);
	qpush(queue, aloc(init, dest));
	mutarihan(aux, init, dest, n - 1, queue);
}

void qreverse(TCoada dest, TCoada sursa)
{//pune coada sursa in fata elementelor din coada dest (nu e chiar rasturnare)
	TLista aux = qpop(sursa), ax;
	if(aux)
	{
		ax = dest->S;
		dest->S = aux;
		dest->S->urm = ax;
		ax = dest->S->urm;
		dest->F = dest->S; 
	}
	while(sursa->S)
	{
		qpush(dest, qpop(sursa));
	}
	dest->F->urm = ax;
}

void show_moves(FILE* out, TLista L,char str[22], int nr)
{
	TLista p = L;
	while(strcmp(cul(p), str) != 0)
		p = p->urm;
	if(!sis(p)->val)
	{//verifica daca a fost construita coada
		mutarihan('A', 'B', 'C', sis(p)->nrdisc, sis(p)->Q);
		sis(p)->val = 1;
	}
	else 
		if(!sis(p)->Q->S)
		{//verifica daca mai sunt elem in coada
			fprintf(out, "M_%s:\n", str);
			return;
		}
	TCoada aux = (TCoada)malloc(sizeof(struct coada));
	if(!aux)
		return;
	aux->S = NULL;
	TLista cel;
	fprintf(out, "M_%s:", str);
	while(nr-- && sis(p)->Q->S)
	{//arata mutarile si le pune intr-o coada auxiliara
		cel = qpop(sis(p)->Q);
		fprintf(out, " %s", (char*)cel->info);
		qpush(aux, cel);
	}
	fprintf(out,"\n");
	qreverse(sis(p)->Q, aux);//reconstituie coada initiala
	free(aux);
}

void play(TLista L, char str[22], int nr)
{
	TLista p = L, cel, aux;
	while(strcmp(cul(p), str) != 0)
		p = p->urm;
	if(!sis(p)->val)
	{//verifica daca s-a construit coada
		mutarihan('A', 'B', 'C', sis(p)->nrdisc, sis(p)->Q);
		sis(p)->val = 1;
	}
	else 
		if(!sis(p)->Q->S)
	return;//daca nu sunt elemente in coada
	while(nr-- && sis(p)->Q->S)
	{//face mutarile in stive
		cel = qpop(sis(p)->Q);
		if(((char*)cel->info)[0] == 'A')
		aux = pop(sis(p)->A);
		else
			if(((char*)cel->info)[0] == 'B')
			{
				aux = pop(sis(p)->B);
			}
			else
				if(((char*)cel->info)[0] == 'C')
					aux = pop(sis(p)->C);
		if(((char*)cel->info)[3] == 'A')
			push(sis(p)->A, aux);
		else
			if(((char*)cel->info)[3] == 'B')
				push(sis(p)->B, aux);
			else
				if(((char*)cel->info)[3] == 'C')
					push(sis(p)->C, aux);
				free(cel->info);
				free(cel);
	}
}

int main(int argc, char *argv[])
{
	if(argc != 3)
		return 0;
	FILE *in = fopen(argv[1], "rt");
	FILE *out = fopen(argv[2], "wt");
	TLista L = NULL;
	int n, x;
	char str[22], s[22];
	fscanf(in, "%d", &n);
	while(n--)
	{
		fscanf(in, "%s", s);
		if(strcmp(s, "add") == 0)
		{
			fscanf(in, "%s%d", str, &x);
			add(&L, str, x);
		}
		else
			if(strcmp(s, "show") == 0)
			{
				fscanf(in, "%s", str);
				show(out, L, str);
			} 
			else 
				if(strcmp(s, "show_moves") == 0)
				{
					fscanf(in, "%s%d", str, &x);
					show_moves(out, L, str, x);
				} 	
				else
					if(strcmp(s, "play") == 0)
					{
						fscanf(in, "%s%d", str, &x);
						play(L, str, x);
					}
	}
	eliberare(L);
	fclose(in);
	fclose(out);
	return 0;
}