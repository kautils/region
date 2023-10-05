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
        auto data_size = prf->size();
        for(auto i = from ;i < data_size;){
            auto write_size = i+extend_size+buffer; 
            write_size = (write_size < data_size)*buffer + !(write_size < data_size)*( buffer -(write_size-data_size)); 
            prf->write(i+extend_size,i,write_size);
            i+=buffer;
        }
    }
};




#endif