
#ifdef TMAIN_KAUTIL_REGION_INTERFACE

#include "kautil/region/region.hpp"
#include "sys/stat.h"
#include "fcntl.h"
#include "unistd.h"
#include <string>


int main(){
//    {
//        auto data = std::string{};
//        for(auto i = 0; i < 100; ++i) data.append(std::to_string(i%10));
//        struct memory_pref{
//            using offset_type = uint64_t;
//            std::string * data=0;
//
//            void extend(offset_type extend_size){ data->resize(data->size()+extend_size); }
//            int shift(offset_type dst,offset_type src,offset_type size){ return !memcpy(data->data()+dst,data->data()+src,size); }
//            offset_type size(){ return data->size(); }
//
//
//        } prf;
//        prf.data=&data;
//
//        auto from = 0;
//        auto extend_size = 100;
//        auto buffer = 4096;
//        auto reg = region<memory_pref>{&prf};
//        reg.claim(from,extend_size,buffer);
//
//        for(auto i = 0; i < extend_size;++i)data[from+i] = '0';
//        auto cnt = 0;
//        for(auto & c : data){
//            if(0==(cnt)%10) printf("\n");
//            printf("%c ",c);
//            ++cnt;
//        }
//    }
//
//
//    printf("\n++++++++++++++++++++++++++++++\n");fflush(stdout);
    
    
    {
        auto p = "./claim_test";
        remove(p);
        auto fd = -1;
        struct stat st; 
        if(stat(p,&st)){
            fd = open(p,O_CREAT|O_EXCL|O_RDWR,0777);
        }else{
            fd = open(p,O_RDWR);
        }
        
        for(auto i = 0; i < 100; ++i){
            write(fd,std::to_string(i%10).data(),1);
        }
        lseek(fd,0,SEEK_SET);
        
        struct syscall_pref{
            using offset_type = long;
            int fd = -1;
            char * buffer = 0;
            offset_type buffer_size = 0;
            struct stat st;
            
            ~syscall_pref(){ free(buffer); }
            int extend(offset_type extend_size){ 
                fstat(fd,&st);
                return ftruncate(fd,st.st_size+extend_size);
            }
            int shift(offset_type dst,offset_type src,offset_type size){
                if(buffer_size < size){
                    if(buffer)free(buffer);
                    buffer = (char*) malloc(buffer_size = size);
                }
                lseek(fd,src,SEEK_SET);
                ::read(fd,buffer,size);
                lseek(fd,dst,SEEK_SET);
                ::write(fd,buffer,size);
                return 0;
            }
            
            
            offset_type size(){ 
                fstat(fd,&st);
                return static_cast<offset_type>(st.st_size); 
            }
        } prf{.fd=fd};
        
        auto from = 0;
        auto extend_size = 100;
        auto buffer = 128;
        auto reg = kautil::region<syscall_pref>{&prf};
        reg.claim(from,extend_size,buffer);
        
        auto c = '0';
        for(auto i = 0; i< extend_size; ++i){
            lseek(fd,from+i,SEEK_SET);
            write(fd,&c,sizeof(c));
        }
        
        
        fstat(fd,&st);
        auto cnt = 0;
        lseek(fd,0,SEEK_SET);
        for(auto i = 0;i < st.st_size;++i){
            lseek(fd,i,SEEK_SET);
            read(fd,&c,sizeof(c));
            if(0==(cnt)%10) printf("\n");
            if(c==0){
                printf("%c ",'0');
            }else{
                printf("%c ",c);
            }
            ++cnt;
        }
        
        
    }
    
    
    
    return 0;
}


#endif
