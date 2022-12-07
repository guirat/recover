#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    //if argc > 1
 if (argc < 2)
 {
    printf("Usage: ./recover Name.\n");
    return 1;
 }

    //if file doesn't open
 FILE *file = fopen(argv[1], "r");
 if (!file)
 {
    printf("Could not open %s.\n", argv[1]);
    return 1;
 }
//  BYTE *buffer = malloc(sizeof(uint8_t) * BLOCK_SIZE);
 BYTE buffer[BLOCK_SIZE];

//  if (buffer == NULL)
//  {
//     printf("Not enough memory to store the block.\n");
//     return 1;
//   }

 int v = 0;

//  BYTE *tmpbuf = malloc(sizeof(uint8_t));;
 FILE *tmpimg;
char NAME_FILE[8];
 int check = 0;

 while (fread(&buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
 {
       for (int i = 0; i < BLOCK_SIZE; i++)
       {
           if (buffer[i] == 0xff && buffer[i+1] == 0xd8 && buffer[i+2] == 0xff && (buffer[i+3] & 0xf0) == 0xe0)
           {
               if (v > 0)
               {
                  fclose(tmpimg);
               }

               // char *NAME_FILE = malloc(sizeof(char) * 4);
               // if (NAME_FILE == NULL)
               // {
               //    printf("Not enough memory to store %s.\n", NAME_FILE);
               //    return 2;
               // }

               sprintf(NAME_FILE,"%03i.jpg",v);

               FILE *img = fopen(NAME_FILE,"w");
               // free(NAME_FILE);
               if (img == NULL)
               {
                  printf("Could not be created %s.\n", NAME_FILE);
                  fclose(file);
                  return 5;
               }

               tmpimg = img;
               // tmpbuf = &buffer[i];
               fwrite(&buffer[i], sizeof(uint8_t), 1, tmpimg);

               v++;

               check = 1;
           }

           else  if (check == 1) // no new jpeg and continue write on the current jpeg.
           {
                  // tmpbuf = &buffer[i];
                  fwrite(&buffer[i], sizeof(uint8_t), 1, tmpimg);
            }

       }

 }

fclose(file);
// free(buffer);
fclose(tmpimg);
return 0;

}
