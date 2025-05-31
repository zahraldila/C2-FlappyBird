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

void initPipaList(); //modul untuk inisialisasi linked list
void initList(Singlelinkedlist *L);
address buatNodePipa(int i); //modul untuk menambahkan data ke linked list (plist)
address buatNodeTPipa(int i, int tinggi); //modul untuk menambahkan data ke linked list (tplist) = tutup pipa
void insertBelakang(address newNodePipa, address newNodeTPipa); //modul untuk menginsert (menambah node) dibelakang list
void deleteFirst(); //modul untuk menghapus node pertama
void freeList();//modul untuk membebaskan semua node dari memori

#endif //pipa_ll 