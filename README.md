# soal-shift-sisop-modul-4-E07-2021

* Nizar Mayraldo	      05111940000004
* Rihan farih bunyamin	05111940000165 
* Fayha Syifa Qalbi   	05111940000185

## no1
### soal:

Di suatu jurusan, terdapat admin lab baru yang super duper gabut, ia bernama Sin. Sin baru menjadi admin di lab tersebut selama 1 bulan. Selama sebulan tersebut ia bertemu orang-orang hebat di lab tersebut, salah satunya yaitu Sei. Sei dan Sin akhirnya berteman baik. Karena belakangan ini sedang ramai tentang kasus keamanan data, mereka berniat membuat filesystem dengan metode encode yang mutakhir

a) Jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.

b) Jika sebuah direktori di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.

c) Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.

d) Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : /home/[USER]/Downloads/[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori]

e) Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya.(rekursif)

### penjelasan

### kendala


## no2
### 2a
**Soal** : Jika sebuah direktori dibuat dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai kasus nomor 1 dengan algoritma tambahan ROT13 (Atbash + ROT13).

### 2b
**Soal** : Jika sebuah direktori di-rename dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai dengan kasus nomor 1 dengan algoritma tambahan Vigenere Cipher dengan key “SISOP” (Case-sensitive, Atbash + Vigenere).

### 2c
**Soal** : Apabila direktori yang terencode di-rename (Dihilangkan “RX_” nya), maka folder menjadi tidak terencode dan isi direktori tersebut akan terdecode berdasar nama aslinya.

### 2d
**Soal** : Setiap pembuatan direktori terencode (mkdir atau rename) akan tercatat ke sebuah log file beserta methodnya (apakah itu mkdir atau rename).

### 2e
**Soal** : Pada metode enkripsi ini, file-file pada direktori asli akan menjadi terpecah menjadi file-file kecil sebesar 1024 bytes, sementara jika diakses melalui filesystem rancangan Sin dan Sei akan menjadi normal.


### kendala yang dialami

## no3

## no4
**soal** : Log system yang akan terbentuk bernama “SinSeiFS.log” pada direktori home pengguna (/home/[user]/SinSeiFS.log). Log system ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.
Karena Sin dan Sei suka kerapian maka log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.

berikut adalah format INFO dan WARNING yang dimana Membuat format info dan warning fungsi `makeLog`
```
...
}else if(strcmp(sys_call,"RMDIR")==0 || strcmp(sys_call,"UNLINK")==0){
        fprintf(LOGFILE4, "WARNING::%d%02d%02d-%02d:%02d:%02d:%s::/%s::/%s\n",timeinfo->tm_mday, 
        timeinfo->tm_mon+1, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, sys_call, data.path1, data.path2);
    }else{
        fprintf(LOGFILE4, "INFO::%d%02d%02d-%02d:%02d:%02d:%s::/%s::%s\n",timeinfo->tm_mday, 
        timeinfo->tm_mon+1, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, sys_call, data.path1, data.path2);
    }
...
```

fungsi makeLog yang dibuat bersamaan dengan log nomor 1. sehingga percabangannya lebih banyak dan lebih spesifik. format sudah sesuai dengan permintaan soal dimana timestamp harus mengoutputkan 2 digit pada bagian tanggal dan waktunya. kemudian diikuti jenis sys_call dan detail perubahannya.
sedangkan untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.WARNING dioutputkan saat system_call adalah RMDIR dan UNLINK 
```
...
else if(strcmp(sys_call,"RMDIR")==0 || strcmp(sys_call,"UNLINK")==0){
        fprintf(LOGFILE4, "WARNING::%d%02d%02d-%02d:%02d:%02d:%s::/%s::/%s\n",timeinfo->tm_mday, 
        timeinfo->tm_mon+1, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, sys_call, data.path1, data.path2);
    }
...
```

INFO dioutputkan saat system_call selain RMDIR dan UNLINK dimana WARNING hanya dioutputkan selain ketika UNLINK atau RMDIR pada source code dibawah diambil sebagai contoh MKDIR dan RENAME.
```
...
  if(strcmp(sys_call,"RENAME")==0){
        fprintf(LOGFILE4, "INFO::%d%02d%02d-%02d:%02d:%02d:%s::/%s::/%s\n",timeinfo->tm_mday, 
        timeinfo->tm_mon+1, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, sys_call, data.path1, data.path2);
        fprintf(LOGFILE1, "%s : %s -> %s\n", sys_call, data.path1, data.path2);	
    }else if(strcmp(sys_call,"MKDIR")==0 ){
    	fprintf(LOGFILE1, "%s : %s\n", sys_call, data.path1);
        fprintf(LOGFILE4, "INFO::%d%02d%02d-%02d:%02d:%02d:%s::/%s::/%s\n",timeinfo->tm_mday, 
        timeinfo->tm_mon+1, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, sys_call, data.path1, data.path2);
    }
...
```

Adapun berikut adalah format baris pada log.

CMD : System Call yang terpanggil, DESC : informasi dan parameter tambahan, Level : Level logging, dd : tanggal, mm : bulan, yyyy : tahun, HH : jam (format 24 Jam), MM : menit,  dan SS : detik.

## output
