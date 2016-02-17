#pragma once
#include "base.h"

struct SplayTree
{
    shared_ptr<SplayTree> left, right;
    weak_ptr<SplayTree> parent;
    
    SplayTree() = delete;

    SplayTree(int n_value)
    {
        left.reset();
        right.reset();
        parent.reset();
        value = top_left = top_right = sum = n_value;
        size = 1;
        rev = false;
        is_sorted = is_rev_sorted = true;
    }

    int value, top_left, top_right, sum, size;
    bool rev, is_sorted, is_rev_sorted;

};

typedef weak_ptr<SplayTree> pTree;
typedef shared_ptr<SplayTree> PtrTree;

bool IsExist(pTree root)
{
    return !root.expired();
}

void SetParent(PtrTree child, pTree root)
{
    if (IsExist(child))
        child->parent = root;
}

bool Rev(PtrTree root)
{
    if (!IsExist(root))
        return false;
    return root->rev;
}

bool IsSorted(PtrTree root)
{
    if (!IsExist(root))
        return true;
    return root->is_sorted;
}

bool IsRevSorted(PtrTree root)
{
    if (!IsExist(root))
        return true;
    return root->is_rev_sorted;
}

int GetSum(PtrTree root)
{
    if (!IsExist(root))
        return 0;
    return root->sum;
}

int GetSize(PtrTree root)
{
    if (!IsExist(root))
        return 0;
    return root->size;
}

void Push(PtrTree root)
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
                root->left->is_sorted = false;
            }
            if (root->left->is_rev_sorted)
            {
                root->left->is_sorted = true;
                root->left->is_rev_sorted = false;
            }
        }
        if (IsExist(root->right))
        {
            root->right->rev ^= true;
            if (root->right->is_sorted)
            {
                root->right->is_rev_sorted = true;
                root->right->is_sorted = false;
            }
            if (root->right->is_rev_sorted)
            {
                root->right->is_sorted = true;
                root->right->is_rev_sorted = false;
            }
        }
        root->rev = false;
    }
}

void Update(PtrTree root)
{
    if (!IsExist(root))
        return;

    if (IsExist(root->left))
        root->top_left = root->left->top_left;
    else 
        root->top_left = root->value;

    if (IsExist(root->right))
        root->top_right = root->right->top_right;
    else
        root->top_right = root->value;

    root->sum = root->value + GetSum(root->left) + GetSum(root->right);
    root->size = 1 + GetSize(root->left) + GetSize(root->right);

    if (IsSorted(root->left) && IsSorted(root->right) &&
        (!IsExist(root->left) || IsExist(root->left) && root->left->top_right <= root->value) &&
        (!IsExist(root->right) || IsExist(root->right) && root->value <= root->right->top_left))
    {
        root->is_sorted = true;
    }
    else
        root->is_sorted = false;

    if (IsRevSorted(root->left) && IsRevSorted(root->right) &&
        (!IsExist(root->left) || IsExist(root->left) && root->left->top_right >= root->value) &&
        (!IsExist(root->right) || IsExist(root->right) && root->value >= root->right->top_left))
    {
        root->is_rev_sorted = true;
    }
    else
        root->is_rev_sorted = false;

}

void KeepParent(PtrTree root)
{
    if (!IsExist(root))
        return;
    if (IsExist(root->left))
        root->left->parent = root;
    if (IsExist(root->right))
        root->right->parent = root;
}

void RightRotation(PtrTree root)
{
    pTree weak_gparent = root->parent;
    PtrTree child = root->left;

    if (IsExist(weak_gparent))
    {
        PtrTree gparent = weak_gparent.lock();
        if (gparent->left == root)
            gparent->left = child;
        else if (gparent->right == root)
            gparent->right = child;
        else
            assert(false);
    }


    SetParent(child, root->parent);
    root->left = child->right;
    child->right = root;
    SetParent(root, child);
    KeepParent(child);
    KeepParent(root);
    Update(root);
    Update(child);
}

void LeftRotation(PtrTree root)
{
    pTree weak_gparent = root->parent;
    PtrTree child = root->right;

    if (IsExist(weak_gparent))
    {
        PtrTree gparent = weak_gparent.lock();
        if (gparent->left == root)
            gparent->left = child;
        else if (gparent->right == root)
            gparent->right = child;
        else
            assert(false);
    }

    SetParent(child, root->parent);
    root->right = child->left;
    child->left = root;
    SetParent(root, child);
    KeepParent(child);
    KeepParent(root);
    Update(root);
    Update(child);
}

