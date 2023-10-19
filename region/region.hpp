#ifndef KAUTIL_REGION_REGION_REGION_HPP
#define KAUTIL_REGION_REGION_REGION_HPP

#include <error.h>
#include <stdio.h>

namespace kautil{

template<typename pref>
struct region{
    
    using offset_type = typename pref::offset_type;
    pref * prf=0;
    
    region(pref * prf) : prf(prf){}
    ~region(){}
    int claim(offset_type from,offset_type extend_size,offset_type buffer){
        if(!prf->extend(extend_size)) return 1;
        auto current_data_size = (prf->size()>extend_size)*(prf->size()-extend_size);

        //auto is_overflow = false;
        auto write_size = 
                  bool(current_data_size)*((buffer<current_data_size-from)*buffer + !(buffer<current_data_size-from)*(current_data_size-from))
                +!bool(current_data_size)*extend_size;
        for(auto i = offset_type(current_data_size);;i-=write_size){
            auto is_overflow = i < from+write_size;
            auto cur = !is_overflow*(i-write_size) + is_overflow*from;
            write_size=
                     !is_overflow*write_size
                    + is_overflow*(i-from);
                    //+ is_overflow*(write_size-(-i+write_size));
            
            //printf("%ld %ld\n",cur+extend_size,cur);fflush(stdout);
            prf->shift(cur+extend_size,cur,write_size);
            if(is_overflow)break;
        }
        return 0;
    }
    
    int shrink(offset_type from,offset_type shrink_size,offset_type write_buffer_size){
        auto eof = prf->size();
        auto write_size = eof-from;
        write_size = (write_size > write_buffer_size)*write_buffer_size + !(write_size > write_buffer_size)*write_size;
        auto is_overflow=false;
        for(auto i = from; i < eof; i+=write_size){
            auto src = !is_overflow*i + is_overflow*(eof-1);
            prf->shift(src-shrink_size,src,write_size);
            is_overflow=(i+write_size>=eof);
        }
        return !prf->extend(-shrink_size);
    }

    
};

}// kautil



#endif