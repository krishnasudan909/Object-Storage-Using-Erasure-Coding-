#include "helper/helper.h"
#include "erasure_code.h"
#define mapSize 5
struct FileData
{
    char* file_name;
    const char* file_type;
    char* file_path;
    off_t  file_size;
    size_t UID;

    struct FileData* next;

};
///////////
int d = 4;
int p = 3;
int m = 7;

unsigned char gen[7*4];
char g_tbls[3 * 4 * 32];
//////////
struct FileData *hashTable[mapSize];
int totId = 1;

void put(struct FileData** head_ref) {
  struct FileData* new_FileData = (struct FileData*)malloc(sizeof(struct FileData));
  struct FileData* last = *head_ref; /* used in step 5*/

  char path[100];
//   printf("Enter file path: \n");
  scanf("%s", &path);
  if (access(path, F_OK) == 0){
    new_FileData->file_name = fullName(path);
    new_FileData->file_path = absolutePath(path);
    new_FileData->file_size = fullSize(path);
    new_FileData->file_type = get_filename_ext(fullName(path));
    new_FileData->UID = totId;
    new_FileData->next = NULL;


    size_t maxSize = ceil((double)fullSize(path)/4);
    char databuffs[4][maxSize+1];
    char paritybuffs[3][maxSize+1];
    printf("The file has been split to %d parts with maximum size as %zu\n", splitFile(path, maxSize, databuffs), maxSize);
    printf("%s\n",databuffs[0]);
    printf("%s\n",databuffs[1]);
    printf("%s\n",databuffs[2]);
    printf("%s\n",databuffs[3]);
    // ec_encode_data_base(maxSize, 4, 3, g_tbls, databuffs, paritybuffs);
    printf("The file has been added with ID %jd\n", new_FileData->UID);

    if (*head_ref == NULL) {
    *head_ref = new_FileData;
    return;
    }

    while (last->next != NULL) last = last->next;

    last->next = new_FileData;

    
  }
  else
  {
    printf("The file is not accesible\n");
  }
  
  return;
}


void get() {
  size_t key;
  scanf("%zd", &key);
  int hashIndex = key%5;
  struct FileData* current = hashTable[hashIndex];
  while (current != NULL) {
  if (current->UID == key){
    printf("The file is present at %d index of the hash table\n",hashIndex);
    printf("The name of the file is: %s\n", current->file_name);
    printf("The path of the file is: %s\n", current->file_path);
    printf("The size of the file is: %jd Bytes\n", current->file_size);
    printf("The type of the file is: %s\n", current->file_type);
    
    mergeFile(current->file_name, ceil((double)current->file_size/4));
    
    return;
  }
  current = current->next;
  }
  printf("The ID specified was not found\n");
  return ;
}

void list() {
  int hashIndex = 0;
  while (hashIndex<5) {
    if(hashTable[hashIndex]==NULL) printf("No files at hashIndex %d\n", hashIndex);
    else{
    printf("Files at HashIndex %d are --> \n", hashIndex);
    struct FileData* head = hashTable[hashIndex];
    while(head!=NULL){
    printf("The name of the file is: %s\n", head->file_name);
    printf("The path of the file is: %s\n", head->file_path);
    printf("The size of the file is: %jd Bytes\n", head->file_size);
    printf("The type of the file is: %s\n", head->file_type);
    printf("---------------------------\n");
    head = head->next;
    }
    }
    hashIndex++;
  }
}


int main(int argc, char const *argv[])
{

  gf_gen_rs_matrix(gen, 7, 4);
  ec_init_tables(4, 3, &gen[4*4], g_tbls);
    //INITIALIZING MAP ARRAY TO NULL
    for(int i=0; i<mapSize; i++) hashTable[i]=NULL;
    int input;
    int flag = 1;
    char command[4];
    printf("\t\t Veritas object storage Major Project\n");
    while (flag)
    {
        printf("\n--------------------------\n");
        // printf("Enter 1 for put \nEnter 2 for get \nEnter 3 for list \nEnter 0 for exit\n" );
        scanf("%s", &command);
        if (strcmp(command, "put") == 0)
        {
            int key = (totId % mapSize);
            printf("File added in the %d index of the hash table\n",key);
            put(&hashTable[key]);
            totId++;
        }
        else if (strcmp(command, "get") == 0)
        {
            get();
        }
        else if (strcmp(command, "list") == 0)
        {
            list();
        }
        else
        {
            printf("Invalid command\n");
            // getchar();
            // system("clear");
        }

    }
    return 0;
}


