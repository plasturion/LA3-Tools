#include <cstdio>
#include <cstdlib>

int main(int argc, char *argv[]){

    int pTable[10000],pCounter=0,segm,nPointers;
    unsigned int trashBytes[2];
    unsigned char *buffer=NULL;
    unsigned char dest[500000],nZeros[256]={};//nZeros - zeroed
    size_t result;

    FILE *pout = NULL;
    FILE *pIn = NULL;
    if( argc == 3){
        pIn = fopen(argv[1],"rb");
        pout = fopen(argv[2],"wb");
    } else {
        pIn = fopen("inTencoded.txt","rb");
        pout = fopen("strout.bin","wb");
    }
    if(pIn == NULL || pout == NULL){ return -1;}

    //------readfile
        fseek (pIn , 0 , SEEK_END);
        long lSize = ftell (pIn);
        rewind (pIn);

        buffer = (unsigned char*) malloc (sizeof(char)*(lSize));
        if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
        if(lSize <16)
            return -1;
    //------ write buffer to the file
        fscanf(pIn,"%d %c %X ",&nPointers,&nZeros[0], &trashBytes[0]);
        rewind(pIn);
        fread (buffer , sizeof(char), 16, pIn);//read 16 bytes of textheader - no need anymore
        result = fread (buffer , sizeof(char), lSize-16, pIn);
        fclose(pIn);
    //--------------
        pCounter = 2;
        int i = 0;
        bool secondZero = false;//make new pointer every second one new line

        for( int i=0; i<lSize-16; i++){
            dest[i] = buffer[i];
            if(buffer[i] == 0x0A){
                dest[i] = 0;
            }

            //recognize the end of pointers for str*.bin files
            if( buffer[i] == 0x0A){
                if(secondZero){
                    pTable[ pCounter++ ] = i-1;
                    secondZero = false;
                } else {
                    secondZero = true;
                }
            }
        }

        free(buffer);

        //reverse bytes in trashBytes (last 4 bytes of bin file stored in textheader)
        for(int i = 0; i < 4; i++){
            trashBytes[1] <<= 8;
            trashBytes[1] += trashBytes[0] & 0xFF;
            trashBytes[0] >>= 8;
        }

        //provide NULL pointers at the end of pointers table
        for(int i = pCounter-1; i <= nPointers; i++){
            pTable[i+1] = pTable[i] +2;
        }
        pCounter = nPointers+1;//size of pTable counter + nPointers

        pTable[ 0 ] = pCounter - 1 ;//pointers counter
        pTable[ 1 ] = -2;//first pointer (+fix position)

        segm = (pCounter +1) * 4 ;
        for(int i=1;i<=pCounter;i++){
            pTable[i] += segm+2;
        }


        // write buffer to the file
        fwrite (pTable, sizeof(int), pCounter+1, pout);
        fwrite (dest , sizeof(char), lSize-16 , pout);
        fwrite (nZeros + 1 , sizeof(char), nZeros[0]-'A' , pout);
        fwrite (trashBytes+1,sizeof(int), 1, pout);
        fclose(pout);
        free(buffer);

    return 0;
}
