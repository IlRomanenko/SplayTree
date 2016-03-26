#pragma once
#include "base.h"
#include "IShuffler.h"

class Shuffler : IShuffler
{

    //Internal tree's representation
    struct SplayTreeNode
    {
        long long top_left, top_right;
        size_t size;
        long long to_add, value, sum;
        bool rev, is_sorted, is_rev_sorted;

        shared_ptr<SplayTreeNode> left, right;
        weak_ptr<SplayTreeNode> parent;

        SplayTreeNode() = delete;

        SplayTreeNode(long long n_value)
        {
            top_left = top_right = value = sum = n_value;
            rev = false;
            size = 1;
            to_add = 0;
            is_sorted = is_rev_sorted = true;
            left = right = nullptr;
            parent = weak_ptr<SplayTreeNode>();
        }
    };

    typedef weak_ptr<SplayTreeNode> pTree;
    typedef shared_ptr<SplayTreeNode> PtrTree;
    

    //Check, that someone has strong link to root
    //Similar to compare with nullptr
    bool IsExist(const pTree& root) const
    {
        return !root.expired();
    }

    //Return size of tree
    size_t GetSize(const PtrTree& root) const
    {
        if (!IsExist(root))
            return 0;
        return root->size;
    }

    //Return sum of tree
    long long GetSum(const PtrTree& root) const
    {
        if (!IsExist(root))
            return 0;
        return root->sum;
    }

    bool IsSorted(const PtrTree& root) const
    {
        if (!IsExist(root))
            return true;
        return root->is_sorted;
    }

    bool IsRevSorted(const PtrTree& root) const
    {
        if (!IsExist(root))
            return true;
        return root->is_rev_sorted;
    }


    //Update size, sum and top left, top right value for node
    //Also check sorting of node
    void UpdateValues(PtrTree& root)
    {
        if (!IsExist(root))
            return;
        root->size = 1 + GetSize(root->left) + GetSize(root->right);
        root->sum = root->value + GetSum(root->left) + GetSum(root->right) + root->size * root->to_add;

        {
            if (IsExist(root->left))
                root->top_left = root->left->top_left + root->to_add;
            else
                root->top_left = root->value + root->to_add;

            if (IsExist(root->right))
                root->top_right = root->right->top_right + root->to_add;
            else
                root->top_right = root->value + root->to_add;
            if (root->rev)
                swap(root->top_left, root->top_right);
        }

        if (IsSorted(root->left) && IsSorted(root->right) &&
            (!IsExist(root->left) || IsExist(root->left) && root->left->top_right <= root->value) &&
            (!IsExist(root->right) || IsExist(root->right) && root->value <= root->right->top_left))
        {
            if (root->rev)
                root->is_rev_sorted = true;
            else
                root->is_sorted = true;
        }
        else
            root->is_sorted = false;

        if (IsRevSorted(root->left) && IsRevSorted(root->right) &&
            (!IsExist(root->left) || IsExist(root->left) && root->left->top_right >= root->value) &&
            (!IsExist(root->right) || IsExist(root->right) && root->value >= root->right->top_left))
        {
            if (root->rev)
                root->is_sorted = true;
            else
                root->is_rev_sorted = true;
        }
        else
            root->is_rev_sorted = false;
    }

    //Push reverse flag and additional sum for children 
    //Also swap children if reverse is true
    void Push(PtrTree& root)
    {
        if (!IsExist(root))
            return;
        if (root->rev)
        {
            swap(root->left, root->right);
            if (IsExist(root->left))
            {
                root->left->rev ^= true;
                if (root->left->is_sorted)
                {
                    root->left->is_rev_sorted = true;
                    if (root->left->size != 1)
                        root->left->is_sorted = false;
                }
                if (root->left->is_rev_sorted)
                {
                    root->left->is_sorted = true;
                    if (root->left->size != 1)
                        root->left->is_rev_sorted = false;
                }
            }
            if (IsExist(root->right))
            {
                root->right->rev ^= true;
                if (root->right->is_sorted)
                {
                    root->right->is_rev_sorted = true;
                    if (root->right->size != 1)
                        root->right->is_sorted = false;
                }
                if (root->right->is_rev_sorted)
                {
                    root->right->is_sorted = true;
                    if (root->right->size != 1)
                        root->right->is_rev_sorted = false;
                }
            }
            root->rev ^= true;
        }

        if (root->to_add != 0)
        {
            if (IsExist(root->left))
                root->left->to_add += root->to_add;
            if (IsExist(root->right))
                root->right->to_add += root->to_add;
            root->value += root->to_add;
            root->to_add = 0;
        }
        UpdateValues(root->left);
        UpdateValues(root->right);
    }

    //Execute Push and UpdateValue, so after this function
    //node is already up to date
    void Lock(PtrTree& root)
    {
        if (!IsExist(root))
            return;
        Push(root);
        UpdateValues(root);
    }

