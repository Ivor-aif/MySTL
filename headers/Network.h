//
// Created by Ivor_Aif on 2026/2/9.
//

#ifndef NETWORK_H
#define NETWORK_H

#include "Array.h"

namespace mySTL::containers {

template<typename T>
class Network {
public:
    struct Site {
    private:
        size_t id;
        T value;
        Array<Site*> neighbors;
        Network* owner;

        Site(Network* owner, const int id, const T& value) : id(id), value(value), owner(owner) {

        }

        friend class Network;

    public:
        const T& getValue() const {
            return value;
        }

        const Array<Site*>& getNeighbors() const {
            return neighbors;
        }
    };

private:
    size_t nId = 0;
    Array<Site*> sites;

    void dump(std::ostream& os) const {
        for (const Site* st : sites) {
            os << "Site " << st->id << ": ";
            for (const Site* nb : st->neighbors) {
                os << nb->id << " ";
            }
            os << '\n';
        }
    }

    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Network<U>& network);

public:
    Network() = default;
    ~Network() {
        for (const Site* site : sites) {
            delete site;
        }
    }

    Network(const Network&) = delete;
    Network& operator=(const Network&) = delete;

    Network(Network&&) = delete; // TODO: deep move?
    Network& operator=(Network&&) = delete;

    Site* create(const T& value) {
        Site* site = new Site(this, nId++, value);
        sites.pushBack(site);
        return site;
    }

    void remove(Site* site) {
        assert(site->owner == this);
        for (size_t i = 0; i < sites.getSize(); i++) {
            if (sites[i] == site) {
                continue;
            }
            size_t index = -1;
            for (size_t j = 0; j < sites[i]->neighbors.getSize(); ++j) {
                if (sites[i]->neighbors[j] == site) {
                    index = j;
                    break;
                }
            }
            if (index < sites[i]->neighbors.getSize()) {
                std::swap(sites[i]->neighbors[index], sites[i]->neighbors.back());
                sites[i]->neighbors.popBack();
            }
        }
        for (size_t i = 0; i < sites.getSize(); i++) {
            if (sites[i] == site) {
                std::swap(sites[i], sites.back());
                sites.popBack();
                delete site;
                break;
            }
        }
    }

    void connect(Site* from, Site* to) {
        assert(from->owner == this && to->owner == this);
        from->neighbors.pushBack(to);
    }
};

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Network<T>& net) {
        net.dump(os);
        return os;
    }

}

#endif // NETWORK_H
