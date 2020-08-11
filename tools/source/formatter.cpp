#include <cstdio>
#include <cstdlib>
int main(int argc, char *argv[]){
    int pos=0;
    unsigned char *buffer, header[20]={},dest[500000];
    size_t result;

    FILE *p = NULL;
    FILE *pout = NULL;

    if(argc == 3){
        p    = fopen(argv[1],"rb");
        pout = fopen( argv[2], "wb");
    } else {
        p    = fopen("forEdit.txt","rb");
        pout = fopen("inT.txt","wb");
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
//----end reading


        int pw,pr;
        bool secondZero = false;
        bool wasNewline = false;
        for(pr = 16, pw = 0 ; pr < lSize; pr++, pw++){
            //in the main loop read the first string of the structure
            dest[pw] = buffer[pr];
            if (buffer[pr] == 0x0A){
                if(secondZero){
                    //auto formatting the second string (stop reading at "~-" sequence (guarding enough ok)
                    for(wasNewline = false; (!(buffer[pr]== '~' && buffer[pr+1]== '-' )) && (pr<lSize); pr++, pw++){
                        if(buffer[pr] == 0x0A){
                            //if you want to break windows earlier you can do it typing $p at the end of line
                            if(buffer[pr-2]=='$' && buffer[pr-1]=='p'){
                                //this move is a stop for a writing pointer
                                pw--;
                                wasNewline = false;//awaiting for a newline again
                                continue;
                            }
                            if(wasNewline){
                                dest[pw] = '$';
                                dest[++pw] ='p';
                                wasNewline = false;
                                continue;
                            } else {
                                dest[pw] = '$';
                                dest[++pw] = 'n';
                                wasNewline = true;
                                continue;
                            }
                        }
                        dest[pw] = buffer [ pr];
                    }
                    pw -= 2;
                    dest [pw]= 0x0A;
                    //change last special code from $n to $p
                    if(dest[pw-1] == 'n' && dest[pw-2]== '$')
                        dest[pw-1] = 'p';
                    //read and ignore the line
                    for(++pr; (pr < lSize ) && (buffer[pr] != 0x0A); pr++);
                    secondZero = false;
                }else {
                    //at the end of the first string read all of line of and ignore it
                    for(++pr; ( pr<lSize ) && (buffer[pr] != 0x0A); pr++);
                    secondZero = true;
                }
            }
        }
        //rewrite header
        fwrite (buffer , sizeof(char), 16, pout);
        fwrite (dest , sizeof(char), pw, pout);

        fclose(pout);
        free(buffer);

    return 0;
}
