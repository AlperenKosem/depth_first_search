#include <stdio.h>
#include <stdlib.h>
// #define maze_rows 11
// #define maze_cols 16 

#define maze_rows 31
#define maze_cols 44 

// char** maze; //maze'i sakladigim degisken //2D Array olacak
char** visited;

int rows = maze_rows;
int cols = maze_cols ;
int* apples;
int maze[maze_rows][maze_cols];

int baslangic_row;
int baslangic_col;

int bitis_row;
int bitis_col;

struct position{
    int row, col;
};

struct node
{
    struct node* next;
    struct position pos;

};

struct position pop(struct node*);
struct node* push(struct node*, struct position);

struct position pop(struct node* root){
    if( root == NULL ){
        struct position tmp_pos;
        tmp_pos.row = NULL;
        tmp_pos.col = NULL;

        printf("stack bos");
        return tmp_pos;
    }

    if(root->next == NULL) // son bir node kaldisysa iter->next->nexti olmuyor ve hataveriyor bundan kacinmak icin stack'imde son bir node kaldiysa
    {
        struct position tmp_pos = root->pos;

        free(root);
        return tmp_pos;
    }

    struct node* iter = root;
    while (iter->next->next != NULL) // son elemana gelirsem cok gec bu yüzden bir oncekinde durmaliyim ki son elemanın pointeri elimde olsun ki sileyim.
    {
        iter = iter->next;
    }

    struct node* temp = iter->next;
    struct position rvalue = temp->pos; // free yapmadan once degerini gecici degiskende sakladim
    iter->next = NULL;
    free(temp);
    
    return rvalue;

}

struct node* push(struct node* root, struct position data){ // void de yazilabilir ama bos olmasi durumunda bir root döndürmeli bos olmaması durumunda root'a dokunmuyoruz zaten
    if( root == NULL){ //eger stack bos ise
         root = (struct node*) malloc(sizeof(struct node));
         root->pos.row = data.row;
         root->pos.col = data.col;
         root->next = NULL;
         return root;
    }

    struct node* iter = root;
    while(iter->next!= NULL){
        iter = iter->next;
    }
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->pos.row = data.row;
    temp->pos.col = data.col;

    temp->next = NULL;
    iter->next = temp;
    return root;

}




void printMaze(){
     for (int i = 0; i < rows; ++i) {
         
            for (int j = 0; j < cols; ++j) {
                printf("%d ",maze[i][j]);
        }
        printf("\n");
     }

}


void generateMaze(char* file_name)
{
	char c;

	FILE* maze_file = fopen(file_name, "r");
        // int cols_counter = 0;
        // int rows_counter = 1;
        // int toplam_char = 0;
        // while ((c = getc(maze_file) != EOF)) /*  Bu bloktaki gibi degisken satır ve sütunda da program çalışsın istedim. Ancak her mazede farklı donus veriyordu. Ozetlemek gerekirse newline gelene kadar olan karakteri sayiyordu
                                                    //bu kadar sütunum var diyodu kaç kez newline geldiyse de satir sayisini çıkarıyordu ancak mazelerde farklı sonuc veriyordu. Ben de burayı hardcoded yaptım.
        // {
        //     toplam_char += 1;
        //     c = getc(maze_file);
        //     if ( c == '\n')
        //     {
        //         rows_counter += 1;      
        //         rows = rows_counter;  
                

        //     }
        //     else
        //     {
        //         cols_counter += 1;
        //     }
            
        //     // printf("%c\n",c);  
        // }

        // cols = toplam_char / rows;
        // printf("Sütun sayisi: %d\n",cols);
        // printf("Satır sayisi: %d\n",rows);
        
        
        
    int i,j;

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            
            c = getc(maze_file);

            if (c =='+' || c =='-' || c =='|' || c == '\n')
            {
                maze[i][j] = 0;
            }
            else
            {
                maze[i][j] = 1;
            }
            
            if (c =='b') {
                baslangic_row = i;
                baslangic_col = j;
            }

            if (c =='c') {
                bitis_row = i;
                bitis_col = j;
            }
        }
    }

    fclose(maze_file);
    
}



int main()
{

    generateMaze("maze.txt");
    printMaze();
    printf("baslangic row : %d\t",baslangic_row);
    printf("baslangic col : %d\n",baslangic_col);

    printf("bitis row : %d\t",bitis_row);
    printf("bitis col : %d\n",bitis_col);

    return 0;
}