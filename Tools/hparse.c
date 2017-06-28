#include <stdio.h>

char objfile[256];
char outfile[256];
FILE *out = NULL;

void handle(char* buf, int len) {
    int i,off;
    buf[len]=0;
    if(!memcmp(buf,"Note: including file:",21) && len>22) {
        for(off=22;buf[off] && buf[off]==' ';off++) ;
        for(i=off;buf[i];i++) if(buf[i]=='\\') buf[i]='/';
        for(i=0;i<strlen(objfile);i++) {
            if(objfile[i]==' ') fputc('\\',out);
            fputc(objfile[i],out);
        }
        fprintf(out,": ");
        for(i=off;i<strlen(buf);i++) {
            if(buf[i]==' ') fputc('\\',out);
            fputc(buf[i],out);
        }
        fprintf(out,"\n");
        //fprintf(out, "%s: %s", objfile, &(buf[22]));
    }
}

int append(char* from, int from_start, char *to, int to_start, int len) {
    int overflow = 0;
    if(to_start+len>255) len = 255-to_start, overflow = 1;
    memcpy(&(to[to_start]), &(from[from_start]), len);
    if(overflow) to[254] = '\n';
    to[to_start+len] = 0;
    return to_start+len;
}

int main(int argc, char *argv[]) {
    char buf[1024];
    char line[256];
    int lbuf, lline=0, sbuf, cline = 0;
    int i,len;
    if(argc<3) { fprintf(stderr, "usage: header-parse <object-filename> <output-filename>"); exit(0); }
    strcpy(objfile, argv[1]);
    strcpy(outfile, argv[2]);
    out = fopen(outfile, "w");
    if(!out) {
        fprintf(stderr, "hparse: failed to open %s\n", outfile);
        return -1;
    }
    for(i=0;objfile[i];i++) if(objfile[i]=='\\') objfile[i]='/';
    while(len=read(0, buf, 1024)) {
        if(len<0) {
            fprintf(stderr, "error when reading stdin.\n");
            fclose(out);
            return -1;
        }
        for(sbuf=0,i=0;i<len;i++) {
            if(buf[i]=='\n') {
                lline = append(buf,sbuf,line,lline,i-sbuf+1);
                handle(line,lline);
                lline = 0, sbuf = i+1;
            }
        }
        if(sbuf<len) lline = append(buf,sbuf,line,lline,len-sbuf);
    }
    if(lline>0) handle(line, lline);
    fclose(out);
    return 0;
}
