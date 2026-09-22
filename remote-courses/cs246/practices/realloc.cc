struct Node {
    int value;
    Node* next;
};

int main () {
    int* arr = new int[10];

    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }

    //resize the array

    int* temp = new int[20];
    for (int i = 0; i < 10; i++) {
        temp[i] = arr[i];
    }
    delete[] arr;
    arr = temp;

    // avoid memory leak
    delete temp;

    Node* node = new Node[10];
    node[1] = Node{1, nullptr}; 
}
