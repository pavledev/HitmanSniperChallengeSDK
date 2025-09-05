#pragma once

template <typename TElement, typename TStorage>
class TMaxArrayBase : public TStorage
{
public:
    unsigned int Size() const
    {
        return this->m_nSize;
    }

    TElement* operator[](unsigned int index)
    {
        return reinterpret_cast<TElement*>(this->m_data) + index;
    }

    const TElement* operator[](unsigned int index) const
    {
        return reinterpret_cast<const TElement*>(this->m_data) + index;
    }
};
