#include <cstdio>
#include <cstdlib>
#include <cstring>
//------------------------------------
const int N = 80;
char sign[82]=" !\"#%&'()*+,-./:=?0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
//-------------------------------------

char pairs[80*80+2][3];
int pair2shift[80*80+2];
int shiftcode = 0x889F;
int c = 0;
//now make a 2dim mapping array.256kb
int field[256][256];

//data initializing...
void init(){
    //generate all possible combinations
    for(int i=0; i< N; i++)
        for(int j=0; j< N; j++){
            pairs[i*N + j][0] = sign[i];
            pairs[i*N + j][1] = sign[j];
            pairs[i*N + j][2] = 0;
        }
    //iterating all font trhough Shift-JIS
    pair2shift[ c++ ] = shiftcode++;
    for(int i=0; i<6; i++)
        for(int j=0; j<16; j++)
            pair2shift[ c++ ] = shiftcode++;
    for(int i=0; i<15; i++){//0x89 - 0x97
        shiftcode += 64;// set second byte to 0x40
        for(int j=0; j<12; j++)//12 lines
            for(int k=0; k<16; k++)//16 columns
                pair2shift[ c++ ] = shiftcode++;
    }
    shiftcode += 64;
    for(int i=0; i<3; i++)
        for(int j=0; j<16; j++)
            pair2shift[ c++ ] = shiftcode++;
    pair2shift[ c++ ] = shiftcode++;
    pair2shift[ c++ ] = shiftcode++;
    pair2shift[ c++ ] = shiftcode++;//0x9872
    shiftcode += 16;
    for(int i=0; i<13; i++)
        pair2shift[ c++ ] = shiftcode++;//jump and start from 0x9883

    for(int i=0; i<7; i++)
        for(int j=0; j<16; j++)
            pair2shift[ c++ ] = shiftcode++;//fill to the end of 0x98xx
    for(int i=0; i<7; i++){
        shiftcode += 64;
        for(int j=0; j<12; j++)
            for(int k=0; k<16; k++)
                pair2shift[ c++ ] = shiftcode++;
    }
    //e0 segment
    shiftcode = 0xE000;
    for(int i=0; i<10; i++){
        shiftcode += 64;
        for(int j=0; j<12; j++)
            for(int k=0; k<16; k++)
                pair2shift[ c++ ] = shiftcode++;
    }
    --c;//
    c-=3;//end


    for(int i=0; i<256; i++)
        for(int j=0; j<256; j++)
            field[i][j] = 0x8140;

     for(int i=0; i<6400; i++){
        field[  pairs[i][0] ][ pairs[i][1] ] = pair2shift[i];
    }
    return;
}

//print character-pair reference list
void showAll(){
    for(int i=0; i<6400; i++)
        printf("%s %x %x\n",pairs[i],pair2shift[i],field[pairs[i][0]][pairs[i][1]]);
    return;
}



int main(int argc, char *argv[]){

    init();
    //showAll();

    FILE *pFw = NULL;
    FILE *pIn = NULL;
    if(argc == 3){
        pIn = fopen( argv[1],"rb");
        pFw = fopen( argv[2], "wb");
    } else {
        pIn = fopen("inT.txt","rb");
        pFw = fopen("inTencoded.txt", "wb");
    }
    unsigned char *buffer=NULL;
    size_t result;
    long lSize;
    bool strSelect = false;
    if(pIn != NULL){

        fseek (pIn , 0 , SEEK_END);
        lSize = ftell (pIn);
        rewind (pIn);

        buffer = (unsigned char*) malloc (sizeof(char)*(lSize));
        if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

        // write buffer to the file
        result = fread (buffer , sizeof(char), lSize, pIn);
        fclose(pIn);

    }

    int zn1, zn2,pr,pw;
    unsigned char dest[500000];
    //support for strsel.bin, strana.bin
    if(buffer[16]==0x0A)
        strSelect = true;

    for(pr=pw=16; pr<lSize; pr++,pw++){

        //Check first if it's common Shift-JIS character, if so just copy
        if( buffer[pr] > 0x80 && buffer[pr+1] >= 0x40) {
            dest[ pw ] = buffer[ pr ];
            dest[ ++pw ] = buffer[ ++pr ];
            continue;

        //then check if is script control code: recognized - $n $p $e3, also copy
        }else if(buffer[pr]=='$'){
            dest[ pw ] = buffer[ pr ];
            if(buffer[pr+1] == 'n' || buffer[pr+1] == 'p'){
                dest[ ++pw ] = buffer[ ++pr ];
            } else if ( buffer[pr+1] == 'e' ) {
                dest[ ++pw ] = buffer[ ++pr ];
                dest[ ++pw ] = buffer[ ++pr ];
            }
            continue;
        //or check if is cstring type formatting %s
        }else if(buffer[pr]=='%' && buffer[pr+1]=='s' ){
            dest[ pw ] = buffer[ pr ];
            dest[ ++pw ] = buffer[ ++pr ];
            continue;

        //if first byte start with ascii character
        }else if( buffer[pr] < 0x80 && buffer[pr] >= 0x20 ){
            //if it's strSel file and option - just copy single ascii char
            if(strSelect){
                if( buffer[pr] == '-' && buffer[pr+1] == '\n'){
                    dest[pw] = buffer[pr];
                    continue;
                }
            }
            //if second one is not ascii or it's white space or control code '$' or string - %s just make pair with space
            if( buffer[pr+1] > 0x80 || buffer[pr+1] <0x20 || buffer[pr+1] == '$' || (buffer[pr+1]=='%' && buffer[pr+2]=='s')){
                zn1  = ((field[  buffer [ pr ] ][ 0x20 ] & 0xFF00) >> 8);
                zn2  = (field[  buffer [ pr ] ][ 0x20 ] & 0xFF);
                dest[pw]   = zn1;
                dest[++pw] = zn2;
                continue;
            //if both are ascii just encode to character pair
            } else {
                zn1  = ((field[  buffer[ pr ] ][  buffer[ pr+1 ] ] & 0xFF00) >> 8);
                zn2  = (field[  buffer [ pr ] ][  buffer[ pr+1 ] ]& 0xFF);
                dest[pw]   = zn1;
                dest[++pw] = zn2;
                pr++;
                continue;
            }

        }
        //if not recognized just copy
        dest[ pw ] = buffer[ pr ];

    }
    fwrite(buffer, sizeof(char), 16, pFw);
    fwrite(dest + 16 , sizeof(char), pw-16, pFw);

    fclose(pFw);
    free(buffer);
    return 0;
}
