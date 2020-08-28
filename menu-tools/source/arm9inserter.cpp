#include <cstdio>
#include <cstdlib>
#include <cstring>

int pTable[10000];

    FILE *p = NULL;
    FILE *pin = NULL;
    FILE *pout = NULL;
    unsigned char *buffer, *buffArm9;

void freeClose(){
        fclose(pout);
        free(buffer);
        free(buffArm9);
        system("PAUSE");
        exit(0);
}
int main(int argc, char *argv[]){
    int pos=0;
    long lSize, lSizeArm9;
    size_t result;


    if(argc == 4){
        p    = fopen(argv[1],"rb");
        pin = fopen( argv[2], "rb");
        pout = fopen( argv[3], "wb");
    } else {
        p    = fopen("menuListEncoded.txt","rb");
        pin = fopen("arm9.bin","rb");
        pout = fopen("arm9out.bin","wb");
    };
    if( p == NULL || pin == NULL ||pout == NULL){return -1;};
//----file read
        // obtain file size:
        fseek (p , 0 , SEEK_END);
        lSize = ftell (p);
        rewind (p);

        // allocate memory that store the whole file:
        buffer = (unsigned char*) malloc (sizeof(char)*(lSize));
        if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

        // copy the file into the buffer:
        result = fread (buffer,1,lSize,p);
        if (result != lSize) {fputs ("Reading MenuMap error",stderr); exit (3);}
        fclose(p);
//-------------
        // obtain file size:
        fseek (pin , 0 , SEEK_END);
        lSizeArm9 = ftell (pin);
        rewind (pin);

        // allocate memory that store the whole file:
        buffArm9 = (unsigned char*) malloc (sizeof(char)*(lSizeArm9));
        if (buffArm9== NULL) {fputs ("Memory error",stderr); exit (2);}

        // copy the file into the buffer:
        result = fread (buffArm9,1,lSizeArm9,pin);
        if (result != lSizeArm9) {fputs ("Reading Arm9.bin error",stderr); exit (3);}
        fclose(pin);
//-------------
        // repointing new address to string pathnames

        // movie/opening_movie.mods     0xCAFC4
        buffArm9[ 0xCAFC4 ] = 0xF0;
        buffArm9[ 0xCAFC5 ] = 0x73;
        strcpy((char*) buffArm9 + 0x1173F0,"movie/opening_movie.mods");
        buffArm9[ 0x117409 ] = 0; //not neccessary but why not ,for eastethic

        // movie/xmas.mods              0xCAFC8
        buffArm9[ 0xCAFC8 ] = 0x0A;
        buffArm9[ 0xCAFC9 ] = 0x74;
        strcpy((char*) buffArm9 + 0x11740A,"movie/xmas.mods");

        unsigned int pTabBeg = 0x10A724;//begining of pointer table
        unsigned char word[5000];
        int pCounter = 1;
        int pr = 33; //reading pointer - ommit a header and table caption
        int pw = 0x1073D4;//writing pointer - to the first element of the first text block
        pTable[0] = pw + 0x2000000;

        for( int i=pr; i<lSize; i++, pr++, pw++){
            buffArm9[pw] = buffer[i];
            if( buffer[i] == 0x0A){
                buffArm9[pw] = 0;
            }
            //recognize the end of pointers
            if( buffer[i] == 0x0A) {
                    pTable[ pCounter++ ] = pw + 0x2000001;
                    if((i+1<lSize) && (buffer[i+1] == 0x0A)){
                        pr+=19;
                        break;
                    }
            }
        }

        pCounter--; //erease not existing last new pointer
        int pTabEnd = pTabBeg + pCounter * 4;//end of pointers table
        if(pCounter != 1212){
            printf("Reading first table error! Found %d/1212 lines\n",pCounter);
            freeClose();
        }
        printf("Writing first table...\n");
        if(pTabBeg < pw){
            printf("Not enough space for words! Please release %d bytes!\n",-(pTabBeg - pw));
            freeClose();
        }

        fwrite (buffArm9, sizeof(char), pTabBeg, pout);
        fwrite (pTable, sizeof(int), pCounter  , pout);

        printf("...done! %d bytes free\n",pTabBeg - pw);

//second table with pointer's nested subtables
struct pTableBlock{
    int begin;
    int end;
}pTabBlock[10]={ {0x10D92C,0x10D92C + 4*4} , {0x10DE40,0x10DE40 + 9*4} ,{0x10E374,0x10E374 + 14*4}, {0x10EBBC, 0x10EBBC+ 127*4 }  };

struct pointerTables{
    int length;
    int address[128];
}pTables[5];


        int iBlock = 0;
        int iTable = 0;
        pTabBeg = pTabBlock[ iBlock ].begin;//begining of pointer table
        //define size of all nested pointer's subtables
        pTables[0].length = 4;
        pTables[1].length = 9;
        pTables[2].length = 14;
        pTables[3].length = 127;
        pTables[4].length = 10000;//dummy field

        printf("\nWriting second table...\n");
        pCounter = 1;
        int pc=1; //total pointer counter
        pw = 0x10D770 ;//writing pointer - to the first element of the first text block
        pTables[ iTable ].address[0] = pw + 0x2000000;
        int lastWord=pr;
        for( int i=pr, wLen=0; i<lSize; i++, wLen++, pr++, pw++){
            word[ wLen ] = buffer[i];
            if(buffer[i] == 0x0A){
                word[wLen] = 0;
                //if word is too long move to the next block
                if(pw >= pTabBlock[ iBlock ].begin){
                    printf("Last word length:%d, %d bytes free before %d nested pTable.\n",wLen,pTabBlock[ iBlock ].begin - (pw - wLen),iBlock+1);
                    pw = pTabBlock[ iBlock++ ].end;
                    strncpy((char*) buffArm9 + pw,(const char*) word, wLen + 1 );
                    //update last - new address
                    if(pCounter==0 && iTable>0)
                        pTables[iTable-1].address[ pTables[iTable-1].length -1 ] = pw + 0x2000000;
                    else
                        pTables[iTable].address[ pCounter-1 ] = pw + 0x2000000;
                    pw += wLen ;
                } else
                    //copy word to buffer of bin9
                    strncpy((char*) buffArm9 + pw - wLen ,(const char*) word, wLen +1 );
                //set next word index to zero
                wLen = -1;

                if(pCounter >= pTables[ iTable ].length){
                    pCounter = 0;
                    iTable++;

                }
                    pTables[iTable].address[ pCounter++ ] = pw + 0x2000001;

                    if((i+1<lSize) && (buffer[i+1] == 0x0A)){
                        pr+=19;
                    break;
                }
                pc++;
            }

        }


        if(pc != 154){
            printf("Reading second table error! Found %d/154 lines!\n",pc);
            freeClose();
        }

        if(pTabBlock[3].begin < pw){
            printf("Not enough memory! Please release %d bytes!\n",-(pTabBlock[3].begin - pw));
            freeClose();
        }

        fwrite (buffArm9 + pTabEnd, sizeof(char), pTabBlock[0].begin - pTabEnd, pout);
        fwrite (pTables[0].address, sizeof(int), pTables[0].length  , pout);

        fwrite (buffArm9 + pTabBlock[0].end, sizeof(char), pTabBlock[1].begin - pTabBlock[0].end, pout);
        fwrite (pTables[1].address, sizeof(int), pTables[1].length  , pout);

        fwrite (buffArm9 + pTabBlock[1].end, sizeof(char), pTabBlock[2].begin - pTabBlock[1].end, pout);
        fwrite (pTables[2].address, sizeof(int), pTables[2].length  , pout);

        fwrite (buffArm9 + pTabBlock[2].end, sizeof(char), pTabBlock[3].begin - pTabBlock[2].end, pout);
        fwrite (pTables[3].address, sizeof(int), pTables[3].length  , pout);

        printf("...done! %d bytes free\n",pTabBlock[3].begin  - pw);



//--------------------- 3rd table
        pTabBeg = 0x11432C;//begining of pointer table
        pCounter = 1;
        pw = 0x112E30;//writing pointer - to the first element of the first text block
        pTable[0] = pw + 0x2000000;

        for( int i=pr; i<lSize; i++, pr++, pw++){
            buffArm9[pw] = buffer[i];
            if( buffer[i] == 0x0A){
                buffArm9[pw] = 0;
            }
            //recognize the end of pointers
            if( buffer[i] == 0x0A) {
                    pTable[ pCounter++ ] = pw + 0x2000001;
                    if((i+1<lSize) && (buffer[i+1] == 0x0A)){
                        pr+=19;
                        break;
                    }
            }
        }

        pCounter--; //erease not existing last new pointer
        pTabEnd = pTabBeg + pCounter * 4;//end of pointers table
        if(pCounter != 340){
            printf("Reading third table error! Found %d/340 lines\n",pCounter);
            freeClose();
        }

        printf("\nWriting third table...\n");
        if(pTabBeg < pw){
            printf("Not enough space for words! Please release %d bytes!\n",-(pTabBeg - pw));
            freeClose();
        }

        fwrite (buffArm9 + pTabBlock[3].end, sizeof(char), pTabBeg - pTabBlock[3].end, pout);
        fwrite (pTable, sizeof(int), pCounter  , pout);
 //       fwrite (buffArm9 + pTabEnd, sizeof(char), lSizeArm9 - pTabEnd, pout);
        int lBpTabEnd = pTabEnd;
        printf("...done! %d bytes free\n",pTabBeg - pw);



//--------------------- 4rd table
        pTabBeg = 0x11901c;//begining of pointer table
        pCounter = 1;
        pw = 0x11741C; //writing pointer - to the first element of the first text block
        pTable[0] = pw + 0x2000000;

        for( int i=pr; i<lSize; i++, pr++, pw++){
            buffArm9[pw] = buffer[i];
            if( buffer[i] == 0x0A){
                buffArm9[pw] = 0;
            }
            //recognize the end of pointers for str*.bin files
            if( buffer[i] == 0x0A) {
                    pTable[ pCounter++ ] = pw + 0x2000001;
                    if((i+1<lSize) && (buffer[i+1] == 0x0A)){
                        pr+=2;
                        break;
                    }
            }
        }

        pCounter--; //erease not existing last new pointer
        pTabEnd = pTabBeg + pCounter * 4;//end of pointers table
        if(pCounter != 187){
            printf("\nReading fourth table error! Found %d/187 lines\n",pCounter);
            freeClose();
        }
        printf("\nWriting fourth table...\n");
        if(pTabBeg < pw){
            printf("Not enough space for words! Please release %d bytes!\n",-(pTabBeg - pw));
            freeClose();
        }

        fwrite (buffArm9 + lBpTabEnd, sizeof(char), pTabBeg - lBpTabEnd, pout);
        fwrite (pTable, sizeof(int), pCounter  , pout);
        fwrite (buffArm9 + pTabEnd, sizeof(char), lSizeArm9 - pTabEnd, pout);

        printf("...done! %d bytes free\n",pTabBeg - pw);


        freeClose();
        return 0;

}
