#ifndef KAUTIL_REGION_REGION_REGION_HPP
#define KAUTIL_REGION_REGION_REGION_HPP


template<typename pref>
struct region{
    
    using offset_type = typename pref::offset_type;
    pref * prf=0;
    
    region(pref * prf) : prf(prf){}
    ~region(){}
    void claim(offset_type from,offset_type extend_size,offset_type buffer){
        
        prf->extend(extend_size);
        auto current_data_size = prf->size()-extend_size;
        auto write_size = (extend_size < buffer)*extend_size + !(extend_size < buffer)*buffer; 
        for(auto i = offset_type(current_data_size);;i-=write_size){
            auto is_overflow = i<write_size;
            auto cur = !is_overflow*(i-write_size);
            write_size=
                     !is_overflow*write_size
                    + is_overflow*(write_size-(-i+write_size));
            prf->shift(cur+extend_size,cur,write_size);
            if(!cur) break;
        }
        
    }
    
    
    void shrink(offset_type begin,offset_type shrink_size,offset_type write_buffer_size){
        auto end = prf->size();
        auto write_size = end-begin;
        auto new_size = end+shrink_size;
        {
            for(auto i = begin; i < end; ){
                write_size = (write_size > write_buffer_size)*write_buffer_size + !(write_size > write_buffer_size)*write_size;
                prf->shift(begin+shrink_size,i,write_size);
                i+=write_size;
            }
        }
        prf->extend(shrink_size);
    }

    
};




#endif