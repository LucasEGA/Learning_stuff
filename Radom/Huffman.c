#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char letter;             
    int count;
    int visited;
    struct Node *left;
    struct Node *right;
} Node;


/* Liefert ein dynamisches Array von Node (Blätter) zurück
   und schreibt die Anzahl in *out_unique_count. */
Node* counting(const char* word, int* out_unique_count) {
    int word_length = (int)strlen(word);

    // maximal so viele counter wie Zeichen im Wort
    Node* ptr = (Node*)malloc(word_length * sizeof(Node));
    if (!ptr) return NULL;

    int unique_count = 0;

    for (int i = 0; i < word_length; i++) {
        int found = 0;

        // prüfen, ob Buchstabe schon existiert
        for (int j = 0; j < unique_count; j++) {
            if ((ptr + j)->letter == word[i]) {
                (ptr + j)->count++;
                found = 1;
                break;
            }
        }

        // wenn neuer Buchstabe
        if (!found) {
            (ptr + unique_count)->letter  = word[i];
            (ptr + unique_count)->count   = 1;
            (ptr + unique_count)->visited = 0;
            (ptr + unique_count)->left    = NULL;
            (ptr + unique_count)->right   = NULL;
            unique_count++;
        }
    }

    // Anzahl der einzigartigen Buchstaben zurückgeben
    if (out_unique_count) *out_unique_count = unique_count;

    // optional auf passende Größe kürzen
    if (unique_count > 0) {
        Node* tmp = (Node*)realloc(ptr, unique_count * sizeof(Node));
        if (tmp) ptr = tmp;
    }

    return ptr;
}

/* Sortiert ein NULL-terminiertes Array von Node* nach count (aufsteigend). 
   Gibt das gleiche Array zurück (Bequemlichkeit). */
Node** Huffman_order(Node **arr) {
    if (!arr) return NULL;

    int length = 0;
    while (arr[length] != NULL) length++;

    // bubble-sort
    for (int i = 0; i < length - 1; i++) {
        for (int j = i + 1; j < length; j++) {
            if (arr[i]->count > arr[j]->count) {
                Node *temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    return arr;
}

/* Baut aus einem NULL-terminierten Array von Node* den Huffman-Baum
   und liefert die Wurzel zurück. */
Node* Huffman_tree(Node **arr){
    if (!arr) return NULL;

    // Länge bestimmen
    int n = 0;
    while (arr[n] != NULL) n++;

    // Ecke: kein oder nur ein Knoten
    if (n == 0) return NULL;
    if (n == 1) return arr[0];

    // wiederholt zwei kleinste Knoten zusammenfassen
    while (n > 1) {
        // sortieren, damit zwei kleinste vorne stehen
        Huffman_order(arr);

        // neuen Eltern-Knoten anlegen
        Node* parent = (Node*)malloc(sizeof(Node));
        if (!parent) return NULL;

        parent->letter  = '\0';                 // interner Knoten
        parent->count   = arr[0]->count + arr[1]->count;
        parent->visited = 0;
        parent->left    = arr[0];
        parent->right   = arr[1];

        // arr[0] durch parent ersetzen und arr[1] entfernen (links zusammenschieben)
        arr[0] = parent;
        for (int i = 1; i < n - 1; i++) {
            arr[i] = arr[i + 1];
        }
        // neues Ende setzen (NULL-Terminierung beibehalten)
        arr[n - 1] = NULL;

        // ein Element weniger
        n--;
    }

    // jetzt ist arr[0] die Wurzel
    return arr[0];
}


static char CODEBUF[512];   // ausreichend groß wählen
static int  CODELEN = 0;    // aktueller Schreibindex

void print_codes(Node* n){
    if (!n) return;

    // Blatt: Code ausgeben
    if (!n->left && !n->right) {
        CODEBUF[CODELEN] = '\0';
        printf("%c: %s\n", n->letter, CODEBUF);
        return;
    }

    // links -> '0'
    CODEBUF[CODELEN++] = '0';
    print_codes(n->left);
    CODELEN--;  // backtrack

    // rechts -> '1'
    CODEBUF[CODELEN++] = '1';
    print_codes(n->right);
    CODELEN--;  // backtrack

}

int main(void) {
    const char* word = "MISSISSIPPI";
    int unique = 0;

    Node* leaves = counting(word, &unique);
    if (!leaves || unique == 0) return 0;

    // Pointer-Array + NULL am Ende
    Node** arr = (Node**)malloc((unique + 1) * sizeof(Node*));
    for (int i = 0; i < unique; i++) arr[i] = &leaves[i];
    arr[unique] = NULL;

    Node* root = Huffman_tree(arr);

    // Codes ausgeben 
    print_codes(root);

    // Cleanup 
    free(arr);     
    free(leaves); 
    
    return 0;
}

