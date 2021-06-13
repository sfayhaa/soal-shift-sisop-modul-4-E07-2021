#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
 
static  const  char *dirpath = "/home/tsania/Downloads";
static const char *lognomer4 = "/home/tsania/SinSeiFS.log";
static const char *lognomer1 = "/home/tsania/Documents/sisopshift4/log1.log";
 
struct data{
    char path1[100];
    char path2[100];
};

struct rx {
    char DIR[SIZE];    
    int CHIPER;    
};

struct rx rx_directory[ARR_SIZE];
int rx_last_idx = 0;


char* atbash(char message[]) {
    char msg[1024] ;
    strcpy(msg, message) ;
    int x=strlen(msg);
    int i ;
    for(i = 0 ; i < x ; i++) {
        if (msg[i] >= 'A' && msg[i] <= 'Z') 
            msg[i] = 'Z' - (msg[i] - 'A') ;
        else if (msg[i] >= 'a' && msg[i] <= 'z') 
            msg[i] = 'z' - (msg[i] - 'a') ;
 
    }
    char* cipher = msg ;
    return cipher ;
}
 
// Bikin log
void makeLog(char *sys_call, struct data data){
    FILE * LOGFILE1 = fopen(lognomer1, "a");
    FILE * LOGFILE4 = fopen(lognomer4,"a");
	time_t now;
	time ( &now );
	struct tm * timeinfo = localtime (&now);
	
    if(strcmp(sys_call,"RENAME")==0){
        fprintf(LOGFILE4, "INFO::%d%02d%02d-%02d:%02d:%02d:%s::/%s::/%s\n",timeinfo->tm_mday, timeinfo->tm_mon+1, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, sys_call, data.path1, data.path2);
        fprintf(LOGFILE1, "%s : %s -> %s\n", sys_call, data.path1, data.path2);	
    }else if(strcmp(sys_call,"MKDIR")==0 ){
    	fprintf(LOGFILE1, "%s : %s\n", sys_call, data.path1);
        fprintf(LOGFILE4, "INFO::%d%02d%02d-%02d:%02d:%02d:%s::/%s::/%s\n",timeinfo->tm_mday, timeinfo->tm_mon+1, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, sys_call, data.path1, data.path2);
    }else if(strcmp(sys_call,"RMDIR")==0 || strcmp(sys_call,"UNLINK")==0){
        fprintf(LOGFILE4, "WARNING::%d%02d%02d-%02d:%02d:%02d:%s::/%s::/%s\n",timeinfo->tm_mday, timeinfo->tm_mon+1, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, sys_call, data.path1, data.path2);
    }else{
        fprintf(LOGFILE4, "INFO::%d%02d%02d-%02d:%02d:%02d:%s::/%s::%s\n",timeinfo->tm_mday, timeinfo->tm_mon+1, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, sys_call, data.path1, data.path2);
    }
    fclose(LOGFILE1);
    fclose(LOGFILE4);
    return;
}
 
