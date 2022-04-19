// Project Identifier: 2C4A3C53CD5AD45A7BEA3AE5356A3B1622C9D04B
#ifndef HASHTABLE_H
#define HASHTABLE_H

// names / uniquenames
// Calvin Tran (cktran)
// TODO

// INSTRUCTIONS:
// fill out the methods in the class below.

// You may assume that the key and value types can be copied and have default
// constructors.

// You can also assume that the key type has (==) and (!=) methods.

// You may assume that Hasher is a functor type with a
// size_t operator()(const Key&) overload.

// The key/value types aren't guaranteed to support any other operations.

// Do not add, remove, or change any of the class's member variables.
// The num_deleted counter is *optional*. You might find it helpful, but it
// is not required to pass the lab assignment.

// Do not change the Bucket type.

// SUBMISSION INSTRUCTIONS:
// Submit this file, by itself, in a .tar.gz.
// Other files will be ignored.

#include <cstdint>
#include <functional> // where std::hash lives
#include <vector>
#include <cassert> // useful for debugging!

// A bucket's status tells you whether it's empty, occupied, 
// or contains a deleted element.
enum class Status : uint8_t {
    Empty,
    Occupied,
    Deleted
};

template<typename K, typename V, typename Hasher = std::hash<K>>
class HashTable {
    // used by the autograder; do not change/remove.
    friend class Verifier;
public:
    // A bucket has a status, a key, and a value.
    struct Bucket {
        // Do not modify Bucket.
        Status status = Status::Empty;
        K key;
        V val;
    };

    HashTable() {
        // TODO: a default constructor (possibly empty).

        // You can use the following to avoid needing to implement rehashing.
        // However, you will only pass the AG test cases ending in _C.
        // To pass the rest, start with at most 20 buckets and implement rehashing.
        buckets.resize(20);
    }

    size_t size() const {
        return num_elements;
    }

    // returns a reference to the value in the bucket with the key, if it
    // already exists. Otherwise, insert it with a default value, and return
    // a reference to the resulting bucket.
    V& operator[](const K& key) {
        size_t i = hasher(key);
        if (buckets[i].status == Status::Occupied && buckets[i].key == key) {        // If key is at H(k)...
            return buckets[i].val;
        } 
        else {                  // First spot was deleted, occupied (w/ diff key), or empty
            static V val;
            insert(key, val);   // Inserts key (in appropriate spot) if it did not exist
                                // else nothing happens

            i = hasher(key);    // Rehash if needed
            
            while (buckets[i].status != Status::Empty) { // Find correct bucket
                if (buckets[i].status == Status::Occupied && buckets[i].key == key) {
                    return buckets[i].val;
                }
                ++i;
                i %= buckets.size();
            }

            return buckets[i].val;
        }
    }

    // insert returns whether inserted successfully
    // (if the key already exists in the table, do nothing and return false).
    bool insert(const K& key, const V& val) {
        size_t i = hasher(key);
        if (num_deleted < buckets.size()) {
            size_t j = i;
            
            // All buckets are never full
            do {   // Search if key already exists or until first empty spot
                if (buckets[j].status == Status::Occupied && buckets[j].key == key)
                    return false;
                else if (buckets[j].status == Status::Empty)
                    break;
                ++j;
                j %= buckets.size();
            } while (j != i);

            // Key does not already exist

            if (buckets[i].status != Status::Occupied) {   // If first spot was not occupied...
                if (buckets[i].status == Status::Deleted)
                    --num_deleted;
                buckets[i].status = Status::Occupied;
                buckets[i].key = key;
                buckets[i].val = val;
                ++num_elements;
                if ((double)num_elements / (double)buckets.size() > 0.5) {  // Load factor > 0.5
                    rehash_and_grow();
                }
                return true;
            }
            else {          // If first spot was occupied...
                j = i;
                while (buckets[j].status == Status::Occupied) {     // Find first unoccupied spot
                    ++j;
                    j %= buckets.size();
                }
                if (buckets[j].status == Status::Deleted)
                    --num_deleted;
                buckets[j].status = Status::Occupied;
                buckets[j].key = key;
                buckets[j].val = val;
                ++num_elements;
                if ((double)num_elements / (double)buckets.size() > 0.5) {  // Load factor > 0.5
                    rehash_and_grow();
                }
                return true;
            }
        }
        else {
            if (buckets[i].status == Status::Deleted)
                --num_deleted;
            buckets[i].status = Status::Occupied;
            buckets[i].key = key;
            buckets[i].val = val;
            ++num_elements;
            return true;
        }
    }

    // erase returns the number of items remove (0 or 1)
    size_t erase(const K& key) {
        // TODO
        size_t i = hasher(key);
        size_t j = i;
        //std::cout << "\nH(" << key << ") = " << i << "\n";
        if (num_elements > 0) {
            do {        // Search until key is found or until first empty spot
                if (buckets[j].status == Status::Occupied && buckets[j].key == key) {    // If found...
                    buckets[j].status = Status::Deleted;
                    ++num_deleted;
                    --num_elements;
                    return 1;
                }
                else if (buckets[j].status == Status::Empty) {
                    return 0;
                }
                ++j;
                j %= buckets.size();
            } while (j != i);

            // Key was not found
            return 0;
        }           // Everything was deleted
        else
            return 0;
    }

private:
    size_t num_elements = 0;
    size_t num_deleted = 0; // OPTIONAL: you don't need to use this to pass
    std::vector<Bucket> buckets;

    void rehash_and_grow() {
        size_t SIZE = buckets.size();
        buckets.resize(SIZE * 2);               // Double # of buckets
        std::vector<Bucket> new_buckets;
        new_buckets.resize(SIZE * 2);
        if (num_elements > 0) {                 // If there are elements to rehash...
            for (size_t i = 0; i < SIZE; ++i) {
                if (buckets[i].status == Status::Occupied) {
                    size_t j = hasher(buckets[i].key);                  // Rehash
                    while (new_buckets[j].status == Status::Occupied) { // Linear probe to find next empty bucket
                        ++j;
                        j %= new_buckets.size();
                    }
                    new_buckets[j] = buckets[i];

                }
            }
        }

        buckets = new_buckets;
        num_deleted = 0;
    }

    // You can add methods here if you like.
    // TODO
    size_t hasher(const K& key) {
        Hasher hasher;
        size_t i = hasher(key) % buckets.size();
        return i;
    }

};

#endif // HASHTABLE_H