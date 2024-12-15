#include <iostream>
#include <iomanip>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <set>
#include <iterator>

struct Node {
    int value;       // Значение узла
    Node* left;      // Левый сын
    Node* right;     // Правый брат
    int height;      // Высота узла

    Node(int val) : value(val), left(nullptr), right(nullptr), height(1) {}
};

class BinarySearchTree {
public:
    Node* root;

    BinarySearchTree() : root(nullptr) {}

    void insert(int value) {
        root = insertRec(root, value);
    }

    void display() {
        if (root != nullptr) {
            std::cout << "Дерево:\n";
            displayRec(root, 0);
        }
        else {
            std::cout << "Дерево пусто.\n";
        }
    }

    void unionWithSymmetricTraversal(BinarySearchTree& other) {
        if (other.root == nullptr) return;

        std::stack<Node*> stack;
        Node* current = other.root;

        while (current != nullptr || !stack.empty()) {
            while (current != nullptr) {
                stack.push(current);
                current = current->left;
            }

            current = stack.top();
            stack.pop();
            insert(current->value); // Вставляем значение в дерево A

            current = current->right;
        }
    }

    class Iterator {
    public:
        std::stack<Node*> stack;

        Iterator(Node* root) {
            moveToNext(root);
        }

        int operator*() {
            return stack.top()->value;
        }

        Iterator& operator++() {
            Node* current = stack.top();
            stack.pop();
            moveToNext(current->right);
            return *this;
        }

        bool operator!=(const Iterator& other) {
            return !stack.empty() || !other.stack.empty();
        }

    private:
        void moveToNext(Node* node) {
            while (node) {
                stack.push(node);
                node = node->left;
            }
        }
    };

    Iterator begin() {
        return Iterator(root);
    }

    Iterator end() {
        return Iterator(nullptr);
    }

private:
    Node* insertRec(Node* node, int value) {
        if (node == nullptr) {
            return new Node(value);
        }

        // Стандартные правила вставки
        if (value < node->value) {
            node->left = insertRec(node->left, value);
        }
        else {
            node->right = insertRec(node->right, value);
        }

        // Балансировка узла
        return balance(node);
    }

    int height(Node* node) {
        return node ? node->height : 0;
    }

    int getBalance(Node* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        if (x == nullptr) return y; // Если x равен nullptr, ничего не делаем

        Node* T2 = x->right;

        // Выполняем вращение
        x->right = y;
        y->left = T2;

        // Обновляем высоты
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;

        return x; // Возвращаем новый корень
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        if (y == nullptr) return x; // Если y равен nullptr, ничего не делаем

        Node* T2 = y->left;

        // Выполняем вращение
        y->left = x;
        x->right = T2;

        // Обновляем высоты
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;

        return y; // Возвращаем новый корень
    }

    Node* balance(Node* node) {
        // Балансировка
        node->height = std::max(height(node->left), height(node->right)) + 1;
        int balanceFactor = getBalance(node);

        // Левый левый случай
        if (balanceFactor > 1 && getBalance(node->left) >= 0) {
            return rightRotate(node);
        }

        // Правый правый случай
        if (balanceFactor < -1 && getBalance(node->right) <= 0) {
            return leftRotate(node);
        }

        // Левый правый случай
        if (balanceFactor > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Правый левый случай
        if (balanceFactor < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node; // Узел уже сбалансирован
    }

    void displayRec(Node* node, int space) {
        if (node == nullptr) {
            return;
        }
        space += 10; // Увеличиваем пространство между узлами
        displayRec(node->right, space);
        std::cout << std::endl;
        std::cout << std::setw(space) << "[" << node->value << " (h:" << node->height << ")]\n"; // Показываем значение и высоту
        displayRec(node->left, space);
    }
};

BinarySearchTree createRandomTree(int numberOfNodes) {
    BinarySearchTree tree;
    std::set<int> generatedValues;

    while (generatedValues.size() < numberOfNodes) {
        int value = std::rand() % 200; // Генерация случайного значения от 0 до 199
        // Проверяем, чтобы значение было уникальным
        if (generatedValues.insert(value).second) {
            tree.insert(value);
        }
    }
    return tree;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Инициализация генератора случайных чисел

    BinarySearchTree A = createRandomTree(5); // Создание дерева A с 10 уникальными узлами
    BinarySearchTree B = createRandomTree(5); // Создание дерева B с 10 уникальными узлами

    std::cout << "Дерево A:" << std::endl;
    A.display();

    std::cout << std::endl << "Дерево B:" << std::endl;
    B.display();

    // Операция A = A Uсим B
    A.unionWithSymmetricTraversal(B);

    std::cout << std::endl << "Результат A = A Uсим B:" << std::endl;
    A.display();

    // Итератор для дерева A
    std::cout << "\nИтератор по дереву A:\n";
    for (int value : A) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}
