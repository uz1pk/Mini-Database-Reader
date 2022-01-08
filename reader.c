/*
Name: Usman Zaheer
Class: CIS 2520 (Dr. Stefan Kremer)
This program should sufficently be able to read from and print all classes within the given cmd line arguments.
*/

#include "a6.h"

int main(int argc, char *argv[]) {
    if(argc == 3) {
      int buildingIndex = getIdx("building", argv[1]);
      int roomIndex = getIdx("room", argv[2]);
      int *subCodes = malloc(BUFFER);
      int *courCodes = malloc(BUFFER);
      char subject[BUFFER];
      char courseno[BUFFER];
      char days[BUFFER];
      char from[BUFFER];
      char to[BUFFER];

      createSet("code", -1, "building", buildingIndex, "building_indices.set");
      createSet("code", -1, "room", roomIndex, "room_indices.set");
      setIntersection("building_indices.set", "room_indices.set", "building_room_indices.set");
      int totalLength = setIndex("building_room_indices.set", subCodes);

      for(int i = 0; i < totalLength; i++) {
        createSet("code", subCodes[i], "subject", -1, "subject.set");
        setIndex("subject.set", courCodes);
        getString("subject", courCodes[0], subject);

        createSet("code", subCodes[i], "courseno", -1, "courseno.set");
        setIndex("courseno.set", courCodes);
        getString("courseno", courCodes[0], courseno);

        createSet("code", subCodes[i], "days", -1, "days.set");
        setIndex("days.set", courCodes);
        getString("days", courCodes[0], days);

        createSet("code", subCodes[i], "from", -1, "from.set");
        setIndex("from.set", courCodes);
        getString("from", courCodes[0], from);

        createSet("code", subCodes[i], "to", -1, "to.set");
        setIndex("to.set", courCodes);
        getString("to", courCodes[0], to);

        printf("%s*%s %s %s - %s\n", subject, courseno, days, from, to);
      }
      free(courCodes);
      free(subCodes);
    }

    else printf("Invalid number of inputs\n");

    return 0;
}

int entries( char *filebase )
{
  char filename[BUFFER];
  int ent;

  strcpy( filename, filebase );
  strcat( filename, ".idx" );
  FILE *fp = fopen( filename, "rb" );
  fseek( fp, 0, SEEK_END );
  ent = ftell( fp ) / sizeof(long);
  fclose(fp);

  return ent;
}

void createSet(char *code, int until, char *setData, int setDataIndex, char *setDataFile)
{
  char *v1 = code;
  int i1 = until;
  char *v2 = setData;
  int i2 = setDataIndex;

  int n1 = entries( v1 );
  int n2 = entries( v2 );


  char filename[BUFFER];

  strcpy( filename, v1 );
  strcat( filename, "_" );
  strcat( filename, v2 );
  strcat( filename, ".rel" );

  FILE *fp = fopen( filename, "rb" );
  char *array = malloc( n1*n2 );
  fread( array, 1, n1*n2, fp );
  fclose( fp );

  fp = fopen(setDataFile, "wb" );
  if ( (i1==-1) && (i2>=0) )
  {
    for (int i=0;i<n1;i++)
    {
      int index = i*n2 +i2;
      fwrite( array+index, 1, 1, fp );
    }
  }

  if ( (i1>0) && (i2==-1) )
  {
    for (int j=0;j<n2;j++)
    {
      int index = i1*n2 + j;
      fwrite( array+index, 1, 1, fp );
    }
  }

  free(array);
  fclose( fp );
}

long getIdx(char *fileName, char *hashKey)
{
  char idxname[BUFFER];
  char txtname[BUFFER];

  char *basename;
  char *value;
  long hash_table[ HASHSIZE ];

  //identify text file name
  strcpy( idxname, fileName );
  strcat( idxname, ".idx" );
  strcpy( txtname, fileName );
  strcat( txtname, ".txt" );

  //basefile for hashing
  basename = fileName;

  //target value
  value = hashKey;

  // load hashtable from file into memory
  get_hashtable( basename, hash_table );

  // open text file
  FILE *idxfile = fopen( idxname, "r" );
  FILE *txtfile = fopen( txtname, "r" );

  // print result of hash_lookup
  long hashIndex = hash_lookup( value, hash_table, idxfile, txtfile);
  fclose( idxfile );
  fclose( txtfile );

  return hashIndex;
}

void setIntersection(char *setOne, char *setTwo, char *setCreation)
{
  FILE *fp1 = fopen( setOne, "rb" );
  FILE *fp2 = fopen( setTwo, "rb" );
  FILE *fp3 = fopen( setCreation, "wb" );
  char b1, b2, b3;

  while ( fread( &b1, 1, 1, fp1 )==1 && fread( &b2, 1, 1, fp2 ) )
  {
    b3 = b1&&b2;
    fwrite( &b3, 1, 1, fp3 );
  }

  fclose( fp1 );
  fclose( fp2 );
  fclose( fp3 );
}

int setIndex(char *fileName, int *indices)
{
  char boolean;
  int counter = 0;

  FILE *fp = fopen(fileName, "rb" );

  for (int i=0; fread(&boolean,1,1,fp)==1; i++)
  {
    if (boolean) {
        indices[counter] = i;
        counter++;
    }
  }
  return counter;
}

void getString(char *dataType, int index, char *dataSpecific)
{
  char *basename;
  int idx, idx2;

  basename = dataType;
  idx = index;

  char txtfile[BUFFER];
  char idxfile[BUFFER];
  char buffer[BUFFER];

  FILE *fptxt, *fpidx;

  strcpy( txtfile, basename );
  strcat( txtfile, ".txt" );

  strcpy( idxfile, basename );
  strcat( idxfile, ".idx" );

  fptxt = fopen( txtfile, "r" );
  fpidx = fopen( idxfile, "r" );

  fseek( fpidx, sizeof(long)*idx, SEEK_SET );
  
  if ( fread( &idx2, sizeof(long), 1, fpidx ) != 1 )
  {
    fprintf( stderr, "Error: invalid index\n" );
    exit(-1);
  }

  fseek( fptxt, idx2, SEEK_SET );
  fgets( buffer, BUFFER, fptxt );
  strcpy(dataSpecific, buffer);

  fclose( fptxt );
} 
