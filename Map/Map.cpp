#include <iostream>
#include <Windows.h>

using namespace std;

template <class K, class V>
class Map 
{
public:
    struct Node {
        K key{};
        V value{};
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
    };
    Map() : size(0), root(nullptr){}

    Map(const Map& other) : size(other.size), root(Copy(other.root, nullptr)) {}

    Map(Map&& other) : size(other.size), root(other.root)
    {
        other.root = nullptr;
        other.size = 0;
    }

    Map& operator=(const Map& other)
    {
        if (this != &other)
        {
            Clear();
            size = other.size;
            root = Copy(other.root, nullptr);
        }
        return *this;
    }

    Map& operator=(Map&& other)
    {
        if (this != &other)
        {
            Clear();
            size = other.size;
            root = other.root;
            other.root = nullptr;
            other.size = 0;
        }
        return *this;
    }


    bool operator==(const Map& other)
    {
        return size == other.size && Compare(root, other.root);
    }
    bool operator!=(const Map& other)
    {
        return !(*this == other);
    }

    void Insert(const K& k, const V& val)
    {
        if (root == nullptr) {
            root = new Node{ k,val, nullptr, nullptr, nullptr };
            size++;
            return;
        }
        Node* parent = nullptr;
        Node* node = root;
        while (node != nullptr && node->key != k) {
            parent = node;
            node = node->key > k ? node->left : node->right;
        }
        if (node == nullptr) {
            Node* temp = new Node{ k, val, nullptr, nullptr, parent };

            parent->key > k ? parent->left = temp : parent->right = temp;
            size++;
        }
    }

    Node* Min(Node* node)
    {
        if (node != nullptr)
        {
            while (node->left != nullptr)
            {
                node = node->left;
            }
        }
        return node;
    }

    Node* Max(Node* node)
    {
        if (node != nullptr)
        {
            while (node->right != nullptr)
            {
                node = node->right;
            }
        }
        return node;
    }

    void Print() {
        Print(root);
        cout << endl;
    }

    void Print(Node* node) {
        if (node != nullptr) {
            Print(node->left);
            cout << node->key << "\t: " << node->value << "\n";
            Print(node->right);
        }
    }

    void Erase(const K& k)
    {
        if (size > 0)
        {
            Node* node = root;
            Node* parent = nullptr;
            while (node->key != k && node != nullptr)
            {
                parent = node;
                node = node->key > k ? node->left : node->right;
            }
            if (node != nullptr)
            {
                if (node->left == nullptr && node->right == nullptr)
                {
                    if (node == root)
                    {
                        root = nullptr;
                    }
                    else
                    {
                        node == parent->right ? parent->right = nullptr : parent->left = nullptr;
                    }
                    delete node;
                }
                else if (node->left == nullptr)
                {
                    node->right->parent = node->parent;
                    if (node == root)
                    {
                        root = node->right;
                    }
                    else
                    {
                        node == parent->right ? parent->right = node->right : parent->left = node->right;
                    }
                    delete node;
                }
                else if (node->right == nullptr)
                {
                    node->left->parent = node->parent;
                    if (node == root)
                    {
                        root = node->left;
                    }
                    else
                    {
                        node == parent->left ? parent->left = node->left : parent->right = node->left;
                    }
                    delete node;
                }
                else
                {
                    Node* temp = Min(node->right);
                    if (temp->right)
                    {
                        temp->right->parent = temp->parent;
                    }
                    temp == temp->parent->right ? temp->parent->right = temp->right : temp->parent->right = temp->right;
                    node->key = temp->key;
                    node->value = temp->value;
                    delete temp;
                }
                size--;
            }

        }
    }

    void Clear()
    {
        Clear(root);
        root = nullptr;
        size = 0;
    }

    void Clear(Node* node) {
        if (node != nullptr) {
            Clear(node->left);
            Clear(node->right);
            delete node;
        }
    }

    Node* Find(const K& k)
    {
        Node* node = root;
        while (node != nullptr && node->key != k)
        {
            node = node->key > k ? node->left : node->right;
        }
        return node;
    }

    int Count(const K& k)
    {
        return Find(k) == nullptr ? 0 : 1;
    }

    Node* Copy(Node* node, Node* parent)
    {
        if (!node) return node;
        Node* temp = new Node{ node->key, node->value,nullptr, nullptr, parent };
        temp->left = Copy(node->left, temp);
        temp->right = Copy(node->right, temp);
        return temp;
    }

    bool Compare(Node* a, Node* b)
    {
        if (!a && !b)
        {
            return true;
        }
        return (a && b) && (a->key == b->key) && (a->value == b->value) && Compare(a->left, b->left) && Compare(a->right, b->right);
    }

    Node* Next(Node* node)
    {
        if (node)
        {
            if (node->right)
            {
                return Min(node->right);
            }
            Node* parent = node->parent;
            while (parent && node->key > parent->key)
            {
                parent = parent->parent;
            }
            return parent;
        }
    }

    Node* Prev(Node* node)
    {
        if (node)
        {
            if (node->left)
            {
                return Max(node->left);
            }
            Node* parent = node->parent;
            while (parent && node->key < parent->key)
            {
                parent = parent->parent;
            }
            return parent;
        }
    }

    Node* Begin()
    {
        return Min(root);
    }
    Node* End()
    {
        return Max(root);
    }

    int Size() {
        return size;
    }

    ~Map()
    {
        Clear();
    }

private:
    int size;
    Node* root;
};


void main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Map <string,string> book;
    book.Insert("Оля", "+79145769385");
    book.Insert("Димон", "+79145768584");
    book.Insert("Света", "+79145761651");
    book.Insert("Катя", "+79145760956");
    book.Insert("Макс", "+79154669854");
    book.Print();
    book.Erase("Димон");
    book.Print();
    auto number = book.Find("Света");
    cout << number->key <<"\t: " << number->value << "\n";
    cout << book.Count("Димон") << "\n";
    Map <string, string> book2;
    book2 = move(book);
    book2.Print();
}