#include<stdio.h>
#include<stdlib.h>

int m,limit=1;
int minterms;       //stores the number of minterms given excluding don't cares
int gp,newgp;   //denotes the maximum and next maximum group
int input[10000];  //stores given minterms as input

struct node                  //node declaration
{
    struct node* next;    //link to next node in the list
    int count;            //stores 1 or 0 to denote the state of pairing
    int count1;   //stores the number of ones in the minterm
    int paired[10000]; // array to store the paired minterms value
    int grpno;                  //denote the grpno according to the number of ones
    int binary[10000];   //stores the binary of the term then formed paired expression
    int pairs;    //tells about no. of pairs
}*start,*start2;

struct implicantsTable         //Prime Implicants Table
{
    int a[10000][10000];       //stores the binary form of paired group
    int b[10000][10000];       //stores the 1's corresponding to the minterms
    int top;                   //tells about no. of prime implicants in a row
    int countmin[10000];    //stores the number of the minterms in the particular column
}tab;
//declarations of function prototype
struct node* getnode(int);
void createList(int);
void inputbinary(struct node*,struct node*,struct node*);
void pairing();
void print();
void printtable();
struct node* createnewnode(struct node*,struct node*);
void table();
int change(struct node*,struct node*);
void add(struct node*,struct node*);
void addToTable();
void primetable();
void expression(int);
int greatest(int*);
int imp(int,int*);
void del(int);
void tabulation();

void main()
{
    tabulation();
}

void tabulation()
{
    printf("\n\t\t\t\t\t\t Tabulation Method\n");
    printf("\n\t\t\t\t~ The Complement is shown by small alphabet.\n\n\n");
    int i,temp;
    gp=newgp=-1;
    printf("Enter Number Of Variables:");
    scanf("%d",&m);
    for(i=0;i<m;i++)
    {
        limit*=2;
    }
    printf("\nEnter the number of Minterms: ");
    scanf("%d",&minterms);
        if(minterms==0)
        return;
    for(i=0; i<limit; i++)
        input[i]=-1;
    for(i=0; i<minterms; i++)
        {
            printf("\nEnter the minterms:");
            scanf("%d",&temp);
            input[temp]=1;
            createList(temp);
        }
    tab.top=0;
    table();    //initialize the Prime Implicants Table with all cells -1 to denote empty.
    pairing();           //do the pairing
    printtable(); //print the Prime Implicants Table
    printf("\n Final Minimized Expression is: ");
    primetable();  // Analyse the table and print the final expression
    printf("\n");
   
}
struct node* getnode(int n)    //creates a node to store the minterm data
{
    int c=m-1;
    struct node *p;
    p=(struct node *)malloc(sizeof(struct node));
    p->next=NULL;
    if(p==NULL)
        printf("\n Can't Insert\n");
    else
    {
        p->count1=0;
        p->paired[0]=n;
        p->pairs=1;
        while(n!=0)
        {
            p->binary[c]=n%2;
            if(p->binary[c]==1)
                p->count1++;
            c--;
            n=n/2;
        }
        while(c!=-1)
        {
            p->binary[c]=0;
            c--;
        }
        p->count=0;
    }
    p->grpno=p->count1;
    if(p->grpno>gp)
        gp=p->grpno;
    return p;
}
void createList(int n)     //creates a linked list for given minterms
{
    struct node *p,*q,*temp;
    p=getnode(n);
    if(p!=NULL)
        {
        if(start==NULL)
            {
            start=p;
            start->next=NULL;
            return;
            }
        else
            {
            q=start;
            if(p->grpno < start->grpno)
                {
                    p->next=start;
                    start=p;
                    return;
                }
            while(q->next!=NULL && ((q->next->grpno)<=(p->grpno)))
            {
                q=q->next;
            }
            if(q->next==NULL)
            {
                q->next=p;
                p->next=NULL;
            }
            else
            {
                temp=q->next;
                q->next=p;
                p->next=temp;
            }
            }
        }
}
void inputbinary(struct node *p,struct node *q,struct node *r)  //take input in binary value of r using p and q
{
    int c=m-1;
    while(c!=-1)
        {
        if(p->binary[c]==q->binary[c])
            {
            r->binary[c]=p->binary[c];
            }
        else
            {
            r->binary[c]=-1;
            }
        c--;
        }
}
struct node* createnewnode(struct node *p,struct node *q)    //creates a new node using given nodes
{
    int i,j;
    struct node *r;
    r=(struct node *)malloc(sizeof(struct node));
    if(r==NULL)
        printf("\n Can't Insert\n");
    else
        {
        for(i=0; i<p->pairs; i++)
            {
            r->paired[i]=p->paired[i];
            }
        r->pairs=p->pairs*2;
        for(j=0; j<q->pairs; j++)
            {
            r->paired[i++]=q->paired[j];
            }
        r->count=0;
        r->next=NULL;
        r->grpno=p->grpno;
        inputbinary(p,q,r);
        }
    return r;
}
void add(struct node *p,struct node *q)   //create a linked list to store the paired minterms
{
    struct node *r,*temp;
    r=createnewnode(p,q);
    if(start2==NULL)
        {
        start2=r;
        }
    else
        {
        temp=start2;
        while(temp->next!=NULL)
            temp=temp->next;
        temp->next=r;
        }
}
void table()
{
    int i,j;
    for(i=0; i<=limit-1; i++)
        for(j=0; j<=limit-1; j++)
        {
            tab.b[i][j]=-1;
        }
}
void print()     //prints the minterms and their pairing and binary values at each pass
{
    int c=1,count=0,j=0;
    struct node *p;
    p=start;
    while(p!=NULL)
    {
        j=0;
        while(count<(p->pairs))
        {
            printf("%d,",p->paired[count]);
            count++;
        }
        printf("\b");
        count=0;
        printf("   ");
        while(j<m)
        {
            if(p->binary[j]==-1)
                printf("%c",'-');
            else
                printf("%d",p->binary[j]);
            j++;
        }
        printf("\n");
        c++;
        p=p->next;
    }
}
void printtable()   //print the prime implicants table
{
    int i,j;
    printf("\nPrime Implicants Table : \n");
    for(i=0; i<tab.top; i++)
    {
        expression(i);
        for(j=0; j<=limit-1; j++)
        {
            if(tab.b[i][j]==1)
                printf("   %d  ",j);
        }
        printf("\n");
    }
}
void addToTable()
{
    int i,j,k,same;
    struct node *p;
    p=start;
    while(p!=NULL)
    {
        if(!(p->count))
        {
            if(tab.top!=0)         //checking for more than one 1's
            {
                for(j=0; j<tab.top; j++)
                {
                    same=1;
                    for(k=0; k<p->pairs; k++)
                    {
                        if(tab.b[j][p->paired[k]]==1)
                            continue;
                        else
                        {
                            same=0;
                            break;
                        }
                    }
                    if(same==1)
                    {
                        break;
                    }
                }
                if(same==1)
                {
                    p=p->next;
                    continue;
                }
            }
            i=m-1;
            while(i!=-1)
            {
                tab.a[tab.top][i]=p->binary[i];
                i--;
			}
            for(i=0; i<p->pairs; i++)
            {
                tab.countmin[tab.top]++;
                tab.b[tab.top][p->paired[i]]=1;
            }
            tab.top++;
        }
        p=p->next;
    }
}
void pairing()    //does the pairing work
{
    struct node *p,*q;
    int same=0;
    static int tableno=1;  //stores the table number
    p=start;
    q=p;
    printf("\nTable %d :\n",tableno);
    tableno++;
    print();
    newgp=-1;
    p=start;
    while(p->grpno!=gp)
    {
        q=q->next;
        while(q!=NULL&&(q->grpno==p->grpno))
        {
            q=q->next;
        }
        if(q==NULL)
        {
            p=p->next;
            q=p;
            continue;
        }
        else
        {
            if(q->grpno!=(p->grpno+1))
            {
                p=p->next;
                q=p;
                continue;
            }
            if(change(p,q))        //execute when pairing can occur
            {
                same=1;
                p->count=1;
                q->count=1;
                add(p,q);
                if((p->grpno)>newgp)
                    newgp=p->grpno;
            }
        }
    }
    addToTable();
    if(same)      //execute only when atleast one pair has made
    {
        start=start2;
        start2=NULL;
        gp=newgp;
        pairing();
    }
}

