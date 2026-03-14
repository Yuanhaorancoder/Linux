#ifndef __SHM_HPP
#define __SHM_HPP

#include <iostream>
#include <cstdio>
#include <sys/shm.h>
#include <string>
#include <unistd.h>

const std::string proj_name = "/home";
const int proj_id = 0x6666;
const int g_size=4096;

static std::string ToHex(int data)
{
    char hex[64];
    snprintf(hex,sizeof(hex),"0x%x",data);
    return hex;
}

class Shm
{
public:
    Shm(int size=g_size)
        : _shmid(-1)
        ,_size(size)
        ,_key(0)
    {}
    ~Shm() {}

private:
    key_t GetKey()
    {
        _key = ftok(proj_name.c_str(), proj_id);
        if (_key < 0)
        {
            perror("ftok");
            return _key;
        }
        return _key;
    }
    bool CreateCoreHelper(int flags)
    {
        // 1.获取key值
        key_t k=GetKey();
        // 2.创建共享内存
        _shmid=shmget(k,_size,flags);
        if(_shmid<0)
        {
            perror("shmget");
            return false;
        }
        return true;
    }

public:
    // 1.创建
    bool Create()
    {
        return CreateCoreHelper(IPC_CREAT | IPC_EXCL | 0666);
    }
    // 2.获取共享内存
    bool Get()
    {
        return CreateCoreHelper(IPC_CREAT);
    }
    // 3.删除共享内存
    bool Delete()
    {
        int n=shmctl(_shmid,IPC_RMID,NULL);
        return n < 0 ? false : true;
    }
    // 4.获取共享内存属性
    void GetShmAttr()
    {
        struct shmid_ds ds;
        int n=shmctl(_shmid,IPC_STAT,&ds);
        if(n<0)
        {
            perror("shmctl");
            return;
        }
        std::cout<<"pid:"<<getpid()<<std::endl;

        std::cout<<ds.shm_cpid<<std::endl;
        std::cout<<ds.shm_segsz<<std::endl;
        std::cout<<ToHex(ds.shm_perm.__key)<<std::endl;
    }
    void *Attach()
    {
        return shmat(_shmid,NULL,0);
    }
    void Debug()
    {
        std::cout<<"key: "<<ToHex(_key)<<std::endl;
        std::cout<<"shmid: "<<_shmid<<std::endl;
    }
private:
    key_t _key;
    int _shmid;
    int _size;
};

#endif
