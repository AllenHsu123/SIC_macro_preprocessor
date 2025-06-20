#include <stdio.h>
#include <string.h>
FILE *sc;
FILE *dest;
char deftab[100][100];
char arr[100];
//char nam_tab[100][20];
struct namtab_table
{
    char macro_name[20];
    int pstart;
    int pend;
    char argtab[10][20];
};
struct namtab_table namtab[100];
char opcode[20];
char macro_name[20];
int macro_count=0;
int expanding = 0;
int cur_def_index = 0;
int def_index=0;
int macro_num = 0;
int cur_macro = 0;
char argtab[20][100];
void expand();
void getline();
void processline();
void define();

int main()
{
    sc = fopen("sc.txt","r");
    dest = fopen("dest.txt","w");
    while(1)
    {
        getline();
        processline();
        if (strcmp(opcode,"END") == 0) 
            break;
    }
   
    fclose(sc);
    fclose(dest);

    return 0;

}

void expand()
{
    
    expanding = 1;
    cur_macro = macro_num;
    def_index = namtab[macro_num].pstart+1;
    int end_num = namtab[macro_num].pend-1;
    if(arr[0] != 9)
        fputc('.',dest);
    fputs(arr,dest);
    putc('\n',dest);

    char arg[20];
    int i=0;
    int arg_num=0;
    int k=2;
    while( k > 0 )
    {
        if(arr[i] == 9)
            k--;
        i++;
    }
    while(arr[i]!=0)
    {
        int j=0;
        while((arr[i]!=',')&&(arr[i]!=0))
        {
            arg[j] = arr[i];
            j++;
            i++;
        }
        arr[j] = 0;
        strcpy(argtab[arg_num],arg);
        arg_num++;
        
        if(arr[i] == ',')
            i++;
    }
    argtab[arg_num][0] = 0;
    /*
    int aa = 0;
    while(argtab[aa][0]!= 0)
    {
        printf("arg: %s\n",argtab[aa]);
        aa++;
    }*/
    
    while(def_index < end_num)
    {
        getline();
        processline();
        def_index++;
    }
    expanding = 0;
};
void getline()
{
    char c;
    int count=0;
    if(expanding)
    {
        char tmp_arr[100];
        //strcpy(arr, deftab[def_index]);
        strcpy(tmp_arr, deftab[def_index]);
        int tmp_index = 0;
        int arr_index = 0;
        
        //==============
        
        while(tmp_arr[tmp_index]!=0)
        {
            int count = 0;//記錄第n個變數
            char ch[20];
            int i=0;
            
            if(tmp_arr[tmp_index] == '&')
            {
                tmp_index++;
                while((tmp_arr[tmp_index]!=',')&&(tmp_arr[tmp_index]!=0)&&(tmp_arr[tmp_index]!=39)&&(tmp_arr[tmp_index]!=9))// 39士丹
                {
                    ch[i] = tmp_arr[tmp_index];
                    i++;
                    tmp_index++;
                }
                ch[i] = 0;     
                //tmp_index++;
                count = 0;
                while(1)
                {
                    if(strcmp(namtab[cur_macro].argtab[count],ch) == 0)
                        break;
                    count++;
                }
                i=0;
                while(argtab[count][i] != 0)
                {
                    arr[arr_index] = argtab[count][i];
                    i++;
                    arr_index++;
                }
                //=========
            }
            else
            {
                arr[arr_index] = tmp_arr[tmp_index];
                arr_index++;
                tmp_index++;
            }
            /*
            arr[arr_index] = tmp_arr[tmp_index];
            arr_index++;
            tmp_index++;*/
        }
        arr[arr_index] = 0;
        
    }
    else
    {
        while(((c = getc(sc))!='\n')&&(c!=EOF))
        {
            arr[count] = c;
            count++;
        }
        arr[count] = 0; 
    }
      

// tab == 9
    int i=0;
    while(arr[i] != 9)
    {
        macro_name[i] = arr[i];
        i++;
    }
    macro_name[i] = 0;
//arr+i+1 => opcode
    int j=0;
    while((*(arr+i+1)!= '\n')&&(*(arr+i+1)!= 9)&&(*(arr+i+1)!= 0))
    {
        opcode[j] = *(arr+i+1);
        j++;
        i++;
    }
    opcode[j] = 0; 
    printf("%s\n",arr);
};
void processline()
{
//=====================查表
    int check=0;
    macro_num = 0;
    while(namtab[macro_num].macro_name[0]!=0)
    {
        if (strcmp(namtab[macro_num].macro_name, opcode) == 0) 
        {
            check = 1;
            break;
        }
        macro_num++; 
    }
//=====================
    if(check)
        expand();
    else if(strcmp(opcode,"MACRO")==0)
        define();
    else
    {
        fputs(arr,dest);
        putc('\n',dest);
    }
};

void define()
{
    strcpy(namtab[macro_count].macro_name, macro_name);
    int i=0;
    int arg_num = 0;
    char arg[20];
    while(arr[i]!=0)
    {
        if(arr[i] == '&')
        {
            int j = 0;
            while((arr[i]!=',')&&(arr[i]!=0))
            {
                arg[j] = arr[i];
                j++;
                i++;
            }
            strcpy(namtab[macro_count].argtab[arg_num],arg+1);
            arg_num++;
        }
        i++;
    }



    strcpy(deftab[cur_def_index], arr);
    namtab[macro_count].pstart = cur_def_index;
    cur_def_index++;
     //==========================
    int level = 1;
    while(level > 0)
    {
        getline();
        strcpy(deftab[cur_def_index], arr);
        cur_def_index++;
        if(strcmp(opcode,"MEND") == 0)
        level--;
    }
    namtab[macro_count].pend = cur_def_index;
    macro_count++;
};
