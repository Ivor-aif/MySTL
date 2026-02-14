//
// Created by Ivor_Aif on 2026/2/9.
//

#ifndef NETWORK_H
#define NETWORK_H

#include <cassert>

#include "Array.h"

namespace mySTL::containers {

template<typename T, typename Alloc = std::allocator<T>>
class Network {
public:
    struct Site {
    private:
        size_t id;
        T value;
        Array<Site*> neighbors;
        Network* owner;

        friend class Network;

    public:
        Site(Network* owner, const size_t id, const T& value) : id(id), value(value), owner(owner) {

        }

        const T& getValue() const noexcept{
            return value;
        }

        const Array<Site*>& getNeighbors() const noexcept {
            return neighbors;
        }
    };

    using siteAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Site>;
    using siteTraits = std::allocator_traits<siteAlloc>;

private:
    size_t nId = 0;
    Array<Site*> sites;
    siteAlloc alloc;

    void destroySite(Site* site) noexcept {
        siteTraits::destroy(alloc, site);
        siteTraits::deallocate(alloc, site, 1);
    }

    void clear() noexcept {
        for (Site* site : sites) {
            destroySite(site);
        }
        sites.clear();
        nId = 0;
    }

    void dump(std::ostream& os) const {
        for (const Site* st : sites) {
            os << "Site " << st->id << ": ";
            for (const Site* nb : st->neighbors) {
                os << nb->id << " ";
            }
            os << '\n';
        }
    }

    template<typename U, typename A>
    friend std::ostream& operator<<(std::ostream& os, const Network<U, A>& network);

public:
    Network() = default;

    explicit Network(const Alloc& alloc) noexcept : alloc(alloc) {

    }

    ~Network() {
        clear();
    }

    Network(const Network&) = delete;
    Network& operator=(const Network&) = delete;

    Network(Network&& other) noexcept : nId(other.nId), sites(std::move(other.sites)), alloc(std::move(other.alloc)) {
        for (Site* site : sites) {
            site->owner = this;
        }
        other.sites.clear();
        other.nId = 0;
    }

    Network& operator=(Network&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        clear();
        if constexpr (siteTraits::propagate_on_container_move_assignment::value) {
            alloc = std::move(other.alloc);
        }
        nId = 0;
        sites = std::move(other.sites);
        for (Site* site : sites) {
            site->owner = this;
        }
        other.sites.clear();
        other.nId = 0;
        return *this;
    }

    const Array<Site*>& getSites() const noexcept {
        return sites;
    }

    Site* create(const T& value) {
        Site* site = siteTraits::allocate(alloc, 1);
        try {
            siteTraits::construct(alloc, site, this, nId, value);
            sites.pushBack(site);
            ++nId;
            return site;
        } catch (...) {
            siteTraits::destroy(alloc, site);
            siteTraits::deallocate(alloc, site, 1);
            throw;
        }
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
                destroySite(site);
                break;
            }
        }
    }

    void connect(Site* from, Site* to) {
        assert(from && to);
        assert(from->owner == this && to->owner == this);
        from->neighbors.pushBack(to);
    }
};

template<typename T, typename Alloc>
std::ostream& operator<<(std::ostream& os, const Network<T, Alloc>& net) {
    net.dump(os);
    return os;
}

}

#endif // NETWORK_H
