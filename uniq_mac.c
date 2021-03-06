#include <stdio.h>
#include <fcntl.h> //for open
#include <unistd.h> //for close

char buf[1024], templine[512], currline[512], outputline[1024];

int strcmp(const char *p, const char *q) {
  while (*p && *p == *q)
    p++, q++;
  return (unsigned char)*p - (unsigned char)*q;
}

char to_lower(char chr){
  if ( chr >= 65 && chr <= 90 && chr != '\n')
    chr += 32;
  return chr;
}

void uniq(int fd, int count, int duplicate, int nodifcase){
  int readchr, i1, i2, i3, i4, i5 = -1;
  //i5 declare to -1 because the group array index should not increment when templine compares with empty currline(only once)
  int elemc1, elemc2, elemc3, elemc4, group[1024] = {[0 ... 1023] = 1};
  //group array is to store grouping elements, every line exist at least once, thus contains 1024's 1s.
  elemc1 = 0;
  elemc2 = 0;
  elemc3 = 0;

  while ((readchr = read(fd, buf, sizeof(buf))) > 0){
    for (i1 = 0; i1 < readchr; i1++){
      elemc3++;//track characters
      if (buf[i1] != '\n' && elemc3 != readchr){ //2nd condition to deal no newline character at the end of file
        if (nodifcase){
          templine[elemc1] = to_lower(buf[i1]);
        }
        else{
          templine[elemc1] = buf[i1];
        }
        elemc1++;//keep templine index starts from 0
      }
      else{ //init when reach newline

          templine[elemc1++] = buf[i1];//elemc1 increment for \n
          if (strcmp(templine, currline) != 0){ //if they r't equal
            elemc4 = 0;// specifically use for last line without \n character, to count length & use for later printing
            for (i2 = 0; i2 < elemc1; i2++){
              currline[i2] = templine[i2];
              elemc2++;//count uniq line total characters when uniq line is found
              elemc4++;
            }
            i5++;//count groupings
            i4 = elemc3-elemc1;//buffer "pointer"
            for (i3 = (elemc2-elemc1); i3 < elemc2; i3++){
              outputline[i3] = buf[i4];
              i4++;
            }
          }//default uniq
          else{
            group[i5]++;//increment when two strings are the same
          }
        elemc1 = 0; //the element counter here is to make sure every new reading stream into templine starts from [0]
      }//most outer else
    }//most outer for loop
}//1st while loop
int i6 = i5;//another break condition specifically for last line without \n
i5++;//for the loop range

int line = 0, i, j = 0, newlinec2 = 0, newlinec1 = 0;
  if (count){
    for (i = 0; i < i5; i++){
      printf("%d%c",group[i],' ' );
      for(;;line++){
        printf("%c", outputline[line]);
        if (outputline[line] == '\n'){
          line++;
          break;
        }//if
        if (i == i6 && elemc4 == 0){

          break;
        }
        else if (i == i6){
          elemc4--;
        }
      }//2nd for
    }//1st for
  }//if
  else if (duplicate){
    for (i = 0; i < i5; i++){
      newlinec1++;
      if (group[i] > 1){
        for (;;j++){
          if (outputline[j] == '\n'){
            newlinec2++;
          }
          if (newlinec2 == newlinec1-1 && outputline[j] != '\n'){
            printf("%c", outputline[j]);
          }
          else if (newlinec2 == newlinec1){
            printf("\n");
            j++;
            break;
          }
        }//inner for
      }//if
    }//outer for
  }//else if
  else{//default uniq printing
    for(i = 0; i < elemc2 ; i++){
      printf("%c", outputline[i]);
    }//for
  }//else
}//void


int main(int argc, char *argv[]){
  int fd, count, duplicate, nodifcase;
  char c;
  count = 0;
  duplicate = 0;
  nodifcase = 0;

  if (argc <= 1) {//e.g. cat README.md | uniq
    uniq(0, 0, 0, 0); //uniq is the only one argumt
            //the 0 is standard input for file descriptor, the std-I connect to std-O
            //std-O in this case is the result from cat README.md
    return 0;
  }else{
    while((++argv)[0] && argv[0][0] == '-'){//probably should use getopt() instead of this
    while((c = *++argv[0])){
      switch(c){
        case 'c':
        count = 1;
        break;

        case 'd':
        duplicate = 1;
        break;

        case 'i':
        nodifcase = 1;
        break;

        default:
         printf("%s%c%s\n","Option ", c, " is unknown." );
         printf("%s\n", "Usage: uniq <-c -d -i>");
        break;
      }//switch
    }//inner while
  }//outer while
  if (duplicate == 1 && count == 1){
    printf("invalid usage.\n");
    printf("usage: uniq [-c | -d] [-i]\n");
    return 0;
  }
  if ((fd = open(argv[0], O_RDONLY)) < 0){ // O_RDONLY = 0
      //open() establishes a connection between file descriptor and the file
      //int open(const char *path, int oflag, ... );
      //the path pointer points to a pathname naming the file
      printf("uniq: cannot open %s\n", argv[0]);
    }
    uniq(fd, count, duplicate, nodifcase);
    close(fd);
  }//else
  return 0;
}//main
