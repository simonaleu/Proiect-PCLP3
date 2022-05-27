/*LEU-DOBRE SIMONA - 313CB*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tlg.h"
#include "thash.h"

typedef struct info_list {
	char *substanta;
}Tinfos;

typedef struct elems_list {
	char* denumire;
	int doza;
	int pret;
	TLG list;
}Telems;

void afisareinfos(void *element) {
	Tinfos *el = (Tinfos*)element;
	printf("%s ", el->substanta);
}

void afisareelems(void *element) {
	Telems *el = (Telems*) element;
	printf("Denumire: %s\n", el->denumire);
	printf("Doza %d mg\n",el->doza);
	printf("Pret: %d lei\n", el->pret);
	printf("Substante continute : ");
	Afisare (&(el->list), afisareinfos);
}

void free_info_list(void *element) {
	Tinfos *el = (Tinfos*)element;
	free (el->substanta);
	free (el);
}

void free_elems_list(void *element) {
	Telems *el = (Telems*) element;
	free(el->denumire);
	DistrugeLG (&el->list, free_info_list);
	free(el->list);
	free (el);
}

int codHash(void *el) {
	char *element = (char*)el;
	if (*element >= 'a') return *element - 'a';
	if (*element >= 'A') return *element - 'A';

	return 0; 
}

int cmpElem(void *e1, void *e2) {
	Telems *pers1 = (Telems*)e1;
	Telems *pers2 = (Telems*)e2;
	if (strcmp(pers1->denumire, pers2->denumire) == 0)
		if(pers1->doza == pers2->doza)
		return 0; 
	return 1;
}

int cmpEleminfo(void *e1, void *e2) {
	Tinfos *pers1 = (Tinfos*)e1;
	Tinfos *pers2 = (Tinfos*)e2;
	if (strcmp(pers1->substanta, pers2->substanta) == 0)
		return 0; 
	return 1;
}

int cmpDenumire(void *e1, void *e2) {
	Telems *pers1 = (Telems*)e1;
	Telems *pers2 = (Telems*)e2;
	if (strcmp(pers1->denumire, pers2->denumire) > 0)
			return 0; 
	return 1;
}

int cmp_subst(void *e1, void *e2) {
	Tinfos *s1 = (Tinfos*)e1;
	Tinfos *s2 = (Tinfos*)e2;
	if (strcmp(s1->substanta, s2->substanta) > 0)
		return 0; 
	return 1;
}

int inserare(TLG *L, void *elem, TFCmp cmp) {
	TLG p;
	int ok = 0;
	for (p = *L; p != NULL; p = p->urm) {
		if (cmp (p->info,elem) == 0) {
			ok = 1;
			free_info_list(elem);
			break;
		}
	}
	if (ok == 0) {
		TLG aux = malloc(sizeof(TCelulaG));
		if (!aux)
			return 0;
		aux->info = elem;
		aux->urm = *L;
		*L = aux;
	}
	return 1;
}


void sortare(TLG *aL, TFCmp comp) {
	TLG a, b, aux;
	for (a = *aL;  a != NULL; a = a->urm) {
		for (b = a;  b != NULL; b = b->urm) {
			if (comp(a->info,b->info) == 0) {
				aux=a->info;
				a->info=b->info;
				b->info=aux;
			}
		}	
	}
}


int GenerareHash(TH *a, void *ae, TFCmp fcmp_elem, TFCmp fcmp_denumire) {
	Telems* elem_aux = (Telems*)ae;
	int cod,rez; 
	cod = a->fh(elem_aux->denumire);
	TLG el;
	int ok = 0;
	for (el = a->v[cod]; el != NULL; el = el->urm) {
		if (fcmp_elem(el->info, ae) == 0) {
			ok = 1;
			break;
		} 
	}
	if (ok == 0) {
		int rez = inserare(&a->v[cod],ae, fcmp_elem);
		sortare(&(a->v[cod]), fcmp_denumire);
	}
	sortare(&(a->v[cod]), fcmp_denumire);
	return 1;
}

void afisare_pret(TH *ah, int prt) {
	int cod;
	TLG p,aux;
	for(int i = 0;i < ah->M; i++) {
		for(p=ah->v[i]; p != NULL; p = p->urm) {
			if(((Telems*)(p->info))->pret == prt) {
				afisareelems(p->info);
				printf("\n");
			}
		}
	}
}

int calcul_reteta(TH *ah, char *med) {
	TLG p,aux;
	int cost = 0; 
	for(int i = 0;i < ah->M; i++) {
		for(p=ah->v[i]; p != NULL; p = p->urm) {
			if(strcmp(((Telems*)(p->info))->denumire,med) == 0) {
				cost += ((Telems*)(p->info))->pret;
				break;
			}
		}
	}
	return cost;
}

void modif_pret(TH *ah, int pret_nou, char *med, int d) {
	TLG p,aux;
	int cost = 0; 
	for(int i = 0;i < ah->M; i++) {
		for(p=ah->v[i]; p != NULL; p = p->urm) {
			if(strcmp(((Telems*)(p->info))->denumire,med) == 0) {
				if(((Telems*)(p->info))->doza == d) {
					((Telems*)(p->info))->pret = pret_nou;
				}
			}
		}
	}
}

int main() {
	TH *h = NULL;
	size_t M = ('Z'-'A') + 1;
	h = (TH *) InitTH(M, codHash);
	if (h == NULL) {
		printf("Tabela hash nu a putut fi generata\n");
		return 0;
	}

	while(1) {
		// printf("Introduceti comanda:\n");
		char *line = NULL;
		size_t len;
		if( getline(&line, &len, stdin) == -1) {
			free(line);
			break;
		}
		char *token;
		char *comanda;

		token = strtok(line, " .,\n");
		if(token == NULL) continue;

		comanda = malloc(strlen(token) + 1);
		if (!comanda) {
			free(line);
			return 0;
		}
		strcpy(comanda,token);
		// < denumire doza pret [lista substante continute] >
		if (strcmp(comanda, "insert") == 0) {
			token = strtok(NULL, " .,\n");
			char *denumire;
			denumire = malloc(strlen(token) + 1);
			if(!denumire) {
				free(comanda);
				free(line);
				break;
			}
			strcpy(denumire,token);
			token = strtok(NULL, " .,\n");
			int doza;
			doza = atoi(token);
			token = strtok(NULL, " .,\n");
			int pret;
			pret = atoi(token);
			Telems *aux;
			aux = (Telems*)malloc(sizeof(Telems));
			if(!aux) {
				free(comanda);
				free(denumire);
				free(line);
				break;
			}
			aux->denumire = denumire;
			aux->doza = doza;
			aux->pret = pret;
			aux->list = NULL;
			token = strtok(NULL, " .,\n");
			while(token != NULL) {
				Tinfos *auxi;
				auxi = (Tinfos*)malloc(sizeof(Tinfos));
				if(!auxi) {
					break;
				}
				char * substanta;
				substanta = malloc(strlen(token) + 1);
				if(!substanta) {
					break;
				}
				strcpy(substanta, token);
				
				auxi->substanta = substanta;
				int rezult = inserare(&aux->list,auxi, cmpEleminfo);
				sortare(&aux->list, cmp_subst);
				token = strtok(NULL, " .,\n");
				
			}
			int rez = GenerareHash(h,aux,cmpElem, cmpDenumire);
			if(rez == 0) {
				free (comanda);
				free (line);
				break;
			}
		}
		if (strcmp (comanda, "print") == 0) {
			token = strtok(NULL, " .,\n");
			if (token == NULL) {
				AfiTH(h, afisareelems);
			}
		}
		// < pretul dupa care se realizeaza afisarea >
		if(strcmp(comanda,"afisare_pret") == 0) {
			token = strtok(NULL, " ,.\n");
			int ptr = atoi(token);
			afisare_pret(h,ptr);
		}
		// < denumirile medicamentelor pentru reteta >
		if(strcmp(comanda, "calcul_reteta") == 0) {
			int suma = 0;
			token = strtok(NULL, " ,.\n");
			while(token!=NULL) {
				
				char* med;
				med = malloc(strlen(token) + 1);
				if(!med) {
					return 0;
				}
				strcpy(med,token);
				suma += calcul_reteta(h, med);
				token = strtok(NULL, " ,.\n");
				free(med);
			}
			printf("cost total: %d\n", suma);	
		}
		// < pretul_nou denumire >
		if(strcmp(comanda,"modif_pret") == 0) {
			int d = 0;
			char* med;
			int pret_nou = 0;
			token = strtok(NULL, " ,.\n");
			pret_nou = atoi(token);
			token = strtok(NULL, " ,.\n");
			med = malloc(strlen(token) + 1);
			if(!med) {
				break;
			}
			strcpy(med, token);
			token = strtok(NULL, " ,.\n");
			d = atoi(token);
			modif_pret(h,pret_nou,med,d);
			free(med);
		}
		if(strcmp(comanda,"exit") == 0) {
			free (comanda);
			free (line);
			break;
		}

		free (comanda);
		free (line);
	}
	DistrTH(&h,free_elems_list);

	return 0;
}