//==================================================
// C++ Atomic types explained
// A lock-free single-producer single-consumer queue
//==================================================
#include <iostream>
#include <memory>
#include <atomic>
#include <thread>

template<typename T>
class Lock_free_Queue
{
private:
    struct Node
    {
        std::shared_ptr<T> my_data;
        Node* my_next_node;
        Node() : my_next_node(nullptr)
        {}
    };
    
    std::atomic<Node*> my_head_node;
    std::atomic<Node*> my_tail_node;
    
    Node* pop_head_node()
    {
        Node* const old_head_node = my_head_node.load();
        if(old_head_node == my_tail_node.load())
        {
            return nullptr;
        }
        my_head_node.store(old_head_node->my_next_node);
        return old_head_node;
    }
    
public:
    Lock_free_Queue() : my_head_node(new Node), my_tail_node(my_head_node.load())
    {}
    Lock_free_Queue(const Lock_free_Queue& other) = delete;
    Lock_free_Queue& operator= (const Lock_free_Queue& other) = delete;
    
    ~Lock_free_Queue()
    {
        while(Node* const old_head_node = my_head_node.load())
        {
            my_head_node.store(old_head_node->my_next_node);
            delete old_head_node;
        }
    }
    
    std::shared_ptr<T> dequeue()
    {
        Node* old_head_node = pop_head_node();
        if(!old_head_node)
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const result(old_head_node->my_data);
        delete old_head_node;
        return result;
    }
    
    void enqueue(T new_value)
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(new_value));
        Node* p = new Node;
        Node* const old_tail_node = my_tail_node.load();
        old_tail_node->my_data.swap(new_data);
        old_tail_node->my_next_node = p;
        my_tail_node.store(p);
    }
};

void push(Lock_free_Queue<int>* q)
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "pushing " << i << std::endl;
        q->enqueue(i);
    }
}

void pop(Lock_free_Queue<int>* q)
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "poping " << *q->dequeue() << std::endl;
    }
}

int main()
{
    Lock_free_Queue<int> q;
    std::thread t1(push,&q);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::thread t2(pop,&q);
    t1.join();
    t2.join();
}