    //Set children parent's itself 
    void KeepParent(PtrTree& root)
    {
        if (!IsExist(root))
            return;
        if (IsExist(root->left))
            root->left->parent = root;
        if (IsExist(root->right))
            root->right->parent = root;
    }

    //Set, that children of node doesn't have parent
    void ReleaseChild(PtrTree& root)
    {
        if (!IsExist(root))
            return;
        if (IsExist(root->left))
            root->left->parent = pTree();
        if (IsExist(root->right))
            root->right->parent = pTree();
    }

    //Set, that node doesn't have parent
    void ReleaseParent(PtrTree root)
    {
        if (!IsExist(root))
            return;
        root->parent = pTree();
    }

    //Split in two parts. Size of left part <= count
    void Split(PtrTree root, PtrTree &Left, PtrTree &Right, size_t count)
    {
        Lock(root);
        ReleaseChild(root);

        if (!IsExist(root))
        {
            Left = Right = nullptr;
            return;
        }
        if (GetSize(root->left) + 1 <= count)
        {
            Split(root->right, root->right, Right, count - GetSize(root->left) - 1);
            Left = root;
        }
        else
        {
            Split(root->left, Left, root->left, count);
            Right = root;
        }
        ReleaseParent(Left);
        ReleaseParent(Right);
        KeepParent(Left);
        KeepParent(Right);
        Lock(Left);
        Lock(Right);
    }

    //Rotate left through node
    void LeftRotation(PtrTree& node)
    {
        pTree parent = node->parent;
        PtrTree child = node->right;

        if (IsExist(parent))
        {
            PtrTree strong_parent = parent.lock();
            if (node == strong_parent->left)
                strong_parent->left = child;
            else if (node == strong_parent->right)
                strong_parent->right = child;
            else
                assert(false); //check for undefined behaviour
        }

        child->parent = parent;
        node->right = child->left;
        child->left = node;
        KeepParent(node);
        KeepParent(child);
        Lock(node);
        Lock(child);
    }

    //Rotate right through node
    void RightRotation(PtrTree& node)
    {
        pTree parent = node->parent;
        PtrTree child = node->left;

        if (IsExist(parent))
        {
            PtrTree strong_parent = parent.lock();
            if (node == strong_parent->left)
                strong_parent->left = child;
            else if (node == strong_parent->right)
                strong_parent->right = child;
            else
                assert(false); //check for undefined behaviour
        }

        child->parent = parent;
        node->left = child->right;
        child->right = node;
        KeepParent(node);
        KeepParent(child);
        Lock(node);
        Lock(child);
    }

    //Pull node up
    PtrTree Splay(PtrTree& root)
    {
        if (!IsExist(root))
            return nullptr;

        if (!IsExist(root->parent))
            return root;

        PtrTree parent = root->parent.lock(), gparent = parent->parent.lock();

        Lock(gparent);
        Lock(parent);
        Lock(root);

        if (!IsExist(gparent))
        {
            if (root == parent->left)
                RightRotation(parent);
            else if (root == parent->right)
                LeftRotation(parent);
            else
                assert(false);
            Lock(root);
            return root;
        }
        else
        {
            if ((gparent->left == parent) && (parent->left == root)) // zig - zig
            {
                RightRotation(gparent);
                RightRotation(parent);
            }
            else if ((gparent->left == parent) && (parent->right == root)) // zig - zag
            {
                LeftRotation(parent);
                RightRotation(gparent);
            }
            else if ((gparent->right == parent) && (parent->left == root)) // zag - zig
            {
                RightRotation(parent);
                LeftRotation(gparent);
            }
            else if ((gparent->right == parent) && (parent->right == root)) // zag- zag
            {
                LeftRotation(gparent);
                LeftRotation(parent);
            }
            else
                assert(false); //check for undefined behaviour

            return Splay(root);
        }
    }

    //Find root, in which left part size plus root == count
    //Using with count >= 1
    PtrTree Find(PtrTree& root, size_t count)
    {
        Lock(root);

        if (!IsExist(root))
            return nullptr;
        if (GetSize(root->left) + 1 == count)
            return root;
        else if (GetSize(root->left) + 1 > count)
        {
            return Find(root->left, count);
        }
        else
            return Find(root->right, count - GetSize(root->left) - 1);
    }

    //Split in three part. Size of first part is l - 1
    //Size of middle part is r - l
    //Using with [l, r] where l >= 1
    vector<PtrTree> Split3(PtrTree& root, int l, int r)
    {
        PtrTree left, middle, right;
        Split(root, left, right, l - 1);
        Lock(left);
        Lock(right);
        Split(right, middle, right, r - l + 1);
        Lock(middle);
        Lock(right);
        return{ left, middle, right };
    }

