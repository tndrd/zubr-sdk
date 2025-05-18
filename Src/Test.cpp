#include <ZubrRPC.hpp>
#include <iostream>
#include <cassert>

using ZP = Zubr::RPC::Procedures;

int main(int argc, char** argv) {
  assert(argc == 2);

  size_t nRecv = std::stoul(argv[1]);

  Zubr::IO::TTYConfig conf;

  conf.Baudrate = 3000000;
  conf.ParityBit = false;
  conf.Stopbits = Zubr::IO::TTYConfig::StopbitsCount::One;
  conf.Timeout = 1;

  conf.Port = "/dev/ttyACM0";

  auto io = Zubr::IO(conf);
  
  /*
  Zubr::CsMessageOut msgOut;
  msgOut.hostBeginQuery(RB_CB_UNIT_PARAM);
  msgOut.addInt8(0);
  msgOut.addInt8(Zubr::RPC::Messages::UnitParamCtl::Read);
  msgOut.addInt8(1);
  msgOut.addInt16(0);
  msgOut.addInt32(0);
  msgOut.hostEnd();
  io.Send(msgOut);
  */
  
  try {
    //auto msgIn = io.Recv(nRecv);
    
    Zubr::RPC::Messages::UnitParam param;
    param.Sio() = 0;
    param.Oper() = param.Read;
    param.Id() = 0;
    param.Index() = 0;
    param.Value() = 0;

    auto resp = Zubr::RPC::Call<ZP::UnitParam>(io, param);
    
    std::cout << "Sio: " << +resp.Sio() << std::endl;
    std::cout << "Oper: " << +resp.Oper() << std::endl;
    std::cout << "Id: " << +resp.Id() << std::endl;
    std::cout << "Index: " << +resp.Index() << std::endl;
    std::cout << "Value: " << +resp.Value() << std::endl;

  } catch (std::exception &e) {
    std::cout << "Got exception: " << e.what() << std::endl;
  }

  std::cout << std::hex;
  for (int i = 0; i < nRecv; ++i)
    std::cout << +io.RxBuf[i] << " ";
  std::cout << std::dec << std::endl;
  
  return 0;
}