int greatest(int *row)       //finds the prime implicant which has the greatest number of unused minterms

{
    int i,large=-1;
    for(i=0; i<tab.top; i++)
    {
        if(tab.countmin[i]>large)
        {
            *row=i;
            large=tab.countmin[i];
        }
    }
    return large;
}
void primetable()      // analysing of the prime table
{
    int i,j,k,maxrow,s=1;
    int ess[limit];      //stores the row number of all essential prime implicants
    int temp,c;
    for(i=0; i<=limit-1; i++)
        ess[i]=-1;
    for(i=0; i<=limit-1; i++)
    {
        if(input[i]==1)
        {
            if(imp(i,&temp)==1)
            {
                ess[i]=temp;
            }
        }
    }
    for(i=0; i<=limit-1; i++)
    {
        if(ess[i]!=-1)
        {
            if(s!=1)
                printf(" + ");
            else
                s=0;
            expression(ess[i]);
            del(ess[i]);
            for(j=i+1; j<=limit-1; j++)
            {
                if(ess[j]==ess[i])
                    ess[j]=-1;
            }
            ess[i]=-1;
        }
    }
    while(greatest(&maxrow)!=0)
    {
        if(s!=1)
            printf(" + ");
        else
            s=0;
        expression(maxrow);
        del(maxrow);
    }
    printf("\b");
}
void del(int n)    //delete all the minterms from a implicants row and columns
{
    int i,j;
    for(i=0; i<=limit-1; i++)
    {
        if(tab.b[n][i]==1)
        {
            input[i]=-1;
            for(j=0; j<tab.top; j++)
            {
                if(tab.b[j][i]==1)
                {
                    tab.b[j][i]=-1;
                    tab.countmin[j]--;
                }
            }
        }
    }
}
int imp(int n,int *temp)     //tells about no. of prime implicants having a particular minterm
{
    int i,j;
    int count=0;
    for(i=0; i<tab.top; i++)
    {
        if(tab.b[i][n]==1)
        {
            j=i;
            count++;
        }
    }
    *temp=j;
    return count;
}
void expression(int n)   //converts binary into its expression form
{
    int c=0;
    char x[]= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    char y[]= {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    while(c!=m)
    {
        if(tab.a[n][c]!=-1)
        {
            if(tab.a[n][c]==1)
                printf("%c",x[c]);
            else
                printf("%c",y[c]);
        }
        c++;
    }
}
int change(struct node *a,struct node *b)   //checks for change of only one bit
{
    int c=m-1;
    int difference=0;
    while(c!=-1)
    {
        if(a->binary[c]!=b->binary[c])
        {
            if(difference)
                return 0;
            else
                difference=1;

        }
        c--;
    }
    return 1;
}

