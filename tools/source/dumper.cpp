#include <cstdio>
#include <cstdlib>
int main(int argc, char *argv[]){
    int pos=0;
    unsigned char *buffer;
    size_t result;

    FILE *p = NULL;
    FILE *pout = NULL;

    if(argc == 3){
        p    = fopen(argv[1],"rb");
        pout = fopen( argv[2], "wb");
    } else {
        p    = fopen("str.bin","rb");
        pout = fopen("outOrg.txt","wb");
    };
    if(p == NULL || pout == NULL){return -1;};
//----file read
        // obtain file size:
        fseek (p , 0 , SEEK_END);
        long lSize = ftell (p);
        rewind (p);

        // allocate memory that store the whole file:
        buffer = (unsigned char*) malloc (sizeof(char)*(lSize));
        if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

        // copy the file into the buffer:
        result = fread (buffer,1,lSize,p);
        if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
        fclose(p);

        //textheader - some additional info from header and end of file
        int nPointers = buffer[0] + buffer[1]*256 + buffer[2]*256*256;
        unsigned int trashBytes = 0;
        for(int i=lSize-4; i<lSize; i++){
            trashBytes <<= 8;
            trashBytes += buffer[i];
        }


        //dump text from first pointer to the end
        int first = buffer[4] + buffer[5]*256 + buffer[6]*256*256;
        int i=0;
        for(i = first ; i < lSize-4; i++){
            if(buffer[i] == 0){
                buffer[i] = '\n';
                if(buffer[i+1] == 0 && buffer[i+2] == 0){
                    break;
                }
            }
        }
        //text header 5-digit number of real pointers DEC, 'A'+number of zeroes at the end of file, last 4 bytes in HEX
        fprintf(pout,"%04d %c %08X\n",nPointers,'A'+(lSize-4)-(i+1),trashBytes);
        fwrite (buffer+first , sizeof(char), i-first+1, pout);
        fclose(pout);
        free(buffer);

    return 0;
}
