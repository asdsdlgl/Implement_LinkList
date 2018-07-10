#include<stdio.h>
#include<stdlib.h>
#include<time.h>
struct listnode{	//strcut�إ�
        unsigned int data;
        struct listnode *next;
};
typedef struct listnode Listnode;	//struct typedef
Listnode *insert(Listnode *head,unsigned int data){
        Listnode *current = (Listnode *)malloc(sizeof(Listnode));//insert malloc
        Listnode *ptr = head;
        Listnode *last = NULL;
        current->data = data;
        if(head == NULL){//�p�Gnull�^��
                current->next = NULL;
                return current;
        }//�N��ƥѤp��j��_��
        while(ptr!=NULL && ptr->data < data){
                last = ptr;
                ptr = ptr->next;
        }//�p�G��J���̤j�Acurrent���s��head
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
        }//�j�Mhead
        return head;
}//�Q���������ЦP�ɯ�����ܨ�head,�åB�^��delete��node
Listnode *delete(Listnode **head,unsigned int data){
        Listnode *temp = (*head);
        Listnode *last = NULL;//�p�G�Ĥ@�ӭn�R��
        if((*head)==NULL)return NULL;
        if((*head)->data==data){
                temp = (*head);//�Nhead����temp
                (*head) = (*head)->next;
                return temp;//�ñNhead���U�@��
        }
        for(;temp!=NULL;temp = temp->next){
                if(temp->data==data){//���O�Ĥ@�ӭndelete
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
}//�p��
int main(int argc,char* argv[]){
        FILE *fp;
        unsigned long long int begin,end, result= 0;
        int k = atoi(argv[1]);
        int n = atoi(argv[2]);
        int m = atoi(argv[3]);
        int i,count=0;
        unsigned int a,r,bit,temp;
        unsigned int arrsearch[m];
        unsigned int deletenum[m];//�ɮ׼g�J
        fp = fopen("result.txt","w");
        Listnode *head[1<<k];//�ھ�k�n�ǳƦh�֪�head
        Listnode *deletenode[m];
        Listnode *match[m];
        srand(time(NULL));
        for(i=0;i<m;i++){//��l��
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
                bit = r>>(32-k);//�Q��insert�إ߮ɨæP�ɱƧ�
                head[bit] = insert(head[bit],r);
                if(i<m)arrsearch[i] = r;//�O���em��node data
        }

        begin = rdtsc();//�p��
        for(i=0;i<m;i++){
                temp = arrsearch[i]>>(32-k);//�ĴX��head
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
                deletenum[i] = r; //����deletenum
        }
        end = rdtsc();
        result = end-begin;
        printf("insert : %u\n",result);

        begin = rdtsc();
        for(i=0;i<m;i++){//�̫�^�Ǫ�����R��node�åi�H��head
                temp = deletenum[i]>>(32-k);
                deletenode[i] = delete(&head[temp],deletenum[i]);
        }
        end = rdtsc();
        result = end-begin;
        printf("delete : %u\n",result);

        for(i=0;i<m;i++)fprintf(fp,"%u\n",match[i]->data);
	//�g�J�ɮ�
        fclose(fp);//���ɮ�
}

