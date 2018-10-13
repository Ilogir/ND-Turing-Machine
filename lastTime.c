#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//pulizia già includa, vanno increasing e mind
typedef struct Config{
	int cur;
	int pos;
	char* r;
	int size;
	struct Config* next;
	struct Config* prev;
}cfg;

typedef struct Transizione{
	int n2;
	char r;
	char w;
	int m; //mossa convertita in valore numerico da sommare a pos
	struct Transizione* next;
	struct Transizione* nd;
}tr;

tr** trTable; //tabella delle transizioni, colonne stati, righe carattere letto

long int max; //massimo numero mosse

int n_stati = 0;

int success = 0;
int u = 0;

char* r;

int max_int(int n1, int n2);
	
void readTr();

void run();

void readDetails();

void handleTape(cfg* curCfg);

cfg* executeMove(tr* temp, cfg* curCfg);


int main(){
	readTr();
	readDetails();
	while(scanf("%ms", &r) != EOF){
		run();
		if(u == 1 && success == 0)
			printf("U\n");
		if(success == 0 && u == 0)
			printf("0\n");
		success = 0;
		u = 0;
	}
	return 0;
}

void readTr(){
    int n1, n2, n;
    char w,r,m; //scrivi,leggi,mossa nastro
    char buffer[30];
    int* check = malloc(sizeof(int));
    check[0] = 1;
    tr** temp1,**pozzi;
    fgets(buffer,sizeof(buffer),stdin);
    while(scanf("%d %c %c %c %d",&n1,&r,&w,&m,&n2) == 5){
    	n = max_int(n1,n2);
        if(n_stati < n){
            trTable = realloc(trTable,(n+1)*sizeof(tr*));
            check = realloc(check,(n+1)*sizeof(int));
            for(int i = n_stati+1; i <= n; i++)
                check[i] = 1;
            n_stati = n;
}
        temp1 = (trTable + n1);
        if(check[n1] == 1){
            (*temp1) = malloc(sizeof(tr));
            (*temp1)->n2 = n2;
            (*temp1)->w = w;
            (*temp1)->r = r;
            switch(m){
				case 'R':
					(*temp1)->m = 1;
					break;
				case 'S':
					(*temp1)->m = 0;
					break;
				case 'L':
					(*temp1)->m = -1;
					break;
			}
            (*temp1)->nd = NULL;
            (*temp1)->next = NULL;
            check[n1] = 0;
        }
        else{
            while((*temp1)->next != NULL && r != (*temp1)->r){
        		temp1 = &((*temp1)->next);
            }
            if((*temp1)->r == r){
				while((*temp1)->nd != NULL)
					temp1 = &((*temp1)->nd);
				(*temp1)->nd = malloc(sizeof(tr));
				(*temp1)->nd->n2 = n2;
				(*temp1)->nd->w = w;
				switch(m){
				case 'R':
					(*temp1)->nd->m = 1;
					break;
				case 'S':
					(*temp1)->nd->m = 0;
					break;
				case 'L':
					(*temp1)->nd->m = -1;
					break;
				}
				(*temp1)->nd->nd = NULL;
				(*temp1)->nd->next = NULL;	
            }
            else{
				(*temp1)->next = malloc(sizeof(tr));
				(*temp1)->next->n2 = n2;
				(*temp1)->next->w = w;
				(*temp1)->next->r = r;
				switch(m){
					case 'R':
						(*temp1)->next->m = 1;
						break;
					case 'S':
						(*temp1)->next->m = 0;
						break;
					case 'L':
						(*temp1)->next->m = -1;
						break;
				}
				(*temp1)->next->nd = NULL;
				(*temp1)->next->next = NULL;
}}}
    int i = 0;
    while(i<=n_stati){
    	if(check[i] == 1){
    		pozzi = (trTable+i);
    		(*pozzi) = malloc(sizeof(tr));
    		(*pozzi)->next = NULL;
    		(*pozzi)->nd = NULL;
    		(*pozzi)->r = '?';		
    	}
    	i++;
    }
    free(check);
}

int max_int(int n1, int n2){
	if(n1 > n2)
		return n1;
	else 
		return n2;
}

