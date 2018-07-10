#include<stdio.h>
#include<stdlib.h>
#include<time.h>
struct listnode{	//strcut建立
        unsigned int data;
        struct listnode *next;
};
typedef struct listnode Listnode;	//struct typedef
Listnode *insert(Listnode *head,unsigned int data){
        Listnode *current = (Listnode *)malloc(sizeof(Listnode));//insert malloc
        Listnode *ptr = head;
        Listnode *last = NULL;
        current->data = data;
        if(head == NULL){//如果null回傳
                current->next = NULL;
                return current;
        }//將資料由小到大串起來
        while(ptr!=NULL && ptr->data < data){
                last = ptr;
                ptr = ptr->next;
        }//如果輸入為最大，current為新的head
        if(last == NULL){
                current->next = head;
                return current;
        }else{
                current->next = last->next;
                last->next = current;
                return head;
        }
}
Listnode *search(Listnode *head,unsigned int data){
        for(;head!=NULL;head = head->next){
                if(head->data==data)break;
        }//搜尋head
        return head;
}//利用雙重指標同時能夠改變到head,並且回傳delete的node
Listnode *delete(Listnode **head,unsigned int data){
        Listnode *temp = (*head);
        Listnode *last = NULL;//如果第一個要刪除
        if((*head)==NULL)return NULL;
        if((*head)->data==data){
                temp = (*head);//將head給予temp
                (*head) = (*head)->next;
                return temp;//並將head往下一個
        }
        for(;temp!=NULL;temp = temp->next){
                if(temp->data==data){//不是第一個要delete
                        last->next = temp->next;
                        break;
                }
                last = temp;
        }
        return temp;
}
inline unsigned long long int rdtsc(){
        unsigned long long int x;
        asm volatile ("rdtsc" : "=A" (x));
        return x;
}//計時
int main(int argc,char* argv[]){
        FILE *fp;
        unsigned long long int begin,end, result= 0;
        int k = atoi(argv[1]);
        int n = atoi(argv[2]);
        int m = atoi(argv[3]);
        int i,count=0;
        unsigned int a,r,bit,temp;
        unsigned int arrsearch[m];
        unsigned int deletenum[m];//檔案寫入
        fp = fopen("result.txt","w");
        Listnode *head[1<<k];//根據k要準備多少的head
        Listnode *deletenode[m];
        Listnode *match[m];
        srand(time(NULL));
        for(i=0;i<m;i++){//初始化
                arrsearch[i] = 0;
                deletenum[i] = 0;
                match[i] = NULL;
                deletenode[i] = NULL;
        }
        for(i=0;i<(1<<k);i++)head[i] = NULL;
        for(i=0;i<n;i++){
                a = rand();//32bit rand
                a = (a & 1) << 31;
                r = rand();
                r = r | a;
                bit = r>>(32-k);//利用insert建立時並同時排序
                head[bit] = insert(head[bit],r);
                if(i<m)arrsearch[i] = r;//記錄前m個node data
        }

        begin = rdtsc();//計時
        for(i=0;i<m;i++){
                temp = arrsearch[i]>>(32-k);//第幾個head
                match[i] = search(head[temp],arrsearch[i]);
        }//search
        end = rdtsc();
        result = end-begin;
        printf("search : %u\n",result);

        begin = rdtsc();
        for(i=0;i<m;i++){
                a = rand();
                a = (a & 1) << 31;
                r = rand();
                r = r | a;
                bit = r>>(32-k);
                head[bit] = insert(head[bit],r);//insert
                deletenum[i] = r; //紀錄deletenum
        }
        end = rdtsc();
        result = end-begin;
        printf("insert : %u\n",result);

        begin = rdtsc();
        for(i=0;i<m;i++){//最後回傳的為其刪除node並可以改head
                temp = deletenum[i]>>(32-k);
                deletenode[i] = delete(&head[temp],deletenum[i]);
        }
        end = rdtsc();
        result = end-begin;
        printf("delete : %u\n",result);

        for(i=0;i<m;i++)fprintf(fp,"%u\n",match[i]->data);
	//寫入檔案
        fclose(fp);//關檔案
}