// Untuk (Path Fuse Relatif) --> (Path Asli (/home/erzajanitra/Downloads))
// /home/erzajanitra/modul4/cobaa (Path Fuse Absolut) --> / (Path Fuse Relatif)
char* prosesPath(char* path) {
    char fpath[1024] ;
    bzero(fpath, 1024) ;
    int x, cekk = 0 ;
 
    // Cek Apakah ada /AtoZ_ atau nggak...
    char *atoz;
    if (strcmp(path, "/") != 0) {
        atoz= strstr(path, "/AtoZ_");
        if (atoz) {
            // Jika ada, set cekk = 1 
            cekk = 1 ;
            cekaz = 1 ;
            // Geser pointer kekanan agar / nya ga kena
            atoz++ ;
        }
    }
     if(strcmp(path, "/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else if (cekk) {
        char realPath[1024] ;
        bzero(realPath, 1024) ;
        strncpy(realPath, path, strlen(path) - strlen(atoz)) ;
 
        // Jaga - Jaga alamat asli keubah
        char t[1024] ;
        strcpy(t, atoz) ;
 
        char* encName;
        char* cut = t;
 
        // Ini juga sama
        char temp[1024] ;
 
        // Untuk nandain loop keberapa
       x = 0 ;
        while ((encName = strtok_r(cut, "/", &cut))) {
            bzero(temp, 1024) ;
            if(x == 0) {
                strcpy(temp, encName) ;
                // Sambungkan realpath
                strcat(realPath, temp) ;
                x = 1 ;
                continue ;
            }
 
            // Cek tipe data
            char isFullPath[1024] ;
            bzero(isFullPath, 1024) ;
            strcpy(isFullPath, realPath) ;
            strcat(isFullPath, "/") ; 
            strcat(realPath, "/") ;
            strcat(isFullPath, encName) ;
 
            if (strlen(isFullPath) == strlen(path)) {
                char pathFolder[1024] ;
                sprintf(pathFolder, "%s%s%s", dirpath, realPath, encName) ;
 
                DIR *dp = opendir(pathFolder);
                if (!dp) {
                    char *dot = strchr(encName, '.') ;
                    char fileName[1024] ;
                    bzero(fileName, 1024) ;
                    // Kalau ada extension
                    if (dot) {
                        // Untuk dapetin nama file nya doang
                        strncpy(fileName, encName, strlen(encName) - strlen(dot)) ;
                        strcpy(fileName, atbash(fileName)) ;
                        strcat(fileName, dot) ;
                    }
                    // Kalau ga ada extension
                    else {
                        strcpy(fileName, encName) ;
                        strcpy(fileName, atbash(fileName)) ;
                    }
                    strcat(realPath, fileName) ;
                    printf("%s\n", encName) ;
                }
                // sukses dibuka = benar ini folder
                else {
                    closedir(dp) ;
                    char folderName[1024] ;
                    bzero(folderName, 1024) ;
                    strcpy(folderName, encName) ;
                    strcpy(folderName, atbash(folderName)) ;
                    strcat(realPath, folderName) ;
                }
            }
            else {
                // Menyambungkan realpath dengan nama folder
                char folderName[1024] ;
                bzero(folderName, 1024) ;
                strcpy(folderName, encName) ;
                strcpy(folderName, atbash(folderName)) ;
                strcat(realPath, folderName) ;
            }
 
        }
        sprintf(fpath, "%s%s", dirpath, realPath) ; 
    }
    else sprintf(fpath, "%s%s",dirpath,path) ;
 
    char* return_fpath = fpath ;
    return return_fpath ;
}

// fungsi encode dengan viginere cipher
char *vignereEncrypt(char *str, char *key) {    
    char *str_copy = malloc((strlen(str)+1) * sizeof(char));
    sprintf(str_copy, "%s", str);
    char temp[SIZE]; sprintf(temp, "%s", str);

    int i = 0, curKey = 0;
    for(i = 0; i < strlen(str_copy); i++) {
        if(str_copy[i] >= 'a' && str_copy[i] <= 'z') {
            str_copy[i] = str_copy[i] - 'a' + 'A';
        }
    }

    for(int i = 0; i < strlen(str_copy); i++) {
        if(curKey == strlen(key)) curKey = 0;

        if(str_copy[i] >= 'A' && str_copy[i] <= 'Z')
            str_copy[i] = ((str_copy[i] + key[curKey]) % 26);
            
        if(temp[i] >= 'a' && temp[i] <= 'z')
            str_copy[i] += 'a';
        else if(temp[i] >= 'A' && temp[i] <= 'Z')
            str_copy[i] += 'A';
        else
            curKey--;
        
        curKey++;
    }

    str_copy[strlen(str)] = 0;
    return str_copy;
}
char* vignereDecrypt(char *str, char *key) {    
    char *str_copy = malloc((strlen(str)+1) * sizeof(char));
    sprintf(str_copy, "%s", str);
    char temp[SIZE]; sprintf(temp, "%s", str);

    int i = 0, curKey = 0;
    for(i = 0; i < strlen(str_copy); i++) {
        if(str_copy[i] >= 'a' && str_copy[i] <= 'z') {
            str_copy[i] = str_copy[i] - 'a' + 'A';
        }
    }

    for(int i = 0; i < strlen(str_copy); i++) {
        if(curKey == strlen(key)) curKey = 0;

        if(str_copy[i] >= 'A' && str_copy[i] <= 'Z') {
            str_copy[i] = str_copy[i] - key[curKey];

            if(str_copy[i] < 0)
                str_copy[i] += 26;
        }

        if(temp[i] >= 'a' && temp[i] <= 'z')
            str_copy[i] += 'a';
        else if(temp[i] >= 'A' && temp[i] <= 'Z')
            str_copy[i] += 'A';
        else
            curKey--;
        
        curKey++;
    }

    str_copy[strlen(str)] = 0;
    return str_copy;
}

// fungsi encode biner nomor 3
// ubah jadi lowercase insensitive
void ambilBiner(char *fname, char *bin, char *lowercase){
	int idAkhir = extensionId(fname);
	int idAwal = pemisahId(fname, 0);
	int i;
	
	for(i=idAwal; i<idAkhir; i++){
		if(isupper(fname[i])){
			bin[i] = '1';lowercase[i] = fname[i] + 32;
		}
		else{
			bin[i] = '0';lowercase[i] = fname[i];
		}
	}
	bin[idAkhir] = '\0';
	
	for(; i<strlen(fname); i++){
		lowercase[i] = fname[i];
	}
	lowercase[i] = '\0';
}

//encrypyt binary no 3
void encryptBinary(char *filepath){
	chdir(filepath);
	DIR *dp;
	struct dirent *dir;
	struct stat lol;
	dp = opendir(".");
	if(dp == NULL) return;
	
	char dirPath[1000];
	char filePath[1000];
	char filePathBinary[1000];
	
    while ((dir = readdir(dp)) != NULL){
		if (stat(dir->d_name, &lol) < 0);
		else if (S_ISDIR(lol.st_mode)){
			if (strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0) continue;sprintf(dirPath,"%s/%s",filepath, dir->d_name);encryptBinary(dirPath);
		}else{
			sprintf(filePath,"%s/%s",filepath, dir->d_name);
			char bin[1000], lowercase[1000]; ambilBiner(dir->d_name, bin, lowercase);
			int dec = bin2dec(bin);
			sprintf(filePathBinary,"%s/%s.%d",filepath,lowercase,dec); rename(filePath, filePathBinary);
		}
	}
    closedir(dp);
}
// decrypt binary no 3
void decryptBinary(char *filepath){
	chdir(filepath);
	DIR *dp;
	struct dirent *dir;
	struct stat lol;
	dp = opendir(".");
	if(dp == NULL) return;
	
	char dirPath[1000];
	char filePath[1000];
	char filePathDecimal[1000];
	
    while ((dir = readdir(dp)) != NULL){
		if (stat(dir->d_name, &lol) < 0);
		else if (S_ISDIR(lol.st_mode)){
			if (strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0) continue;sprintf(dirPath,"%s/%s",filepath, dir->d_name);decryptBinary(dirPath);
		}
		else{
			sprintf(filePath,"%s/%s",filepath, dir->d_name);
			char fname[1000], bin[1000], normalcase[1000], clearPath[1000];
			
			strcpy(fname, dir->d_name);
			char *ext = strrchr(fname, '.');
			int dec = convertDec(ext+1);
			for(int i=0; i<strlen(fname)-strlen(ext); i+
