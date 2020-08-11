#include <cstdio>
#include <cstdlib>
#include <cstring>
//#include <ctime>

    int pos=0,fileSize;
    unsigned char *buffer, dest[500000];
    char fileName[10]={};
    size_t result;
    unsigned char sign[82]=" !\"#%&'()*+,-./:=?0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int pw=0,pr=0,liner=1;
    long lSize;
    bool selectionFile = false;


void ignoreLines(int n){
    for(int i=0; i<n; i++)
        for(++pr; ( pr<lSize ) && (buffer[pr] != 0x0A); pr++);
    return;
}

void printFileLineID(int mode){
    //print filename
    if(mode >0){
        for(int i=0; i<fileSize; i++)
            dest[pw++] = fileName[i];
        dest[pw]=' ';
    }
    //print line number
    dest[++pw]='0'+(liner/1000)%10;
    dest[++pw]='0'+(liner/100)%10;
    dest[++pw]='0'+(liner/10)%10;
    dest[++pw]='0'+liner%10;
    if(mode >0) dest[++pw]='\n';
}

int main(int argc, char *argv[]){

    //srand (time(NULL));
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
        lSize = ftell (p);
        rewind (p);

        // allocate memory that store the whole file:
        buffer = (unsigned char*) malloc (sizeof(char)*(lSize));
        if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

        // copy the file into the buffer:
        result = fread (buffer,1,lSize,p);
        if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
        fclose(p);
//----end reading

        //retrive filename in string /str000.txt
        if( argc==3 ){
            int i=0;//position of filename begining
            int k=0;//end position
            int length = strlen(argv[2]);
            //find begining of the filename
            for(i = length-3; i>=0; i--)
                if(argv[2][i]=='s' && argv[2][i+1]=='t' && argv[2][i+2]=='r'){
                    break;
                }
            //now change direction and find first dot
            for( k = i; k < length-1; k++){
                if(argv[2][k]=='.'){
                    fileSize = k - i;
                    strncpy(fileName,argv[2]+i,(fileSize<=6)?fileSize:6);
                    break;
                }
            }
        } else {
            fileSize = 4;
            strcpy(fileName,"Line");
        }
        //selectionFile = false;
        bool wasNewline = false;
        if(buffer[16] == 0x0A)
            selectionFile = true;

        for(pr = 16, pw = 0 ; pr < lSize; pr++, pw++){
            if(pw == 0){
                    ignoreLines(1);
                    if(!selectionFile){
                        ignoreLines(1);
                        printFileLineID(1);
                    } else {
                        dest[pw]= 0x0A;
                        printFileLineID(0);
                    }
                    continue;
            }
            dest[pw] = buffer[pr];
            if (buffer[pr] == 0x0A){

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
                    //if(selectionFile && !nothing)
                    //    pw -= fileSize + 5; //cut the end of selection file (make sure it'll fit)
                    //dest [pw]= 0x0A;
                    //change last special code from $n to $p
                    if(dest[pw-1] == 'n' && dest[pw-2] == '$')
                        dest[pw-1] = 'p';
                    pw--;
/*-- random strings ------------
                    for(int i=0; i<40;i++)
                        dest[++pw] = sign[ rand() % 80];
                    dest[++pw] = '$';
                    dest[++pw] = 'n';
                    for(int i=0; i<32;i++)
                        dest[++pw] = sign[ rand() % 80];

------------------------------*/
                    dest[++pw]= 0x0A;

                    //read and ignore the line
                    ignoreLines(3);
                    //print filename and struct number in the first string (box-caption)
                    liner++;
                    pw++;
                    if(!selectionFile)
                        printFileLineID(1);
                    else {
                        dest[pw] = 0x0A;
                        if(buffer[pr+1]!='-')
                            printFileLineID(0);
                    }
            }
        }
        if(!selectionFile){
            //erase last line
            for(pw-=2;dest[pw]!=0x0A;pw--);
            pw++;
        } else
            pw-=5;
        //rewrite header
        fwrite (buffer , sizeof(char), 16, pout);
        fwrite (dest , sizeof(char), pw, pout);

        fclose(pout);
        free(buffer);

    return 0;
}