PtrTree Splay(PtrTree vertex)
{
    if (!IsExist(vertex->parent))
        return vertex;
    PtrTree parent = vertex->parent.lock(), gparent = parent->parent.lock();

    Push(gparent);
    Push(parent);
    Push(vertex);

    if (!IsExist(gparent))
    {
        if (parent->left == vertex) // zig
            RightRotation(parent);
        else                        // zag
            LeftRotation(parent);
        return vertex;
    }
    else
    {
        if ((gparent->left == parent) && (parent->left == vertex))          //zig-zig
        {
            RightRotation(gparent);
            RightRotation(parent);
        }
        else if ((gparent->right == parent) && (parent->right == vertex))   //zag-zag
        {
            LeftRotation(gparent);
            LeftRotation(parent);
        }
        else if ((gparent->left == parent) && (parent->right == vertex))    //zig-zag
        {
            LeftRotation(parent);
            RightRotation(gparent);
        }
        else                                                                //zag-zig
        {
            RightRotation(parent);
            LeftRotation(gparent);
        }
        return Splay(vertex);
    }
}

PtrTree Add(PtrTree root, int key)
{
    PtrTree n_root = shared_ptr<SplayTree>(new SplayTree(key));
    n_root->left = root;
    SetParent(root, n_root);
    Update(n_root);
    return n_root;
}

PtrTree Find(PtrTree root, int count)
{
    Push(root);
    if (!IsExist(root))
        return root;
    if (GetSize(root->left) + 1 == count)
        return root;
    if (GetSize(root->left) + 1 < count)
        return Find(root->right, count - 1 - GetSize(root->left));
    else
        return Find(root->left, count);
}

int CountRevSorted(PtrTree root)
{
    Push(root);
    Push(root->left);
    Push(root->right);

    if (!IsExist(root))
        return 0;
    if (root->is_rev_sorted)
        return root->size;
    int result = 0;
    if (IsRevSorted(root->right))
    {
        result += GetSize(root->right);
        if (IsExist(root->right))
        {
            if (root->right->top_left <= root->value)
            {
                result++;
                if (IsExist(root->left) && root->left->top_right >= root->value)
                    result += CountRevSorted(root->left);
            }
        }
        else
        {
            if (IsExist(root->left) && root->left->top_right >= root->value)
                result += CountRevSorted(root->left) + 1;
            else
                result = 1;
        }
    }
    else
        result = CountRevSorted(root->right);
    return result;
}

PtrTree FindLeastBig(PtrTree root, int value)
{
    if (!IsExist(root))
        return nullptr;

    PtrTree result = nullptr;
    if (root->value >= value)
    {
        result = FindLeastBig(root->right, value);
        if (!IsExist(result))
            result = root;
    }
    else
        result = FindLeastBig(root->left, value);
    return result;
}

PtrTree GenPermutation(PtrTree root)
{
    Push(root);
    if (root->is_rev_sorted)
    {
        root->rev ^= true;
        return root;
    }
    
    int right_rev_sorted = CountRevSorted(root);

    int total_size = GetSize(root);

    PtrTree left_part = Splay(Find(root, total_size - right_rev_sorted));
    PtrTree right_part = left_part->right;
    left_part->right = nullptr;
    right_part->parent.reset();
    Update(left_part);
    Update(right_part);


    PtrTree pivot = nullptr;
    if (GetSize(left_part) != 1)
    {
        left_part = Splay(Find(left_part, GetSize(left_part) - 1));

        pivot = left_part->right;
        left_part->right = nullptr;
        pivot->parent.reset();
        Update(pivot);
        Update(left_part);

        right_part = Splay(FindLeastBig(right_part, pivot->value));

        pivot->left = right_part->left;
        pivot->right = right_part->right;
        right_part->left = right_part->right = nullptr;
        KeepParent(pivot);

        pivot->rev = true;
        Push(pivot);
        Update(right_part);
        Update(pivot);

        right_part->right = pivot;
        KeepParent(right_part);
        Update(right_part);

        left_part->right = right_part;
        KeepParent(left_part);
        Update(left_part);
        root = left_part;
    }
    else
    {
        pivot = left_part;

        right_part = Splay(FindLeastBig(right_part, pivot->value));

        pivot->left = right_part->left;
        pivot->right = right_part->right;
        right_part->left = right_part->right = nullptr;
        KeepParent(pivot);

        pivot->rev = true;
        Push(pivot);
        Update(right_part);
        Update(pivot);

        right_part->right = pivot;
        KeepParent(right_part);
        Update(right_part);

        root = right_part;
    }
    return root;
}

PtrTree NextPermutation(PtrTree root, int l, int r)
{
    PtrTree left, middle, right;
    root = Splay(Find(root, l + 1));
    
    left = root->left;
    SetParent(left, weak_ptr<SplayTree>());
    root->left = nullptr;

    root = Splay(Find(root, r - l));

    right = root->right;
    SetParent(right, weak_ptr<SplayTree>());
    root->right = nullptr;

    middle = GenPermutation(root);

    if (IsExist(left))
        left->right = middle;
    else
        left = middle;
    KeepParent(left);

    if (IsExist(right))
        right->left = left;
    else
        right = left;
    KeepParent(right);
    return right;
}

void PrintTree(PtrTree root)
{
    if (!IsExist(root))
        return;
    if (!root->rev)
    {
        PrintTree(root->left);
        cout << root->value << ' ';
        PrintTree(root->right);
    }
    else
    {
        PrintTree(root->right);
        cout << root->value << ' ';
        PrintTree(root->left);
    }
}