    //Merge two parts, result is a new root
    PtrTree Merge(PtrTree& Left, PtrTree& Right)
    {
        Lock(Left);
        Lock(Right);
        if (!IsExist(Left) || !IsExist(Right))
        {
            return (IsExist(Left) ? Left : Right);
        }
        Right = Splay(Find(Right, 1));

        Right->left = Left;
        KeepParent(Right);
        Lock(Right);
        return Right;
    }

    //Count the biggest reverse sorted suffix
    size_t CountRevSorted(PtrTree& root)
    {
        Lock(root);
        Lock(root->left);
        Lock(root->right);

        if (!IsExist(root))
            return 0;
        if (IsRevSorted(root))
            return GetSize(root);
        int result = 0;

        if (IsExist(root->right))
        {
            if (IsRevSorted(root->right))
            {
                result = GetSize(root->right);
                if (root->value >= root->right->top_left)
                {
                    result++;
                    if (IsExist(root->left) && root->left->top_right >= root->value)
                        result += CountRevSorted(root->left);
                }
            }
            else
                result = CountRevSorted(root->right);
        }
        else
        {
            result = 1;
            if (IsExist(root->left) && root->left->top_right >= root->value)
                result += CountRevSorted(root->left);
        }
        return result;
    }

    //Find the leftest node from suffix, that doesn't less than pivot
    PtrTree FindLeastBig(PtrTree& root, long long pivot)
    {
        Lock(root);

        if (!IsExist(root))
            return nullptr;

        PtrTree result = nullptr;
        if (root->value > pivot)
        {
            result = FindLeastBig(root->right, pivot);
            if (!IsExist(result))
                result = root;
        }
        else
            result = FindLeastBig(root->left, pivot);
        return result;
    }

    //Generate a new permutation for all tree
    PtrTree GenPermutation(PtrTree& root)
    {
        Lock(root);

        if (IsRevSorted(root))
        {
            root->rev ^= true;
            Lock(root);
            return root;
        }

        int total_size = GetSize(root);

        int total_rev_sorted_size = CountRevSorted(root);

        PtrTree left, pivot, right;

        auto v_split = Split3(root, total_size - total_rev_sorted_size, total_size - total_rev_sorted_size);
        left = v_split[0];
        pivot = v_split[1];
        right = v_split[2];
        Lock(left);
        Lock(pivot);
        Lock(right);

        assert(IsRevSorted(right));
        PtrTree n_element = Splay(FindLeastBig(right, pivot->value));
        Lock(n_element);
        swap(pivot->value, n_element->value);
        n_element->rev ^= true;
        Lock(n_element);
        pivot->right = n_element;
        Lock(pivot);
        root = Merge(left, pivot);
        return root;
    }

    //Generate permutation for subsequence from l to r
    //Using with [l, r] where l >= 1 and r >= l
    PtrTree SplayNextPermutation(PtrTree& root, int l, int r)
    {
        auto v_split = Split3(root, l, r);
        v_split[1] = GenPermutation(v_split[1]);

        return Merge(Merge(v_split[0], v_split[1]), v_split[2]);
    }

    //Print all tree
    void PrintTree(PtrTree& root)
    {
        if (!IsExist(root))
            return;

        Lock(root);
        PrintTree(root->left);
        cout << root->value << ' ';
        PrintTree(root->right);
    }

public:

    Shuffler()
    {
        shf_root = nullptr;
    }

    void Print() override
    {
        PrintTree(shf_root);
    }

    long long GetSum(int l, int r) override
    {
        PtrTree left, right, middle;
        auto v_split = Split3(shf_root, l + 1, r + 1);
        left = v_split[0];
        middle = v_split[1];
        right = v_split[2];

        long long result = GetSum(middle);

        shf_root = Merge(Merge(left, middle), right);
        Lock(shf_root);

        return result;
    }

    size_t Size() const override
    {
        return GetSize(shf_root);
    }

    void Insert(int position, long long value) override
    {
        PtrTree left, right;
        Split(shf_root, left, right, position);
        shf_root = Merge(Merge(left, PtrTree(new SplayTreeNode(value))), right);
        Lock(shf_root);
    }

    void Replace(int position, long long value) override
    {
        shf_root = Splay(Find(shf_root, position + 1));
        shf_root->value = value;
        Lock(shf_root);
    }

    void AddValue(int l, int r, long long value) override
    {
        auto v_split = Split3(shf_root, l + 1, r + 1);
        v_split[1]->to_add += value;
        Lock(v_split[1]);
        shf_root = Merge(Merge(v_split[0], v_split[1]), v_split[2]);
        Lock(shf_root);
    }

    bool CanPermutate() const override
    {
        return !shf_root->is_rev_sorted;
    }

    void NextPermutation(int l, int r) override
    {
        shf_root = SplayNextPermutation(shf_root, l + 1, r + 1);
        Lock(shf_root);
    }

private:
    PtrTree shf_root;
};