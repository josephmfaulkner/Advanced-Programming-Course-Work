// $Id: listmap.tcc,v 1.7 2015-04-28 19:22:02-07 - - $

#include "listmap.h"
#include "trace.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
// DONE
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   TRACE ('l', (void*) this);
   while (anchor_.next != anchor()){
       erase(begin());
       TRACE('r',"ERASING: ");
   }
}


//
// iterator listmap::insert (const value_type&)
// IMPLEMENTED
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   TRACE ('l', &pair << "->" << pair);
   iterator iter;
   for(iter = this->begin();
       (iter != this->end()) and (less(pair.first,iter->first));
       ++iter);
   // Check to see if the key is already there.
   // If it's not less, than the two are equal
   if((iter != this->end())
           and not (less(pair.first,iter->first))
           and not (less(iter->first,pair.first))){
       //Update the key's value
       TRACE('r',"FOUND_KEY: " <<&iter)
       iter->second = pair.second;
       return iter;

   } else {
       //The key wasn't found, so we need to
       //insert a new node at the end of the list.
       TRACE('r',"INSERTING: " << &iter)
       node* new_node = new node(iter.where,iter.where->prev,pair);
       iter.where->prev->next = new_node;
       iter.where->prev = new_node;
       TRACE('r',"FOUND_KEY: " <<&iter)
       return iterator(new_node);

   }

}

//
// listmap::find(const key_type&)
// IMPLEMENTED
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) {
   TRACE ('l', that);
   iterator iter = begin();
   while( ((less(that,iter->first))
           or (less(iter->first,that)))
          and(iter != end())){
       TRACE('r',"LOOKING_IN: "
             <<iter->first <<" :: "
             << iter->second)
       ++iter;
   }
   if((iter != end())
           and not (less(that,iter->first))
           and not (less(iter->first,that)) ){
       return iter;
   } else{
       return end();
   }



   return iterator();
}

//
// iterator listmap::erase (iterator position)
// IMPLEMENTED
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   TRACE ('r', "POSITION :" << &*position);
   TRACE ('r',"next : " << position.where->next);
   TRACE ('r',"prev : " << position.where->prev);
   // prev -> [x] <- next
   position.where->prev->next = position.where->next;
   position.where->next->prev = position.where->prev;

   iterator itor(position.where->next);
   delete position.where;
   return itor;

}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
// DONE
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   TRACE ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
// DONE
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   TRACE ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
// DONE
//

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   TRACE ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
// DONE
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   TRACE ('l', where);
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
// DONE
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
// DONE
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

