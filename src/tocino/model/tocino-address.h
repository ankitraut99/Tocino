/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __TOCINO_ADDRESS_H__
#define __TOCINO_ADDRESS_H__

#include <stdint.h>

#include "ns3/mac48-address.h"

namespace ns3 {

class TocinoAddress
{
    public:

    TocinoAddress()
    {
        m_address.raw = 0;
    }

    TocinoAddress( uint32_t a )
    {
        m_address.raw = a;
    }
    
    TocinoAddress( uint8_t x, uint8_t y, uint8_t z, uint8_t res = 0 )
    {
        m_address.x = x;
        m_address.y = y;
        m_address.z = z;
        m_address.reserved = res;
    }

    Address ConvertTo() const
    {
        return Address( GetType(),
                reinterpret_cast<const uint8_t*>( &m_address.raw ),
                sizeof( m_address ) );
    }
    
    operator Address() const
    {
        return ConvertTo();
    }

    static TocinoAddress ConvertFrom( const Address &a )
    {
        TocinoAddress ta;
        NS_ASSERT( a.CheckCompatible( GetType(), sizeof( m_address ) ) );
        a.CopyTo( reinterpret_cast<uint8_t*>( &ta.m_address.raw ) );
        return ta;
    }

    private:

    static uint8_t GetType()
    {
        static uint8_t type = Address::Register();
        return type;
    }

    Mac48Address AsMacAddress() const
    {
        // FIXME: stub
        return Mac48Address(0);
    }
    
    union
    {
        struct
        {
            uint8_t x, y, z;
            uint8_t reserved;
        };
        uint32_t raw;
    }
    m_address;
};

}

#endif /* __TOCINO_ADDRESS_H__ */

