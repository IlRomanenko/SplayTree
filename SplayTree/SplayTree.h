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
        to_add = 0;
    }

    int value, top_left, top_right, sum, size, to_add;
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
    return root->sum + root->to_add * root->size;
}

int GetSize(PtrTree root)
{
    if (!IsExist(root))
        return 0;
    return root->size;
}

void CheckBoundValues(PtrTree root)
{
    if (!IsExist(root))
        return;
    if (root->rev)
    {
        if (IsExist(root->left))
            root->top_right = root->left->top_left + root->to_add;
        else
            root->top_right = root->value + root->to_add;

        if (IsExist(root->right))
            root->top_left = root->right->top_right + root->to_add;
        else
            root->top_left = root->value + root->to_add;
    }
    else
    {
        if (IsExist(root->left))
            root->top_left = root->left->top_left + root->to_add;
        else
            root->top_left = root->value + root->to_add;

        if (IsExist(root->right))
            root->top_right = root->right->top_right + root->to_add;
        else
            root->top_right = root->value + root->to_add;
    }
}

void LockNode(PtrTree root)
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

    root->size = 1 + GetSize(root->left) + GetSize(root->right);
    root->sum = root->value + GetSum(root->left) + GetSum(root->right) + root->to_add * root->size;

    if (root->to_add != 0)
    {
        root->value += root->to_add;
        if (IsExist(root->left))
            root->left->to_add += root->to_add;
        if (IsExist(root->right))
            root->right->to_add += root->to_add;
        root->to_add = 0;
    }

    CheckBoundValues(root->left);
    CheckBoundValues(root->right);
    CheckBoundValues(root);

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
    LockNode(root);
    LockNode(child);
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
    LockNode(root);
    LockNode(child);
}

PtrTree Splay(PtrTree vertex)
{
    if (!IsExist(vertex))
        return nullptr;
    if (!IsExist(vertex->parent))
        return vertex;
    PtrTree parent = vertex->parent.lock(), gparent = parent->parent.lock();

    LockNode(gparent);
    LockNode(parent);
    LockNode(vertex);

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
    LockNode(n_root);
    return n_root;
}

PtrTree Find(PtrTree root, int count)
{
    LockNode(root);
    if (!IsExist(root))
        return root;
    if (GetSize(root->left) + 1 == count)
        return root;
    if (GetSize(root->left) + 1 < count)
        return Find(root->right, count - 1 - GetSize(root->left));
    else
        return Find(root->left, count);
}

void ReleaseChild(PtrTree root)
{
    if (!IsExist(root))
        return;
    if (IsExist(root->left))
        root->left->parent = pTree();
    if (IsExist(root->right))
        root->right->parent = pTree();
}

void Split(PtrTree root, PtrTree &left, PtrTree &right, int count)
{
    LockNode(root);
    ReleaseChild(root);
    if (!IsExist(root))
    {
        left = right = nullptr;
        return;
    }
    if (GetSize(root->left) + 1 <= count)
    {
        Split(root->right, root->right, right, count - 1 - GetSize(root->left));
        left = root;
    }
    else
    {
        Split(root->left, left, root->left, count);
        right = root;
    }
    KeepParent(left);
    KeepParent(right);
    LockNode(left);
    LockNode(right);
}

vector<PtrTree> Split(PtrTree root, int l, int r)
{
    PtrTree left, middle, right;

    root = Splay(Find(root, l + 1));

    left = root->left;
    SetParent(left, weak_ptr<SplayTree>());
    root->left = nullptr;
    LockNode(root);

    root = Splay(Find(root, r - l));

    
    right = root->right;
    SetParent(right, weak_ptr<SplayTree>());
    root->right = nullptr;
    LockNode(root);

    middle = root;

    return{ left, middle, right };
}

PtrTree Merge(PtrTree left, PtrTree right)
{
    LockNode(left);
    LockNode(right);
    if (!IsExist(left) || !IsExist(right))
    {
        return (IsExist(left) ? left : right);
    }
    right = Splay(Find(right, 1));
    right->left = left;
    KeepParent(right);
    LockNode(right);
    return right;
}

int CountRevSorted(PtrTree root)
{
    if (!IsExist(root))
        return 0;

    LockNode(root);
    LockNode(root->left);
    LockNode(root->right);

    if (IsRevSorted(root))
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
    LockNode(root);

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
    LockNode(root);
    if (root->is_rev_sorted)
    {
        root->rev ^= true;
        LockNode(root);
        return root;
    }

    int right_rev_sorted = CountRevSorted(root);

    int total_size = GetSize(root);

    PtrTree left_part = Splay(Find(root, total_size - right_rev_sorted));
    PtrTree right_part = left_part->right;
    left_part->right = nullptr;
    right_part->parent = pTree();
    LockNode(left_part);
    LockNode(right_part);


    PtrTree pivot = nullptr;
    if (GetSize(left_part) != 1)
    {
        left_part = Splay(Find(left_part, GetSize(left_part) - 1));

        pivot = left_part->right;
        left_part->right = nullptr;
        pivot->parent = pTree();
        LockNode(pivot);
        LockNode(left_part);

        right_part = Splay(FindLeastBig(right_part, pivot->value));

        pivot->left = right_part->left;
        pivot->right = right_part->right;
        right_part->left = right_part->right = nullptr;
        KeepParent(pivot);

        pivot->rev = true;
        LockNode(pivot);
        LockNode(right_part);

        right_part->right = pivot;
        KeepParent(right_part);
        LockNode(right_part);

        left_part->right = right_part;
        KeepParent(left_part);
        LockNode(left_part);
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
        LockNode(pivot);
        LockNode(right_part);

        right_part->right = pivot;
        KeepParent(right_part);
        LockNode(right_part);

        root = right_part;
    }
    return root;
}

PtrTree SplayNextPermutation(PtrTree root, int l, int r)
{
    auto v_split = Split(root, l, r);

    v_split[1] = GenPermutation(v_split[1]);

    return Merge(Merge(v_split[0], v_split[1]), v_split[2]);
}

void PrintTree(PtrTree root)
{
    LockNode(root);
    if (!IsExist(root))
        return;
    PrintTree(root->left);
    cout << root->value << ' ';
    PrintTree(root->right);
}
