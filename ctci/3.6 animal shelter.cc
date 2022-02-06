#include <iostream>
#include <list>
#include <cassert>

struct Pet {
    enum Type {
        Cat = 0,
        Dog
    } type;
    
    uint64_t timestamp;
    
    Pet() {};
    Pet(Pet::Type t, uint64_t arrival_time) : type(t), timestamp(arrival_time) {}
};

struct AnimalShelter {
    void enqueue(Pet::Type t, uint64_t arrival_time)
    {
        struct Pet p(t, arrival_time);
        if (t == Pet::Type::Cat) {
            enqueue(cats, p);
        } else {
            enqueue(dogs, p);
        }
    }
    
    bool dequeueDog(struct Pet& pet)
    {
        return dequeue(dogs, pet);
    }

    bool dequeueCat(struct Pet& pet)
    {
        return dequeue(cats, pet);
    }

    bool dequeue(struct Pet& pet)
    {
        if (dogs.empty())
            return dequeueCat(pet);
        else if (cats.empty())
            return dequeueDog(pet);
        else {
            struct Pet& cat = cats.front();
            struct Pet& dog = dogs.front();
            if (cat.timestamp > dog.timestamp)
                return dequeueDog(pet);
            else
                return dequeueCat(pet);
        }
    }
    
private:

    void enqueue(std::list<Pet>& l, const Pet& p)
    {
        l.emplace_back(p);
    }
    
    bool dequeue(std::list<Pet>& l, Pet& p)
    {
        if (l.empty())
            return false;
        p = l.front();
        l.pop_front();
        return true;
    }
    
    std::list<struct Pet> cats;
    std::list<struct Pet> dogs;
};

int main()
{
    AnimalShelter shelter;
    shelter.enqueue(Pet::Type::Cat, 0);
    shelter.enqueue(Pet::Type::Cat, 1);
    shelter.enqueue(Pet::Type::Dog, 2);
    shelter.enqueue(Pet::Type::Cat, 3);
    
    struct Pet p;
    assert(shelter.dequeue(p) && p.type == Pet::Type::Cat && p.timestamp == 0);
    assert(shelter.dequeueDog(p) && p.type == Pet::Type::Dog && p.timestamp == 2);
    assert(shelter.dequeueCat(p) && p.type == Pet::Type::Cat && p.timestamp == 1);
    assert(shelter.dequeue(p) && p.type == Pet::Type::Cat && p.timestamp == 3);
    assert(!shelter.dequeueCat(p));
    assert(!shelter.dequeueDog(p));
    assert(!shelter.dequeue(p));
    
    return 0;
}
