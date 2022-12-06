typedef uint8_t BYTE;
int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    //if argc > 1
 if (argc < 1)
 {
    printf("Usage: ./recover IMAGE");
    return 1;
 }

    //if file doesn't open
 FILE *file = fopen(argv[1], "r");
 if (file == NULL)
 {
    printf("Could not open %s.\n", argv[1]);
    return 1;
 }

 BYTE *buffer = malloc(sizeof(uint8_t) * BLOCK_SIZE);

 if (buffer == NULL)
 {
    printf("Not enough memory to store the block.\n");
    return 1;
  }

 char *NAME_FILE = malloc(sizeof(char) * 4);

 if (NAME_FILE == NULL)
 {
    printf("Not enough memory to store %s.\n", NAME_FILE);
    return 2;
 }

 int v = 0;

 while (fread(buffer, sizeof(uint8_t), BLOCK_SIZE, file) == BLOCK_SIZE)
 {

       for (int i = 0; i < BLOCK_SIZE; i++)
       {
           if (buffer[i] == 0xff || buffer[i+1] == 0xd8 || buffer[i+2] == 0xff || (buffer[i+3] & 0xf0) == 0xe0)
           {
               v++;

               // The first jpeg ;; si le 1er jpeg
               if (v == 1)
               {
                   sprintf(NAME_FILE,"%03i.jpg",0);
                   FILE *img = fopen(NAME_FILE,"w");
                   if (img == NULL)
                   {
                       printf("Could not be created %s.\n", NAME_FILE);
                       fclose(file);
                       return 3;
                   }

                   fwrite(buffer, sizeof(uint8_t), 1, img);
               }
              // IF new jpg bucket found but not the first ;; si un nouveau jpeg mais ne pas le 1er:
               else
               {
                   sprintf(NAME_FILE,"%03i.jpg",v);

                   FILE *img = fopen(NAME_FILE,"w");
                   if (img == NULL)
                   {
                      printf("Could not be created %s.\n", NAME_FILE);
                      fclose(file);
                      return 3;
                   }

                  fwrite(buffer, sizeof(uint8_t), 1, img);
               }
           }

            else   //si pas de nouveau jpg dans le nouveau block: il continue à ecrire dans img de block précédent
            {
                  FILE *pk = img;
                  fwrite(buffer, sizeof(uint8_t), 1, pk);
            }

       }

 }


free(buffer);
free(NAME_FILE);
fclose(pk);
fclose(img);
return 0;

}
