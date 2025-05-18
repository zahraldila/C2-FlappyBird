#ifndef PIPA_LL
#define PIPA_LL

typedef int infotype;
typedef struct tNode* address;

typedef struct tNode {
    infotype korx;
    infotype tinggi;
    infotype status;
    address next;
}Node;

typedef struct Singlelinkedlist{
    address head;
    address tail;
}Singlelinkedlist;

void initList(Singlelinkedlist *L);
address buatNodePipa(int i);
address buatNodeTPipa(int i, int tinggi);
void insertBelakang(address newNodePipa, address newNodeTPipa);
void deleteFirst();
void freeList();

#endif //pipa_ll 