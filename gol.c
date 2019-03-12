#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#define ALIVE 1
#define DEAD 0
#define THREADS_NUM 4 /*Number of threads*/

char **cells;
int **nextGenState;
int rows,columns;
/*Reads the cells table from the input file, and creates it*/
void createTable(char *fname);

/*Calculates the next generation state of a cell and updates the table with the next generation states*/
void calculateNextGenState(int i,int j);

/*Returns the number of alive neighbours for a certain cell*/
int getNoOfAliveNeighbours(int i,int j);

/*Reads the nextGenState array and updates the state of given cell for the next gen*/
void updateCell(int i ,int j);

/*Produces the output file*/
void produceOutput(char *fname);

int main(int argc, char *argv[]){
    int i,j,k=0,num_of_gens;
    createTable(argv[1]);
    num_of_gens=atoi(argv[2]);
    while(k<=num_of_gens){
    #pragma omp parallel default(none) \
        private(i,j) shared(rows,columns,cells,nextGenState) num_threads(THREADS_NUM)
	{
	#pragma omp for nowait 
        	for(i=0;i<rows;i++){
        	        for(j=0;j<columns;j++){
                    		calculateNextGenState(i,j);
                    		updateCell(i,j);
        		}
        	}
     	}
        k++;
    }
    produceOutput(argv[3]);

}








void createTable(char *fname){
    FILE *f;
    char *line=NULL,*q,*tmp;
    size_t len=0;
    int i,j;
    f=fopen(fname,"r");
    if(f==NULL){
        perror("COULD NOT OPEN FILE\n");
        return;
    }
    if(getline(&line,&len,f)==-1){
        perror("GETLINE() COULD NOT READ LINE\n");
        return;
    }
    q=line;
    while(*q!=' ')
        q++;
    *q='\0';
    rows=atoi(line);
   
    q++;
    line=q;
    while(*q!='\n')
        q++;
    *q='\0';
    columns=atoi(line);
    line=NULL;

    /*Allocating memory for the cells array*/
    cells=malloc(sizeof(char*)*rows); 
    nextGenState=malloc(sizeof(int*)*rows);
#pragma omp parallel default(none) \
   private(i,j) shared(rows,columns,cells,nextGenState) num_threads(THREADS_NUM)
    {
    	#pragma omp for nowait
    	for(i=0;i<rows;i++){
        	cells[i]=malloc(sizeof(char)*columns);
    		for(j=0;j<columns;j++){
            	cells[i][j]='-';
            
        	}

    	}
    
    	#pragma omp for nowait
    	for(i=0;i<rows;i++){
    		nextGenState[i]=malloc(sizeof(int)*columns);
		for(j=0;j<columns;j++){
        		nextGenState[i][j]=-1;
        	}

    	}
    }


          
    i=0;
    while(getline(&line,&len,f)!=-1){
        q=line;
        j=0;
        while(1){
            if(*q=='|'){
                tmp=q;
                tmp+=2;
                if(*tmp!='|'){
                	break; //Line done
                }
            }else{
                cells[i][j]=*q;
                j++;
            }
            q++;
        }
        i++;
    }
    fclose(f);
}

void updateCell(int i,int j){
        if(cells[i][j]!='-'){
            if(nextGenState[i][j]==ALIVE)
                cells[i][j]='*';
            else
                cells[i][j]=' ';
        }
}


void calculateNextGenState(int i,int j){
    int aliveNeighbours=getNoOfAliveNeighbours(i,j);
    if(cells[i][j]=='*'){
        if(aliveNeighbours<2||aliveNeighbours>3)
            nextGenState[i][j]=DEAD;
        else
            nextGenState[i][j]=ALIVE;
    }else if(cells[i][j]==' '){
        if(aliveNeighbours==3)
            nextGenState[i][j]=ALIVE;
    }
}

int getNoOfAliveNeighbours(int i,int j){
    int aliveCounter=0;
    if(i-1>=0&&j-1>=0){
        if(cells[i-1][j-1]=='*')
            aliveCounter++;
    }
    if(j-1>=0){
        if(cells[i][j-1]=='*')
            aliveCounter++;
    }
    if(i+1<=rows-1&&j-1>=0){
        if(cells[i+1][j-1]=='*')
            aliveCounter++;
    }
    if(i-1>=0){
        if(cells[i-1][j]=='*')
            aliveCounter++;
    }
    if(i-1>=0&&j+1<=columns-1){
        if(cells[i-1][j+1]=='*')
            aliveCounter++;
    }
    if(j+1<=columns-1){
        if(cells[i][j+1]=='*')
            aliveCounter++;
    }
    if(i+1<=rows-1&&j+1<=columns-1){
        if(cells[i+1][j+1]=='*')
            aliveCounter++;
    }
    if(i+1<=rows-1){
        if(cells[i+1][j]=='*')
            aliveCounter++;
    }
    return aliveCounter;
}

void produceOutput(char *fname){
    int i,j;
    FILE *f;

    f=fopen(fname,"w");

    for(i=0;i<rows;i++){
        fprintf(f,"|");
        for(j=0;j<columns;j++){
            if(cells[i][j]!='-'){
                fprintf(f,"%c|",cells[i][j]);
            }else{
                fprintf(f," ");
            }
        }
        fprintf(f,"\n");
        printf("\n");
    }
    fclose(f);
}
