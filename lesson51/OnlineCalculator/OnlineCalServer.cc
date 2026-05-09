#include "Protocol.hpp"

int main()
{
    Request req1(10,20,'+'); 
    std::string s;
    req1.Serialize(&s);

    std::cout<<s<<std::endl;

    // std::string jsonstring = "{\"datax\":-30,\"datay\":100,\"oper\":43}";
    // Request req(0,0,'-');
    // req.Print();

    // std::cout<<"---------------------"<<std::endl;
    // req.Deserialize(jsonstring);
    // req.Print();
    
    return 0;
}