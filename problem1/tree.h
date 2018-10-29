#pragma once

#include <memory>

namespace bintree {
    template <typename T>
    struct TNode : public std::enable_shared_from_this<TNode<T>> {
        using TNodePtr = std::shared_ptr<TNode<T>>;
        using TNodeConstPtr = std::shared_ptr<const TNode<T>>;

        bool hasLeft() const {
            return bool(left);
        }

        bool hasRight() const {
            return bool(right);
        }

        bool hasParent() const {

            return bool(parent.lock());
        }

        T& getValue() {
            return value;
        }

        const T& getValue() const {
            return value;
        }

        TNodePtr getLeft() {
            return left;
        }

        TNodeConstPtr getLeft() const {
            return left;
        }

        TNodePtr getRight() {
            return right;
        }

        TNodeConstPtr getRight() const {
            return right;
        }

        TNodePtr getParent() {
            return parent.lock();  // because parent is weak_ptr
        }

        TNodeConstPtr getParent() const {
            return parent.lock(); // because parent is weak_ptr
        }

        static TNodePtr createLeaf(T v) {
            return TNodePtr(new TNode(v));  // constructor is private, so we can't use make_shared
        }

        static TNodePtr fork(T v, TNodePtr l, TNodePtr r) {
            TNodePtr ptr = TNodePtr(new TNode(v));  // constructor is private, so we can't use make_shared
            ptr->replaceLeft(l);
            ptr->replaceRight(r);
            return ptr;
        }

        TNodePtr replaceLeft(TNodePtr l) {
            setParent(l, this->shared_from_this());  // creation shared ptr from this is bad idea in order memory release twice
            setParent(left, nullptr);
            std::swap(l, left);
            return l;
        }

        TNodePtr replaceRight(TNodePtr r) {
            setParent(r, this->shared_from_this());  // creation shared ptr from this is bad idea in order memory release twice
            setParent(right, nullptr);
            std::swap(r, right);
            return r;
        }

        TNodePtr replaceRightWithLeaf(T v) {
            return replaceRight(createLeaf(v));
        }

        TNodePtr replaceLeftWithLeaf(T v) {
            return replaceLeft(createLeaf(v));
        }

        TNodePtr removeLeft() {
            return replaceLeft(nullptr);
        }
        TNodePtr removeRight() {
            return replaceRight(nullptr);
        }

    private:
        T value;
        TNodePtr left = nullptr;
        TNodePtr right = nullptr;
        std::weak_ptr<TNode> parent = std::weak_ptr<TNode>(); // to avoid cycle link (because it can leads to memory leak)

        TNode(T v)
            : value(v)
        {
        }
        TNode(T v, TNodePtr l, TNodePtr r)  // the same problem memory release twice
            : value(v)
            , left(l)
            , right(r)
        {
        }

        static void setParent(TNodePtr node, TNodePtr parent) {
            if (node)
                node->parent = parent;
        }
    };
}