void readDetails(){
    char buffer[30];
    int temp;
    fgets(buffer,sizeof(buffer),stdin);
    while(scanf("%d",&temp) == 1 ){
        if(temp > n_stati)
        	trTable = realloc(trTable,(temp+1)*sizeof(tr*));
        *(trTable+temp) = malloc(sizeof(tr));
        (*(trTable+temp))->r = '&';
        (*(trTable+temp))->next = NULL;
        (*(trTable+temp))->nd = NULL;
    }
    fgets(buffer,sizeof(buffer),stdin);
    scanf("%ld",&max);
    fgets(buffer,sizeof(buffer),stdin);
    fgets(buffer,sizeof(buffer),stdin);
}

cfg* executeMove(tr* temp, cfg* curCfg){
	cfg* new = malloc(sizeof(cfg));
	new->size = curCfg->size;
	new->cur = temp->n2;
	new->r = malloc(sizeof(char)*(new->size));
	memcpy(new->r, curCfg->r, sizeof(char) * new->size);
	new->r[curCfg->pos] = temp->w;
	new->pos = curCfg->pos + temp->m;
	new->prev = NULL;
	return new;
}

void handleTape(cfg* curCfg){
	if(curCfg->pos > (curCfg->size - 1)){
		curCfg->r = realloc(curCfg->r, sizeof(char) * (curCfg->size + 256));
		memset(curCfg->r + curCfg->size, '_', sizeof(char) * 256);
		curCfg->size += 256;
	}
	if(curCfg->pos < 0){
		curCfg->r = realloc(curCfg->r, sizeof(char) * (curCfg->size + 256));
		memmove(curCfg->r + 256, curCfg->r, sizeof(char) * curCfg->size);
		memset(curCfg->r, '_', sizeof(char) * 256);
		curCfg->size += 256;
		curCfg->pos = 255;
	}
}
		
void run(){
	cfg* curCfg;
	cfg* tempCfg;
	cfg* start;
	tr* temp;
	long int mov = 0;
	start = malloc(sizeof(cfg));
	start->r = r;
	start->size = strlen(start->r);
	start->pos = 0;
	start->cur = 0;
	start->next = NULL;
	start->prev = NULL;
	while(success == 0 && start != NULL && mov<=max){
		curCfg = start;
START:	while(curCfg != NULL){
			handleTape(curCfg);
			temp = *(trTable + curCfg->cur);
			while(temp->next != NULL && temp->r != curCfg->r[curCfg->pos])
				temp = temp->next;
			if(temp->r == curCfg->r[curCfg->pos]){
				if(temp->nd != NULL){
					while(temp->nd != NULL){
						start->prev = executeMove(temp, curCfg);
						start->prev->next = start;
						start = start->prev;
						temp = temp->nd;
					}
					curCfg->cur = temp->n2;
					curCfg->r[curCfg->pos] = temp->w;
					curCfg->pos += temp->m;
				}
				else{
					if(temp->m == 0 && temp->n2 == curCfg->cur && temp->w == curCfg->r[curCfg->pos]){
						u = 1;
						goto CLEAN;
					}
					if(temp->m == -1 && curCfg->pos == 0 && temp->n2 == curCfg->cur && temp->r == '_'){
						u = 1;
						goto CLEAN;
					}
					if(temp->m == 1 && curCfg->pos == (curCfg->size - 1) && temp->n2 == curCfg->cur && temp->r == '_'){
						u = 1;
						goto CLEAN;
					}
					curCfg->cur = temp->n2;
					curCfg->r[curCfg->pos] = temp->w;
					curCfg->pos += temp->m;
				}			
			}
			else{
				if(temp->r == '&'){
					printf("1\n");
					success = 1;
					break;
				}
				else{
CLEAN:				if(curCfg->prev == NULL){
						if(curCfg->next == NULL){
							free(curCfg->r);
							free(curCfg);
							curCfg = NULL;
							start = NULL;
							break;
						}
						else{
							tempCfg = curCfg;
							curCfg = curCfg->next;
							free(tempCfg->r);
							free(tempCfg);
							curCfg->prev = NULL;
							start = curCfg;
							goto START;
						}
					}
					else if(curCfg->next == NULL){
						curCfg = curCfg->prev;
						free(curCfg->next->r);
						free(curCfg->next);
						curCfg->next = NULL;
					}
					else{
						tempCfg = curCfg;
						curCfg->prev->next = curCfg->next;
						curCfg = curCfg->prev;
						curCfg->next->prev = curCfg;
						free(tempCfg->r);
						free(tempCfg);
					}
				}
			}
			curCfg = curCfg->next;
		}
		mov++;
	}
	if(mov > max)
		u = 1;
	}
			
					
		
		
		
		
		
		
		
		
		
		
