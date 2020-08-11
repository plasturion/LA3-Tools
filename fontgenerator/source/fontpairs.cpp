#include <stdio.h>
#include <stdlib.h>
char font[94][18][18],zn[18][18],syf[256];
unsigned char buf[16][16],bc[128];
void show(int item){
    printf("----- %03d -----\n",item);
    for(int i=0; i<16; i++)
        printf("%s\n",font[item][i]);
    return;
}

void showPair(int item){
    printf("----- %04d -----\n",item);
    for(int i=0; i<16; i++)
        printf("%s\n",zn[i]);
    return;
}

void showBinItem(){
    printf("----- Encoded -----\n");
    for(int i=0; i<16; i++){
        for(int j=0; j<16; j++)
            printf("%c",buf[i][j]);
        printf("\n");
    }
    return;
}

void join(int x, int y){
    for(int i=0; i<16; i++){
        for(int j=0; j<17; j++)
            zn[i][j] = font[x][i][j];
        for(int j=5; j<10; j++)
            zn[i][j] = font[y][i][j-5];
    }

    return;
}

void makeBinItem(){
    int c=-1;
        for(int i=0; i<4; i++){
            for(int j=0; j<8; j++){
                buf[i][j] = zn[i*2][j];
            }
            for(int j=8; j<16; j++){
                buf[i][j] = zn[i*2+1][j-8];
            }
        }
        for(int i=4; i<8; i++){
            for(int j=0; j<8; j++){
                buf[i][j] = zn[i*2-8][j+8];
            }
            for(int j=8; j<16; j++){
                buf[i][j] = zn[i*2+1-8][j];
            }
        }
        //--------- next
        for(int i=8; i<12; i++){
            for(int j=0; j<8; j++){
                buf[i][j] = zn[8+(i-8)*2][j];
            }
            for(int j=8; j<16; j++){
                buf[i][j] = zn[8+(i-8)*2+1][j-8];
            }
        }
        for(int i=12; i<16; i++){
            for(int j=0; j<8; j++){
                buf[i][j] = zn[8+(i-12)*2][j+8];
            }
            for(int j=8; j<16; j++){
                buf[i][j] = zn[8+(i-12)*2+1][j];
            }
        }
         //zeroing bc;
         for(int i=0; i<128; i++)
                bc[i] = 0;

         //making binary char
         for(int i=0; i<16; i++)
            for(int j=0; j<8; j++){
                if(buf[ i][j*2]=='#')
                    bc[i *8 + j] |= 0x01;
                if(buf[ i][j*2+1] =='#')
                    bc[i * 8 + j] |= 0x10;
            }

    return;
}
int main ()
{
  int n;
  scanf("%d",&n);
  for(int k=0; k<n; k++){
    scanf(" %s %s %s",&syf,&syf,&syf);
    scanf("%c",&syf[0]);
        for(int i=0; i<16; i++){
            for(int j=0; j<16; j++)
                scanf("%c",&font[k][i][j]);
            scanf("%c",&syf[0]);
            font[k][i][16]= 0;
        }

  }

    FILE * pFile;
    long lSize;
    unsigned char * buffer=NULL;
    size_t result;

    pFile = fopen ( "font1010_16.bin" , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (unsigned char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
    fclose(pFile);

    FILE * pFw;
    pFw = fopen ("font1010_16out.bin", "wb");

    int nfonts= n;
    int firstone = 623*128;
    fwrite (buffer , sizeof(char), firstone, pFw);
    int c=0;//counter;
    for(int i=0; i<nfonts; i++)
        for(int j=0; j<nfonts; j++){

			join(i,j);
            makeBinItem();
            showPair(i*nfonts+j);
            //showBinItem();
            fwrite (bc , sizeof(char), sizeof(bc), pFw);
            c++;
        }
    fclose (pFw);
    free(buffer);

  return 0;
}
