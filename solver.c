#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #define maze_rows 11
// #define maze_cols 16 

#define maze_rows 31
#define maze_cols 44 

char** maze_str; //maze'i sakladigim degisken 

int rows = maze_rows;
int cols = maze_cols ;
int maze[maze_rows][maze_cols];

int toplam_puan;

int cikmaz_sokak; // cikmaz sokaga girdiginde 1 olur


struct position baslangic_position;
struct position bitis_position;


////////////stack declaration and functions
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
void printAllStruct(struct node*);


void delay(int number_of_seconds) // ref: https://www.geeksforgeeks.org/time-delay-c/
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
  
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}


void printAllStruct(struct node* root){
    if (root == NULL)
    {
        root = (struct node*) malloc(sizeof(struct node));
        // root->data = a;
        root->next = NULL;
    }
    
    while ( root != NULL)
    {
        printf(" row %d\t  column: %d\n",root->pos.row,root->pos.col);
        root = root->next;
    }
    
}

struct position pop(struct node* root){
    // printf("POP\n");
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
         root = (struct node*)malloc(sizeof(struct node));
         root->pos.row = data.row;
         root->pos.col = data.col;
         root->next = NULL;
        //  printf("ROOT BOS\n");
         return root;
    }

    struct node* iter = root;
    while(iter->next!= NULL){
        iter = iter->next;
        // printf("ROOT DOLU\n");
    }
   

    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->pos.row = data.row;
    temp->pos.col = data.col;

    temp->next = NULL;
    iter->next = temp;
    return root;

}

/////////////////////////


void allocateMaze()
{
    maze_str = malloc(rows * sizeof(char*));

    for( int i = 0 ; i < rows ; i++ )
    {
        maze_str[i] = malloc(cols * sizeof(char*));
    }
    
}

void printMazeStr(){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%c",maze_str[i][j]);
        }
    }
    printf("Toplam Puan : %d\n",toplam_puan);
    delay(100);
}

void printMazeBinary(){
     for (int i = 0; i < rows; ++i) {
         
            for (int j = 0; j < cols; ++j) {
                printf("%d",maze[i][j]);
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
        // while ((c = getc(maze_file) != EOF))   Bu bloktaki gibi degisken satır ve sütunda da program çalışsın istedim. Ancak her mazede farklı donus veriyordu. Ozetlemek gerekirse newline gelene kadar olan karakteri sayiyordu
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
            maze_str[i][j] = c; // charlardan maze'imi olusturuyorum
            if (c =='+' || c =='-' || c =='|' || c == '\n')
            {
                maze[i][j] = 0;
            }
            else if (c == 'o')
            {
                maze[i][j] = 3; // elmali olan yollar 3 olarak isaretlendi
            }
            
            else
            {
                maze[i][j] = 1;
            }
            
            if (c =='b') {
                baslangic_position.row = i;
                baslangic_position.col = j;
            }

            if (c =='c') {
                bitis_position.row = i;
                bitis_position.col = j;
            }
        }
    }

    fclose(maze_file);
    
}


int dfs(struct position* pos, struct node* root)
{   
    struct position current_pos;
    current_pos.col = pos->col;
    current_pos.row = pos->row;
    if ((current_pos.col) == (bitis_position.col) && ( current_pos.row) == (bitis_position.row))
    {
        return 1;
    }

    if (maze[current_pos.row][current_pos.col] == 1 || maze[current_pos.row][current_pos.col] == 3 ) // elma veya gidilebilir alan ise
    {   
        if (cikmaz_sokak)
        {
            // printf("YANLIS YOL !! \n\n");
            toplam_puan -= 5;
        }
        cikmaz_sokak = 0;
        

        if (maze[current_pos.row][current_pos.col] == 3)
        {
            // printf("Elma Buldum!\n\n");
            toplam_puan += 10;
        }
        maze[current_pos.row][current_pos.col] = 4; //visited olarak isaretledim

        maze_str[current_pos.row][current_pos.col] = '*'; // gosterimsel olarak da char maze'imde isaretledim.
        printMazeStr();


        root = push(root,current_pos); //geldigimiz noktayı stack'e pushladik.

        struct position tmp_sol;
        tmp_sol.row = current_pos.row;
        tmp_sol.col = current_pos.col - 1; // sola bakıyoruz.

        struct position tmp_sag;
        tmp_sag.row = current_pos.row;
        tmp_sag.col = current_pos.col + 1; // saga bakıyoruz.

        struct position tmp_ust;
        tmp_ust.row = current_pos.row - 1;
        tmp_ust.col = current_pos.col; // uste bakıyoruz.

        struct position tmp_alt;
        tmp_alt.row = current_pos.row + 1;
        tmp_alt.col = current_pos.col; // alta bakıyoruz.

        
        if (dfs(&tmp_sol,root)) // sol kutuya bakıyoruz
        {   
            return 1;
        }

        if (dfs(&tmp_sag,root)) // sag kutuya bakıyoruz
        {
            return 1;
        }

        if (dfs(&tmp_ust,root)) // ust kutuya bakıyoruz
        {
            return 1;
        }

        if (dfs(&tmp_alt,root)) // alt kutuya bakıyoruz
        {
            return 1;
        }

        struct position tmp_yanlisyol;
        tmp_yanlisyol = pop(root); //gittigim yanlis yollari stack'ten cikariyorum.
        
        maze_str[current_pos.row][current_pos.col] = ' ';
        cikmaz_sokak = 1;
    }

    return 0 ; //cikmaz sokak    
}

int main()
{

    allocateMaze();

    struct node* root = (struct node*)malloc(sizeof(struct node));
    root = NULL;
    
    // root->next = NULL;
    generateMaze("maze.txt");

    printf("MAZE COZULMEDEN ONCE\n\n");


    printf("Integerlardan olusan hali\nElmalar: 3\nGidilebilir Yollar: 1\nDuvarlar ise 0 ile gösterilmistir\n\n");


    printMazeBinary();


    printf("\nMaze'in anlasilabilir hali:\n");

    printMazeStr();

    printf("baslangic row : %d\t",baslangic_position.row);
    printf("baslangic col : %d\n",baslangic_position.col);

    printf("bitis row : %d\t",bitis_position.row);
    printf("bitis col : %d\n",bitis_position.col);

    delay(1000); //kullanıcının incelemesi icin süre

    root = push(root,baslangic_position); // baslangic pozisyonumu stack'e attım. Gittigim yollar boyu stacki dolduracağım yanlıs gidersem bosalticagim.


    printAllStruct(&root);
    
    
    int a = dfs(&baslangic_position,root);

    printf("================= \n");

    printf("Sırasıyla gidilen Yollar: \n");
    printAllStruct(root);


    return 0;
}