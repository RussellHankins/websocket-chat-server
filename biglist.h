#ifndef __BIGLIST_H
#define __BIGLIST_H
// A biglist is a linked list of fifty item arrays.
// If you add more than fifty items, another fifty item node is added.
// If you delete an item, the used property is set to false.
// When you iterate through the list using biglist_iterator, it 
// skips the unused items.
// Concurrency problems are unlikely with this design.
template <class T>
class biglist_item
{
	public:
	T item;
	bool used;
};
template <class T>
class biglist
{
	public:
	static const int BIGLIST_SIZE = 50;
	biglist_item<T> items[BIGLIST_SIZE];
	biglist *next;
	biglist_item<T> *operator[](int index)
	{
		biglist *loop;		
		if (index < 0) {
			return nullptr;
		}
		loop = this;
		while (index >= BIGLIST_SIZE) {
			loop = loop->next;
			index -= BIGLIST_SIZE;
			if (loop == nullptr) {
				return nullptr;
			}
		}
		return &loop->items[index];
	}
	biglist()
	{
		int loop;
		next = nullptr;
		for(loop=0;loop<BIGLIST_SIZE;loop++) {
			items[loop].used = false;
		}
	}
	~biglist()
	{
		biglist *loop1 = next;
		biglist *loop2;
		
		while (loop1 != nullptr) {
			loop2 = loop1;
			loop1 = loop1->next;
			loop2->next = nullptr;
			delete loop2;
		}
		next = nullptr;	
	}
	void clear(bool shorten) // If shorten is true, remove extra list items > BIGLIST_SIZE.
	{
		biglist *loop1;
		int loop2;		
		if (shorten) {
			for(loop2=0;loop2<BIGLIST_SIZE;loop2++) {
				items[loop2].used = false;
			}
			while (next != nullptr) {
				loop1 = next;
				next = next->next;
				loop1->next = nullptr;
				delete loop1;
			}
		} else {
			for(loop1 = this;loop1 != nullptr; loop1 = loop1->next) {
				for(loop2=0;loop2<BIGLIST_SIZE;loop2++) {
					loop1->items[loop2].used = false;
				}
			}
		}
	}	
	bool find(T item)
	{
		biglist *loop1 = this;
		int loop2;
		
		while (loop1 != nullptr) {
			for(loop2=0;loop2 < BIGLIST_SIZE;loop2++) {
				if ((items[loop2].used) && (items[loop2].item == item)) {
					return true;
				}
			}
			loop1 = loop1->next;
		}
		return false;
	}
	void add(T item)
	{
		biglist *loop1 = this;
		int loop2;
		biglist *empty1 = nullptr;
		int empty2;
		
		while (loop1 != nullptr) {
			for(loop2=0;loop2 < BIGLIST_SIZE;loop2++) {
				if (items[loop2].used) {
					if (items[loop2].item == item) {
						return;
					}
				} else {
					if (empty1 == nullptr) {
						empty1 = loop1;
						empty2 = loop2;
					}
				}				
			}
			loop1 = loop1->next;
		}
		
		while (empty1 != nullptr) {
			if (empty1->items[empty2].used) {
				if (++empty2 >= biglist<T>::BIGLIST_SIZE) {
					empty2 = 0;
					empty1 = empty1->next;
				}					
			} else {
				empty1->items[empty2].used = true;
				empty1->items[empty2].item = item;
				return;
			}
		}
		empty1 = new biglist<T>();
		empty1->items[0].used = true;
		empty1->items[0].item = item;
		empty1->next = next;
		next = empty1;
		return;
	}
	void remove(T item)
	{
		biglist *loop1 = this;
		int loop2;		
		while (loop1 != nullptr) {
			for(loop2=0;loop2 < BIGLIST_SIZE;loop2++) {
				if ((items[loop2].used) && (items[loop2].item == item)) {					
					items[loop2].used = false;
					break;
				}
			}
			loop1 = loop1->next;
		}
		return;
	}
	int length()
	{
		biglist *loop1 = this;
		int loop2;
		int total = 0;
		while (loop1 != nullptr) {
			for(loop2=0;loop2 < BIGLIST_SIZE;loop2++) {
				if (items[loop2].used) {
					total++;
				}
			}
			loop1 = loop1->next;
		}
		return total;
	}	
};
template <class T>
class biglist_iterator
{
	private:
	biglist<T> *_list;
	biglist<T> *_current;	
	int _currentpos;
	public:
	biglist_item<T> *row;	
	T item;
	void movenext()
	{
		do
		{
			if (_current == nullptr) {
				row = nullptr;
				return;
			}
			if (++_currentpos >= biglist<T>::BIGLIST_SIZE) {
				_currentpos = 0;
				_current = _current->next;
				if (_current == nullptr) {
					row = nullptr;
					return;
				}
			}
		} while (!_current->items[_currentpos].used);
		row = _current->items+_currentpos;
		item = row->item;
		return;
	}
	void movefirst()
	{
		row = nullptr;
		_current = _list;
		_currentpos = -1;
		movenext();
	}	
	biglist_iterator(biglist<T> *list)
	{
		_list = list;
		item = nullptr;
		movefirst();
	}	
	bool eof()
	{
		return _current == nullptr;
	}
};
#endif
