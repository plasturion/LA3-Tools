#include <cstdio>
#include <cstdlib>

// caption of table
char textCaption[20]= {0x81,0x79,0x82,0x73,0x82,0x60,0x82,0x61,0x82,0x6B,0x82,0x64,0x82,0x50,0x81,0x7a,0,0};

int pos=0;
unsigned char *buffer;
size_t result;
long lSize;

FILE *p = NULL;
FILE *pout = NULL;


int printList(unsigned int pFirst, int c){
    unsigned int pCurrent;

    for(int i=0; i<c; i++){

            //calculate current pointer
            pCurrent = buffer[ pFirst++ ];
            pCurrent += (buffer[ pFirst++] << 8);
            pCurrent += (buffer[ pFirst++ ] << 16);
            pFirst++;

        fprintf(pout, "\n");
        for(int j = pCurrent; buffer[j]!=0; j++ ){
            fwrite(buffer+j, sizeof(char), 1, pout);
            if(buffer[j]==0x0A){
                fprintf(pout, "\nNewline value Error! Source file is invalid or different encoded!");
                fclose(pout);
                free(buffer);
                exit(2);
            }
            if((pCurrent ==  j) && buffer[j]==0 ){
                fprintf(pout, "\nError while reading - empty pointer!");
                fclose(pout);
                free(buffer);
                exit(2);
            }
        }
    }
    return 0;
}
int main(int argc, char *argv[]){


    if(argc == 3){
        p    = fopen(argv[1],"rb");
        pout = fopen( argv[2], "wb");
    } else {
        p    = fopen("arm9.bin","rb");
        pout = fopen("menuList.txt","wb");
    };
    if(p == NULL || pout == NULL){return -1;};
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
        if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
        fclose(p);


//write Header
        fprintf(pout, "Arm9.bin  Menus");


//-------------- First Table

    fprintf(pout, "\n%s",textCaption);
        printList(0x10A724,1212);

//--------------- Second Table -------

    textCaption[13]++;
    fprintf(pout, "\n\n%s",textCaption);
        printList(0x10D92C,4);
        printList(0x10DE40,9);
        printList(0x10E374,14);
        printList(0x10EBBC,127);

//-------------- Third Table

    textCaption[13]++;
    fprintf(pout, "\n\n%s",textCaption);
        printList(0x11432C,340);


//-------------- Fourth Table
    textCaption[13]++;
    fprintf(pout, "\n\n%s",textCaption);
        printList(0x11901c,187);
    fprintf(pout, "\n");
        fclose(pout);
        free(buffer);

    return 0;
}
