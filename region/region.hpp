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
    
    void shrink(offset_type from,offset_type shrink_size,offset_type write_buffer_size){
        auto eof = prf->size();
        auto write_size = eof-from;
        write_size = (write_size > write_buffer_size+from)*write_buffer_size + !(write_size > write_buffer_size)*write_size;
        for(auto i = from; i < eof; i+=write_size){
            auto is_overflow=(i+write_size>=eof);
            auto src = !is_overflow*i + is_overflow*(eof-1);
            prf->shift(src-shrink_size,src,write_size);
        }
        prf->extend(-shrink_size);
    }

    
};




#endif