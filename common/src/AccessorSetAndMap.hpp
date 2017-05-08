//Copyright (c) 2017 Finjin
//
//This file is part of Finjin Exporter (finjin-exporter).
//
//Finjin Exporter is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//Finjin Exporter is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with Finjin Exporter.  If not, see <http://www.gnu.org/licenses/>.


#pragma once


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

#if FINJIN_EXPORTER_HOST == FINJIN_EXPORTER_HOST_3DSMAX || FINJIN_EXPORTER_HOST == FINJIN_EXPORTER_HOST_MOTIONBUILDER

    /**
     * This accessor set is pretty much just an alias for std::set, and assumes that
     * T has properly supports the < comparison operator
     */
    template <class T>
    class AccessorSet : public std::set<T>
    {
    public:
        void remove(const T& object)
        {
            iterator i = find(object);
            if (i != end())
                erase(i);
        }
    };

    /**
     * This accessor set is pretty much just an alias for std::map, and assumes that
     * T has properly supports the < comparison operator
     */
    template <class K, class V>
    class AccessorMap : public std::map<K, V>
    {
    };

#else

    /**
     * This accessor provides an interface that looks like std::set but uses
     * T's equality operator
     */
    template <class T>
    class AccessorSet : public std::list<T>
    {
    public:
        typedef typename std::list<T> base;
        typedef typename base::iterator iterator;
        typedef typename base::const_iterator const_iterator;

        iterator begin()
        {
            return base::begin();
        }

        const_iterator begin() const
        {
            return base::begin();
        }

        iterator end()
        {
            return base::end();
        }

        const_iterator end() const
        {
            return base::end();
        }

        void insert(const T& object)
        {
            if (find(object) == end())
                base::push_back(object);
        }

        iterator find(const T& object)
        {
            for (iterator i = begin(); i != end(); ++i)
            {
                if (*i == object)
                    return i;
            }
            return end();
        }

        const_iterator find(const T& object) const
        {
            for (const_iterator i = begin(); i != end(); ++i)
            {
                if (*i == object)
                    return i;
            }
            return end();
        }

        void erase(const T& object)
        {
            iterator i = find(object);
            if (i != end())
                i = std::list<T>::erase(i);
        }

        iterator erase(iterator i)
        {
            return std::list<T>::erase(i);
        }

        const_iterator erase(const_iterator i)
        {
            return std::list<T>::erase(i);
        }
    };

    template <class K, class V>
    struct AccessorMapItem : std::pair<K, V>
    {
    public:
        AccessorMapItem& operator = (const V& v)
        {
            this->second = v;
            return *this;
        }

        operator const V& () const
        {
            return this->second;
        }

        operator V& ()
        {
            return this->second;
        }
    };

    /**
     * This accessor provides an interface that looks like std::map but uses
     * T's equality operator
     */
    template <class K, class V>
    class AccessorMap : public std::list<AccessorMapItem<K, V> >
    {
    public:
        typedef typename std::list<AccessorMapItem<K, V> > base;
        typedef typename base::iterator iterator;
        typedef typename base::const_iterator const_iterator;

        iterator begin()
        {
            return base::begin();
        }

        const_iterator begin() const
        {
            return base::begin();
        }

        iterator end()
        {
            return base::end();
        }

        const_iterator end() const
        {
            return base::end();
        }

        AccessorMapItem<K, V>& back()
        {
            return base::back();
        }

        AccessorMapItem<K, V>& operator [] (const K& key)
        {
            iterator i = find(key);
            if (i != end())
                return *i;
            else
            {
                AccessorMapItem<K, V> item;
                item.first = key;
                base::push_back(item);
                return back();
            }
        }

        iterator find(const K& key)
        {
            for (iterator i = begin(); i != end(); ++i)
            {
                if (i->first == key)
                    return i;
            }
            return end();
        }

        const_iterator find(const K& key) const
        {
            for (const_iterator i = begin(); i != end(); ++i)
            {
                if (i->first == key)
                    return i;
            }
            return end();
        }

        void erase(const K& key)
        {
            iterator i = find(key);
            if (i != end())
                erase(i);
        }

        iterator erase(iterator i)
        {
            return base::erase(i);
        }

        const_iterator erase(const_iterator i)
        {
            return base::erase(i);
        }
    };

#endif

} }
