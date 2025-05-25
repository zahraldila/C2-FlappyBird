#ifndef PIPA_LL
#define PIPA_LL

typedef int infotype;
typedef struct tNode* address;

typedef struct tNode {
    infotype korx;
    infotype tinggi; // Akan merepresentasikan tinggi pipa ATAS
    infotype status; // 0 = belum dilewati, 1 = sudah dilewati & skor ditambah
    address next;
} Node;

typedef struct Singlelinkedlist{
    address head;
    address tail;
}Singlelinkedlist;

void initList(Singlelinkedlist *L);
address buatNodePipaGenerik(int tinggi_pipa_atas);
void insertBelakang(Singlelinkedlist *list, address newNode);
void deleteFirst(Singlelinkedlist *list);
void freeList(Singlelinkedlist *list); // Membersihkan satu list

// Fungsi untuk mengelola plist dan tplist secara bersamaan
void insertBelakangPasangan(address newNodePipa, address newNodeTPipa);
void deleteFirstPasangan(void);
void freeListPasangan(void); // Akan bekerja pada plist dan tplist global

#endif //PIPA_LL