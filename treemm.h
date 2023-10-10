#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED
#include <vector>
#include<assert.h>
using namespace std;

template <typename KeyType, typename ValueType>
class TreeMultimap
{
public:
    
    class Iterator
    {
    public:
        Iterator()
        {
            m_isValid = false;
        }

        Iterator(std::vector <ValueType> valueVector) {
            m_isValid = true;
            iteratorVector = valueVector;
            vectorIt = iteratorVector.begin();
        }

        ValueType& get_value() const
        {
            if (m_isValid) {
                return *vectorIt;
            }
            //Assumption-- program crashes if the movie ID is not found in the movie Database
            else {
                assert(0);
            }
        }

        bool is_valid() const
        {
            return m_isValid;  // Replace this line with correct code.
        }

        void advance()
        {
            if (vectorIt != iteratorVector.end()) {
                vectorIt++;
                if (vectorIt == iteratorVector.end()) {
                    m_isValid = false;
                }
            }
        }

    private:
        bool m_isValid;
        //ValueType m_currentValueAddress;
        vector <ValueType> iteratorVector;
        typename vector <ValueType>::iterator vectorIt;
    };
    
    TreeMultimap()
    {
        m_root = nullptr;
    }

    ~TreeMultimap()
    {
        FreeTree(m_root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        if (m_root == nullptr) {
            m_root = new Node(key, value);
            return;
        }
        Node* cur = m_root;
        for (;;) {
            if (key < cur->nodeKey) {
                if (cur->left != nullptr) {
                    cur = cur->left;
                }
                else {
                    cur->left = new Node(key, value);
                    return;
                }
            }
            else if (key > cur->nodeKey) {
                if (cur->right != nullptr) {
                    cur = cur->right;
                }
                else {
                    cur->right = new Node(key, value);
                    return;
                }
            }
            else {
                cur->nodeValue.push_back(value);
                return;
            }
        }

    }

    Iterator find(const KeyType& key) const
    {
        Node* cur = m_root;
        while (cur != nullptr) {
            if (key == cur->nodeKey) {
                Iterator it(cur->nodeValue);
                return it;
            }
            else if (key < cur->nodeKey)
                cur = cur->left;
            else
                cur = cur->right;
        }
        Iterator it;
        return it;  //no match found -- return an invalid iterator
    }

private:
    struct Node {
    public:
        Node(KeyType key, ValueType value) {
            nodeKey = key;
            left = nullptr;
            right = nullptr;
            nodeValue.push_back(value);
        }
        Node* left;
        Node* right;
        KeyType nodeKey;
        std::vector <ValueType> nodeValue;
    };
    Node* m_root;
};

template <typename KeyType, typename ValueType>
void FreeTree(typename TreeMultimap<KeyType, ValueType>::Node* cur)
{
    if (cur == nullptr) {
        return;
    }

    // Recursively delete the left and right subtrees
    FreeTree(cur->left);
    FreeTree(cur->right);

    // Delete the current node
    delete cur;
}
#endif // TREEMULTIMAP_INCLUDED