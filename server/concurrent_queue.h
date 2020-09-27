#ifndef __CIRCULAR_QUEUE_H
#define __CIRCULAR_QUEUE_H

// This concurrent queue is thread safe if ...
// if only one thread calls enque and only one thread calls dequeue.
// This class is useful for one thread giving another thread a list of things to do.
// The idea is that it's a linked list queue with always one empty node in it.
// If front == back then the queue is empty. back always points to an empty node.
// Because the two threads are adding and removing items from two different
// ends of the linked list, it's "thread safe".
template <class T>
class concurrent_queue_node
{
	public:
	T item;
	concurrent_queue_node<T> *next;
};
template <class T>
class concurrent_queue
{
	private:
	concurrent_queue_node<T> *front; 
	concurrent_queue_node<T> *back; // Back always points to an empty node.
	
    public:
	concurrent_queue()
	{
		// There's always one item in the queue.
		// If front == back, then the queue is empty.
		front = new  concurrent_queue_node<T>();
		front->next = nullptr;
		back = front;		
	}
	bool empty()
	{
		return front == back;
	}
	void enqueue(T item)
	{		
		concurrent_queue_node<T> *new_node;
		new_node = new concurrent_queue_node<T>();
		new_node->next = nullptr;
		back->item = item;
		back->next = new_node;
		back = new_node;	
		return;
	}
	bool dequeue(T &item)
	{
		concurrent_queue_node<T> *item_to_delete;
		if (front == back) {
			return false;
		}
		item = front->item;
		item_to_delete = front;
		front = front->next;		
		delete item_to_delete;
		return true;
	}    
	~concurrent_queue()
	{
		while (front != nullptr) {
			back = front;
			front = front->next;
			delete back;
		}		
	}
};
#endif
